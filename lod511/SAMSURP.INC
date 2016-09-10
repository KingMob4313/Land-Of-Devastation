(* Sam's Super Surplus Store
   Scott M. Baker, 1995

   Purpose: Sam's Surplus stores are primarily intended to be used as a quick
            way for users to sell off junk without having to navigate all of
            the city maps.

            Sam's also has a daily special at 50% off. The particular Sam's
            will not purchase it's special back, but one Sam's may purchase
            the specials from another Sam's, if the user is willing to go
            to the work of carrying the items between them.

            Also provided is LAPTOP_SAMSURP which lists all of the Sam's
            locations and attributes, and USE_SAMLINK which supports a device
            for linking up with a Sam's remotely.

   Notes:   Each Sam's Surplus has a "spec" entry in registry.dat
            corresponding to the daily special. These entries are keyed to
            the X/Y/Z locations of the Sam's Surplus stores.

            A limit is provided on the number of specials per day that the
            user may purchase. This is done to prevent abuse of the SSSS
            by re-selling the specials to other SSSS stores, city stores,
            etc.
*)


const
 sam_specpercent = 60; (* percentage to sell special for *)
 sam_maxspecials = 15; (* number of specials per day that user can buy *)

(* Macro sam_devapp_ok. This macro is used by maint_samsurplus to determine
     which devices are eligable for daily specials.
*)

#define sam_devapp_ok(d) (\
               ((d.devapp=devapp_medkit) or               \
               (d.devapp=devapp_weapon) or               \
               (d.devapp=devapp_rweapon) or              \
               (d.devapp=devapp_armor) or                \
               (d.devapp=devapp_lrscan) or               \
               (d.devapp=devapp_ammo) or                 \
               (d.devapp=devapp_medline) or              \
               (d.devapp=devapp_rations) or              \
               (d.devapp=devapp_scanner) or              \
               (d.devapp=devapp_stealth) or              \
               (d.devapp=devapp_grenade) or              \
               (d.devapp=devapp_warper) or               \
               (d.devapp=devapp_laptop) or               \
               (d.devapp=devapp_rom) or                  \
               (d.devapp=devapp_vortex) or               \
               (d.devapp=devapp_mine) or                 \
               (d.devapp=devapp_boat) or                 \
               (d.devapp=devapp_mapper) or               \
               (d.devapp=devapp_waypointman) or          \
               (d.devapp=devapp_hypervault) or           \
               (d.devapp=devapp_tricorder) or            \
               (d.devapp=devapp_chem) or                 \
               (d.devapp=devapp_labeller) or             \
               (d.devapp=devapp_shield))                 \
               & (~teststore(d,store_basemisc))          \
               )

#define sam_specid(z,x,y) ("spec"+itoa(z)+"_"+itoa(x)+"_"+itoa(y))
#define sam_bought ("SamSpecBought")

(* Procedure maint_samsurplus. Loads new daily specials for the Sam's Surplus
     stores. The bought counters are reset via a call to delparam_global.
*)

procedure maint_samsurplus;
var
 numdevices: integer;
 devnum: integer;
 z,x,y: byte;
 ternum: integer;
 ter: terraintype;
begin;
 (* reset bought counters for all users *)
 delparam_global(sam_bought);

 (* loop through terrain and setup new daily specials *)
 for ternum:=1 to numterrain do
  getterrain(ternum,ter);
  if ter.ent=eSSSS then
   z:=0;
   x:=0;
   y:=0;
   while enumterrain(z,x,y,ternum) do
    repeat;
     devnum:=randi(querylodint(querylod_numdev))+1;
     with d:devicedef^[devnum]^ do
      if (d.num=0) or (~sam_devapp_ok(d)) or (d.cost<10000) then
       devnum:=0;
      endif;
     endwith;
    until devnum<>0;
    putparam("SAMSURP",sam_specid(z,x,y),devnum);
   endwhile;
  endif;
 endfor;
end maint_samsurplus;

