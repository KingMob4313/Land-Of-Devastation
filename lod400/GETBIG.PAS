program gifslow;

uses crt,dos,graph;

type

    RasterArray = Array [0..63999] of byte;
    RasterP = ^RasterArray;
    imagetype= array[1..11138] of byte;

var
    ofsx,ofsy: word;
    tx,ty: word;
    m1,m2,m3,m4: word;
    cursorx,cursory: word;
    cursorimg: imagetype;
    tempimg: imagetype;

    GifFile:File of RasterArray;  {The input file}
    GifStuff:RasterP;   {The heap array to hold it, raw}
    Raster:RasterP;     {The raster data stream, unblocked}
    Raster2:RasterP;    {More raster data stream if needed}
    Regs:Registers;     {Turbo's predefined record}

    Byteoffset,         {Computed byte position in RASTER array}
    Bitoffset           {Bit offset of next code in RASTER array}
    :LongInt;

    Width,      {Read from GIF header, image width}
    Height,     { ditto, image height}
    LeftOfs,    { ditto, image offset from left}
    TopOfs,     { ditto, image offset from top}
    RWidth,     { ditto, raster width}
    RHeight,    { ditto, raster height}
    ClearCode,  {GIF clear code}
    EOFCode,    {GIF end-of-information code}
    OutCount,   {Decompressor output 'stack count'}
    MaxCode,    {Decompressor limiting value for current code size}
    Code,       {Value returned by ReadCode}
    CurCode,    {Decompressor variable}
    OldCode,    {Decompressor variable}
    InCode,     {Decompressor variable}
    FirstFree,  {First free code, generated per GIF spec}
    FreeCode,   {Decompressor, next free slot in hash table}
    GIFPtr,     {Array pointers used during file read}
    RasterPtr,
    XC,YC,      {Screen X and Y coords of current pixel}
    Pindex,     {Index into screen save array}
    ReadMask,   {Code AND mask for current code size}
    I           {Loop counter, what else?}
    :word;


    Interlace,  {True if interlaced image}
    NextRaster, {True if file > 64000 bytes}
    ColorMap    {True if colormap present}
    :Boolean;

    ch           {Utility}
    :char;

    a,              {Utility}
    Resolution,     {Resolution, read from GIF header}
    BitsPerPixel,   {Bits per pixel, read from GIF header}
    Background,     {Background color, read from GIF header}
    ColorMapSize,   {Length of color map, from GIF header}
    CodeSize,       {Code size, read from GIF header}
    InitCodeSize,   {Starting code size, used during Clear}
    FinChar,        {Decompressor variable}
    Pass,           {Used by video output if interlaced pic}
    BitMask,        {AND mask for data size}
    R,G,B
    :byte;


    {The hash table used by the decompressor}
    Prefix: Array [0..4095] of word;
    Suffix: Array [0..4095] of byte;

    {An output array used by the decompressor}
    Outcode:Array [0..1024] of byte;

    {The color map, read from the GIF header}
    Red,Green,Blue: array [0..255] of byte;

    {The EGA palette, derived from the color map}
    Palette: Array [0..255] of byte;

    {Strings to hold the filenames}
    FileString:String [80];

    outpf: text;


Const

    MaxCodes: Array [0..9] of Word = (4,8,16,$20,$40,$80,$100,$200,$400,$800);

    CodeMask:Array [1..4] of byte= (1,3,7,15);

    PowersOf2: Array [0..8] of word=(1,2,4,8,16,32,64,128,256);

    Masks: Array [0..9] of Integer = (7,15,$1f,$3f,$7f,$ff,$1ff,$3ff,$7ff,$fff);

    Rastersize:Word = 64000;


procedure SaveImage(x,y: word);
var
 imagefile: file of imagetype;
begin;
 setcolor(lightblue);
 line(x,y,x,y+120);
 line(x,y,x+180,y);
 line(x,y+120,x+180,y+120);
 line(x+180,y,x+180,y+120);
 getimage(x,y,x+180,y+120,tempimg);

 assign(imagefile,'GETBIG.DAT');
 rewrite(imagefile);
 seek(imagefile,filesize(imagefile));
 write(imagefile,tempimg);
 close(imagefile);
end;

Procedure AllocMem (Var P:RasterP);
Var
 ASize:Longint;
Begin
     ASize:=MaxAvail;
     If ASize<RasterSize then
        Begin
             Textmode (15);
             Writeln ('Insufficient memory available!');
             Halt;
        End
        Else
        Getmem (P,RasterSize);
End;


Function Getbyte:Byte;
Begin
 If GIFPtr=RasterSize then Exit;
 Getbyte:=GIFStuff^[GIFPtr];
 GIFPtr:=Succ(GIFPtr);
End;


Function Getword:Word;
Var
 A,B:Byte;
Begin
 A:=Getbyte;
 B:=Getbyte;
 Getword:=A+(256*B);
End;

Procedure ReadRaster;

Var BlockLength:Byte;
    I,IOR:Integer;

Begin
   RasterPtr:=0;
   Repeat
   BlockLength:=Getbyte;
     For I:=0 to Blocklength-1 do
       Begin
         If Gifptr = RasterSize then
            Begin
                 {$I-}
                 Read (GIFFile,GIFStuff^);
                 {$I+}
                 IOR:=IOResult;
                 GIFPtr:=0;
            End;
         If not Nextraster then
                  Raster^[RasterPtr]:=Getbyte else
                         Raster2^[RasterPtr]:=Getbyte;
         RasterPtr:=Succ (RasterPtr);
         If RasterPtr=RasterSize then
         Begin
            NextRaster:=True;
            Rasterptr:=0;
            AllocMem (Raster2);
         End;
       End;
   Until Blocklength=0;
End;


{Fetch the next code from the raster data stream. The codes can be any
length from 3 to 12 bits, packed into 8-bit bytes, so we have to maintain
our location in the Raster array as a BIT offset. We compute the byte offset
into the raster array by dividing this by 8, pick up three bytes, compute
the bit offset into our 24-bit chunk, shift to bring the desired code to
the bottom, then mask it off and return it. If the unblocked raster data
overflows the original RASTER array, we switch to the second one}

Procedure ReadCode;

Var RawCode:LongInt;
    A,B:Word;


Begin
     ByteOffset:=BitOffset div 8;

{Pick up our 24-bit chunk}

     A:=Raster^[Byteoffset]+(256*Raster^[ByteOffset+1]);
     If CodeSize>=8 then
     Begin
     B:=Raster^[Byteoffset+2];
     RawCode:=A+(65536*B);
     End
     Else Rawcode:=A;

{Doing the above calculation as a single statement, i.e.
Rawcode:=Raster^[Byteoffset]+(256*Raster^[Byteoffset+1])+
         (65536*Raster[Byteoffset+2])
sometimes returns incorrect results. This may or may not be a bug.}


     RawCode:=RawCode shr (BitOffset mod 8);
     Code:=RawCode and ReadMask;

{Cope with overflow of the first RASTER array}

     If (Nextraster) and (Byteoffset>=63000) then
        Begin
             Move (Raster^[Byteoffset],Raster^[0],RasterSize-Byteoffset);
             Move (Raster2^[0],Raster^[RasterSize-Byteoffset],63000);
             Bitoffset:=Bitoffset mod 8;
             FreeMem (Raster2,RasterSize);
        End;

     BitOffset:=BitOffset+CodeSize;

End;


Procedure AddToPixel (Index:Byte);


Begin
     putpixel(xc,yc,index);
{     Regs.AH:=12;
     Regs.AL:=Index;
     Regs.CX:=XC;
     Regs.DX:=YC;
     Intr ($10,Regs);}

{Update the X-coordinate, and if it overflows, update the Y-coordinate}

     XC:=Succ (XC);
     If XC=Width then

{If a non-interlaced picture, just increment YC to the next scan line. If
it's interlaced, deal with the interlace as described in the GIF spec. Put
the decoded scan line out to the screen if we haven't gone past the bottom
of it}

        Begin

        XC:=0;
        If not Interlace then YC:=Succ (YC) else
            Begin
               Case Pass of
               0: Begin
                  YC:=YC+8;
                  If YC>=Height then
                  Begin
                     Pass:=Succ(Pass);
                     YC:=4;
                  End;
                  End;
               1: Begin
                  YC:=YC+8;
                  If YC>=Height then
                     Begin
                       Pass:=Succ(Pass);
                       YC:=2;
                     End;
                  End;
               2: Begin
                  YC:=YC+4;
                  If YC>=Height then
                     Begin
                          Pass:=Succ(Pass);
                          YC:=1;
                     End;
                  End;
               3: Begin
                  YC:=YC+2;
                  End;
               End;    {Case}
            End;  {If interlace}
        End;

End;

Procedure InitEGA;
var
 grdriver,grmode: integer;
 a,b,c: integer;
 pal2: palettetype;
 p: word;
Begin
 grdriver:=ega;
 grmode:=egahi;
 initgraph(grdriver,grmode,'');
 getpalette(pal2);
 {for a:=0 to 15 do setpalette(a,palette[a]);
 for a:=0 to 15 do begin;
  c:=0;
  for b:=0 to 15 do if pal2.colors[b]=palette[a] then c:=b;
  if c<>0 then begin; write(^g); delay(1000); end;
  if c<>0 then setpalette(a,pal2.colors[c]);
 end;}

 setfillstyle(1,7);
 floodfill(1,1,1);
 getimage(0,0,180,120,cursorimg);
End;

procedure translate;
var
 a,b,c: word;
 p: byte;
 pal2: palettetype;
 trans: array[0..15] of byte;
begin;
 getpalette(pal2);
 for a:=0 to 15 do begin;
  trans[a]:=a;
  c:=0;
  for b:=0 to 15 do if pal2.colors[b]=palette[a] then c:=b;
  if c<>0 then trans[a]:=c;
 end;
{ trans[lightcyan]:=14;
 trans[crt.lightblue]:=magenta;
 trans[yellow]:=lightcyan;}
{ trans[lightblue]:=4;
 trans[lightcyan]:=14;
 trans[3]:=lightcyan;}
 for a:=0 to 180 do for b:=0 to 120 do begin;
  p:=trans[getpixel(a,b)];
  putpixel(a,b,p);
 end;
end;


{Determine the palette value corresponding to the GIF colormap intensity
value.}

Procedure DetColor (Var PValue:Byte;MapValue:Byte);

Var Local:Byte;

Begin
     PValue:=MapValue div 64;
     If PValue=1 then PValue:=2 else
     If PValue=2 then PValue:=1;
End;

{Set the key variables to
their necessary initial values.}

Procedure ReInitialize;
Begin
     XC:=0;          {X and Y screen coords back to home}
     YC:=0;
     Pass:=0;        {Interlace pass counter back to 0}
     Bitoffset:=0;   {Point to the start of the raster data stream}
     GIFPtr:=0;      {Mock file read pointer back to 0}
End;

{React to GIF clear code, or reset GIF decompression values back to their
initial state when restarting.}

Procedure DoClear;

    Begin
      CodeSize:=InitCodeSize;
      MaxCode:=MaxCodes [CodeSize-2];
      FreeCode:=FirstFree;
      ReadMask:=Masks [CodeSize-3];
    End;

procedure mouse(var m1,m2,m3,m4: word);
var
 regs: registers;
begin;
 regs.ax:=m1;
 regs.bx:=m2;
 regs.cx:=m3;
 regs.dx:=m4;
 intr(51,regs);
 m1:=regs.ax;
 m2:=regs.bx;
 m3:=regs.cx;
 m4:=regs.dx;
end;

procedure clear_mouse;
begin;
 repeat;
  m1:=3;
  mouse(m1,m2,m3,m4);
 until ((m2 and 1)=0);
end;

procedure show_cursor;
begin;
 m1:=1; mouse(m1,m2,m3,m4);
end;

procedure hide_cursor;
begin;
 m1:=2; mouse(m1,m2,m3,m4);
end;

procedure init_mouse;
begin;
 m1:=0; mouse(m1,m2,m3,m4);
end;

procedure getmouse(var x,y: word);
begin;
 m1:=3; mouse(m1,m2,m3,m4);
 x:=m3;
 y:=m4;
end;

Begin    {the main program}

{Initialize a bunch of variables}

     ReInitialize;         {Initialize common vars}
     Nextraster:=False;    {Over 64000 flag off}

{Get memory for the raster data array, and the input file data array}

     AllocMem (Raster);
     AllocMem (GIFStuff);

{Prompt the user for the filename}

     writeln('A mouse driver must be used with this program.');
     writeln('Captured image will be written to GETBIG.DAT.');
     writeln('Press [space] to capture, [ESC] to quit, and mouse or 8/4/6/2 to move');
     writeln;
     Write ('Input Filename (.GIF File in 640x350x16 res): ');
     Readln (Filestring);


{Open the file}

{$I-}
     Assign (giffile,FileString);
     Reset (giffile);
{$I+}

{Cope with I/O error should one occur}

     I:=IOResult;
     If I<>0 then
        Begin
             Writeln ('Error opening file ',FileString,'. Press any key ');
             Readln;
             Exit;
        End;

{Read in the GIF file. Reading it as one big hunk rather than N bytes results
in far faster disk I/O; see user notes. Error checking is turned off in
order to avoid 'attempt to read past EOF' errors. If the file does not exist,
this will be detected at RESET}

     Writeln ('Reading . . . ');
{$I-}
     Read (GIFFile,GIFStuff^);
{$I+}

{Note that 4.0 requires this assignment, or else if an error results (as it
will if the file is smaller than 64000 bytes) no I/O will be allowed for
the remainder of the run}

I:=IOResult;

{Deal with the GIF header. Start by checking the GIF tag to make sure this
is a GIF file}

     FileString:='';
     for i:=1 to 6 do
     Begin
         FileString:=FileString+chr(Getbyte);
     End;
     If FileString<>'GIF87a' then
        Begin
             Writeln ('Not a GIF file, or header read error. Press any key ');
             Readln;
             Exit;
        End;

{Get variables from the GIF screen descriptor}

     RWidth:=Getword;         {The raster width and height}
     RHeight:=Getword;
     {Get the packed byte immediately following and decode it}
     B:=Getbyte;
     If B and $80=$80 then Colormap:=True else Colormap:=False;
     Resolution:=B and $70 shr 5 +1;
     BitsPerPixel:=B and 7 +1;
     If BitsPerPixel=1 then I:=2 else I:=1 shl BitsPerPixel;
     Write ('Colors: ',I);
     BitMask:=CodeMask [BitsPerPixel];
     Background:=Getbyte;
     B:=Getbyte;         {Skip byte of 0's}

{Compute size of colormap, and read in the global one if there. Compute
values to be used when we set up the EGA palette}

     ColorMapSize:=1 shl BitsPerPixel;
     If Colormap then Begin
      assign(outpf,'PAL.TXT');
      rewrite(outpf);
      For I:=0 to ColorMapSize-1 do Begin
         Red [I]:=Getbyte;
         Green [I]:=Getbyte;
         Blue [I]:=Getbyte;
         DetColor (R,Red[I]);
         DetColor (G,Green [I]);
         DetColor (B,Blue [I]);
         writeln(outpf,i,' ',r,' ',g,' ',b);
         Palette [I]:=B and 1+(2*(G and 1))+(4*(R and 1))+(8*(B div 2))+(16*(G div 2))+(32*(R div 2));
      End;
      close(outpf);
      Writeln;
      Palette [16]:=Background;
     End;

{Now read in values from the image descriptor}

     B:=Getbyte;  {skip image seperator}
     Leftofs:=Getword;
     Topofs:=Getword;
     Width:=Getword;
     Writeln ('Width: ',Width);
     Height:=Getword;
     Writeln ('Height: ',Height);
     A:=Getbyte;
     If A and $40=$40 then Interlace:=True else Interlace:=False;


{Note that we ignore the possible existence of a local color map. I've yet
to encounter an image that had one, and the spec says it's defined for
future use. This could lead to an error reading some files}

{Start reading the raster data. First we get the intial code size}

     Codesize:=Getbyte;

{Compute decompressor constant values, based on the code size}

     ClearCode:=PowersOf2 [Codesize];
     EOFCode:=ClearCode+1;
     FirstFree:=ClearCode+2;
     FreeCode:=FirstFree;

{The GIF spec has it that the code size is the code size used to compute the
above values is the code size given in the file, but the code size used in
compression/decompression is the code size given in the file plus one.}

     Codesize:=Succ (Codesize);
     InitCodeSize:=Codesize;
     Maxcode:=Maxcodes [Codesize-2];
     ReadMask:=Masks [Codesize-3];

{Read the raster data. Here we just transpose it from the GIF array to the
Raster array, turning it from a series of blocks into one long data stream,
which makes life much easier for ReadCode}

     Writeln ('Unblocking');
     ReadRaster;

{Get ready to do the actual read/display. Free up the heap used by the
GIF array since we don't need it any more, and if the user wants to save,
reclaim it for the Picture array}

     FreeMem (GIFStuff,RasterSize);
     OutCount:=0;

{Set up the EGA}

     InitEGA;

{Decompress the file, continuing until you see the GIF EOF code. One
obvious enhancement is to add checking for corrupt files here.}

   Repeat

     {Get the next code from the raster array}

          ReadCode;

          If Code <> EOFCode then
          Begin

     {Clear code sets everything back to its initial value, then reads
      the immediately subsequent code as uncompressed data.}

            If Code = ClearCode then
               Begin
                 DoClear;
                 ReadCode;
                 CurCode:=Code;
                 OldCode:=Code;
                 FinChar:=Code and BitMask;
                 AddToPixel (FinChar);
               End
               Else

     {If not a clear code, then must be data: save same as CurCode and InCode}

               Begin
                CurCode:=Code;
                InCode:=Code;

     {If greater or equal to FreeCode, not in the hash table yet; repeat
      the last character decoded}

                If Code>=FreeCode then
                  Begin
                    CurCode:=OldCode;
                    OutCode [OutCount]:=FinChar;
                    OutCount:=Succ (OutCount);
                  End;

     {Unless this code is raw data, pursue the chain pointed to by CurCode
     through the hash table to its end; each code in the chain puts its
     associated output code on the output queue.}

                If CurCode>BitMask then
                   Repeat
                     OutCode [OutCount]:=Suffix [CurCode];
                     OutCount:=Succ (OutCount);
                     CurCode:=Prefix [CurCode];
                   Until CurCode<=BitMask;

      {The last code in the chain is treated as raw data.}

               FinChar:=CurCode and BitMask;
               OutCode [OutCount]:=FinChar;
               OutCount:=Succ (OutCount);

     {Now we put the data out to the using routine. It's been stacked
      LIFO, so deal with it that way}

               For I:=OutCount-1 downto 0 do
                    AddToPixel (Outcode [I]);

      {Make darned sure OutCount gets set back to start}

               OutCount:=0;

      {Build the hash table on-the-fly. No table is stored in the file.}

               Prefix [FreeCode]:=OldCode;
               Suffix [FreeCode]:=FinChar;
               OldCode:=InCode;

     {Point to the next slot in the table. If we exceed the current MaxCode
      value, increment the code size unless it's already 12. If it is, do
      nothing: the next code decompressed better be CLEAR}

               FreeCode:=Succ (FreeCode);
               If FreeCode>=MaxCode then
                Begin
                  If CodeSize < 12 then
                  Begin
                     CodeSize:=Succ (CodeSize);
                     MaxCode:=MaxCode*2;
                     ReadMask:=Masks [CodeSize-3];
                  End;
                End;
               End {not Clear};

               If Keypressed then
                  Begin
                       Ch:=Readkey;
                       If Ch=#27 then
                          Begin
                               Textmode (15);
                               Exit;
                          End;
                  End;
            End; {not EOFCode}
       Until Code=EOFCode;

       Writeln (^G); {signals whole picture decoded}

    {Read one key, then pack it in}


    init_mouse;
    hide_cursor;
    cursorx:=0;
    cursory:=0;
    ofsx:=0;
    ofsy:=0;
    ch:=' ';
    repeat;
     putimage(cursorx+ofsx,cursory+ofsy,cursorimg,xorput);
     getmouse(tx,ty);
     while (tx=cursorx) and (ty=cursory) and (not keypressed) do getmouse(tx,ty);
     putimage(cursorx+ofsx,cursory+ofsy,cursorimg,xorput);
     getmouse(cursorx,cursory);
     if keypressed then begin;
      ch:=readkey;
      case ch of
       'T': translate;
       '6': inc(ofsx);
       '4': dec(ofsx);
       '8': dec(ofsy);
       '2': inc(ofsy);
       ' ': begin;
             write(^G);
             saveimage(cursorx+ofsx,cursory+ofsy);
            end;
      end;
     end;
    until ch=#27;
    cursorx:=imagesize(0,0,180,120);
    restorecrtmode;
    writeln(cursorx);
    Close (GifFile);
    FreeMem (Raster,RasterSize);
End.

