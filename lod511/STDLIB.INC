#ifndef stdlib

#define stdlib

#define assert(condition) \
  if (~(condition)) then \
    writeln("assertion failed: File ",#file,", line ",#linenum,": ",#condition); \
    halt; \
  endif;

#define visassert(condition) \
  if (~(condition)) then \
    writeln("assertion failed: File ",#file,", line ",#linenum,": ",#condition); \
    halt; \
  else \
    writeln("assertion passed: File ",#file,", line ",#linenum,": ",#condition); \
  endif;

#define inc(i) i:=i+1
#define dec(i) i:=i-1

#define min(a,b) (temp(t1,a,temp(t2,b,cond(t1<t2,t1,t2))))
#define max(a,b) (temp(t1,a,temp(t2,b,cond(t1>t2,t1,t2))))

#define upcase(ch) (temp(t1,ch,cond((t1>='a') & (t1<='z'),t1-'a'+'A',t1)))
#define locase(ch) (temp(t1,ch,cond((t1>='A') & (t1<='Z'),t1-'A'+'a',t1)))

procedure upstr(s: string): string;
var
 i: integer;
begin;
 for i:=1 to length(s) do
   s[i]:=upcase(s[i]);
 endfor;
 return s;
end upstr;

procedure lostr(s: string): string;
var
 i: integer;
begin;
 for i:=1 to length(s) do
   s[i]:=locase(s[i]);
 endfor;
 return s;
end lostr;

procedure rpad(s: string; num: integer; s2: string): string;
begin;
 if length(s)>num then
  s[0]:=chr(num)
 else
  while length(s)<num do
    s:=strcat(s,s2);
  endwhile;
 endif;
 return s;
end rpad;

#define ljust(s,num,s2) rpad(s,num,s2)

procedure lpad(s: string; num: integer; s2: string): string;
begin;
 if length(s)>num then
  s[0]:=chr(num)
 else
  while length(s)<num do
    s:=strcat(s2,s);
  endwhile;
 endif;
 return s;
end lpad;

#define rjust(s,num,s2) lpad(s,num,s2)

external 98 procedure itoa(i: integer): string;
external 99 procedure ftoi(f: double): integer;

#endif