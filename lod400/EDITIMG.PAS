uses graph, crt, mouse;

const
 imatx=639-180;
type
 imagetype= array[1..12000] of word;
 im2type = array[1..250,1..200] of byte;
 im2ptr=^im2type;
var
 xsize,ysize: word;
 image: imagetype;
 im: im2ptr;
 curcolor: byte;
 imgx,imgy: word;
 numx,numy: word;
 fn: string;
 lx1,lx2,ly1,ly2: word;

procedure init;
var
 grmode, grdriver: integer;
 f: file of imagetype;
begin;
 imgx:=60;
 imgy:=40;
 grmode:=egahi;
 grdriver:=ega;
 initgraph(grdriver,grmode,'');
 writeln(graphresult);
 assign(f,fn);
 {$I-}
 reset(f);
 {$i+}
 if ioresult=0 then begin;
  {$I-}
  read(f,image);
  {$I+}
  if ioresult<>0 then ;
  close(f);
  putimage(imatx,0,image,normalput);
  imgx:=image[1];
  imgy:=image[2];
 end;
 numx:=imgx+1;
 numy:=imgy+1;
end;

procedure DrawImg;
var
 x,y: integer;
 xs,ys: word;
begin;
 xs:=xsize-1;
 ys:=ysize-1;
 for x:=1 to numx do for y:=1 to numy do begin;
  setfillstyle(1,im^[x,y]);
  bar((x-1)*xsize+1,(y-1)*ysize+1,(x-1)*xsize+xs,(y-1)*ysize+ys);
 end;
end;

procedure GetFlip;
var
 x,y: integer;
begin;
 for x:=1 to numx do for y:=1 to numy do begin;
  im^[numx-x+1,y]:=getpixel(imatx+x-1,y-1);
 end;
 for x:=1 to numx do for y:=1 to numy do begin;
  putpixel(imatx+x-1,y-1,im^[x,y]);
 end;
 drawimg;
end;

procedure MoveDown;
var
 x,y: integer;
begin;
 for x:=1 to numx do for y:=1 to numy-1 do begin;
  im^[x,y+1]:=getpixel(imatx+x-1,y-1);
 end;
 for x:=1 to numx do for y:=1 to numy do begin;
  putpixel(imatx+x-1,y-1,im^[x,y]);
 end;
 drawimg;
end;

procedure GetOrig;
var
 x,y: integer;
begin;
 for x:=1 to numx do for y:=1 to numy do begin;
  im^[x,y]:=getpixel(imatx+x-1,y-1);
 end;
end;

procedure DrawGrid;
var
 x,y: word;
begin;
 xsize:=((imatx-1) div (imgx+1));
 ysize:=325 div imgy;
 for x:=1 to numx+1 do line((x-1)*xsize,0,(x-1)*xsize,(numy)*ysize);
 for y:=1 to numy+1 do line(0,(y-1)*ysize,(numx)*xsize,(y-1)*ysize);
end;

procedure updatecolor;
begin;
 setfillstyle(1,curcolor);
 bar(550,300,550+50,300+40);
end;

procedure SaveImage;
var
 f: file;
begin;
 getimage(imatx,0,imatx+numx-1,numy-1,image);
 assign(f,fn);
 rewrite(f,1);
 blockwrite(f,image,imagesize(imatx,0,imatx+numx-1,numy-1));
 close(f);
end;

procedure checkmouse;
var
 mousex,mousey,buttons: word;
 imx,imy: word;
begin;
 buttons:=mousegetbuttonstatus(mousex,mousey);
 if ((buttons and 1)<>0) and (mousex<numx*xsize) and (mousey<numy*ysize) then begin;
  mousehidecursor;
  imx:=(mousex div xsize)+1;
  imy:=(mousey div ysize)+1;
  im^[imx,imy]:=curcolor;
  setfillstyle(1,curcolor);
  bar((imx-1)*xsize+1,(imy-1)*ysize+1,(imx-1)*xsize+(xsize-1),(imy-1)*ysize+(ysize-1));
  putpixel((imatx-1)+imx,(imy-1),curcolor);
  mouseshowcursor;
 end;
 if (buttons and 2)<>0 then begin;
  inc(curcolor);
  if curcolor=16 then curcolor:=0;
  mousehidecursor;
  updatecolor;
  mouseshowcursor;
  while (buttons and 2)<>0 do buttons:=mousegetbuttonstatus(mousex,mousey);
 end;
end;

procedure LoadZTImg;
type
 nmtype= array[1..20,1..18] of byte;
var
 nm: nmtype;
 f: file of nmtype;
 s: string;
 a,b: integer;
begin;
 readln(s);
 val(s,a,b);
 if a=0 then exit;
 assign(f,'LOD.DAT');
 reset(f);
 seek(f,a-1);
 read(f,nm);
 close(f);
 for a:=1 to 20 do for b:=1 to 18 do begin;
  putpixel(550+(a-1)*3,50+(b-1)*2,nm[a,b]);
  putpixel(551+(a-1)*3,50+(b-1)*2,nm[a,b]);
  putpixel(552+(a-1)*3,50+(b-1)*2,nm[a,b]);
  putpixel(550+(a-1)*3,50+(b-1)*2+1,nm[a,b]);
  putpixel(551+(a-1)*3,50+(b-1)*2+1,nm[a,b]);
  putpixel(552+(a-1)*3,50+(b-1)*2+1,nm[a,b]);
 end;
 getorig;
 drawimg;
end;

var
 done: boolean;
 ch: char;
 buttons,mousex,mousey: word;
begin;
 if paramcount=1 then fn:=paramstr(1) else begin;
  writeln('Format: EDITIMG FILENAME.DAT');
  writeln;
  writeln('Q    ... quit and save');
  writeln('A    ... abort w/o save');
  writeln('+/-  ... cycle colors');
 end;
 new(im);
 init;
 if not mousereset then begin;
  restorecrtmode;
  writeln('Mouse driver not loaded!');
  halt;
 end;
 mousehidecursor;
 drawgrid;
 getorig;
 drawimg;
 mouseshowcursor;
 mouseshowcursor;
 done:=false;
 curcolor:=7;
 updatecolor;
 repeat;
  checkmouse;
  if keypressed then begin;
   ch:=upcase(readkey);
   if ch='1' then begin;
    buttons:=mousegetbuttonstatus(mousex,mousey);
    lx1:=imatx+(mousex div xsize);
    ly1:=mousey div ysize;
   end;
   if ch='2' then begin;
    buttons:=mousegetbuttonstatus(mousex,mousey);
    lx2:=imatx+(mousex div xsize);
    ly2:=mousey div ysize;
   end;
   if ch='3' then begin;
    setcolor(curcolor);
    setfillstyle(1,curcolor);
    line(lx1,ly1,lx2,ly2);
    getorig;
    drawimg;
   end;
   if ch='!' then movedown;
   if ch='R' then getflip;
   if ch='L' then LoadZTImg;
   if ch='A' then done:=true;
   if ch='Q' then done:=true;
   if ch='+' then begin;
    inc(curcolor);
    if curcolor>15 then curcolor:=0;
    mousehidecursor;
    updatecolor;
    mouseshowcursor;
   end;
   if ch='-' then begin;
    dec(curcolor);
    if curcolor=255 then curcolor:=15;
    mousehidecursor;
    updatecolor;
    mouseshowcursor;
   end;
  end;
 until done;
 if ch='Q' then saveimage;
 restorecrtmode;
end.