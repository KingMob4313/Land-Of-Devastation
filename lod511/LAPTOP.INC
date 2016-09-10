procedure laptop_addcommand(dir,mask: integer; name,ext: string; cmdsize: integer; date,time: string);
begin;
 assert(numlaptop<maxlaptopcommands);
 with l:lapcmds[numlaptop] do
  l.code:=numlaptop+1;
  l.dir:=dir;
  l.mask:=mask;
  l.name:=name;
  l.ext:=ext;
  l.cmdsize:=cmdsize;
  l.date:=date;
  l.time:=time;
 endwith;
 inc(numlaptop);
end laptop_addcommand;

procedure laptop_swap(i,j: integer);
var
 y: tlaptopcommand;
begin;
 y:=lapcmds[i];
 lapcmds[i]:=lapcmds[j];
 lapcmds[j]:=y;
end laptop_swap;

procedure laptop_sort(l,r:integer);
var
  i,j: integer;
  x: string[8];
begin
  i:=l; j:=r; x:=lapcmds[(l+r) div 2].name;
  repeat
    while lapcmds[i].name<x do i:=i+1; endwhile;
    while x<lapcmds[j].name do j:=j-1; endwhile;
    if i<=j then
      laptop_swap(i,j);
      inc(i);
      dec(j);
    endif;
  until i>j;
  if l<j then laptop_sort(l,j); endif;
  if i<r then laptop_sort(i,r); endif;
end laptop_sort;

