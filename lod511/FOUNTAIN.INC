#define fountaintag(n) ("fountdrank_"+itoa(n))

procedure go_fountain(n: integer);
var
 drank,maxcount: integer;
begin;
 write("&3The water is ");

 switch n of
  case 1: write("&9shimmering blue");     (* hunger reduction *)
          maxcount:=largest(integer);
          break;

  case 2: write("&Aglowing green");       (* dex increaser *)
          maxcount:=1;
          break;

  case 3: write("&Ctranslucent red");     (* agl increaser *)
          maxcount:=1;
          break;

  case 4: write("&8foul black");          (* hitpoint reducer *)
          maxcount:=largest(integer);
          break;

  case 5: write("&Dpretty pink");         (* change to female *)
          maxcount:=largest(integer);
          break;

  case 6: write("&Fcrystal clear");       (* shrine of hercules *)
          maxcount:=1;
          break;

 end;

 if checkyesno("&3. Have a drink (&FY&3/&FN&3) ? ") then
  if ~getparam(appuser(usernum^),fountaintag(n),drank) then
   drank:=0;
  endif;
  if (drank>=maxcount) then
   writeln("&CThere is no effect. Perhaps you have drank from this fountain before?");
  else
   inc(drank);
   putparam(appuser(usernum^),fountaintag(n),drank);
   switch n of
    case 1: writeln("&EThe hunger pains in your stomach have subsided.");
            user^.hunger:=0;
            break;

    case 2: writeln("&EYou feel more dextrous.");
            user^.dex:=user^.dex+5;
            break;

    case 3: writeln("&EYour agility has increased.");
            user^.agl:=user^.agl+5;
            break;

    case 4: writeln("&CUggghhh! You just lost 5 permanent hitpoints!");
            user^.hitmax:=max(1,user^.hitmax-5);
            user^.hitpts:=min(user^.hitpts,user^.hitmax);
            break;

    case 5: if user^.sex=sex_male then
             user^.sex:=sex_female;
             writeln("&CNow you know what it feels like to be a woman....");
            else
             writeln("&CYou feel a strange tingling sensation.");
            endif;
            break;

    case 6: writeln("&EThe might of Hercules surges through your veins!");
            user^.str:=user^.str+25;
            break;
   end;
  endif;
 endif;
end go_fountain;