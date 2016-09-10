procedure keyring_view(var d: devicetype);
var
 devnum,count,loop1: integer;
begin;
 writeln;
 count:=0;
 for loop1:=1 to querylodint(querylod_numkeys) do
  if testcap(d,loop1-1) then
   devnum:=querylodint(querylod_keymap+loop1-1);
   inc(count);
   writeln("&F",count,"&B] ",devname_num(devnum));
  endif;
 endfor;
 if count=0 then
  writeln("&CThe keyring is empty.");
 endif;
end keyring_view;

procedure keyring_add(var d: devicetype);
var
 s: string;
 a,found,loop1: integer;
 done: boolean;
begin;
 done:=false;
 repeat;
  writeln;
  write("&BInsert which item into the key ring (&F?&B=List,&FQ&B=Quit) ? &F");
  s:=upstr(readln());
  a:=atoi(s);
  if (s="?") or (s="L") then
    ListUserInv(user^,false);
  elsif (s="Q") or (s="") then
   done:=true;
  elsif (a>0) & (a<=numudev) then
   dec(a);   (* because inventory is a zero-based array *)
   if user^.devices[a].num=0 then
    writeln("&CThat inventory slot is empty! Stop fooling around!");
   else
    found:=0;
    for loop1:=1 to querylodint(querylod_numkeys) do
     if querylodint(querylod_keymap+loop1-1)=user^.devices[a].num then
      found:=loop1;
     endif;
    endfor;
    if found=0 then
     writeln("&CYou can find no conceivable way to attack that device to the keyring!");
    elsif testcap(d,found-1) then
     writeln("&CYou've already got one of those on the keyring!");
    else
     writeln("&EKey transferred.");
     done:=true;
     setcap(d,found-1);
     nukedevice(user^.devices[a]);
    endif;
   endif;
  endif;
 until done;
end keyring_add;

procedure keyring_remove(var d: devicetype);
var
 s: string;
 a: integer;
 done: boolean;
 count,found,devnum,loop1,freeinv: integer;
begin;
 done:=false;
 repeat;
  writeln;
  write("&BRemove which item from the key ring (&F?&B=List,&FQ&B=Quit) ? &F");
  s:=upstr(readln());
  a:=atoi(s);
  if (s="?") or (s="L") then
   keyring_view(d);
  elsif (s="Q") or (s="") then
   done:=true;
  elsif (a>0) then
   count:=0;
   found:=0;
   for loop1:=1 to querylodint(querylod_numkeys) do
    if testcap(d,loop1-1) & (found=0) then
     devnum:=querylodint(querylod_keymap+loop1-1);
     if devnum<>0 then
      inc(count);
      if count=a then found:=loop1; endif;
     endif;
    endif;
   endfor;
   if (found=0) then
    writeln("&CThat item is not on the key ring!");
   elsif ~findfreeinv(user^,freeinv) then
    writeln("&CThere is no room in your inventory!");
   else
    writeln("&EKey transferred.");
    done:=true;
    resetcap(d,found-1);
    user^.devices[freeinv]:=createdevice(devnum);
   endif;
  endif;
 until done;
end keyring_remove;

procedure use_keyring(var d: devicetype);
var
 done: boolean;
begin;
 done:=false;
 repeat;
  write("&3Keyring: &FA&3]dd, &FR&3]emove, &FV&3]iew, &FQ&3]uit ? &F");
  switch upcase(readkey()) of

    case 'A': writeln("&FAdd");
              keyring_add(d);
              writeln;
              break;

    case 'R': writeln("&FRemove");
              keyring_remove(d);
              writeln;
              break;

    case 'V': writeln("&FView");
              keyring_View(d);
              writeln;
              break;

    default:  writeln("&FQuit");
              done:=true;
              resetneedpause;
              break;
  end;
 until done;
end use_keyring;