(* Procedure samsurp_list. Lists the locations and attributes of all of the
     Sam's Surplus stores. Basically just a helper procedure for
     laptop_samlist and use_samlink.
*)

procedure samsurp_list;
var
 z,x,y: byte;
 ternum: integer;
 ter: terraintype;
 counter: integer;
 devnum: integer;
begin;
 writeln("&9## Z  X   Y   Buy  Special");
 writeln("-- -- --- --- ---- ----------------------");
 counter:=0;
 for ternum:=1 to numterrain do
  getterrain(ternum,ter);
  if ter.ent=eSSSS then
   z:=0;
   x:=0;
   y:=0;
   while enumterrain(z,x,y,ternum) do
    inc(counter);
    getparam("SAMSURP",sam_specid(z,x,y),devnum);

    (* split the following write statement in two to avoid a stack overflow *)

    write("&F",
          rjust(itoa(counter),2," "),
          " &3",
          rjust(itoa(z),2," "));

    writeln(rjust(itoa(x),4," "),
            rjust(itoa(y),4," "),
            "&E",rjust(itoa(ter.evr),4," "),"% ",
            "&2",devname_num(devnum));
   endwhile;
  endif;
 endfor;
end samsurp_list;

(* Procedure laptop_samsurp. Called when the user uses the samlist rom
     cartridge. Simply outputs a textual intro and executes samsurp_list to
     display the actual list.
*)

procedure laptop_samsurp;
begin;
 writeln("&7Welcome to Sam's Super Surplus Store's automated electronic catalog, featuring");
 writeln("the locations, purchase percentages, and special item sale listings for all of");
 writeln("our outlets. Remember, wherever you go, there's always a Sam's Surplus");
 writeln("located conventiently near you!");
 writeln;
 samsurp_list;
end laptop_samsurp;

(* Procedure samsurp_checkspecok. Checks how many times the user has
     purchased Sam's Specials the current day and increments the count
     if appropriate.
*)

procedure samsurp_checkspecok(z,x,y: byte; devnum: integer): boolean;
var
 numpurchased: integer;
begin;
 getparam(appuser(usernum^),sam_bought,numpurchased);
 if numpurchased=sam_maxspecials then
  return FALSE;
 else
  inc(numpurchased);
  putparam(appuser(usernum^),sam_bought,numpurchased);
  return TRUE;
 endif;
end samsurp_checkspecok;

(* Procedure go_samsurplus. Performs the main routine of Sam's Surplus. The
     z/x/y coords are important because the "special" is keyed to the
     location of the store. The "sam_percent" is the percentage that will
     be paid for used items.
*)

procedure go_samsurplus(z,x,y: byte; sam_percent: integer);
var
 special: devicetype;
 specprice: integer;
 done: boolean;
 sellprice: integer;
 i: integer;
 s: string;
 special_num: integer;
 freeinv: integer;
