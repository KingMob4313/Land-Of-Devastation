const
 maxtrivcheck=50;
 roulette_payback=50;
 megajackpot_start = 1000000;
var
 trivcheck: array (maxtrivcheck+1) of word;
 trivcheck_ptr: word;
type
 tdeck = record
          cards: array 52 of integer; (* 2-10 = numeric, 11=jack, 12=queen, 13=king, 14=ace *)
          current: integer;
         end;

 thand = record
          cards: array 5 of integer;
          held: array 5 of boolean;
          visible: array 5 of boolean;
         end;

procedure checkbet(l: integer): boolean;
var
 limit: integer;
begin;
 limit:=querylodint(querylod_caslimit);
 if l>limit then
  writeln("&CThere is a &F$",limit,"&C house limit.");
  return false;
 elsif l>user^.money then
  writeln("&CYou do not have that much money!");
  return false;
 else
  return (l>0);
 endif;
end checkbet;

procedure coin_flip;
var
 s: string;
 a,l: integer;
 ch: char;
begin;
 writeln;
 writeln("&AIn this game, you bet on whether or not the flipped coin will come up");
 writeln("heads or tails.");
 writeln;
 repeat;
  write("&2How much money do you wish to bet (&FQ&2=Quit) ? &F");
  s:=upstr(readln());
  if s="Q" then return; endif;
  l:=atoi(s);
 until checkbet(l);
 LogCasPlay(1,l);
 write("&2[&FH&2]eads or [&FT&2]ails ? &F");
 repeat;
  ch:=upcase(readkey());
 until (ch='H') or (ch='T');
 if ch='H' then
  writeln("Heads.")
 else
  writeln("Tails.");
 endif;
 write("&3Flipping the coin: &E");
 for a:=1 to 24 do
  switch (a mod 4) of
   case 0: write("-");
           break;

   case 1: write("\\");
           break;

   case 2: write("|");
           break;

   case 3: write("/");
           break;
  end;
  delay(100);
  if a<24 then write(08X," "); endif;
 endfor;
 if randi(100)>50 then
  if ch='H' then
   writeln("&7 \"Heads!\"")
  else
   writeln("&7 \"Tails!\"");
  endif;
  writeln("&9You win!");
  user^.money:=user^.money+l;
  LogCasWin(1,l);
 else
  if ch='H' then
   writeln("&7 \"Tails!\"")
  else
   writeln("&7 \"Heads!\"");
  endif;
  writeln("&CSorry, you lose.");
  user^.money:=user^.money-l;
  LogCasLose(1,l);
 endif;
end coin_flip;

procedure guess_number;
var
 s: string;
 guesses: byte;
 num: word;
 a,l,w: integer;
begin;
 writeln;
 writeln("&AIn this game, you bet on your chances to guess a number between 1");
 writeln("and 1000. You will get 9 tries and I will tell you wether the correct");
 writeln("number is higher or lower than your guess. If you win, I will give");
 writeln("you back double your bet, if you lose, I will take your money!");
 writeln;
 repeat;
  write("&2How many credits do you wish to bet (&FQ&2=Quit) ? &F");
  s:=upstr(readln());
  if s="Q" then return; endif;
  l:=atoi(s);
 until checkbet(l);
 LogCasPlay(2,l);
 num:=randi(999)+1;
 guesses:=0;
 repeat;
  guesses:=guesses+1;
  writeln;
  write("&2Guess # &F",guesses,"&2: (&F1&2-&F1000&2) ? &F");
  w:=atoi(readln());
  if w>num then writeln("&BLower!"); endif;
  if w<num then writeln("&BHigher!"); endif;
  if w=num then writeln("&BCorrect!"); endif;
 until (guesses=9) or (w=num);
 if w<>num then
  writeln("&E\"I'm very sorry sir, but you seem to have lost!\"");
  user^.money:=user^.money-l;
  LogCasLose(2,l);
 else
  writeln("&E\"We have a winner!\"");
  user^.money:=user^.money+l;
  LogCasWin(2,l);
 endif;