procedure laptop_init;
begin;
 numlaptop:=0;
 laptop_addcommand(1, rom_dir,     "DIR",       "COM",    13142,  "04-27-12",  "10:04p");
 laptop_addcommand(1, rom_exit,    "EXIT",      "COM",     2714,  "04-27-12",  "10:04p");
 laptop_addcommand(1, rom_chkdsk,  "CHKDSK",    "COM",     9535,  "04-27-12",  "10:04p");
 laptop_addcommand(1, rom_copy,    "COPY",      "COM",    31244,  "04-27-12",  "10:04p");
 laptop_addcommand(0, rom_notes,   "NOTES",     "EXE",    41797,  "08-12-13",  " 3:39a");
 laptop_addcommand(0, rom_autonav, "AUTONAV",   "EXE",   237421,  "07-26-14",  " 8:17a");
 laptop_addcommand(0, rom_transfer,"TRANSFER",  "EXE",    77842,  "11-09-13",  " 2:32p");
 laptop_addcommand(0, rom_rankings,"RANKINGS",  "EXE",    23292,  "02-17-13",  " 7:24a");
 laptop_addcommand(0, rom_viewlog, "VIEWLOG",   "COM",    10654,  "09-30-13",  "11:24p");
 laptop_addcommand(0, rom_emwarp,  "EMWARP",    "EXE",   139772,  "03-11-14",  " 2:49a");
 laptop_addcommand(0, rom_fortlist,"FORTLIST",  "EXE",    34229,  "08-14-14",  " 6:52p");
 laptop_addcommand(0, rom_diet,    "DIET",      "EXE",   124772,  "12-03-13",  "11:24a");
 laptop_addcommand(0, rom_cstats,  "CSTATS",    "COM",    27429,  "07-22-13",  " 3:33a");
 laptop_addcommand(0, rom_lrscan,  "LRSCAN",    "EXE",   118245,  "09-27-13",  " 1:24p");
 laptop_addcommand(0, rom_scanner, "SCANNER",   "EXE",    96014,  "04-26-14",  " 7:30a");
 laptop_addcommand(0, rom_troyinfo,"TROYINFO",  "EXE",   107323,  "07-19-14",  "10:29p");
 laptop_addcommand(0, rom_commlink,"COMMLINK",  "COM",    27094,  "01-21-14",  " 2:36p");
 laptop_addcommand(0, rom_tetris,  "TETRIS",    "EXE",    63294,  "03-18-13",  " 7:14a");
 laptop_addcommand(0, rom_appraise,"APPRAISE",  "EXE",    72036,  "04-13-14",  " 9:15p");
 laptop_addcommand(0, rom_tcode,   "TCODE",     "EXE",   102997,  "04-26-14",  " 6:39a");
 laptop_addcommand(0, rom_history, "HISTORY",   "COM",    56024,  "04-23-14",  " 7:04p");
 laptop_addcommand(0, rom_windows, "WINDOWS",   "EXE", 69045234,  "10-02-13",  " 2:29p");
 laptop_addcommand(0, rom_fortinv, "FORTINV",   "EXE",    10324,  "05-23-14",  " 6:19a");
 laptop_addcommand(0, rom_bartalk, "BARTALK",   "COM",    25329,  "01-19-14",  " 2:22p");
 laptop_addcommand(0, rom_spweap,  "SPWEAP",    "COM",    19234,  "03-22-14",  "11:59p");
 laptop_addcommand(0, rom_weapsmth,"WEAPSMTH",  "EXE",    10926,  "07-31-14",  " 1:14p");
 laptop_addcommand(1, rom_ver,     "VER",       "COM",     1936,  "04-27-12",  "10:04p");
 laptop_addcommand(1, rom_help,    "HELP",      "COM",    22349,  "04-27-12",  "10:04p");
 laptop_addcommand(0, rom_samlist, "SAMLIST",   "EXE",    28726,  "07-16-15",  "11:26a");
 laptop_addcommand(0, rom_slothack,"SLOTHACK",  "EXE",     7860,  "04-11-15",  " 5:26a");
 laptop_addcommand(0, rom_ping,    "PING",      "EXE",    19732,  "06-22-14",  " 6:12p");
 laptop_addcommand(0, rom_antiping,"ANTIPING",  "EXE",     8146,  "06-22-14",  " 6:13p");
 laptop_addcommand(1, rom_del,     "DEL",       "COM",     9064,  "04-27-12",  "10:04p");
 laptop_addcommand(0, rom_fortspy, "FORTSPY",   "EXE",    19624,  "03-19-14",  " 3:11p");
 laptop_addcommand(0, rom_minescan,"MINESCAN",  "EXE",    85327,  "08-24-13",  " 7:24a");
 laptop_addcommand(0, rom_macro,   "MACRO",     "COM",    26119,  "06-28-12",  " 6:11a");
 laptop_addcommand(1, directory,   "DOS",       "",           0,  "04-27-12",  "10:04p");
 laptop_sort(0,numlaptop-1);
end laptop_init;

procedure LapTop_Chkdsk;
const
 laptopsize=79929344;
var
 a: integer;
 filessize: integer;
 numfiles: integer;
begin;
 filessize:=0;
 numfiles:=0;
 for a:=0 to numlaptop-1 do
  if (lapcmds[a].mask<>directory) & (testcap(user^.laptop,lapcmds[a].mask)) then
   inc(numfiles);
   filessize:=filessize+lapcmds[a].cmdsize;
  endif;
 endfor;
 writeln;
 writeln("&7Volume GS-224 created Fed 11, 2012 5:34p");
 writeln;
 writeln(rjust(itoa(laptopsize),9," ")," bytes total disk space");
 writeln(rjust("0",9," ")," bytes in 0 hidden files");
 writeln(rjust(itoa(filessize),9," ")," bytes in ",numfiles," user files");
 writeln(rjust(itoa(laptopsize-filessize),9," ")," bytes available on disk");
 writeln;
 writeln(rjust(itoa(655360),9," ")," bytes total memory");
 writeln(rjust(itoa(604290),9," ")," bytes free");
end laptop_chkdsk;

procedure LapTop_Dir(s: string);
const
 laptopsize=79929344;
var
 filessize: integer;
 a,dirnum,numfiles: integer;
