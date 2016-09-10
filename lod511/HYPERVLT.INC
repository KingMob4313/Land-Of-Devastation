(* HyperVault
   Scott M. Baker, 1995

   Purpose: The hypervault is a method for players to store items in
            "hyperspace". Items can be transferred from the user's inventory
            to and from this special hypserspace vault.

   Notes:   The items in the vault are stored directly in the LOD data
            registry. The registry is provided as a place for programmers
            to store data to disk that may be reloaded later.

            Registry data is addressed by means of an "appcode" and a
            "tag". For more information, please see LGPL.DOC and/or
            WARPWAY.INC which is fully commented and uses the registry
            as well.

   Format:  The Hypervault stores it's devices as a series of integers
            corresponding to the fields of the devicetype record. The Z,X,
            and Y fields of the devicetype record are not used and are
            set to zero.

            The hypervault writes one device per line to the registry in this
            format. A typical registry entry may be:

               [USER1]
               HYPERVAULT1 3 0 0 0 1 9
               HYPERVAULT2 53 0 0 0 11 5
*)

(* htag: a quick macro for generating tags while using the getparam and
     putparam calls.
*)
#define htag(n) ("hypervault"+itoa(n))

(* maxvaultslots: The number of slots that will be supported in the
     hypervault. It's always a good idea to code things like these as
     constants to make it easy to change them in the future.
*)
const
 maxvaultslots = 8;

type
 hypervault_list = array maxvaultslots of devicetype;