end guess_number;

procedure slotsym(n: integer): string;
begin;
 switch n of
  case 1: return strcat("&CCherry","");
  case 2: return strcat("&BOrange","");
  case 3: return strcat("&1Grape ","");
  case 4: return strcat("&EBell  ","");
  case 5: return strcat("&91-bar ","");
  case 6: return strcat("&92-bar ","");
  case 7: return strcat("&93-bar ","");
  case 8: return strcat("&FMega  ","");
 end;
end slotsym;

procedure slotcontinue(n: integer): boolean;
begin;
 if (user^.money<n) then
  writeln("&CYou're broke! Better find a cheaper slot machine!");
  return false;
 else
  write("&3You have &F$",user^.money,"&3. Give it &F$",n,"&3 and pull the handle (&FY&3/&FN&3) ? &F");
  return checkyesno("");
 endif;
end slotcontinue;

(* The following routines attempt to set up some fairly realistic odds.
   (i.e. Cherries appear more often than 3-Bars). It's a simplistic approach
   and not entirely realistic. It should really be re-written to use the
   actual symbols off of a real-world slot machine.
*)

procedure choosesym_side(megajack: boolean): integer; (* used for reels 1 & 3 *)
begin
 switch randi(cond(megajack,20,19)) of
  case 0: return 1;   (* cherry *)
  case 1: return 2;
  case 2: return 2;
  case 3: return 2;
  case 4: return 2;
  case 5: return 2;
  case 6: return 3;
  case 7: return 3;   (* Grape *)
  case 8: return 3;
  case 9: return 3;
  case 10: return 3;
  case 11: return 4;  (* Bell *)
  case 12: return 4;
  case 13: return 4;
  case 14: return 5;
  case 15: return 5;
  case 16: return 6;
  case 17: return 6;
  case 18: return 7;
  case 19: return 8;
 end;
 return 1; (* we shouldn't ever get here... but just in case... *)
end choosesym_side;

procedure choosesym_middle(megajack: boolean): integer; (* used for reel 2 *)
begin
 switch randi(cond(megajack,20,19)) of
  case 0: return 1;   (* cherry *)
  case 1: return 1;
  case 2: return 1;
  case 3: return 1;
  case 4: return 1;
  case 5: return 2;
  case 6: return 2;
  case 7: return 2;
  case 8: return 3;
  case 9: return 3;
  case 10: return 3;
  case 11: return 4;  (* Bell *)
  case 12: return 4;
  case 13: return 4;
  case 14: return 5;  (* 1-bar *)
  case 15: return 5;
  case 16: return 1;  (* Cherry; used to be 2-bar *)
  case 17: return 6;
  case 18: return 7;  (* 3-bar *)
  case 19: return 8;
 end;
 return 1; (* we shouldn't ever get here... but just in case... *)
end choosesym_middle;

procedure play_slots(money: integer; megajack: boolean);
var
 a,b,c: integer;
 payment,megajackpot: integer;
 slothack: boolean;
begin;
 GetParam("casino","megajackpot",megajackpot);
 if megajackpot<megajackpot_start then
  megajackpot:=megajackpot_start;
 endif;

 GetParam(appuser(usernum^),"slothack",slothack);
 if slothack & (~checklaptop(user^,rom_slothack)) then
  slothack:=false;
 endif;

 writeln;
 writeln("Cost per pull is &F$",money,".");
 writeln;
 writeln("&7Payoffs:");
 writeln;
 writeln("&CCherry  &7Any     &7Any     &F$",money*2);
 writeln("&CCherry  &CCherry  &7Any     &F$",money*4);
 writeln("&CCherry  &CCherry  &CCherry  &F$",money*6);
 writeln("&BOrange  &BOrange  &BOrange  &F$",money*10);
 writeln("&1Grape   &1Grape   &1Grape   &F$",money*10);
 writeln("&EBell    &EBell    &EBell    &F$",money*15);
 writeln("&9Any-Bar &9Any-Bar &9Any-Bar &F$",money*20);
 writeln("&91-bar   &91-bar   &91-bar   &F$",money*40);
 writeln("&92-bar   &92-bar   &92-bar   &F$",money*80);
 writeln("&93-bar   &93-bar   &93-bar   &F$",money*500);
 if megajack then
  writeln("&FMega    &FMega    &FMega    &F$",megajackpot," &E(Progressive)");
 endif;

 writeln;
 while slotcontinue(money) do
  user^.money:=user^.money-money;

  if megajackpot<largest(integer)-money then
   megajackpot:=megajackpot+(money div 4);
  endif;

  LogCasPlay(3,money);

  writeln;

  repeat;
   a:=choosesym_side(megajack);
   b:=choosesym_middle(megajack);
   c:=choosesym_side(megajack);

   writeln("&2ÉÍÍÍÍÍÍÑÍÍÍÍÍÍÑÍÍÍÍÍÍ»");
   writeln("&2º",slotsym(a),"&2³",slotsym(b),"&2³",slotsym(c),"&2º");
   writeln("&2ÈÍÍÍÍÍÍÏÍÍÍÍÍÍÏÍÍÍÍÍÍ¼");
   writeln;

   payment:=0;
   if a=1 then payment:=2; endif;
   if (a=1) & (b=1) then payment:=4; endif;
   if (a=1) & (b=1) & (c=1) then payment:=6; endif;
   if (a=2) & (b=2) & (c=2) then payment:=10; endif;
   if (a=3) & (b=3) & (c=3) then payment:=10; endif;
   if (a=4) & (b=4) & (c=4) then payment:=15; endif;
   if ((a=5) or (a=6) or (a=7)) &
      ((b=5) or (b=6) or (b=7)) &
      ((c=5) or (c=6) or (c=7)) then payment:=20; endif;
   if (a=5) & (b=5) & (c=5) then payment:=40; endif;
   if (a=6) & (b=6) & (c=6) then payment:=80; endif;
   if (a=7) & (b=7) & (c=7) then payment:=500; endif;
   payment:=payment*money;

   if (a=8) & (b=8) & (c=8) & (megajack) then
    payment:=megajackpot;
    megajackpot:=megajackpot_start;
   endif;

   if (payment=0) & (slothack) then
    writeln("&C(SlotHack ROM cartridge engaged)");
    if randi(1000)<5 then
     writeln;
     writeln("&3You were so excited when you began to play that you failed to notice the pit");
     writeln("bosses watching you. Two leg-breakers grab you and thrust you up against the");
     writeln("wall while a pit boss points some kind of really big weapon at you. The");
     writeln("manager approaches and says, \"You know what we do with hackers....\" They");
     writeln("take your laptop and smash it into a thousand pieces. Then they take every");
     writeln("last bit of cash that you have on you. \"Let him go, I think we're 'even'");
     writeln("now!\", says the manager as he looks over your trashed belongings.");
     nukedevice(user^.laptop);
     user^.money:=0;
     return;
    end;
   end;
  until (payment>0) or (~slothack);

  if payment=0 then
   logcaslose(3,money);
   writeln("&CNO JACKPOT PAID!");
  else
   if (payment>=50000) then
    writelog("    ",namestr(user^.alias)," won $",payment," on a slot machine!");
   endif;
   logcaswin(3,payment-money);
   writeln("&FJACKPOT PAID: $",payment,".");
   user^.money:=user^.money+payment;
  endif;
 endwhile;

 putparam("casino","megajackpot",megajackpot);
end play_slots;

procedure check_trivia(n: word): boolean;
var
 nogood: boolean;
 a: integer;
begin;
 nogood:=false;
 for a:=1 to maxtrivcheck do
  if trivcheck[a]=n then
   nogood:=true;
  endif;
 endfor;

 inc(trivcheck_ptr);
 if trivcheck_ptr>maxtrivcheck then
  trivcheck_ptr:=1;
 endif;

 trivcheck[trivcheck_ptr]:=n;
 return ~nogood;
end check_trivia;

procedure pick_trivia(maxq: word): integer;
var
 a: integer;
 trivnum: integer;
begin;
 for a:=1 to 1000 do
  trivnum:=randi(maxq)+1;
  if check_trivia(trivnum) then
   return trivnum;
  endif;
 endfor;
 return 0;
end pick_trivia;

procedure trivcontinue(n,numq: integer; var question: integer): boolean;
begin;
 writeln;
 question:=pick_trivia(numq);
 if question=0 then
  writeln("&CWe're all out of trivia for today. Come back later.");
  return false;
 elsif (user^.money<n) then
  writeln("&CYou're broke! Better find a cheaper trivia counter!");
  return false;
 else
  write("&3You have &F$",user^.money,"&3. Bet &F$",n,"&3 and test your wisdom (&FY&3/&FN&3) ? &F");
  return checkyesno("");
 endif;
end trivcontinue;

procedure trivia(money: integer);
var
 numtrivia: word;
 question,trivpay: integer;
begin;
 numtrivia:=querylodint(querylod_triviacount);
 if numtrivia=0 then
  writeln("&CWe have no trivia available today.");
 endif;

 trivpay:=querylodint(querylod_triviapay);

 writeln;
 writeln("&ATrivia costs &F$",money," &Aper question. If you win, we'll pay you &F$",
         money*trivpay,"&A.");

 while trivcontinue(money,numtrivia,question) do
  LogCasPlay(4,money);
  user^.money:=user^.money-money;
  if trytrivia(question) then
   logcaswin(4,money*trivpay-money);
   writeln("&FYou win $",money*trivpay,"!");
   user^.money:=user^.money+money*trivpay;
  else
   logcaslose(4,money);
   writeln("&CNope.");
  endif;
 endwhile;
end trivia;

procedure dots(n: word);
begin;
 while n>0 do
  dec(n);
  write(".");
  delay(350);
 end;
end dots;

procedure Casino_getstoragecoords(var z,x,y: byte): boolean;
var
 ternum: integer;
 ter: terraintype;
begin;
 for ternum:=1 to numterrain do
  getterrain(ternum,ter);
  if ter.ent=eCasStorage then
   z:=0;
   x:=0;
   y:=0;
   if enumterrain(z,x,y,ternum) then
    return TRUE;
   endif;
  endif;
 endfor;
 return FALSE;
end casino_getstoragecoords;

(* Procedure killuser_incasino. This is used when the user needs to be killed
     by a particular game. In this case, the user's inventory is deposited
     in the "Casino Storage Room". This is determined by scanning the current
     map for any tiles which have eCasStorage set for their ENT parameter.

     KillUser_SpecLoc(...) is then called to kill the user and leave his
     inventory in that specific location.
*)

procedure killuser_incasino(data: string);
var
 z,x,y: byte;
begin;
 if ~casino_getstoragecoords(z,x,y) then
  z:=user^.z;
  x:=user^.x;
  y:=user^.y;
 endif;
 killuser_specloc(usernum^,
                  user^,
                  9,
                  data,
                  z,
                  x,
                  y);
end killuser_incasino;

procedure poker_initdeck(var deck: tdeck);
var
 a,tempcard,l1,l2: integer;
begin;
 (* fill the deck with cards *)
 a:=0;
 for l1:=1 to 4 do
  for l2:=2 to 14 do
   deck.cards[a]:=l1*100+l2;
   inc(a);
  endfor;
 endfor;

 (* shuffle the deck *)
 for a:=1 to 1000 do
  l1:=randi(52);
  l2:=randi(52);
  tempcard:=deck.cards[l1];
  deck.cards[l1]:=deck.cards[l2];
  deck.cards[l2]:=tempcard;
 endfor;

 deck.current:=0;
end poker_initdeck;

procedure poker_inithand(var hand: thand);
var
 l: integer;
begin;
 for l:=0 to 4 do
  hand.cards[l]:=0;
  hand.held[l]:=false;
  hand.visible[l]:=false;
 endfor;
end poker_inithand;

procedure poker_setvisible(var hand: thand);
var
 l: integer;
begin;
 for l:=0 to 4 do
  hand.visible[l]:=true;
 endfor;
end poker_setvisible;

procedure poker_deal(var deck: tdeck; var hand: thand);
var
 a: integer;
begin;
 for a:=0 to 4 do
  if hand.held[a]=false then
   hand.cards[a]:=deck.cards[deck.current];
   inc(deck.current);
  endif;
 endfor;
end poker_deal;

procedure cardname(n: integer): string;
begin;
 n:=n mod 100;
 switch n of
  case 2:
  case 3:
  case 4:
  case 5:
  case 6:
  case 7:
  case 8:
  case 9:
  case 10: return itoa(n);

  case 11: return strfix("J");

  case 12: return strfix("Q");

  case 13: return strfix("K");

  case 14: return strfix("A");

  default: return strfix("");
 end;
end cardname;

procedure cardsuit(n: integer): string;
begin;
 n:=n div 100;
 switch n of
  case 1: return strfix("&4");

  case 2: return strfix("&4");

  case 3: return strfix("&8");

  case 4: return strfix("&8");

  default: return strfix("");
 end;
end cardsuit;

procedure poker_showheld(var hand: thand);
var
 a: integer;
begin;
 for a:=0 to 4 do
  if hand.held[a] then
   write("&C  HELD   ");
  else
   write("         ");
  endif;
 endfor;
 writeln;

 for a:=0 to 4 do
  write("&E   ",a+1,"     ");
 endfor;
 writeln;
end poker_showheld;

procedure double_showfooter(var hand: thand; usercard: integer);
var
 a: integer;
begin;
 for a:=0 to 4 do
  if (a=0) then
   write("&C Dealer  ")
  elsif (usercard=a) then
   write("&C Player  ")
  else
   write("         ");
  endif;
 endfor;
 writeln;

 write("         ");
 for a:=1 to 4 do
  write("&E   ",a+1,"     ");
 endfor;
 writeln;
end double_showfooter;

procedure poker_showhand(var hand: thand);
var
 a,line: integer;
begin;
 for line:=1 to 7 do
  for a:=0 to 4 do
   if (hand.visible[a]=false) & (line>1) & (line<7) then
    write("&7³&8°°°°°&7³");
   else
    switch line of
     case 1: write("&7ÚÄÄÄÄÄ¿");
             break;

     case 2: write("&7³&F",ljust(cardname(hand.cards[a]),2," "),"&7   ³");
             break;

     case 3: write("&7³     ³");
             break;

     case 4: write("&7³  ",cardsuit(hand.cards[a]),"&7  ³");
             break;

     case 5: write("&7³     ³");
             break;

     case 6: write("&7³   &F",rjust(cardname(hand.cards[a]),2," "),"&7³");
             break;

     case 7: write("&7ÀÄÄÄÄÄÙ");
             break;
    end;
   endif;
   write("  ");
  endfor;
  writeln;
 endfor;
end poker_showhand;

procedure CalcJackpot(var hand: thand): integer;
var
 values: array 15 of integer;
 pairs,triples,quads: integer;
 flush: boolean;
 straight: boolean;
 royal: boolean;
 found: boolean;
 lowcard,a,b: integer;
begin;
 for a:=0 to 14 do values[a]:=0; endfor;
 for a:=0 to 4 do
  inc(values[(hand.cards[a] mod 100)]);
 endfor;
 pairs:=0;
 triples:=0;
 quads:=0;
 for a:=0 to 14 do
  if values[a]=2 then
   inc(pairs);
  elsif values[a]=3 then
   inc(triples);
  elsif values[a]=4 then
   inc(quads);
  endif;
 endfor;
 royal:=true;
 flush:=true;
 for a:=0 to 4 do
  if (hand.cards[a] mod 100)<10 then royal:=false; endif;
  if (a<4) & ((hand.cards[a] div 100)<>(hand.cards[a+1] div 100)) then flush:=false; endif;
 endfor;

 lowcard:=255;
 for a:=0 to 4 do
  if lowcard>(hand.cards[a] mod 100) then lowcard:=hand.cards[a] mod 100; endif;
 endfor;

 straight:=true;
 for a:=1 to 4 do
  inc(lowcard);
  found:=false;
  for b:=0 to 4 do
   if (hand.cards[b] mod 100)=lowcard then found:=true; endif;
  endfor;
  if ~found then straight:=false; endif;
 endfor;

 if royal & flush & straight then return 9; endif;
 if flush & straight then return 8; endif;
 if (quads>0) then return 7; endif;
 if (pairs>0) & (triples>0) then return 6; endif;
 if flush then return 5; endif;
 if straight then return 4; endif;
 if (triples>0) then return 3; endif;
 if (pairs>=2) then return 2; endif;
 if (values[11]>=2) or (values[12]>=2) or (values[13]>=2) or (values[14]>=2) then return 1; endif;
 return 0;
end CalcJackpot;

procedure jackpot_name(n: integer): string;
begin;
 switch n of
  case 1: return strfix("Jacks or better");
  case 2: return strfix("2 pair");
  case 3: return strfix("3 of a kind");
  case 4: return strfix("Straight");
  case 5: return strfix("Flush");
  case 6: return strfix("Full house");
  case 7: return strfix("4 of a kind");
  case 8: return strfix("Straight flush");
  case 9: return strfix("Royal flush");
  default: return strfix("None");
 end;
end jackpot_name;

procedure jackpot_mult(n: integer): integer;
begin;
 switch n of
  case 1: return 1;
  case 2: return 2;
  case 3: return 3;
  case 4: return 4;
  case 5: return 5;
  case 6: return 7;
  case 7: return 25;
  case 8: return 50;
  case 9: return 250;
  default: return 0;
 end;
end jackpot_mult;

procedure view_paytable;
var
 a: integer;
begin;
 writeln;
 for a:=1 to 9 do
  writeln("&D",ljust(jackpot_name(a),16," "),"&F",
          rjust(itoa(jackpot_mult(a)),4," "),"&D x your bet");
 endfor;
 writeln;
end view_paytable;

procedure play_poker;
var
 deck: tdeck;
 hand: thand;
 ch: char;
 l: integer;
 s: string;
 win,winnings: integer;
 upcards: boolean;
begin;
 writeln("&AThe game is standard draw poker. The dealer will deal out five cards to");
 writeln("you. You may then decide which card(s) you wish to keep. Press the");
 writeln("corresponding hold button for each card you wish to keep. Then press deal");
 writeln("and the dealer will replace any cards that were not held. If your hand");
 writeln("contains a winning combination, then you will be paid accordingly.");
 writeln;
 l:=100;
 repeat;
  repeat;
   writeln("&9You currently have &F$",user^.money,"&9.");
   write("&2How much money do you wish to bet (&FQ&2=Quit,&FV&2=View Paytable) [&F",l,"&2] ? &F");
   s:=upstr(readln());
   if s="Q" then
    return;
   elsif s="V" then
    view_paytable;
   elsif s<>"" then
    l:=atoi(s);
   endif;
  until (s<>"V") & checkbet(l);
  user^.money:=user^.money-l;
  logcasplay(6,l);

  poker_initdeck(deck);
  poker_inithand(hand);
  poker_deal(deck,hand);
  poker_setvisible(hand);

  upcards:=true;
  repeat;
   if (upcards) or (querylodint(querylod_colorchg)<>1) then
    clrscr;
    poker_showhand(hand);
    upcards:=false;
   else
    write(1BX,"[4A",0DX); (* ANSI: move up four lines & CR *)
   endif;
   poker_showheld(hand);
   writeln;
   writeln("&DHand value: ",jackpot_name(calcjackpot(hand)),"               ");
   write("&2Toggle held cards [&F1&2,&F2&2,&F3&2,&F4&2,&F5&2] or [&FD&2]eal ? ");
   repeat;
    ch:=upcase(readkey());
   until (ch='1') or (ch='2') or (ch='3') or (ch='4') or (ch='5') or (ch='D');
   switch ch of
    case '1': hand.held[0]:=~hand.held[0];
              break;

    case '2': hand.held[1]:=~hand.held[1];
              break;

    case '3': hand.held[2]:=~hand.held[2];
              break;

    case '4': hand.held[3]:=~hand.held[3];
              break;

    case '5': hand.held[4]:=~hand.held[4];
              break;

    case 'D': poker_deal(deck,hand);
              upcards:=true;
              break;
   end;
  until ch='D';
  clrscr;
  poker_showhand(hand);
  poker_showheld(hand);
  win:=calcjackpot(hand);
  if (win>0) then
   winnings:=l*jackpot_mult(win);
   writeln;
   writeln("&AWe have a winner! &9<<<",jackpot_name(win),">>> &F$",winnings," &Apaid!");
   writeln;
   user^.money:=user^.money+winnings;
   logcaswin(6,winnings-l);
  else
   writeln;
   writeln("&8GAME OVER. YOU LOSE.");
   writeln;
   logcaslose(6,l);
  endif;
 until false;
end play_poker;

procedure play_double;
var
 deck: tdeck;
 hand: thand;
 ch: char;
 usercard,l: integer;
 s: string;
begin;
 writeln("&AThe game is simple. Five cards are dealt. The dealer is automatically");
 writeln("assigned the first card. You must then select one of the remaining four");
 writeln("cards, in hopes that your card will be of HIGHER value than the dealer's");
 writeln("card. If you win, we will double your money. Cards are ordered according to");
 writeln("this order: 2-3-4-5-6-7-8-9-10-J-Q-K-A.");
 writeln;
 l:=100;
 repeat;
  repeat;
   writeln("&9You currently have &F$",user^.money,"&9.");
   write("&2How much money do you wish to bet (&FQ&2=Quit) [&F",l,"&2] ? &F");
   s:=upstr(readln());
   if s="Q" then
    return;
   elsif s<>"" then
    l:=atoi(s);
   endif;
  until checkbet(l);
  user^.money:=user^.money-l;
  logcasplay(7,l);

  poker_initdeck(deck);
  poker_inithand(hand);
  poker_deal(deck,hand);
  hand.visible[0]:=true;

  clrscr;
  poker_showhand(hand);
  double_showfooter(hand,10);

  writeln;
  write("&2Guess which card [&F2&2,&F3&2,&F4&2,&F5&2] ? ");
  repeat;
   ch:=upcase(readkey());
  until (ch='2') or (ch='3') or (ch='4') or (ch='5');

  switch ch of
   case '2': usercard:=1;
             break;

   case '3': usercard:=2;
             break;

   case '4': usercard:=3;
             break;

   case '5': usercard:=4;
             break;
  end;
  clrscr;
  poker_setvisible(hand);
  poker_showhand(hand);
  double_showfooter(hand,usercard);

  if (hand.cards[usercard] mod 100)=hand.cards[0] mod 100 then
   writeln;
   user^.money:=user^.money+l;
   writeln;
   writeln("&ATie! Let's try again!");
   writeln;
  elsif (hand.cards[usercard] mod 100)>hand.cards[0] mod 100 then
   writeln;
   writeln("&AWe have a winner! You win &F$",l*2,"&A!");
   writeln;
   user^.money:=user^.money+l*2;
   logcaswin(7,l); (* l*2-l = l *)
  else
   writeln;
   writeln("&8Sorry, you lose!!!");
   writeln;
   logcaslose(7,l);
  endif;
 until false;
end play_double;

procedure play_roulette;
var
 s: string;
 winnings,l: integer;
begin;
 writeln("&ARussian Roulette is a simple game. You make a bet, we give you a revolver");
 writeln("with two bullets in random chambers of the cylinder. You spin the cylinder,");
 writeln("and pull the trigger. If you fire on an empty chamber, then we'll pay you");
 writeln("your original bet plus ",roulette_payback,"%! If you fire on a loaded chamber, then you lose");
 writeln("your bet and your LIFE!");
 writeln;
 writeln("By the way, even if you have one of those nifty EM-Warp devices, it aint");
 writeln("gonna help you out here!");
 writeln;
 repeat;
  write("&2How much money do you wish to bet (&FQ&2=Quit) ? &F");
  s:=upstr(readln());
  if s="Q" then return; endif;
  l:=atoi(s);
 until checkbet(l);
 logcasplay(5,l);
 writeln;
 write("&EPlacing two bullets in the cylinder");
 dots(5);
 writeln;
 write("&ESpinning the cylinder");
 dots(5);
 writeln;
 write("&EPulling the trigger");
 dots(5);
 delay(500);
 switch randi(6) of
  case 0:
  case 1:
     writeln(" &CBang!");
     writeln;
     writeln("&CThe last thing you hear is one of the pit bosses yelling, \"We need a cleanup");
     writeln("at the roulette table again!\"");
     user^.money:=user^.money-l;
     killuser_incasino(" bought it while playing russian roulette.");
     logcaslose(5,l);
     break;

  default: writeln(" &BClick!");
           writeln;
           writeln("&2Well, it looks like you made it.... this time....");
           winnings:=ftoi(l/100*roulette_payback);
           user^.money:=user^.money+winnings;
           logcaswin(5,winnings);
           break;
 end;
end play_roulette;

procedure go_casstorage;
begin;
 writeln("&EThis room is where the casino authorities store various items and deposit");
 writeln("the daily winnings. Items may \"show up\" here from time to time.");
end go_casstorage;

procedure maint_casino;
var
 delta: comp;
 d: devicetype;
 z,x,y: byte;
 a: integer;
begin;
 delta:=0;

 (* set delta = net player losses - net player winnings *)

 for a:=1 to querylodint(querylod_maxcasgames) do
  delta:=delta+getcasinostat(a,7)-getcasinostat(a,6);
 endfor;

 (* add a money bag to the casino storage room *)

 if (delta>0) & (casino_getstoragecoords(z,x,y)) then
  d:=createdevice(querylodint(querylod_moneybag));

  delta:=delta div 10;  (* 10% of winnings will be put in bag *)

  if delta>largest(integer) then
   d.var1:=largest(integer);
  else
   d.var1:=delta;
  endif;

  addobject(z,x,y,d);
 endif;
end maint_casino;

procedure CasinoStatistics;
var
 a: integer;
 s: string;
begin;
 clrscr;
 writeln("&9Game              Played  Won     Lost    Money Won   Money Lost");
 writeln("&9----------------  ------  ------  ------  ----------  ----------");
 for a:=1 to querylodint(querylod_maxcasgames) do
  if getcasinostat(a,1)>0 then
   switch a of
    case 1: write("&3Coin Flip         ");
            break;
    case 2: write("&3Number Guess      ");
            break;
    case 3: write("&3Slot Machines     ");
            break;
    case 4: write("&3Trivia Booths     ");
            break;
    case 5: write("&3Russian Roulette  ");
            break;
    case 6: write("&3Video Poker       ");
            break;
    case 7: write("&3Double Up         ");
            break;
   end;
   write(rjust(itoa(getcasinostat(a,1)),6," "));
   write(rjust(itoa(getcasinostat(a,2)),8," "));
   write(rjust(itoa(getcasinostat(a,3)),8," "));
   write(rjust(itoa(getcasinostat(a,4)),12," "));
   write(rjust(itoa(getcasinostat(a,5)),12," "));
   writeln;
  endif;
 endfor;
end CasinoStatistics;

