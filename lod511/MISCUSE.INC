procedure use_rations;
begin;
 user^.hunger:=user^.hunger-min(hinc,user^.hunger);
 writeln("&CThe food tastes terrible as it goes down.");
end use_rations;

procedure use_medkit;
var
 c: integer;
begin;
 c:=min(randi(10)+3,max(0,user^.hitmax-user^.hitpts));
 if (c>0) then
   user^.hitpts:=user^.hitpts+c;
   writeln("&CHit points regained: &F",c," pts&C. Your health is now: &F",
     user^.hitpts,'/',user^.hitmax," pts&C.");
 else
   writeln("&CSince you were already at full health, you just WASTED a medkit dose!");
 endif;
end use_medkit;

procedure use_radio;
begin;
 write("&BDo you wish to &FR&Bead or &FW&Brite messages (&FR&B/&FW&B) ? ");
 switch upcase(readkey()) of
   case 'R': writeln("&FRead");
             ReadMail;
	     break;

   case 'W': writeln("&Fwrite");
             EnterMessage("","");
	     break;

   default: writeln;
	    break;
 end;
end use_radio;

procedure use_unitran;
begin;
 writeln("&7Universal Translator");
 writeln("Standard Starfleet Issue. Stardate 20214.1");
 writeln;
 writeln("Universal translator will be automatically engaged when an unknown language is");
 writeln("encountered.");
end use_unitran;

procedure stealth_mastertoggle;
var
 a,z: integer;
begin;
 writeln;
 writeln("&AMaster stealth field toggle - switch all SFG's in inventory at once.");
 write("&BMaster switch to [&FO&B]ff, [&FQ&B]uiet, or [&FN&B]oisy ? ");
 z:=-1;
 switch upcase(readkey()) of
  case 'O': writeln("&FOff");
            z:=0;
            break;

  case 'Q': writeln("&FQuiet");
            z:=1;
            break;

  case 'N': writeln("&FNoisy");
            z:=2;
            break;

  default:  writeln;
            break;
 end;
 if (z<>-1) then
  for a:=0 to numudev-1 do (* remeber, inv is 0-based array! *)
   with d:user^.devices[a] do
    if (d.num<>0) & (d.devapp=devapp_stealth) then
     writeln("&A (#",a+1,") ",devname(d),": mode set.");
     d.var1:=z;
    endif;
   endwith;
  endfor;
 endif;
end stealth_mastertoggle;

procedure use_stealth(var o: devicetype);
var
 ch: char;
begin;
 write("&BCurrent stealth field operating mode: ");
 if o.var1=0 then
   writeln("&FOff");
 elsif o.var1=1 then
   writeln("&FQuiet");
 elsif o.var1=2 then
   writeln("&FNoisy");
 else
   writeln;
 endif;

 write("&BChange operating mode to [&FO&B]ff, [&FQ&B]uiet, [&FN&B]oisy, or [&FM&B]aster ? ");
 switch upcase(readkey()) of
  case 'O': writeln("&FOff");
	    o.var1:=0;
	    writeln("&BStealth field operating mode set.");
	    break;

  case 'Q': writeln("&FQuiet");
	    o.var1:=1;
	    writeln("&BStealth field operating mode set.");
	    break;

  case 'N': writeln("&FNoisy");
	    o.var1:=2;
	    writeln("&BStealth field operating mode set.");
	    break;

  case 'M': writeln("&FMaster");
            stealth_mastertoggle;
            break;

  default: writeln;
	   break;
 end;
end use_stealth;

procedure warpermessage(var u: usertype);
begin;
 writeln("&7Current warper anchor coordinates are ",
         "(z=&F",u.anchorz,
         "&7,x=&F",u.anchorx,
         "&7,y=&F",u.anchory,
         "&7).");

 writeln;

 writeln("&7A dark mist surrounds your body and you are knocked unconscious. When you");
 writeln("&7awake, you are not in the same place as you were!");
end warpermessage;

procedure use_warper(): boolean;
var
 cx,cz,cy: byte;