begin;
 if ~verifyparam("samsurp",sam_specid(z,x,y)) then
  maint_samsurplus;
 endif;

 getparam("samsurp",sam_specid(z,x,y),special_num);
 special:=createdevice(special_num);

 writeln("&2\"Welcome to Sam's Super Surplus Store (SSSS), where the rule is if you can");
 writeln("pick it up, we'll buy it from you. We're currently prepared to pay you &F",sam_percent,"%");
 writeln("&2of new cost for anything you've got.\"");
 writeln;
 writeln("&2\"Our special today is ",adev(special)," &F",devname(special),
         "&2. We'll give you one at &F",100-sam_specpercent,"% &2off!\"");

 done:=false;
 repeat;
  writeln;
  write("&3Sell which item (&F?&3=List,&FB&3=Buy Special,&FQ&3=Quit) ? &F");
  s:=upstr(readln());
  i:=atoi(s);
  if s="?" then
   listuserinv(user^,false);
  elsif (s="B") then
   writeln;
   specprice:=ftoi(devicedef^[special.num]^.cost*sam_specpercent/100);
   write("&3Buy ",adev(special)," &F",devname(special),"&3 for &F$",
         specprice," &3(&FY&3/&FN&3) ? ");
   if checkyesno("") then
    if user^.money<specprice then
     writeln("&C\"Whoa there partner, you don't got enough cash!\"");
    elsif ~findfreeinv(user^,freeinv) then
     writeln("&CYou don't have any room in your inventory!");
    elsif ~samsurp_checkspecok(z,x,y,special_num) then
     writeln("&C\"Sorry partner... I can only sell ya so many of these. Word has gotten around");
     writeln("that you've been buyin' specials right and left. While we respect your");
     writeln("patronage, we have to maintain some limits so we may continue to serve our");
     writeln("other customers.\"");
    else
     writeln("&2\"It's yours... Have fun!\"");
     user^.money:=user^.money-specprice;
     user^.devices[freeinv]:=special;
    endif;
   endif;
  elsif (s="Q") or (s="") then
   done:=true;
  elsif (i>0) & (i<=numudev) then
   writeln;
   with ud:user^.devices[i-1] do
    if ud.num=0 then
     writeln("&CThat inventory slot is empty!");
    elsif ud.num=special.num then
     writeln("&C\"Sorry, those are on special today. We're not interested in buying any.\"");
    else
     if ud.devapp=devapp_chest then
      writeln("&A\"Well there, since this thing is full of money, we can pay you full price!\"");
      writeln;
      sellprice:=itemval(ud);
     else
      sellprice:=ftoi(itemval(ud)*sam_percent/100);
     endif;
     write("&B\"Will ya take &F$",sellprice," &Bfor it?\" (&FY&B/&FN&B) ? ");
     if checkyesno("") then
      user^.money:=user^.money+sellprice;
      nukedevice(ud);
     endif;
    endif;
   endwith;
  else
   writeln("&CInvalid option.");
  endif;
 until done;
end go_samsurplus;

(* Procedure go_samnumber. Enters a particular sam's surplus based on the
     number of the store. It does this by finding the location and percent
     of the desired store and then calls go_samsurplus.
*)

procedure go_samnumber(n: word): boolean;
var
 z,x,y: byte;
 ternum: integer;
 ter: terraintype;
 counter: integer;
begin;
 counter:=0;
 for ternum:=1 to numterrain do
  getterrain(ternum,ter);
  if ter.ent=eSSSS then
   z:=0;
   x:=0;
   y:=0;
   while enumterrain(z,x,y,ternum) do
    inc(counter);
    if counter=n then
     go_samsurplus(z,x,y,ter.evr);
     return TRUE;
    endif;
   endwhile;
  endif;
 endfor;
 return FALSE;
end go_samnumber;

(* Procedure use_samlink. The samlink is a special device that allows remote
     linkup with any desired sam's surplus store. The DECUSE field of the
     devicedef entry should be set to FALSE, because use_samlink has built-
     in check of the useleft variable.
*)

procedure use_samlink(var d: devicetype);
var
 done: boolean;
 s: string;
 a: integer;
begin;
 writeln("&3Welcome to the Sam's Super Surplus Store remote linkup. This nifty gadget is");
 writeln("capable of trasnferring items to and from the Sam's Surplus of your choice.");
 writeln;
 writeln("&2There area &F",d.useleft," &2applications remaining.");
 writeln;
 samsurp_list;
 done:=false;
 repeat;
  writeln;
  write("&BLink-up to which Sam's Surplus (&FL&B=List,&FQ&B=Quit) ? &F");
  s:=upstr(readln());
  a:=atoi(s);
  if (s="Q") then
   done:=true;
  elsif (s="?") or (s="L") then
   writeln;
   samsurp_list;
  elsif (a>0) then
   writeln;
   if go_samnumber(a) then
    decuseleft(d);
    done:=true;
   else
    writeln("&CInvalid store number.");
   endif;
  endif;
 until done;
end use_samlink;