begin;
 writeln;
 writeln("&7 Volume in drive C is GS-224");
 if s="" then
  writeln(" Directory of C:\\");
  dirnum:=0;
 else
  dirnum:=0;
  for a:=0 to numlaptop-1 do
   if (lapcmds[a].mask=directory) & (lapcmds[a].name=s) then
    dirnum:=lapcmds[a].dir;
   endif;
  endfor;
  if dirnum=0 then
   writeln("Invalid directory");
   return;
  else
   writeln(" Directory of C:\\",s);
  endif;
 endif;

 writeln;
 filessize:=0;
 numfiles:=0;

 if dirnum=0 then
  for a:=0 to numlaptop-1 do
   if (lapcmds[a].mask=directory) then
     writeln(ljust(lapcmds[a].name,9," "),
             ljust(lapcmds[a].ext,3," "),
             " <DIR>    ",
             lapcmds[a].date,"   ",
             lapcmds[a].time);
     inc(numfiles);
   endif;
  endfor;
 endif;

 for a:=0 to numlaptop-1 do
  if (lapcmds[a].dir=dirnum) & (lapcmds[a].mask<>directory) &
   testcap(user^.laptop,lapcmds[a].mask) then
    writeln(ljust(lapcmds[a].name,9," "),
            ljust(lapcmds[a].ext,3," "),
            rjust(itoa(lapcmds[a].cmdsize),9," "),
            " ",
            lapcmds[a].date,"   ",
            lapcmds[a].time);
    inc(numfiles);
    filessize:=filessize+lapcmds[a].cmdsize;
  endif;
 endfor;
 writeln(rjust(itoa(numfiles),9," ")," File(s)",
         rjust(itoa(laptopsize-filessize),10," ")," bytes free");
end laptop_dir;

procedure laptop_split(s: string; var c,p: string);
const
 foo=1;
var
 a: integer;
 next: boolean;
begin;
 c:="";
 p:="";
 next:=false;
 for a:=1 to length(s) do
  if (s[a]=' ') or (s[a]='/') then
   if c<>"" then next:=true; endif;
  else
   if next then
    p[0]:=p[0]+chr(foo);
    p[ord(p[0])]:=s[a];
   else
    c[0]:=c[0]+chr(foo);
    c[ord(c[0])]:=s[a];
   endif;
  endif;
 endfor;
end laptop_split;

procedure laptop_lookup(s: string; var loop1: integer): boolean;
begin;
 for loop1:=0 to numlaptop-1 do
  if s=lapcmds[loop1].name then
   return true;
  endif;
 endfor;
 return false;
end laptop_lookup;

procedure laptop_slothack;
begin;
 writeln("&3SLACKHACK (c) Joe Hacker");
 writeln;
 writeln("&7I wrot 'diz hack ta fix de slot mach's in da cas'no. Us Hackerz rulez");
 writeln("da world. When 'diz cheat be act'vated, de slot be spinning 'till it");
 writeln("winz sometin. Way c00l d00d!");
 writeln;
 putparam(appuser(usernum^),"slothack",
          checkyesno("Engage SLOTHACK (&FY&7/&FN&7) ? "));
end laptop_slothack;

type
 tpingrecord = record
                unum: byte;
                dist: byte;
               end;
 tpingarray = array maxusers of tpingrecord;

procedure ping_sort(l,r: integer; var p: tpingarray);
var
  i,j: integer;
  x: integer;
  y: tpingrecord;
begin
  i:=l; j:=r; x:=p[(l+r) div 2].dist;
  repeat
    while p[i].dist<x do i:=i+1; endwhile;
    while x<p[j].dist do j:=j-1; endwhile;
    if i<=j then
      y:=p[i];
      p[i]:=p[j];
      p[j]:=y;
      inc(i);
      dec(j);
    endif;
  until i>j;
  if l<j then ping_sort(l,j,p); endif;
  if i<r then ping_sort(i,r,p); endif;
end ping_sort;

