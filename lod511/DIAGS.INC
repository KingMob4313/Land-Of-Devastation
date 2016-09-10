procedure diags_itemval;
var
 dev: devicetype;
 loop1: integer;
 cost_min,cost_max: integer;
 cost_min_num,cost_max_num: integer;
begin;
 cost_min:=largest(integer);
 cost_max:=0;
 writeln("&3Scanning devicedef:");
 for loop1:=0 to numdev do
  dev:=createdevice(loop1);
  if itemval(dev)<>devicedef^[loop1]^.cost then
   write(" &CError in dev #&F",loop1,"&C: name = &F",devicedef^[loop1]^.name);
   writeln("&C, c = &F",devicedef^[loop1]^.cost,"&C, iv = &F",itemval(dev));
  endif;
  with d:devicedef^[loop1]^ do
   if (d.cost<>0) & (d.cost<cost_min) then
    cost_min:=d.cost;
    cost_min_num:=loop1;
   endif;
   if d.cost>cost_max then
    cost_max:=d.cost;
    cost_max_num:=loop1;
   endif;
  endwith;
 endfor;
 writeln(" &2Minimum (non-zero) cost: &F$",cost_min,"&2 (",
         devicedef^[cost_min_num]^.name," #",cost_min_num,")");
 writeln(" &2Maximum cost: &F$",cost_max,"&2 (",
         devicedef^[cost_max_num]^.name," #",cost_max_num,")");
end diags_itemval;

procedure diags_bigpic;
var
 s: string;
begin;
 write("&3Bigpic name: &F");
 s:=upstr(readln());
 if s<>"" then
  gtermaddbigpic(s);
  waitkey;
  gtermdecbigpic;
 endif;
end diags_bigpic;

procedure diags_move;
var
 x,y,z,xs,ys: integer;
begin;
 write("&2Z coordinate: &F");
 z:=atoi(readln());
 if (z<1) or (z>querylodint(querylod_maxmaps)) then
  writeln("&CZ coordinate out of range.");
  return;
 endif;
 querymapsize(z,xs,ys);
 write("&2X coordinate: &F");
 x:=atoi(readln());
 if (x<1) or (x>xs) then
  writeln("&CX coordinate out of range.");
  return;
 endif;
 write("&2Y coordinate: &F");
 y:=atoi(readln());
 if (y<1) or (y>ys) then
  writeln("&CY coordinate out of range.");
  return;
 endif;
 user^.z:=z;
 user^.x:=x;
 user^.y:=y;
end diags_move;

procedure diags_showstring;
var
 n: integer;
begin;
 write("&2String number: &F");
 n:=atoi(readln());
 if (n<0) or (n>querylodint(querylod_numstrdef)) then
  writeln("&CInvalid");
  return;
 end;
 outstr(n);
end diags_showstring;

procedure diags_giveitem;
var
 devnum,freeinv: integer;
 s: string;
begin;
 if ~findfreeinv(user^,freeinv) then
  writeln("&CYou have no free inventory slot available.");
  return;
 end;
 write("&3Item name: &F");
 s:=upstr(readln());
 devnum:=0;
 if s<>"" then
  devnum:=ckdevstr(s);
  if devnum=0 then
   writeln("&CUnable to find a device by that name.");
  else
   user^.devices[freeinv]:=createdevice(devnum);
  endif;
 endif;
end diags_giveitem;

procedure diags_memory;
begin;
 writeln("&3String defintions..: &F",querylodint(querylod_strdefbytes),
         "&3 bytes (&F",querylodint(querylod_numstrdef),
         "&3 strings)");
 writeln("&3Map Array..........: &F",querylodint(querylod_mapbytes),
         "&3 bytes (max size &F",querylodint(querylod_maxmapx),
         "&3x&F",querylodint(querylod_maxmapy),
         ")");
 writeln("&3Wastelands objects.: &F",querylodint(querylod_objbytes),
         "&3 bytes (&F",querylodint(querylod_numolist)," &3objects)");
 writeln("&3Monster index......: &F",querylodint(querylod_monindexbytes),
         "&3 bytes (&F",querylodint(querylod_nummonindex),
         "&3 monsters)");
 writeln("&3Combat data (pchar): &F",querylodint(querylod_sizeofpchar),
         "&3 bytes");
 writeln("&3Combat data (mchar): &F",querylodint(querylod_sizeofmchar),
         "&3 bytes");
 writeln("&3Overlay buffer size: &F",querylodint(querylod_ovrbufsize),
         "&3 bytes");
 writeln("&3Memavail/Maxavail..: &F",querylodint(querylod_memavail),
         "&3/&F",querylodint(querylod_maxavail));
 writeln("&3Devgood/fill/nil...: &F",querylodint(querylod_devgood),
         "&3/&F",querylodint(querylod_devfill),
         "&3/&F",querylodint(querylod_devnil));
 writeln("&3Heap used..........: &F",querylodint(querylod_heapused),
         "&3 bytes");
 writeln("&3EMS Memory used....: &F",querylodint(querylod_emsused),
         "&3 bytes");
end diags_memory;

procedure diags_hunt;
var
 loop1,found: integer;
 s: string;
begin;
 found:=0;
 write("&3Monster name: &F");
 s:=upstr(readln());
 for loop1:=1 to querylodint(querylod_nummonindex) do
  if found=0 then
   loadcombatdata(1,loop1);
   if submatch(s,mchar_player^.alias) then
    found:=loop1;
    writeln("&EHunt set for ",namestr(mchar_player^.alias)," (#",found,")");
   endif;
  endif;
 endfor;
 if found=0 then
  writeln("&CMonster not found; hunt reset.");
 endif;
 sethunt(found);
end diags_hunt;

procedure diags_showmenu;
begin;
 writeln;
 writeln("&3[&FA&3] Ansilib statistics");
 writeln("&3[&FB&3] Display bigpic");
 writeln("&3[&FE&3] Memory statistics");
 writeln("&3[&FG&3] Give item");
 writeln("&3[&FH&3] Hunt monster");
 writeln("&3[&FI&3] Itemval scan");
 writeln("&3[&FL&3] LGPL Stats");
 writeln("&3[&FM&3] Move to new location");
 writeln("&3[&FS&3] Show String");
 writeln("&3[&FV&3] SVGA statistics");
 writeln("&3[&FQ&3] Quit");
end diags_showmenu;

export procedure diags_menu;
var
 done: boolean;
begin;
 diags_showmenu;
 done:=false;
 repeat;
  writeln;
  write("&BDiagnostics option (&FQ&B=Quit,&F?&B=List) ? ");
  switch upcase(readkey()) of
   case 'Q': writeln("&FQuit.");
             resetneedpause;
             done:=true;
             break;

   case 'A': writeln("Ansilib.");
             ansistats;
             break;

   case 'B': writeln("&FBigpic.");
             diags_bigpic;
             break;

   case 'E': writeln("&FMemory Stats.");
             diags_memory;
             break;

   case 'G': writeln("&FGive item.");
             diags_giveitem;
             break;

   case 'H': writeln("&FHunt.");
             diags_hunt;
             break;

   case '?': writeln("&FList Menu.");
             diags_showmenu;
             break;

   case 'L': writeln("&FLGPL Stats.");
             lgplstats;
             break;

   case 'M': writeln("&FMove.");
             diags_move;
             break;

   case 'S': writeln("&FShow-String.");
             diags_showstring;
             break;

   case 'V': writeln("&FSVGA Stats");
             svgastats;
             break;

   case 'I': writeln("&FItemval scan.");
             diags_itemval;
             break;
  end;
 until done;
end diags_menu;