(* RetrieveDevice: reads a device from the registry into a devicetype
     record. All fields of the devicetype record are intialized to
     zero. Then, if a device exists in the registry (via a call to
     verifyparam), the device is loaded from the registry into the
     devicetype record.

     A boolean TRUE is returned if a valid device was loaded (i.e. the
     device's number is nonzero). FALSE is returned if the device is
     empty.
*)
procedure RetrieveDevice(n: integer; var d: devicetype): boolean;
begin;
 d.num:=0;
 d.var1:=0;
 d.var2:=0;
 d.var3:=0;
 d.x:=0;
 d.y:=0;
 d.z:=0;
 d.devapp:=0;
 d.useleft:=0;
 if verifyparam(appuser(usernum^),htag(n)) then
  GetParam(appuser(usernum^),
           htag(n),
           d.num,d.var1,d.var2,d.var3,d.devapp,d.useleft);
 endif;
 return (d.num<>0);
end retrievedevice;

(* RemoveDevice: Removes a device from the hypervault by deleting the
     associated entry from the registry.
*)
procedure RemoveDevice(n: integer);
begin;
 DeleteParam(appuser(usernum^),htag(n));
end removedevice;

(* StoreDevice: Writes a devicetype record to the registry.
*)
procedure StoreDevice(n: integer; var d: devicetype);
begin;
 if d.num=0 then
  removedevice(n);  (* storing a nil device is the same as removing it! *)
 else
  PutParam(appuser(usernum^),
           htag(n),
           d.num,d.var1,d.var2,d.var3,d.devapp,d.useleft);
 endif;
end storedevice;

(* FindFreeSlot: Scans all of the possible hypervault slots in the
     registry and returns the number of the first free slot in "n". Returns
     a boolean TRUE if a record was found, or FALSE if not.
*)
procedure FindFreeSlot(var n: integer): boolean;
var
 d: devicetype;
 loop1: integer;
begin;
 for loop1:=1 to maxvaultslots do
  if ~retrievedevice(loop1,d) then
   n:=loop1;
   return TRUE;
  endif;
 endfor;
 n:=0;
 return FALSE;
end FindFreeSlot;

procedure hypervault_search(s: string): integer;
var
 loop1: integer;
 d: devicetype;
begin;
 for loop1:=1 to maxvaultslots do
  retrievedevice(loop1,d);
  if (submatch(s,devname(d))) then
   return loop1;
  endif;
 endfor;
end hypervault_search;

(* ListVault: Lists all items in the hypervault.
*)
procedure ListVault;
var
 loop1: integer;
 d: devicetype;
begin;
 for loop1:=1 to maxvaultslots do
  retrievedevice(loop1,d);
  if d.num=0 then
   writeln("&E",loop1,"&7: no device stored.");
  else
   writeln("&E",loop1,"&7: device stored: &F",devname(d),"&7.");
  endif;
 endfor;
end ListVault;

procedure TakeFromVault;
var
 i: integer;
 d: devicetype;
 s: string;
 freeinv: integer;
 done: boolean;
begin;
 if ~findfreeinv(user^,freeinv) then
  writeln("&CThere is no room in your inventory for another device!");
  return;
 endif;

 done:=false;
 repeat;
  writeln;
  write("&3Take which device from vault (&FL&3=List,&FC&3=Show Char,&FQ&3=Quit) ? &F");
  s:=upstr(readln());
  i:=atoi(s);
  if (s="L") then
   ListVault;
  elsif (s="C") then
   ShowChar(user);
  elsif (i>0) & (i<=maxvaultslots) then
   if retrievedevice(i,d) then
    writeln("&7",devname(d)," transferred to inventory.");
    user^.devices[freeinv]:=d;
    removedevice(i);
    done:=true;
   else
    writeln("&CThere is no device present in that slot.");
   endif;
  else
   writeln("&CAborted.");
   done:=true;
  endif;
 until done;
end TakeFromVault;

procedure PutToVault;
var
 i: integer;
 d: devicetype;
 s: string;
 freeslot: integer;
 done: boolean;
begin;
 if ~findfreeslot(freeslot) then
  writeln("&CThere are no free slots in the hypervault!");
  return;
 endif;

 done:=false;
 repeat;
  writeln;
  write("&3Put which device into vault (&FL&3=List,&FC&3=Show Char,&FQ&3=Quit) ? &F");
  s:=upstr(readln());
  i:=atoi(s);
  if (s="L") then
   ListUserInv(user^,false);
  elsif (s="C") then
   ShowChar(user);
  elsif (i>0) & (i<=numudev) then
   with ud:user^.devices[i-1] do
    if ud.num=0 then
     writeln("&CThere is no device present in that inventory slot.");
    elsif (ud.devapp<>devapp_hypervault) or
          (checkyesno("&CAre you SURE you want to put a hypervault in hyperspace (&FY&C/&FN&C) ? ")) then
     writeln("&7",devname(ud)," transferred to vault.");
     storedevice(freeslot,ud);
     ud.num:=0;
     done:=true;
    endif;
   endwith;
  else
   writeln("&CAborted.");
   done:=true;
  endif;
 until done;
end PutToVault;

procedure HyperVault_Use;
var
 i: integer;
 d: devicetype;
 s: string;
 freeinv: integer;
 done: boolean;
begin;
 done:=false;
 repeat;
  writeln;
  write("&3Use which device in vault (&FL&3=List,&FQ&3=Quit) ? &F");
  s:=upstr(readln());
  i:=atoi(s);
  if (s="L") or (s="?") then
   ListVault;
  elsif (s="Q") or (s="") then
   writeln("&CAborted.");
   done:=true;
  else
   i:=atoi(s);
   if (i=0) then i:=hypervault_search(s); endif;
   if (i<1) or (i>maxvaultslots) then
    writeln("&CInvalid selection.");
   elsif ~retrievedevice(i,d) then
    writeln("&CThere is no device present in that slot.")
   elsif d.devapp=devapp_hypervault then
    writeln("&CDon't you think using a hypervault within a hypervault would be kind of");
    writeln("redundant???");
   else
    done:=true;
    useitem(d);
    storedevice(i,d);
   endif;
  endif;
 until done;
end Hypervault_Use;

procedure hypervault_sort1(var h: hypervault_list; l,r: integer);
var
  i,j,x: integer;
  y: devicetype;
begin
  write(".");
  i:=l; j:=r; x:=devsort_key(h[(l+r) div 2]);
  repeat
    while devsort_key(h[i])<x do inc(i); endwhile;
    while x<devsort_key(h[j]) do dec(j); endwhile;
    if i<=j then
     y:=h[i]; h[i]:=h[j]; h[j]:=y;
     inc(i);
     dec(j);
    end;
  until i>j;
  if l<j then hypervault_sort1(h,l,j); endif;
  if i<r then hypervault_sort1(h,i,r); endif;
end hypervault_sort1;

procedure hypervault_sort;
var
 h: hypervault_list;
 a: integer;
begin;
 write("&ESorting ");
 for a:=1 to maxvaultslots do
  write(".");
  retrievedevice(a,h[a-1]);
 endfor;
 hypervault_sort1(h,0,maxvaultslots-1);
 for a:=1 to maxvaultslots do
  write(".");
  storedevice(a,h[a-1]);
 endfor;
 writeln;
end hypervault_sort;

procedure use_hypervault;
begin;
 listvault;
 loop;
  writeln;
  write("&3Hypervault: &FL&3]ist, &FG&3]et, &FP&3]ut, &FS&3]ort, &FU&3]se, &FQ&3]uit ? &F");
  switch upcase(readkey()) of
   case 'L': writeln("List");
             writeln;
             ListVault;
             break;

   case 'G': writeln("Get");
             TakeFromVault;
             break;

   case 'U': writeln("Use");
             hypervault_use;
             break;

   case 'D': (* We'll let the user use "D" for drop if he wants *)
   case 'P': writeln("Put");
             PutToVault;
             break;

   case 'S': writeln("Sort");
             hypervault_sort;
             break;

   case 'Q': writeln("Quit");
             resetneedpause;
             return;
             break;

   default:  writeln("&CWhat???");
             break;
  end;
 endloop;
end use_hypervault;