procedure laptop_ping;
var
 p: tpingarray;
 u: usertype;
 numping,numantiping: integer;
 a: integer;
 b: pbaserec;
begin;
 writeln("&3Ping: Laptop Location Utility.");
 writeln;
 writeln("&7This utility will locate any persons who posess GS-224 laptops within the");
 writeln("current map level.");
 writeln;
 write("&2Standby... Scanning...");
 numping:=0;
 numantiping:=0;
 for a:=1 to maxusers do
  loaduser(a,u);
  if (u.alive) & (useronmap(u,user^.z)) & (a<>usernum^) & (u.laptop.num<>0) then
   if checklaptop(u,rom_antiping) then
    inc(numantiping);
   else
    p[numping].unum:=a;
    if (u.z=255) then (* check if user is inside of fortress *)
     b:=getbase(u.x);
     if checkbasebgen(b^,flagbgencloak) then
      p[numping].dist:=255;  (* cloaked fortress; do not report real dist *)
     else
      p[numping].dist:=sqrt(cdist(b^.x,b^.y,user^.x,user^.y));
     endif;
    else
     p[numping].dist:=sqrt(cdist(u.x,u.y,user^.x,user^.y));
    endif;
    inc(numping);
   end;
  endif;
 endfor;
 writeln;
 if numping=0 then
  writeln("&C 0 users detected.");
 else
  ping_sort(0,numping-1,p);
  for a:=0 to numping-1 do
   loaduser(p[a].unum,u);
   if p[a].dist=255 then
    writeln("&7 ",namestr(u.alias)," &E[Distance indeterminant. Probable cloaked fortress.]");
   else
    writeln("&7 ",namestr(u.alias)," (dist= ",p[a].dist,")");
   endif;
  endfor;
 endif;
 if numantiping>0 then
  writeln;
  writeln("&C[Note: Excessive signal masking detected. Probable existance of laptops using");
  writeln("ANTIPING software.]");
 endif;
end laptop_ping;

procedure laptop_antiping;
begin;
 writeln("&3AntiPing: Laptop Location Masker");
 writeln;
 writeln("&7This utility will hide the location of this laptop from any scans that are");
 writeln("perfromed with the \"ping\" utility.");
end laptop_antiping;

procedure laptop_delete(var d: devicetype; s: string);
var
 a,b,command_num: integer;
begin;
 if s="" then
  writeln("&CProper syntax is \"DEL <filename>\" where <filename> is the name to delete.");
  writeln("Example: DEL FORTLIST");
  return;
 endif;

 for a:=length(s) downto 1 do
  if s[a]='.' then
   b:=a-1;
   s[0]:=chr(b);
  endif;
 endfor;

 if ~laptop_lookup(s,command_num) then
  writeln("&7File not found");
  return;
 endif;

 with l:lapcmds[command_num] do
  if (l.mask=directory) or (l.mask=rom_dir) then
   writeln("&CYou cannot delete basic laptop commands.");
  else
   if checkyesno("Are you sure (&FY&7/&FN&7) ? ") then
    resetcap(d,l.mask);
   endif;
  endif;
 endwith;
end laptop_delete;

procedure Laptop_transfer(var d: devicetype);
var
 s: string;
 a,b: integer;
 done,c: boolean;
begin;
 done:=false;
 repeat;
  writeln;
  write("&BCopy files from which laptop (&F?&B=List,&FQ&B=Quit) ? &F");
  s:=upstr(readln());
  a:=atoi(s);
  if (s="?") or (s="L") then
   ListUserInv(user^,false);
  elsif s="Q" then
   done:=true;
  elsif (a>0) & (a<=numudev) then
   dec(a);
   if user^.devices[a].num=0 then
    writeln("&CCopy what?");
   elsif user^.devices[a].devapp<>devapp_laptop then
    writeln("&CYou may only copy from another laptop!");
   else
    c:=false;
    for b:=0 to numlaptop-1 do
     if testcap(user^.devices[a],lapcmds[b].mask) & (~testcap(d,lapcmds[b].mask)) then
      writeln("&F",lapcmds[b].name,".",lapcmds[b].ext,"&B transferred to laptop root directory.");
      c:=true;
      resetcap(user^.devices[a],lapcmds[b].mask);
      setcap(d,lapcmds[b].mask);
     endif;
    endfor;
    if ~c then
     writeln("&CNo files worth transferring were located.");
    endif;
   endif;
  endif;
 until done;
