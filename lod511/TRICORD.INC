const
 scan_human=1;
 scan_object=2;
 scan_struct=3;
 scan_objdet=4;
 scan_objkind=5;
 scan_dna=6;
 tricord_default_objthresh = 25;

var
 tricord_objthresh: integer;

 tricord_objsamp: string[32];

 tricord_objkind: string[32];
 tricord_objapp: byte;

 tricord_dnauser,tricord_dnacode: word;

 tridna_x, tridna_y: byte;

procedure trihit_human(z,x,y: integer): boolean;
var
 unum: integer;
 count: integer;
begin;
 count:=0;
 unum:=enumplayers(0,z,x,y);
 while (unum<>0) do
  inc(count);
  unum:=enumplayers(unum,z,x,y);
 endwhile;
 return (count>0);
end trihit_human;

procedure trihit_object(z,x,y: integer): boolean;
var
 obj: integer;
 count: integer;
begin;
 count:=0;
 obj:=enumobjects(0,z,x,y);
 while (obj<>0) do
  inc(count);
  obj:=enumobjects(obj,z,x,y);
 endwhile;
 return (count>=tricord_objthresh);
end trihit_object;

procedure trihit_struct(z,x,y: integer): boolean;
var
 maptile: word;
 ter: terraintype;
begin;
 maptile:=getmap(z,x,y);
 getterrain(maptile,ter);
 if (ter.ent<>eNone) or (enumforts(0,z,x,y)<>0) then
  return TRUE;
 endif;
 return FALSE;
end trihit_struct;

procedure trihit_objdet(z,x,y: integer): boolean;
var
 obj: integer;
 d: devicetype;
begin;
 obj:=enumobjects(0,z,x,y);
 while (obj<>0) do
  peekobject(obj,d);
  if devicedef^[d.num]^.name=tricord_objsamp then
   return TRUE;
  endif;
  obj:=enumobjects(obj,z,x,y);
 endwhile;
 return FALSE;
end trihit_objdet;

procedure trihit_objkind(z,x,y: integer): boolean;
var
 obj: integer;
 d: devicetype;
begin;
 obj:=enumobjects(0,z,x,y);
 while (obj<>0) do
  peekobject(obj,d);
  if d.devapp=tricord_objapp then
   return TRUE;
  endif;
  obj:=enumobjects(obj,z,x,y);
 endwhile;
 return FALSE;
end trihit_objkind;

procedure trihit_dna(z,x,y: integer): boolean;
begin;
 return (x=tridna_x) & (y=tridna_y);
end trihit_dna;

procedure triscan_header(todo: integer);
var
 xloop,midpoint: integer;
begin;
 midpoint:=(todo div 2)+1;
 write("&2É");
 for xloop:=1 to todo do
  if xloop=midpoint then
   write("Ñ");
  else
   write("Í");
  endif
 endfor;
 writeln("»");
end triscan_header;

procedure triscan_footer(todo: integer);
var
 xloop,midpoint: integer;
begin;
 midpoint:=(todo div 2)+1;
 write("&2È");
 for xloop:=1 to todo do
  if xloop=midpoint then
   write("Ï");
  else
   write("Í");
  endif
 endfor;
 writeln("¼");
end triscan_footer;

procedure triscan(z,x,y,scan_size,scan_type: integer);
var
 xloop: integer;
 yloop: integer;
 todo: integer;
 maxx,maxy: integer;
 blankstr: string;
begin;
 querymapsize(z,maxx,maxy);
 todo:=scan_size div 2;
 triscan_header(todo*2+1);
 for yloop:=y-todo to y+todo do
  if (yloop=y) then
   write("&2Ç");
  else
   write("&2º");
  endif;
  for xloop:=x-todo to x+todo do
   if (yloop<0) or (xloop<0) or (y>maxy) or (x>maxx) then
    write(" ");
   elsif (yloop=y) & (xloop=x) then
    write("&Eþ");
   elsif ((scan_type=scan_human) & trihit_human(z,xloop,yloop)) or
         ((scan_type=scan_object) & trihit_object(z,xloop,yloop)) or
         ((scan_type=scan_objdet) & trihit_objdet(z,xloop,yloop)) or
         ((scan_type=scan_struct) & trihit_struct(z,xloop,yloop)) or
         ((scan_type=scan_objkind) & trihit_objkind(z,xloop,yloop)) or
         ((scan_type=scan_dna) & trihit_dna(z,xloop,yloop)) then
    write("&Cþ");
   else
    write(" ");
   endif;
  endfor;
  if (yloop=y) then
   writeln("&2¶");
  else
   writeln("&2º");
  endif;
 endfor;
 triscan_footer(todo*2+1);
