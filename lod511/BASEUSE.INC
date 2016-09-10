const
 maxfunc=10;
 func_ultrascan = 1;
 func_comm = 2;
 func_robo = 3;
 func_vortex = 4;
var
 functions: array maxfunc of integer;
 numfunc: integer;

procedure addfunc(func: integer);
begin;
 if numfunc<maxfunc-1 then
  functions[numfunc]:=func;
  inc(numfunc);
 endif;
end addfunc;

procedure calcfunc(var d: devicetype): integer;
begin;
 numfunc:=0;
 if testcap(d,flagbgenscan) then
  addfunc(func_ultrascan);
 endif;
 if testcap(d,flagbgencomm) then
  addfunc(func_comm);
 endif;
 if testcap(d,flagbgenrobo1) or testcap(d,flagbgenrobo2) or
    testcap(d,flagbgenrobo4) or testcap(d,flagbgenrobo8) then
  addfunc(func_robo);
 endif;
 if testcap(d,flagbgenvortex) then
  addfunc(func_vortex);
 endif;
 return numfunc;
end calcfunc;

procedure showrobodefender(var d: devicetype);
var
 num: word;
 monnum: word;
begin;
 num:=0;
 if testcap(d,flagbgenrobo1) then
  num:=num+1;
 endif;
 if testcap(d,flagbgenrobo2) then
  num:=num+2;
 endif;
 if testcap(d,flagbgenrobo4) then
  num:=num+4;
 end;
 if testcap(d,flagbgenrobo8) then
  num:=num+8;
 end;
 if findmonster(z_bgenrobo,num,monnum) then
  loadcombatdata(1,monnum);
  showchar(mchar_player);
 endif;
end showrobodefender;

procedure base_vortexlink(var base: baserec);
var
 s: string;
 a,b,units: integer;
 done: boolean;
begin;
 done:=false;
 repeat;
  writeln;
  write("&BTransfer base energy with which cell (&F?&B=List,&FQ&B=Quit) ? &F");
  s:=upstr(readln());
  a:=atoi(s);
  if (s="?") or (s="L") then
   ListUserInv(user^,false);
  elsif (s="Q") or (s="") then
   done:=true;
  elsif (a>0) & (a<=numudev) then
   dec(a);   (* because inventory is a zero-based array *)
   with ud:user^.devices[a] do
    if ud.num=0 then
     writeln("&CTransfer what???");
    elsif ud.devapp<>devapp_vortex then
     writeln("&CYou may only transfer with a cell device!");
    else
     writeln("&7Cell has &F",ud.var2,"&7 energy units available.");
     writeln("&7Fortress has &F",base.energy,"&7 energy units available.");
     writeln;
     write("&3Transfer energy &FT&3]o or &FF&3]rom cell ? ");
     switch upcase(readkey()) of
      case 'T': writeln("&FTo.");
                write("&BTo cell: How many units ? &F");
                units:=atoi(readln());
                if units<1 then
                 writeln("&CAborted.");
                elsif units>base.energy then
                 writeln("&CThere is not that much energy available in the fortress!");
                elsif ud.var2+units>querylodint(querylod_vortexbattmax) then
                 writeln("&CThe vortex cell cannot hold that much!");
                else
                 writeln("&7Energy transferred.");
                 ud.var2:=ud.var2+units;
                 base.energy:=base.energy-units;
                 done:=true;
                endif;
                break;

      case 'F': writeln("&FFrom.");
                write("&BFrom cell: How many units ? &F");
                units:=atoi(readln());
                if units<1 then
                 writeln("&CAborted.");
                elsif units>ud.var2 then
                 writeln("&CThere is not that much energy available in the cell!");
                elsif base.energy+units>querylodint(querylod_maxbaseenergy) then
                 writeln("&CThe fortress cannot hold that much!");
                else
                 writeln("&7Energy transferred.");
                 ud.var2:=ud.var2-units;
                 base.energy:=base.energy+units;
                 done:=true;
                endif;
                break;

      default:  writeln;
                break;
     end; (* switch *)
    endif;
   endwith;
  endif;
 until done;
end base_vortexlink;

export procedure base_useitem;
var
 a: integer;
 s: string;
 done: boolean;
 base: pbaserec;
 dnum,choosefunc,basenum: integer;

begin;
 basenum:=querylodint(querylod_gmenubasenum);
 base:=getbase(basenum);
 done:=false;
 repeat;
  writeln;
  write("&BUse which item number (&FQ&B=Quit,&F?&B=List&B) ? &F");
  s:=upstr(readln());
  if (s="L") or (s="?") then
   showbasespec(basenum);
  elsif (s="Q") or (s="") then
   done:=true;
  else
   dnum:=atoi(s);
   if (dnum>0) & (dnum<=nbspec) then
    dec(dnum); (* because of zero based array *)
    if base^.spec[dnum].num=0 then
     writeln("&CUse what?");
    elsif calcfunc(base^.spec[dnum])=0 then
     writeln("&CThis device doesn't do anything!");
    else
     if (numfunc=1) then
      choosefunc:=numfunc;
     else
      choosefunc:=0;
      writeln("&9Available functions:");
      for a:=1 to numfunc do
       write("&F  ",a,"&7> ");
       switch functions[a-1] of
        case func_ultrascan: writeln("ultrascan");
                             break;

        case func_comm:      writeln("commset");
                             break;

        case func_robo:      writeln("view robodefender settings");
                             break;

        case func_vortex:    writeln("vortex cell link");
                             break;
       end;
      endfor;
      writeln;
      write("&BPick a function (&FQ&B=Quit) ? &F");
      choosefunc:=atoi(readln());
      if (choosefunc<1) or (choosefunc>numfunc) then
       choosefunc:=0;
      endif;
     endif;

     if (choosefunc>0) then
      switch functions[choosefunc-1] of
       case func_ultrascan: displr(base^.z,base^.x,base^.y,19);
                            break;

       case func_comm:      use_radio;
                            break;

       case func_robo:      showrobodefender(base^.spec[dnum]);
                            break;

       case func_vortex:    base_vortexlink(base^);
                            break;
      end;
     endif;
    endif;
   endif;
  endif;
 until done;
end base_useitem;