end laptop_transfer;

procedure laptop_fortspy;
var
 counter,fortnum,objnum,playernum: integer;
 d: devicetype;
 u: usertype;
 ch: char;
 s: string;
begin;
 fortnum:=enumforts(0,user^.z,user^.x,user^.y);
 if fortnum=0 then
  writeln("&CThere is no fortress at the present location.");
 else
  writeln("&3Objects in fortress storage room:");
  write("&7");
  objnum:=enumobjects(0,255,fortnum,0);
  counter:=0;
  while objnum<>0 do
   if (counter>0) & ((counter mod 4)=0) then writeln; endif;
   inc(counter);
   peekobject(objnum,d);
   s:=devname(d);
   if length(s)>18 then s[0]:=12X; endif;
   write(" ",ljust(s,18," "));
   objnum:=enumobjects(objnum,255,fortnum,0);
  endwhile;
  if counter=0 then writeln(" &CThere are no objects in the storage room."); endif;
  if (counter>0) then writeln; endif;
  writeln;
  writeln("&3Occupants of fortress:");
  write("&7");
  playernum:=enumplayers(0,255,fortnum,0);
  counter:=0;
  while playernum<>0 do
   loaduser(playernum,u);
   if u.alive then
    inc(counter);
    writeln(" ",namestr(u.alias));
   endif;
   playernum:=enumplayers(playernum,255,fortnum,0);
  endwhile;
  if counter=0 then writeln(" &CThere are no players inside the fortress."); endif;
 endif;
end laptop_fortspy;

procedure do_minescan(direct: string; z,x,y: integer; var hits: integer);
var
 objnum: integer;
 d: devicetype;
begin;
 objnum:=enumobjects(0,z,x,y);
 while objnum<>0 do
  peekobject(objnum,d);
  if (d.num<>0) & (d.devapp=devapp_mine) then
   inc(hits);
   write("&3",ljust(direct,10," "),
         "&E",ljust(devicedef^[d.num]^.name,20," "),
         "&F",rjust(itoa(d.useleft),8," "));

   if (d.var2=0) then
    writeln(" &C<Unarmed>");
   elsif testcap(d,flagminesmart) & (d.var2>0) & (d.var2<=maxusers) then
    writeln(" &2",getusername(d.var2));
   else
    writeln(" &2n/a");
   endif;
  endif;
  objnum:=enumobjects(objnum,z,x,y);
 endwhile;
end do_minescan;

procedure laptop_minescan;
var
 hits: integer;
 maxx,maxy: integer;
begin;
 writeln("&9Direction Type                 Charges Owner");
 writeln("--------- -------------------- ------- --------------------");

 hits:=0;

 do_minescan("Current",user^.z,user^.x,user^.y,hits);

 querymapsize(user^.x,maxx,maxy);

 if user^.x>1 then
  do_minescan("West",user^.z,user^.x-1,user^.y,hits);
 endif;

 if user^.x<maxx then
  do_minescan("East",user^.z,user^.x+1,user^.y,hits);
 endif;

 if user^.y>1 then
  do_minescan("North",user^.z,user^.x,user^.y-1,hits);
 endif;

 if user^.y<maxy then
  do_minescan("South",user^.z,user^.x,user^.y+1,hits);
 endif;

 if hits=0 then
  writeln("&CNo mines detected within 1-unit radius.");
 endif;
end laptop_minescan;