end triscan;

procedure dnasamplename(dnauser,dnacode: word): string;
var
 u: usertype;
begin;
 if (dnauser<1) or (dnauser>maxusers) then
  return strcat("&C<invalid>","");
 else
  loaduser(dnauser,u);
  if u.dnacode<>dnacode then
   return "&F"+namestr(u.alias)+" &C(expired)";
  else
   return "&F"+namestr(u.alias);
  endif;
 endif;
end dnasamplename;

procedure tricorder_display_cfgmenu;
begin;
 writeln("&3[&FO&3] Object density scan threshold: &F",
         tricord_objthresh,"&3.");
 writeln("&3[&FD&3] Sample for specific object detection scan: &F",
         tricord_objsamp,"&3.");
 writeln("&3[&FK&3] Sample for generic object kind scan: &F",
         tricord_objkind,"&3.");
 writeln("&3[&FN&3] Sample for DNA scan: ",
         dnasamplename(tricord_dnauser,tricord_dnacode),"&3.");
end tricorder_display_cfgmenu;

procedure tricorder_newsample(generic: boolean);
var
 done: boolean;
 s: string;
 i: integer;
begin;
 done:=false;
 listuserinv(user^,false);
 repeat;
  writeln;
  write("&BSample which object from your inventory (&F?&B=List,&FQ&B=Quit) &F");
  s:=upstr(readln());
  i:=atoi(s);
  if (s="?") then
   listuserinv(user^,false);
  elsif (s="Q") or (s="") then
   done:=true;
  elsif (i>0) & (i<=numudev) then
   with ud:user^.devices[i-1] do
    if ud.num=0 then
     writeln("&CThat device slot is empty!");
    else
     if generic then
      tricord_objkind:=devicedef^[ud.num]^.name;
      tricord_objapp:=ud.devapp;
      writeln("&3Sample stored for &F",tricord_objkind,"&3.");
      putparam(appuser(usernum^),"tricord_objkind",tricord_objapp,tricord_objkind);
     else
      tricord_objsamp:=devicedef^[ud.num]^.name;
      writeln("&3Sample stored for &F",tricord_objsamp,"&3.");
      putparam(appuser(usernum^),"tricord_objsamp",tricord_objsamp);
     endif;
     done:=true;
    endif;
   endwith;
  else
   writeln("&CInvalid option.");
  endif;
 until done;
end tricorder_newsample;

procedure tricorder_listusers;
var
 unum: integer;
 counter: integer;
begin;
 counter:=0;
 unum:=enumplayers(0,user^.z,user^.x,user^.y);
 while unum<>0 do
  if (unum<>usernum^) then
   inc(counter);
   writeln("&F",counter,"&3] ",getusername(unum));
  endif;
  unum:=enumplayers(unum,user^.z,user^.x,user^.y);
 endwhile;
end tricorder_listusers;

procedure tricorder_loaddnasample(i: integer): boolean;
var
 unum: integer;
 u: usertype;
 counter: integer;
begin;
 counter:=0;
 unum:=enumplayers(0,user^.z,user^.x,user^.y);
 while unum<>0 do
  if (unum<>usernum^) then
   inc(counter);
   if counter=i then
    loaduser(unum,u);
    writeln("&3DNA Sample for &F",namestr(u.alias)," &3loaded.");
    tricord_dnauser:=unum;
    tricord_dnacode:=u.dnacode;
    putparam(appuser(usernum^),"tricord_dnasamp",tricord_dnauser,tricord_dnacode);
    return TRUE;
   endif;
  endif;
  unum:=enumplayers(unum,user^.z,user^.x,user^.y);
 endwhile;
 writeln("&CInvalid selection.");
 return FALSE;
end tricorder_loaddnasample;

procedure tricorder_newdnasample;
var
 s: string;
 done: boolean;
 i: integer;
begin;
 tricorder_listusers;
 done:=false;
 repeat;
  writeln;
  write("&BSample which camped player at current location (&F?&B=List,&FQ&B=Quit) ? &F");
  s:=upstr(readln());
  i:=atoi(s);
  if (s="Q") then
   done:=true;
  elsif (s="?") or (s="L") then
   writeln;
   tricorder_listusers;
  elsif (i>0) then
   done:=tricorder_loaddnasample(i);
  endif;
 until done;
