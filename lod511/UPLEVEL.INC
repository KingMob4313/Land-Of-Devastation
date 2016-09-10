procedure calcmaxed(var strmaxed,dexmaxed,aglmaxed,hitmaxed: boolean);
var
 n: word;
begin;
 strmaxed:=false;
 dexmaxed:=false;
 aglmaxed:=false;
 hitmaxed:=false;
 if findmonster(z_purboss,4,n) then
  loadcombatdata(1,n);
  if mchar_player^.str*querylodint(querylod_xebocmax)/100<=user^.str then strmaxed:=true; endif;
  if mchar_player^.dex*querylodint(querylod_xebocmax)/100<=user^.dex then dexmaxed:=true; endif;
  if mchar_player^.agl*querylodint(querylod_xebocmax)/100<=user^.agl then aglmaxed:=true; endif;
  if mchar_player^.hitmax*querylodint(querylod_xebocmax)/100<=user^.hitmax then hitmaxed:=true; endif;
 endif;
end calcmaxed;

procedure showaddmenu;
begin;
 writeln("&3You may train in any of the following:");
 writeln("&2  &FS&2]trength: increases damage done in short range combat.");
 writeln("&2  &FD&2]exterity: improves your accuracy in using weapons.");
 writeln("&2  &FA&2]gility: improves your ability to run and dodge attacks.");
 writeln("&2  &FH&2]ealth: increases the amount of damage you may sustain.");
 writeln("&2  &FC&2]ash: trade some of your knowledge for money.");
end showaddmenu;

#define showstat(title,attr,maxed)     \
         write(title,attr);            \
         if maxed then                 \
          writeln(" &C[Maxed-Out!]");  \
         else                          \
          writeln;                     \
         endif;

procedure addlevels;
var
 a: word;
 ch: char;
 cashperlevel: integer;
 strmaxed,dexmaxed,aglmaxed,hitmaxed: boolean;
 needhelp: boolean;
begin;
 cashperlevel:=user^.lvl*querylodint(querylod_moneyperlevel);

 needhelp:=true;

 while user^.addpts>0 do
  calcmaxed(strmaxed,dexmaxed,aglmaxed,hitmaxed);
  clrscr;

  writeln("&EYou have &F",user^.addpts,"&E training points left to allocate.");
  writeln;
  writeln("&3Your current statistics are as follows:");

  showstat("&A  Strength..: &F",user^.str,strmaxed);
  showstat("&A  Dexterity.: &F",user^.dex,dexmaxed);
  showstat("&A  Agility...: &F",user^.agl,aglmaxed);
  showstat("&A  Hitpoints.: &F",user^.hitmax,hitmaxed);
  showstat("&A  Cash......: &F",user^.money,false);

  if needhelp then
   writeln;
   showaddmenu;
   needhelp:=false;
  endif;

  writeln;
  write("&3Your selection [&FS&3,&FA&3,&FD&3,&FH&3,&FC&3,&F?&3=Help] : &F");

  repeat;
   ch:=upcase(readkey());
  until (ch='S') or (ch='A') or (ch='D') or (ch='H') or (ch='C') or (ch='?');

  switch ch of
   case '?': needhelp:=true;
             break;

   case 'S':
         writeln("Strength.");
         if strmaxed then
          writeln;
          writeln("&CWe're sorry, but your skill has progressed to the point that we are unable to");
          writeln("train you any further in this attribute.");
          waitkey;
         else
          dec(user^.addpts);
          inc(user^.str);
         endif;
         break;

   case 'D':
         writeln("Dexterity.");
         if dexmaxed then
          writeln;
          writeln("&CWe're sorry, but your skill has progressed to the point that we are unable to");
          writeln("train you any further in this attribute.");
          waitkey;
         else
          dec(user^.addpts);
          inc(user^.dex);
         endif;
         break;

   case 'A':
         writeln("Agility.");
         if aglmaxed then
          writeln;
          writeln("&CWe're sorry, but your skill has progressed to the point that we are unable to");
          writeln("train you any further in this attribute.");
          waitkey;
         else
          dec(user^.addpts);
          inc(user^.agl);
         endif;
         break;

   case 'H':
         writeln("Hitpoints.");
         if hitmaxed then
          writeln;
          writeln("&CWe're sorry, but your skill has progressed to the point that we are unable to");
          writeln("train you any further in this attribute.");
          waitkey;
         else
          dec(user^.addpts);
          inc(user^.hitmax);
         endif;
         break;

   case 'C':
         writeln("Cash.");
         writeln;
         write("&BYou may trade one training point for &F$",cashperlevel);
         if checkyesno("&B in cash. Do it (&FY&B/&FN&B) ? ") then
          dec(user^.addpts);
          user^.money:=user^.money+cashperlevel;
         endif;
         break;
  end;
 endwhile;
end addlevels;