begin;
 if ~checklaptop(user^,rom_emwarp) then
  (* the user has no emwarp rom, so make sure any anchor point is nulled! *)
  user^.anchorz:=0;
  user^.anchorx:=0;
  user^.anchory:=0;
 endif;

 if ~checkanchorpoint(user^) then
  (* if no valid anchor point can be found, then abort *)
  writeln("&CThere is no warper anchor point on this map. Unable to operate warper.");
  return FALSE;
 else
  warpermessage(user^);

  user^.z:=user^.anchorz;
  user^.x:=user^.anchorx;
  user^.y:=user^.anchory;

  return TRUE;
 endif;
end use_warper;

procedure use_recharger(var d: devicetype);
var
 regenpts: integer;
begin;
 writeln("&B\"Welcome to the ACME general purpose armor recharging system. This device is");
 writeln("capable of recharging any type energy-based armor. The degree of recharging");
 writeln("that may be performed is limited only by the number of energy units stored in");
 writeln("the recharger. Thank you for using another fine ACME product!\"");
 writeln;
 writeln("&7Current armor deflection points: &F",user^.armor.useleft,"/",
         devdef_maxuse(user^.armor.num));
 writeln("&7Remaining recharger energy units: &F",d.useleft);

 if (user^.armor.var1 & flagarmorrecharge)=0 then
  writeln("&CRecharging this type of armor is NOT recommended.");
 endif;

 writeln;

 if checkyesno("&3Apply the recharger to your armor (&FY&3/&FN&3) ? ") then
  if user^.armor.num=0 then
   writeln;
   writeln("&CSince you're not wearing any armor, nothing happened! <duh!>");
  elsif (user^.armor.var1 & flagarmorrecharge)=0 then
   nukedevice(user^.armor);
   writeln;
   writeln("&EYou press the large rectangular button on the recharger. Lightning and");
   writeln("electric flames erupt between your armor and the device. When you're finished,");
   writeln("all that remains of your armor is a pile of melted muck on the ground. I guess");
   writeln("the recharger is not compatible with that type of armor!");
  else
   regenpts:=max(0,min(d.useleft,devdef_maxuse(user^.armor.num)-user^.armor.useleft));
   user^.armor.useleft:=user^.armor.useleft+regenpts;
   d.useleft:=d.useleft-regenpts;
   writeln;
   writeln("&EYou press the large rectangular button on the recharger. Immediately, you feel");
   writeln("a magentic-like attraction between the recharger and your armor. You place the");
   writeln("recharger closer to your armor. Lightning like flashes of energy burst from the");
   writeln("recharger and impact into your armor. As you peer into the inferno, you can see");
   writeln("that the molecular structure of your armor is being regenerated right before");
   writeln("your eyes.");
   writeln;
   writeln("&2Your armor is now at &F",user^.armor.useleft,"/",
           devdef_maxuse(user^.armor.num),"&2 points.");
   writeln("&2The recharger has &F",d.useleft,"&2 energy units remaining.");
  endif;
 endif;
 if d.useleft=0 then
  nukedevice(d);
 endif;
end use_recharger;

procedure mapper_region(n: integer);
begin;
 flagredraw;
 clrscr;
 switch n of
  case 1: AnsiMapper(1,1,65,32,user^.x,user^.y);
	  exit;

  case 2: AnsiMapper(1,32,65,32,user^.x,user^.y);
	  exit;

  case 3: AnsiMapper(1,64,65,32,user^.x,user^.y);
	  exit;

  case 4: AnsiMapper(1,96,65,32,user^.x,user^.y);
	  exit;

  case 5: AnsiMapper(60,1,65,32,user^.x,user^.y);
	  exit;

  case 6: AnsiMapper(60,32,65,32,user^.x,user^.y);
	  exit;

  case 7: AnsiMapper(60,64,65,32,user^.x,user^.y);
	  exit;

  case 8: AnsiMapper(60,93,65,32,user^.x,user^.y);
	  exit;
 end;
 waitkey;
end mapper_region;

procedure use_mapper;
var
 s: string;
 a: integer;
 done: boolean;