end tricorder_newdnasample;

procedure tricorder_config;
var
 done: boolean;
begin;
 tricorder_display_cfgmenu;
 done:=false;
 repeat;
  writeln;
  write("&BConfiguration option (&F?&B=List,&FQ&B=Quit) ? &F");

  switch upcase(readkey()) of
    case '?': writeln("List");
              writeln;
              tricorder_display_cfgmenu;
              break;

    case 'O': writeln("Obj-Density Thresh");
              writeln;
              writeln("&3The object density scan is designed to register when the number of objects");
              writeln("at a location meets or exceeds the threshold value.");
              writeln;
              write("&3New threshold value: &F");
              tricord_objthresh:=atoi(readln());
              putparam(appuser(usernum^),"tricord_objthresh",tricord_objthresh);
              break;

    case 'D': writeln("Obj-Detector Sample");
              writeln;
              writeln("&3The object detection scan can be programmed to search for any object that");
              writeln("you can provide a sample of. Samples may be obtained from your inventory.");
              writeln;
              tricorder_newsample(FALSE);
              break;

    case 'K': writeln("Obj-Kind Sample");
              writeln;
              writeln("&3The generic object kind scan will search for any objects that match the");
              writeln("basic attributes of a sample. For example, if the sample were a weapon, then");
              writeln("the scan would register other weapons. Samples may be obtained from your");
              writeln("inventory.");
              writeln;
              tricorder_newsample(TRUE);
              break;

    case 'N': writeln("Load new DNA Sample");
              writeln;
              tricorder_newdnasample;
              break;

    case 0DX:
    case 'Q': writeln("Quit");
              done:=true;
              break;

    default:  writeln("&CWhat????");
              break;
  end;
 until done;
end tricorder_config;

procedure tricorder_load_parameters;
begin;
 if verifyparam(appuser(usernum^),"tricord_objthresh") then
  getparam(appuser(usernum^),"tricord_objthresh",tricord_objthresh);
 else
  tricord_objthresh:=tricord_default_objthresh;
 endif;
 if verifyparam(appuser(usernum^),"tricord_objsamp") then
  getparam(appuser(usernum^),"tricord_objsamp",tricord_objsamp);
 else
  tricord_objsamp:="(not set)";
 endif;
 if verifyparam(appuser(usernum^),"tricord_objkind") then
  getparam(appuser(usernum^),"tricord_objkind",tricord_objapp,tricord_objkind);
 else
  tricord_objkind:="(not set)";
  tricord_objapp:=0;
 endif;
 if verifyparam(appuser(usernum^),"tricord_dnasamp") then
  getparam(appuser(usernum^),"tricord_dnasamp",tricord_dnauser,tricord_dnacode);
 else
  tricord_dnauser:=0;
  tricord_dnacode:=0;
 endif;
end tricorder_load_parameters;

procedure tricorder_display_menu;
begin;
 writeln("&3[&FC&3] Configure Tricorder");
 writeln("&3[&FT&3] Standard terrain scan");
 writeln("&3[&FL&3] Long range sensor scan");
 writeln("&3[&FH&3] Humanoid lifesign scan");
 writeln("&3[&FO&3] Object density scan");
 writeln("&3[&FS&3] Structure/buildings scan");
 writeln("&3[&FD&3] Specific object detection scan");
 writeln("&3[&FK&3] Generic object kind scan");
 writeln("&3[&FN&3] DNA Scan");
end tricorder_display_menu;

procedure tricorder_checkrange(x,y: integer; x1,y1: integer; scansize: integer): boolean;
begin;
 scansize:=scansize div 2;
 return (x1>=x-scansize) & (x1<=x+scansize) & (y1>=y-scansize) & (y1<=y+scansize);
end tricorder_checkrange;

procedure tricorder_dnascan;
const
 dnascan_size = 17;
var
 u: usertype;
