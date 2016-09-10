procedure VortexAddAll(var o: devicetype);
var
 vortmax,loop1: integer;
 found_ammo: boolean;
begin;
 vortmax:=querylodint(querylod_vortexbattmax);
 found_ammo:=false;
 for loop1:=0 to numudev-1 do
  with ud:user^.devices[loop1] do
   if (ud.num<>0) & (ud.devapp=devapp_ammo) & (ud.var1<vbatt_numammo()) &
      (vbatt_ammoeff(ud.var1)<>0) & (o.var2<vortmax) then
    writeln("&EYou discharge the &F",devname(ud),"&E into your &F",
            devname(o),"&E.");
    o.var2:=min(vortmax,o.var2+(ud.var2*vbatt_ammoeff(ud.var1)));
    nukedevice(ud);
    found_ammo:=true;
   endif;
  endwith;
 endfor;

 if ~found_ammo then
  writeln("&CNo usable ammo found in inventory.");
 endif;

 if o.var2=vortmax then
  writeln("&CYour &F",devname(o)," &Cis full.");
 endif;
end vortexaddall;

procedure DoVortexAdd(var o: devicetype);
var
 s: string;
 a,b: integer;
 done: boolean;
begin;
 done:=false;
 repeat;
  writeln;
  write("&BInsert which ammo cartridge into the cell (&F?&B=List,&FALL&B=All) ? &F");
  s:=upstr(readln());
  a:=atoi(s);
  if (s="?") or (s="L") then
    ListUserInv(user^,false);
  elsif (s="ALL") then
   vortexaddall(o);
  elsif (a>0) & (a<=numudev) then
   dec(a);   (* because inventory is a zero-based array *)
   if user^.devices[a].num=0 then
    writeln("&CInsert what???");
   elsif user^.devices[a].devapp<>devapp_ammo then
    writeln("&CYou may only insert an ammo cartridge!");
   elsif (user^.devices[a].var1>vbatt_numammo()) then
    writeln("&CThe cell will not accept that energy type!");
   elsif (vbatt_ammoeff(user^.devices[a].var1)=0) then
    writeln("&CThe cell will not accept that energy type!");
   else
    writeln("&CEnergy transferred to cell.");
    o.var2:=o.var2+user^.devices[a].var2*vbatt_ammoeff(user^.devices[a].var1);
    nukedevice(user^.devices[a]);
    if o.var2>querylodint(querylod_vortexbattmax) then
     o.var2:=querylodint(querylod_vortexbattmax);
     writeln("&CMaximum cell capacity reached.");
    endif;
   endif;
  endif;
  if (s="") or (s="Q") then
    done:=true;
  endif;
 until done;
end dovortexadd;

procedure DoVortexDischarge(var o: devicetype);
var
 result: boolean;
begin;
 result:=checkyesno("&CWARNING! Discharge will empty cell! Continue (&FY&C/&FN&C) ? ");
 if result then
  writeln("&BCell discharged.");
  o.var2:=0;
 endif;
end dovortexdischarge;

procedure DoVortexView(var o: devicetype);
var
 a: integer;
begin;
 writeln;
 writeln("&BEnergy Cell Status:");
 writeln("&7    Vortex Energy units...............: &F",o.var2);
 for a:=1 to vbatt_numammo() do
  writeln("&7    Supplyable ",rpad(vbatt_ammoname(a),23,"."),": &F",
	ftoi(cond(vbatt_ammoeff(a)=0,0,o.var2/vbatt_ammoeff(a)*o.var1/100)));
 endfor;
end dovortexview;

procedure use_vortex(var o: devicetype);
var
 done: boolean;
begin;
 done:=false;
 repeat;
  write("&3Cell: &FA&3]dd, &FD&3]ischarge, &FV&3]iew, &FQ&3]uit ? &F");

  switch upcase(readkey()) of
    case 'A': writeln("Add.");
	      DoVortexAdd(o);
              writeln;
	      break;

    case 'D': writeln("Discharge.");
	      DoVortexDischarge(o);
              writeln;
	      break;

    case 'V': writeln("View.");
	      DoVortexView(o);
              writeln;
	      break;

    default:  writeln("Quit.");
	      done:=true;
              resetneedpause;
	      break;
  end;
 until done;
end use_vortex;