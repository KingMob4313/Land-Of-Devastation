const
 holostore_lines = 19;
 maxholoitems = 100;
 holopercent = 35;
type
 tholoitem = record
              basenum: byte;
              slotnum: byte;
             end;

 thololist = array maxholoitems of tholoitem;
var
 numholoitems: integer;
 holoinv: thololist;

procedure holostore_load;
var
 loop1,loop2: integer;
 base: pbaserec;
begin;
 numholoitems:=0;

 for loop1:=1 to numbase do
  base:=getbase(loop1);
  with b:base^ do
   if (b.active) & ((b.flags & flagbaseholo)<>0) then
    for loop2:=0 to numbforsale-1 do
     if (b.forsale[loop2].num<>0) & (numholoitems<maxholoitems) then
      inc(numholoitems);
      holoinv[numholoitems-1].basenum:=loop1;
      holoinv[numholoitems-1].slotnum:=loop2;
     endif;
    endfor;
   endif;
  endwith;
 endfor;
end holostore_load;

procedure holostore_delete(n: integer);
var
 loop1: integer;
begin;
 dec(numholoitems);
 if n<numholoitems then
  for loop1:=n to numholoitems-1 do
   holoinv[loop1]:=holoinv[loop1+1];
  endfor;
 endif;
end holostore_delete;

procedure calcholofee(n: integer): integer;
begin;
 return ftoi(n*holopercent/100);
end calcholofee;

procedure calcholoprice(n: integer): integer;
begin;
 return calcholofee(n)+n;
end calcholoprice;

procedure holostore_display(first: integer);
var
 loop1: integer;
 base: pbaserec;
begin;
 clrscr;
 writeln("&9### Fortress name        Object Description   Price");
 writeln("--- -------------------- -------------------- ------------");
 write("&7");
 if numholoitems=0 then
  writeln("&CThere are no items available in the holostore!");
 else
  for loop1:=first to min(first+holostore_lines-1,numholoitems-1) do
   with h:holoinv[loop1] do
    base:=getbase(h.basenum);
    writeln(rjust(itoa(loop1+1),3," ")," ",
            ljust(base^.name,20," ")," ",
            ljust(devname(base^.forsale[h.slotnum]),20," ")," ",
            rjust(itoa(calcholoprice(base^.saleprice[h.slotnum])),12," "));
   endwith;
  endfor;
 endif;
end holostore_display;

procedure holostore_buy(i: integer);
var
 freeinv: integer;
 base: pbaserec;
begin;
 if (i<1) or (i>numholoitems) then
  writeln("&CThat is an invalid item number.");
 elsif ~findfreeinv(user^,freeinv) then
  writeln("&CThere is no room in your inventory.");
 else
  writeln("&E[All holostore prices reflect a ",holopercent,"% transaction fee]");
  writeln;
  dec(i);
  with h:holoinv[i] do
   base:=getbase(h.basenum);
   with o:base^.forsale[h.slotnum] do
    with price:base^.saleprice[h.slotnum] do
     write("&3Purchase a &F",devname(o),"&3 for &F$",calcholoprice(price)," &3(&FY&3/&FN&3) ? &F");
     if checkyesno("") then
      if user^.money<calcholoprice(price) then
       writeln("&CYou don't have enough cash!");
      else
       writeln;
       writeln("&BTransaction completed. ",devname(o)," has been teleported into your inventory.");
       writeln("   &F$",price," &Bcredited to ",baseownername(base^)," at ",base^.name,".");
       writeln("   &F$",calcholofee(price)," &Bcredited to HoloStore, inc., a division of ACME Services, inc.");
       base^.money:=base^.money+price;
       user^.money:=user^.money-calcholoprice(price);
       user^.devices[freeinv]:=o;
       nukedevice(o);
       holostore_delete(i);
      endif;
     endif;
    endwith;
   endwith;
  endwith;
 endif;
end holostore_buy;

procedure holostore;
var
 s: string;
 i: integer;
 done: boolean;
 first: integer;
begin;
 writeln("&BAs you approach the building, a computerized voice greets you: \"Welcome to");
 writeln("HoloStore Incorporated, proudly serving the entire wastelands area. Products");
 writeln("area available via express teleportation from numerous fortresses scattered");
 writeln("throughout the entire wastelands area... And at a very reasonable ",holopercent,"%");
 writeln("acquisition fee! Why venture all over the wastelands when HoloStore");
 writeln("incorporated will do your shopping for you!\"");
 waitkey;
 first:=0;
 done:=false;
 holostore_load;
 holostore_display(first);
 repeat;
  writeln;
  write("&3Holostore: Type item number (&FQ&3=Quit,&FN&3=Next,&FP&3=Prev,&FR&3=Redisplay) ? &F");
  s:=upstr(readln());
  i:=atoi(s);
  if (i>0) then
   holostore_buy(i);
   if first>=numholoitems then first:=max(0,numholoitems-1); endif;
  else
   switch s of
    case "Q": done:=true;
              break;

    case "N": if first+holostore_lines<numholoitems then
               first:=first+holostore_lines;
               holostore_display(first);
              else
               writeln("&CThere are no more pages to display.");
              endif;
              break;

    case "P": if first>0 then
               first:=max(0,first-holostore_lines);
               holostore_display(first);
              else
               writeln("&CThere are no previous pages to display.");
              endif;
              break;

    case "R": holostore_display(first);
              break;

   end;
  endif;
 until done;
end holostore;