begin;
 if querygtermactive() then
  writeln("&E[For GTERM users, the mapper is automatically displayed in the lower right of");
  writeln("the screen]");
  writeln;
 endif;
 done:=false;
 repeat;
  write("&BRegion number [&F1&B..&F8&B,&FC&B=Current,&FQ&B=Quit,&FL&B=List] ? &F");
  s:=upstr(readln());
  a:=atoi(s);
  if (s="C") or (s="") then
   done:=true;
   Mapper_Region((user^.y div 32)+1+((user^.x div 65)*4));
  elsif (s="L") then
   writeln;
   writeln("&EÚÄÄÄÂÄÄÄ¿");
   writeln("³ &F1&E ³ &F5&E ³");
   writeln("ÃÄÄÄÅÄÄÄ´");
   writeln("³ &F2&E ³ &F6&E ³");
   writeln("ÃÄÄÄÅÄÄÄ´");
   writeln("³ &F3&E ³ &F7&E ³");
   writeln("ÃÄÄÄÅÄÄÄ´");
   writeln("³ &F4&E ³ &F8&E ³");
   writeln("ÀÄÄÄÁÄÄÄÙ");
   writeln;
  elsif (s="Q") then
   done:=true;
  elsif (a>=1) & (a<=8) then
   done:=true;
   mapper_region(a);
  else
   writeln("&CInvalid region number. Please try again.");
   writeln;
  endif;
 until done;
end use_mapper;

procedure viewrobomine(var o: devicetype);
var
 mnum,n: word;
begin;
 if testcap(o,flagminerobo) then
   mnum:=getminerobo(o);
   if findmonster(z_robomine,mnum,n) then
     loadcombatdata(1,n);
     showchar(mchar_player);
     writeln;
   endif;
 endif;
end viewrobomine;

procedure use_mine(var d: devicetype);
var
 ch: char;
 cx,cz,cy: byte;
 a: integer;
 done: boolean;
 f: integer;
 result: boolean;
 map: tmapentry;
begin;
 done:=false;
 while ~done do
  write("&3Mine type............: &F");
  if testcap(d,flagminesmart) then
    write("Smart") else write("Dumb");
  endif;
  if testcap(d,flagmineenergy) then
    write("&7/&FEnergy Drainer");
  endif;
  if testcap(d,flagmineteleport) then
    write("&7/&FTeleporter");
  endif;
  if testcap(d,flagminerobo) then
    write("&7/&FRobo");
  endif;
  writeln;
  writeln("&3Blasts remaining.....: &F",d.useleft);
  writeln("&3Minimum damage/blast.: &F",wmindam(d.var1)," hit points");
  writeln("&3Maximum damage/blast.: &F",wmaxdam(d.var1)," hit points");
  writeln("&3Warnings Enabled.....: &F",onoff(~testcap(d,flagminenowarn)));
  writeln;

  if (testcap(d,flagminerobo)) then
    write("&BMine option: (&FI&Bnstall/&FW&Barnings/&FA&Bbort/&FV&Biew) ? &F");
  else
    write("&BMine option: (&FI&Bnstall/&FW&Barnings/&FA&Bbort) ? &F")
  endif;

  switch upcase(readkey()) of
    case 'I': writeln("Install");
              getmaptableentry(user^.z,map);
	      if ((map.flags & flagmapnomine)<>0) or
               (sqrt(cdist(map.zoneorigx,map.zoneorigy,user^.x,user^.y))<map.minminedist) then
		writeln;
		writeln("&BAs you prepare to install the mine, you think to yourself \"This just isn't");
		writeln("right... New recruits and novice soldiers pass through this area from time");
		writeln("to time... \" Suffering from a guilty conscience, you put the mine back in");
		writeln("your pack and decide to pursue a different course of action.");
	      elsif testcap(d,flagminesmart) then
		d.var2:=usernum^;
		writeln;
		writeln("&EMine installed. Will detonate whenever anyone but you enters this location.");
		dropobj(d);
	      else
		d.var2:=65535;
		writeln;
		writeln("&EMine installed. Will detonate whenever anyone enters this location.");
		dropobj(d);
	      endif;
	      done:=true;
	      break;


   case 'A':  writeln("Abort");
	      done:=true;
	      break;

   case 'W':  writeln("Warnings");
	      writeln("&BThe warnings option will cause the mine to display a warning and indicate its");
	      writeln("owner and type. The intruder will then be presented with an option of whether");
	      writeln("to continue or not. Activating warnings produces a useful deterrent effect,");
	      writeln("while deactivating warnings will maximize the element of surprise.");
	      writeln;
              result:=checkyesno("&BActivate warnings (&FY&B/&FN&B) ? ");
              if result then
               resetcap(d,flagminenowarn)
              else
               setcap(d,flagminenowarn);
              endif;
	      break;

   case 'V':  writeln("View");
	      viewrobomine(d);
	      break;

   default:   writeln;
  end;
 endwhile;