begin;
 if (tricord_dnauser<1) or (tricord_dnauser>maxusers) then
  writeln("&CYour DNA sample is invalid.");
 else
  loaduser(tricord_dnauser,u);
  if ~u.alive then
   writeln("&CThe user with this DNA is dead!");
  elsif (u.dnacode<>tricord_dnacode) then
   writeln("&CYour DNA sample has expired. You must obtain a new one.");
  else
   getusercoords(u,tridna_x,tridna_y);
   clrscr;
   if (u.z<>user^.z) then
    writeln("&CWide DNA scan confirms target is not on current map.");
   elsif (tridna_x=user^.x) & (tridna_y=user^.y) then
    writeln("&2DNA Scan has located user in current map square.");
   elsif ~tricorder_checkrange(user^.x,user^.y,tridna_x,tridna_y,dnascan_size) then
    writeln("&CWide DNA scan confirms target presence on current map, but distance is beyond");
    writeln("range limitation of narrow scan. Signal bearing: &F",
            directstring(u.x,u.y,user^.x,user^.y),"&C.");
   else
    writeln("&2Wide DNA scan confirms target presence on current map. Executing narrow scan:");
    writeln;
    triscan(user^.z,user^.x,user^.y,dnascan_size,scan_dna);
   endif;
  endif;
 endif;
end tricorder_dnascan;

procedure use_tricorder;
var
 d: devicetype;
 done: boolean;
begin;
 done:=false;
 tricorder_load_parameters;
 tricorder_display_menu;
 repeat;
  writeln;
  write("&BTricorder option (&F?&B=List,&FQ&B=Quit) ? &F");

  switch upcase(readkey()) of
    case '?': writeln("List");
              writeln;
              tricorder_display_menu;
              break;

    case 'C': writeln("Configure");
              writeln;
              tricorder_config;
              break;

    case 'T': writeln("Terrain Scan");
              writeln;
              nukedevice(d);
              use_scanner(d);
              break;

    case 'L': writeln("LR Scan");
              writeln;
              displr(user^.z,user^.x,user^.y,9);
              break;

    case 'O': clrscr;
              triscan(user^.z,user^.x,user^.y,17,scan_object);
              break;

    case 'K': clrscr;
              writeln("&3Scanning for: &F",tricord_objkind,
                      "&3 (type code = &F",tricord_objapp,
                      "&3).");
              writeln;
              triscan(user^.z,user^.x,user^.y,17,scan_objkind);
              break;

    case 'D': clrscr;
              writeln("&3Scanning for: &F",tricord_objsamp,"&3.");
              writeln;
              triscan(user^.z,user^.x,user^.y,17,scan_objdet);
              break;

    case 'H': clrscr;
              triscan(user^.z,user^.x,user^.y,17,scan_human);
              break;

    case 'S': clrscr;
              triscan(user^.z,user^.x,user^.y,17,scan_struct);
              break;

    case 'N': writeln("DNA Scan");
              writeln;
              tricorder_dnascan;
              break;

    case 0DX:
    case 'Q': writeln("Quit");
              done:=true;
              break;

    default:  writeln("&CWhat????");
              break;
  end;
 until done;
end use_tricorder;

procedure use_dnacontainer(var d: devicetype);
var
 s: string;
 a: integer;
 done: boolean;
begin;
 tricorder_load_parameters;
 done:=false;
 writeln("&2Current sample in container is: &F",dnasamplename(d.var1,d.var2));
 writeln("&2Current sample in tricorder is: &F",dnasamplename(tricord_dnauser,tricord_dnacode));
 repeat
  writeln;
  write("&3Transfer sample &FT&3]o or &FF&3]rom Tricorder (&FQ&3=Quit) ? ");
  switch upcase(readkey()) of
   case 'Q': writeln("&FQuit");
             resetneedpause;
             done:=true;
             break;


   case 'T': writeln("&FTo");
             if (tricord_dnauser=0) or
                (checkyesno("&BTricorder sample &F"+dnasamplename(tricord_dnauser,tricord_dnacode)+
                      "&B will be overwritten. Continue (&FY&B/&FN&B) ? ")) then
              done:=true;
              tricord_dnauser:=d.var1;
              tricord_dnacode:=d.var2;
              putparam(appuser(usernum^),"tricord_dnasamp",tricord_dnauser,tricord_dnacode);
              writeln("&2Tricorder loaded with sample: &F",dnasamplename(tricord_dnauser,tricord_dnacode));
             endif;
             break;

   case 'F': writeln("&FFrom");
             if (d.var1=0) or
                (checkyesno("&BContainer sample &F"+dnasamplename(d.var1,d.var2)+
                            "&B will be overwritten. Continue (&FY&B/&FN&B) ? ")) then
              done:=true;
              d.var1:=tricord_dnauser;
              d.var2:=tricord_dnacode;
              writeln("&2Container loaded with sample: &F",dnasamplename(d.var1,d.var2));
             endif;
             break;

   default: writeln;
            break;
  end;
 until done;
end use_dnacontainer;


