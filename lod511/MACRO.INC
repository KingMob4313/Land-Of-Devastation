#define macrotag(n) ("macro_"+itoa(n))

const
 num_macros = 7;

procedure macromap(n: integer): char;
begin;
 n:=n+64;
 return chr(n);
end macromap;

procedure macro_list;
var
 loop1: integer;
 macro: string;
begin;
 for loop1:=1 to num_macros do
  if ~getparam(appuser(usernum^),macrotag(loop1),macro) then
   macro:="&C<none recorded>";
  endif;
  writeln("&FCTRL-",macromap(loop1),"&7 ... &E",macro);
 endfor;
end macro_list;

procedure macro_record(n: integer);
var
 macro: string;
 loop1: integer;
begin;
 writeln;
 writeln("&7Recording macro for &FCTRL-",macromap(n),"&7.");
 writeln;
 writeln("&2Enter macro below. Press <enter> when you have complete the macro definition.");
 writeln("you may embed a carraige return by using the pipe \"|\" symbol.");
 writeln;
 write("&B]&F");
 macro:=readln();
 if macro="" then
  writeln("&CAborted");
 else
  putparam(appuser(usernum^),macrotag(n),macro);
 endif;
end macro_record;

procedure laptop_macro;
var
 done: boolean;
 ch: char;
begin;
 macro_list;
 repeat;
  writeln;
  write("&3Type macro key to record or (&FQ&3=Quit,&FL&3=List) ? &F");
  ch:=upcase(readraw());
  switch ch of
   case 'Q': writeln("&FQuit.");
             resetneedpause;
             done:=true;
             break;

   case 'L': writeln("&FList.");
             writeln;
             macro_list;
             break;

   case 1X:
   case 2X:
   case 3X:
   case 4X:
   case 5X:
   case 6X:
   case 7X:  writeln("&FRecord.");
             macro_record(ord(ch));
             break;

   default:  writeln;
             break;
  end;
 until done;
end laptop_macro;

