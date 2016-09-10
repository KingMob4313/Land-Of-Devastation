(* LGPL THEFT FOR LOD: Written By Dave Rosson:12/28/95. *)

 (* catch_thief: if cop<>0, then enters combat with the cop, else
                 enters combat with the victim.
 *)

procedure catch_thief(cop,victim: integer);
begin;
 if (cop<>0) then
  loadcombatdata(1,cop);
  dowastecombat;
 else
  doplayercombat(victim);
 endif;
end catch_thief;

 (* attempt_theft: Allows the user to attempt to take an item from *)
 (*                the inventory of whoevers camp they are in.     *)
procedure attempt_theft;
const
 (* FortToTheftRate: How many fort attacks a theft counts as. *)
  FortToTheftRate = 4;
var
  s: string;
  victimnum, freeinv, i, kit_mod, alarm_mod: integer;
  victimdat: usertype;
  done: boolean;
  d: devicetype;
  map: tmapentry;
begin
  writeln("&BStealing costs ",FortToTheftRate," of your daily fort attacks.&7");
  getmaptableentry(user^.z,map);
 (* Check if you don't have enough fort attacks left. *)
  if (((user^.fortattack)+FortToTheftRate) > querylodint(querylod_maxfortattack)) then
    writeln("&BAnd you don't have that many left.&7");
 (* Don't allow theft on no-fighting-allowed maps. *)
  elsif ((map.flags & flagmapnosteal)<>0) then
    writeln;
    writeln("&BYou suddenly have the odd feeling you are being watched.");
    writeln("Perhaps this isn't a good place to rob someone?&7");
  else
    kit_mod:=0;
    alarm_mod:=0;
    done:= false;
    victimnum:= querylodint(querylod_gmenuplayernum);
    loaduser(victimnum, victimdat);
    repeat
      writeln;
 (* Determine what the thief wants. *)
      write("&BSteal what from &C",namestr(victimdat.alias)," &9(&F?&3=List&9,&FQ&3=Quit&9)&B ?&7 ");
      s:=upstr(readln());
      i:=atoi(s);
 (* List victims inventory. *)
      if s="?" then
        listuserinv(victimdat,false);
 (* Is the thief overloaded? *)
      elsif ~findfreeinv(user^,freeinv) then
        done:=true;
        writeln("&CYou can't steal when your inventory is full!&7");
      elsif (s="") or (s="Q") then
        writeln;
        writeln("&DHow very generous of you.&7");
        done:=true;
      elsif (i>0) & (i<=numudev) then
        writeln;
        with d:victimdat.devices[i-1] do
          if d.num=0 then
            writeln("&CThat inventory slot is empty!&7");
          else
            writeln("&BSteal the &C", devname(d), "&B?&7");
            if checkyesno("&3Are you sure you want to do that &9(&FY&9/&FN&9)&3?&7 ") then
              writeln;
              user^.fortattack:=((user^.fortattack)+FortToTheftRate);
              writelog("    ",namestr(user^.alias)," mugged ",namestr(victimdat.alias),".");
              done:=true;
 (* Prepare thief tools if possessed by thief. *)
              if (hasobj(devapp_tkit, user^) = TRUE) then
                writeln("&2You ready your special thief tools and move in.&7");
                kit_mod:=user^.lvl;
              endif;
              delay(90);
 (* Activate alarm if possessed by victim. *)
              if (hasobj(devapp_palarm, victimdat) = TRUE) then
                writeln();
                writeln("&4An annoying beep sounds from your victim's camp!");
                writeln("This will make your job much harder.&7");
                alarm_mod:=victimdat.lvl;
              endif;
              delay(60);
              writeln;
 (* Is the item grabbed? *)
              if (randi(user^.dex+kit_mod) > randi(victimdat.agl+alarm_mod)) then
                writeln("&2Success! You turn to sneak away.&7");
                user^.devices[freeinv]:=d;
                nukedevice(d);
                saveuser(victimnum, victimdat);
              else
                writeln("&2Nooo! You fail to grab it, and turn to sneak away.&7");
              endif;
              delay(120);
              writeln;
 (* Does the thief sneak off into the shadows? *)
              if (randi(user^.agl+kit_mod) > randi(victimdat.dex+alarm_mod)) then
                writeln("&AYou are long gone by the time they notice.&7");
              else
                writeln("&AOh no! You've been spotted.&7");
                waitkey;
                catch_thief(map.cop,victimnum);
              endif;
 (* Various aborts do to stopping theft or entering garbage. *)
            else
              writeln;
              writeln("&DHow very generous of you.&7");
              done:=true;
            endif;
          endif;
        endwith;
      else
        writeln;
        writeln("&CInvalid option!&7");
      endif;
    until done;
  endif;
end attempt_theft;

procedure use_palarm;
begin;
 writeln("&AThe alarm will be set whenever you camp out. It will aid you in catching");
 writeln("anyone who attempts to steal from you.");
end use_palarm;

procedure use_tkit;
begin;
 writeln("&AThe Thief''s kit will automatically be used whenever you try to steal from");
 writeln("somone. It will increase your chances of successfully taking the item and");
 writeln("escaping.");
end use_tkit;