end use_mine;

procedure use_chest(var d: devicetype);
var
 done: boolean;
 l: integer;
begin;
 repeat
  writeln("&2There is &F$",d.var1,"&2 in the ",devicedef^[d.num]^.name,
          ". You are carrying &F$",user^.money,"&2.");
  write("&3Chest: &FD&3]eposit, &FW&3]ithdraw, or &FQ&3]uit ? ");
  switch upcase(readkey()) of

   case 'D': writeln("&FDeposit");
             writeln;
             write("&BDeposit how much ? &F");
             l:=atoi(readln());
             if l<=0 then
              writeln("&CInvalid amount.");
             elsif l+d.var1>largest(integer) then
              writeln("&CThe chest cannot hold that much.");
             elsif l>user^.money then
              writeln("&CYou are not carrying that much.");
             else
              writeln("&ETransaction completed.");
              user^.money:=user^.money-l;
              d.var1:=d.var1+l;
             endif;
             writeln;
             break;

   case 'W': writeln("&FWithdraw");
             writeln;
             write("&BWithdraw how much ? &F");
             l:=atoi(readln());
             if l<=0 then
              writeln("&CInvalid amount.");
             elsif l>d.var1 then
              writeln("&CThe chest does not contain that much money.");
             else
              writeln("&ETransaction completed.");
              user^.money:=user^.money+l;
              d.var1:=d.var1-l;
             endif;
             writeln;
             break;

   case 'Q': writeln("&FQuit");
             resetneedpause;
             done:=true;
             break;

   default:  writeln;
             break;
  end;
 until done;
end use_chest;

procedure use_boatplans;
var
 hidenum,sticksnum,boatnum: integer;
 goodhide,goodsticks,bogushide: integer;
 devnum: integer;
begin;
 writeln("&3These plans diagram the creation of a simple raft from two indregients:");
 writeln;
 writeln("&2 - an assortment of sturdy sticks");
 writeln("&2 - a reasonably intact animal hide");
 writeln;

 (* find the device numbers of the relevent devices *)

 hidenum:=ckdevstr_verbal("Animal Hide");
 sticksnum:=ckdevstr_verbal("Sticks");
 boatnum:=ckdevstr_verbal("Home-made raft");

 goodhide:=-1;
 goodsticks:=-1;
 bogushide:=-1;

 (* Search user's inventory for the ingredients *)

 for devnum:=0 to numudev-1 do
  with ud:user^.devices[devnum] do
   if (ud.num=hidenum) then
    if (ud.var1<(devicedef^[hidenum]^.var1 div 2)) then
     if bogushide=-1 then
      bogushide:=devnum;
     endif;
    elsif goodhide=-1 then
     goodhide:=devnum;
    endif;
   elsif (ud.num=sticksnum) & (goodsticks=-1) then
    goodsticks:=devnum;
   endif;
  endwith;
 endfor;

 (* report missing/damaged ingredients *)

 if (goodhide=-1) & (bogushide>=0) then
  writeln("&CYou have an animal hide, but it is too badly damaged to use.");
 elsif (goodhide=-1) & (bogushide=-1) then
  writeln("&CYou do not have an animal hide.");
 endif;
 if (goodsticks=-1) then
  writeln("&CYou do not have any sticks.");
 endif;

 (* if user has ingredients, then construct the raft *)

 if (goodhide>=0) & (goodsticks>=0) &
    checkyesno("&BYou have the proper materials, construct the raft (&FY&B/&FN&B) ? ") then
  nukedevice(user^.devices[goodsticks]);
  user^.devices[goodhide]:=createdevice(boatnum);
 endif;
end use_boatplans;


