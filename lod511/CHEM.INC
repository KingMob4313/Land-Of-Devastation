const
 regress_odds = 5; (* odds per drug use that chemical will cause regress *)
 regress_percent = 25; (* amount that stats will drop if regress occurs *)

 chem_str = 1;     (* strength enhancer. Var2 = points to add *)
 chem_dex = 2;     (* dexterity enhancer. Var2 = points to add *)
 chem_agl = 3;     (* agility enhancer. Var2 = points to add *)
 chem_hit = 4;     (* hit point increaser. Var2 = points to add *)
 chem_exp = 5;     (* exp enhancer. Var2 = % to increase *)
 chem_unstr = 6;
 chem_undex = 7;
 chem_unagl = 8;
 chem_unhit = 9;
 chem_unexp = 10;  (* neural inhibiter. Var2 = % to decrease *)
 chem_sex = 11;    (* sex changer. Var2 not user *)

procedure use_chemical(var o: devicetype);
var
 addpts: integer;
 chemhits: integer;
begin;
 clrscr;
 writeln("&7Disclaimer: Although ACME Pharmaceuticals, inc., has taken every possible step");
 writeln("to ensure the quality of our products, we can not rule out the possibility that");
 writeln("adverse reactions may occur if chemicals are abused. ACME Pharmaceuticals,");
 writeln("inc., will not be held responsible for attribute regression, physical damage,");
 writeln("or death resulting from frequent or neglegent use of chemical products.");
 writeln;
 if checkyesno("&3Drink the chemicals (&FY&3/&FN&3) ? ") then
  if verifyparam(appuser(usernum^),"chemhits") then
   getparam(appuser(usernum^),"chemhits",chemhits);
  else
   chemhits:=0;
  endif;
  if (devicedef^[o.num]^.var1<>o.var1) then
   writeln;
   writeln("&CAs you guzzle down the fluid from the vial, the label slips off. Some");
   writeln("unscrupulous individual has placed a fake label on the vial! Whatever this");
   writeln("stuff is, it certainly isn''t ",devname(o),"!");
  end;
  addpts:=max(1,ftoi(o.var2/(1.0+chemhits/4)/100*(80+randi(40))));
  writeln;
  switch o.var1 of
    case chem_str: writeln("&7As the chemicals enter your system, you feel a new surge of strength throughout");
                   writeln("your body. You muscles bulge visibly. Your strength has been increased by &F",addpts);
                   writeln("&7points!");
                   user^.str:=user^.str+addpts;
                   break;

    case chem_dex: writeln("&7Immediately, you feel a stunning precision enter the muscles of your hands.");
                   writeln("Your dexterity has been increased by &F",addpts," &7points!");
                   user^.dex:=user^.dex+addpts;
                   break;

    case chem_agl: writeln("&7As soon as the chemical enters your body, your joints feel looser and more");
                   writeln("agile. Your body is more flexible than it has ever been. You agility has been");
                   writeln("increased by &F",addpts," &7points!");
                   user^.agl:=user^.agl+addpts;
                   break;

    case chem_sex: writeln("&7You feel strange.... The shape of your body begins to contort and twist in");
                   write("confusing and unpredictable ways. ");

                   if (user^.sex=sex_male) then
                    writeln("Your body strength drops considerably. Your");
                    writeln("agility increases marginally. You suddenly have a strange desire to find some");
                    writeln("makeup and lipstick. You wonder if your hair would look better with a \"perm\".");
                    writeln("It's all very weird.");
                    user^.str:=max(1,user^.str-15);
                    user^.agl:=user^.agl+5;
                    user^.sex:=sex_female;
                   else
                    writeln("Your agility drops considerably. Your body");
                    writeln("strength increases marginally. You have a strange desire to go to the next");
                    writeln("Sacre Base \"monster truck\" ralley, but you don't know why. It's all very odd.");
                    user^.str:=user^.str+5;
                    user^.agl:=max(1,user^.agl-15);
                    user^.sex:=sex_male;
                   endif;
                   break;

    case chem_hit: writeln("&7You feel your skin toughen. Your limbs strengthen. Your body becomes more");
                   writeln("resistant to damage. You have gained &F",addpts," &7permanent hit points!");
                   user^.hitmax:=user^.hitmax+addpts;
                   user^.hitpts:=user^.hitmax;
                   break;

    case chem_exp: writeln("&7You have a slight headache, but it passes. When you recover, you feel more");
                   writeln("\"experienced\" than you did previously.");
                   user^.exp:=user^.exp+max(0,ftoi(user^.exp/100*min(100,o.var2)));
                   break;

    case chem_unstr:
                   writeln("&7You feel your muscles shrivel up. The strength drains from your body. You");
                   writeln("have lost &F",addpts," &7strength points.");
                   user^.str:=max(1,user^.str-addpts);
                   break;

    case chem_undex:
                   writeln("&7Your hands begin to tremble and shake as you feel your dexterity drain away.");
                   writeln("You lost &F",addpts," &7dexterity points.");
                   user^.dex:=max(1,user^.dex-addpts);
                   break;

    case chem_unagl:
                   writeln("&7Your muscles tighten up and lose their flexibility. Your agility has been");
                   writeln("drained by &F",addpts," &7points.");
                   user^.agl:=max(1,user^.agl-addpts);
                   break;

    case chem_unhit:
                   writeln("&7Your skin becomes fairer and more susceptible to damage. Your bones become");
                   writeln("fragile. You have lost &F",addpts," &7permanent hitpoints.");
                   user^.hitpts:=max(1,user^.hitpts-addpts);
                   break;


    case chem_unexp:
                   writeln("&7You have a pounding headache. It feels as if neurons are burning away right");
                   writeln("out of your head. When it ends, you feel as if you have lost someting; as if");
                   writeln("you are somehow less \"experienced\" than before.");
                   user^.exp:=max(0,user^.exp-ftoi(user^.exp/100*min(100,o.var2)));
                   break;

    default:
                   writeln("&7The chemicals enter your body, but produce no immediate effect.");
                   break;
  end;
  if randi(100)<regress_odds*chemhits then
   writeln;
   writeln("&CSomething has gone wrong! You experience some sort of attribute regression");
   writeln("from the drugs. You become sick -- Very sick.");
   writeln;
   addpts:=ftoi(user^.str*(randi(regress_percent)+1)/100);
   writeln("&7You lose &F",addpts," &7strength points.");
   user^.str:=user^.str-addpts;

   addpts:=ftoi(user^.dex*(randi(regress_percent)+1)/100);
   writeln("&7You lose &F",addpts," &7dexterity points.");
   user^.dex:=user^.dex-addpts;

   addpts:=ftoi(user^.agl*(randi(regress_percent)+1)/100);
   writeln("&7You lose &F",addpts," &7agility points.");
   user^.agl:=user^.agl-addpts;
  endif;

  putparam(appuser(usernum^),"chemhits",chemhits+1);
  decuseleft(o);

  waitkey;
 endif;
end use_chemical;

procedure use_labeller;
var
 done: boolean;
 s: string;
 i,loop1,labnum: integer;
begin;
 writeln("&7This labeller is the property of ACME Pharaceutical, Inc. No unauthorized");
 writeln("operation permitted.");
 writeln;
 listuserinv(user^,false);
 done:=false;
 repeat;
  writeln;
  write("&3Re-label which drug (&F?&3=List,&FQ&3=Quit) ? &F");
  s:=upstr(readln());
  i:=atoi(s);
  if s="?" then
   listuserinv(user^,false);
  elsif (s="Q") or (s="") then
   done:=true;
  elsif (i>0) & (i<=numudev) then
   writeln;
   with ud:user^.devices[i-1] do
    if ud.num=0 then
     writeln("&CThat inventory slot is empty!");
    elsif ud.devapp<>devapp_chem then
     writeln("&CThat item is not a drug!");
    else
     for loop1:=1 to numdev do
      with d:devicedef^[loop1]^ do
       if (d.num<>0) & (d.devapp=devapp_chem) then
        writeln("&E",d.num,"&7: ",d.name);
       endif;
      endwith;
     endfor;
     writeln;
     write("&3Enter label number to emit: &F");
     labnum:=atoi(readln());
     if (labnum>0) & (labnum<numdev) & (devicedef^[labnum]^.devapp=devapp_chem) then
      ud.num:=labnum;
      done:=true;
     endif;
    endif;
   endwith;
  else
   writeln("&CInvalid option.");
  endif;
 until done;
end use_labeller;
