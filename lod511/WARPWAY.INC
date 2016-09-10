(* Warper Waypoint Manager
   Scott M. Baker, 1995

   Purpose: The waypoint manager will allow a player to store different
            warper anchor points. "Anchor points" are the destination
            coordinates that the warper is set to. Anchor points can be
            set by the player by using the EM-Warp ROM.

            The waypoint manager will let the player swap the current
            anchor point in/out of a bank of 5 stored values.

   Notes:   The waypoint manager makes heavy use of LOD's data registry.
            The data registry is provided as a way for people to save data
            to disk so that they may load it again later.

            Data in the registry is addressed by means of an applications
            code (appcode) and a tag. The format of the file looks like the
            following:

                [appcode]
                tag data
                tag data
                tag data

            You may use whatever you wish for your own appcode and tag
            parameters. The macro "appuser" is defined to generate an
            appcode from a user number.

            The following procedures are used to access the registry:

                putparam(appcode,tag,...)
                getparam(appcode,tag,...)
                verifyparam(appcode,tag): boolean
                deleteparam(appcode,tag)

            (the ... signifies that multiple arguments may be added, just
             like the write and writeln commands)
*)

const
 (* It's always good to code limits and such as constants in case you want
    to change them later. For example, to make the waypoint manager store
    10 waypoints, the only line that needs to be changed is "maxwaypoints=5"
 *)
 maxwaypoints=5;
type
 (* Here we define a record type to hold a waypoint. Since a warper anchor
    consists of three coordinates (z,x, and y), we'll group them all into
    one record for convenient storage
 *)
 twaypoint = record
              z,x,y: byte;
             end;

 (* We want to be able to store multiple waypoints, so we'll define an
    array to hold several of them. In LGPL, all arrays are zero-based. That
    means the elements will be numbered (0,1,2,...,4).
 *)
 twaypointarray = array maxwaypoints of twaypoint;
var
 (* Lastly, we need to create a variable of type "twaypointarray" to hold
    the data for the waypoints themselves.
 *)
 waypoints: twaypointarray;

(* Procedure load_waypoints: This procedure will load the waypoints for the
     currently defined user into the waypoints variable. The data is loaded
     from LOD's user data registry by use of the GETPARAM procedure call.
*)

procedure load_waypoints(unum: integer);
var
 loop1: integer;
begin;
 for loop1:=0 to maxwaypoints-1 do
  (* We call verifyparam to ensure that the data we want is present in the
     registry before we try to load it.
  *)
  if verifyparam(appuser(unum),"waypoint"+itoa(loop1)) then
   (* The "with" statement allows you to access a variable by another
      name. Below, we use it to allow us to reference the complicated
      expression "waypoints[loop1]" by the simple identifier "w".
   *)
   with w:waypoints[loop1] do
    (* Use getparam to load the data from the registry. The appuser macro
       is used to create an appcode from a user number. The data is loaded
       into "w.z","w.x", and "w.y". Because of the with statement above,
       "w" is actually the same as "waypoints[loop1]"
    *)
    getparam(appuser(unum),"waypoint"+itoa(loop1),w.z,w.x,w.y);
   endwith;
  else
   (* This is the "else" branch of the "if" statement that performed the
      verifyparam check. If we got here, then the data was not present in
      the registry and was not loaded. Thus, we should initialize the
      waypoint to all zeros.
   *)
   with w:waypoints[loop1] do
    w.x:=0;
    w.y:=0;
    w.z:=0;
   endwith;
  endif;
 endfor;
end load_waypoints;

(* Procedure save_waypoints. Saves the warpoints from the array "waypoints"
     back into the registry. The code is somewhat similar to load_waypoints,
     except that no calls to verifyparam are necessary, since we are writing
     data rather than reading it.
*)

procedure save_waypoints(unum: integer);
var
 loop1: integer;
begin;
 for loop1:=0 to maxwaypoints-1 do
  with w:waypoints[loop1] do
   putparam(appuser(unum),"waypoint"+itoa(loop1),w.z,w.x,w.y);
  endwith;
 endfor;
end save_waypoints;

(* Procedure display_coords. This is a quick little procedure to display
     coordinates in a formatted multi-colored way. The output will look
     like "(terrain) (z=(int),x=(int),y=(int))".
*)

procedure display_coords(z,x,y: byte);
var
 maptile: word;
 ter: terraintype;
begin;
 (* use getmap to read the map tile number of the given z,x,y coordinates *)
 maptile:=getmap(z,x,y);
 (* use getterrain to retrieve the terrain type associated with the map
    tile number that we retrieved above.
 *)
 getterrain(maptile,ter);
 (* write our message to the screen. The & codes are for color. For example,
    &F switches to white and &7 is cyan.
 *)
 write("&F",ter.typ," &7(z=&F",z,"&7,x=&F",x,"&7,y=&F",y,"&7)");
end display_coords;

procedure show_anchor;
begin;
 write("&7Current warper anchor point is: ");
 display_coords(user^.anchorz,user^.anchorx,user^.anchory);
 writeln("&7.");
end show_anchor;

(* Procedure display_waypoints. This proc will display the current warper
     anchor coordinates as well as all of the stored waypoints.
*)

procedure display_waypoints;
var
 loop1: integer;
begin;
 show_anchor;
 writeln;
 for loop1:=0 to maxwaypoints-1 do
  with w:waypoints[loop1] do
   if w.x>0 then
    write("&E",loop1+1,"&7: waypoint at: ");
    display_coords(w.z,w.x,w.y);
    writeln(".");
   else
    writeln("&E",loop1+1,"&7: waypoint not set.");
   endif;
  endwith;
 endfor;
end display_waypoints;

(* Procedure waypoint_store. Prompts the user for the desired slot to store
     the current warper anchor into.
*)

procedure waypoint_store;
var
 s: string;
 i: integer;
begin;
 writeln;
 show_anchor;
 writeln;
 write("&3Save the current warper anchor into which slot [&F1&3..&F",
       maxwaypoints,"&3] ? &F");
 (* readln() reads a string from the user, and atoi converts the string into
    an integer value.
 *)
 i:=atoi(readln());
 (* ALWAYS check the user's input to make sure it is within bounds! *)
 if (i>0) & (i<=maxwaypoints) then
  writeln("&7Coordinates transferred.");
  (* Since "waypoints" is a zero-based array, we must store our data into
     waypoints[i-1], rather than waypoints[i]. Another with statement is
     used here to simplify the job.
  *)
  with w:waypoints[i-1] do
   w.x:=user^.anchorx;
   w.y:=user^.anchory;
   w.z:=user^.anchorz;
  endwith;
 else
  writeln("&CAborted.");
 endif;
end waypoint_store;

(* Procedure waypoint_current. Prompts the user for the desired slot to store
     the current coordinates (NOT the current anchor point!) into. This
     procedure is very similar to waypoint_store.
*)

procedure waypoint_current;
var
 s: string;
 i: integer;
begin;
 writeln;
 write("&7Your current coordinates are: ");
 display_coords(user^.z,user^.x,user^.y);
 writeln("&7.");
 writeln;
 write("&3Save your current coordinates into which slot [&F1&3..&F",
       maxwaypoints,"&3] ? &F");
 i:=atoi(readln());
 if (i>0) & (i<=maxwaypoints) then
  writeln("&7Coordinates transferred.");
  with w:waypoints[i-1] do
   w.x:=user^.x;
   w.y:=user^.y;
   w.z:=user^.z;
  endwith;
 else
  writeln("&CAborted.");
 endif;
end waypoint_current;

(* Procedure waypoint_retrieve. Does the opposite of waypoint_store - loads
     a value from a waypoint slot into the emwarp anchor.
*)

procedure waypoint_retrieve;
var
 s: string;
 i: integer;
begin;
 write("&3Load the current warper anchor from which slot [&F1&3..&F",
       maxwaypoints,"&3] ? &F");
 i:=atoi(readln());
 if (i<1) or (i>maxwaypoints) then
  writeln("&CAborted.");
 elsif (waypoints[i-1].x=0) then
  writeln("&CThe waypoint slot you specified does not hold valid coordinates!");
 else
  writeln("&7Coordinates transferred.");
  with w:waypoints[i-1] do
   user^.anchorx:=w.x;
   user^.anchory:=w.y;
   user^.anchorz:=w.z;
  endwith;
 endif;
end waypoint_retrieve;

procedure waypoint_getrand(unum: integer): boolean;
var
 b: boolean;
begin;
 if verifyparam(appuser(unum),"waypoint_random") then
  getparam(appuser(unum),"waypoint_random",b);
  return b;
 else
  return FALSE;
 endif;
end waypoint_getrand;

procedure waypoint_autorand;
begin;
 writeln("&7If the automatic randomizer is enabled, then the waypoint manager will");
 writeln("automatically select a random EM-Warp anchor point from the available");
 writeln("coordinates that are stored in the bank if another player attacks your");
 writeln("character while you are offline. The randomizer will not effect play while");
 writeln("you are online.");
 writeln;
 write("&2The randomizer is currently ",
       cond(waypoint_getrand(usernum^),"active.","inactive."));
 writeln;
 if checkyesno("&3Enable the auto-randomizer (&FY&3/&FN&3) ? ") then
  putparam(appuser(usernum^),"waypoint_random",TRUE)
 else
  putparam(appuser(usernum^),"waypoint_random",FALSE);
 endif;
end waypoint_autorand;

(* Procedure use_warpway. This is the main routine for our waypoint manager.
     It is called from use_object (in LGPL.IN) when the user uses the
     waypoint manager.
*)

procedure use_warpway;
var
 ch: char;
begin;
 (* load the waypoints from the registry into our data array *)
 load_waypoints(usernum^);

 (* make LOD check the anchor point to ensure it's validity *)
 checkanchorpoint(user^);

 (* display the anchor point and waypoint listing *)
 display_waypoints;

 (* the repeat loop will be used to repeatedly ask the user for commands
    until the user hit's "Q" for quit.
 *)
 repeat;
  writeln;
  write("&3WarpWay: &FC&3]urrent, &FS&3]tore, &FR&3]etrieve, &FL&3]ist, &FA&3]uto-rand, or &FQ&3]uit ? &F");

  (* readkey() will read exactly one character from the user. upcase is
     used to convert the character to upper case.
  *)
  ch:=upcase(readkey());

  (* the switch statement will select various options depending on the
     value of the input that the user provided. Each option in the switch
     statement should be terminated by a break.
  *)
  switch ch of
   case 'C': writeln("Current");
             waypoint_current;
             break;

   case 'S': writeln("Store");
             waypoint_store;
             break;

   case 'R': writeln("Retrieve");
             waypoint_retrieve;
             break;

   case 'L': writeln("List");
             display_waypoints;
             break;

   case 'A': writeln("Auto-rand");
             waypoint_autorand;
             break;

   case 'Q': writeln("Quit");
             resetneedpause;
             break;

   default:  writeln("What???");
             break;
  end;
 until (ch='Q');

 (* save the waypoints from our data array back into the registry *)
 save_waypoints(usernum^);
end use_warpway;

(* Procedure warpway_emwarp. This will be called by the "checkemwarp"
     routine when a player is about to EM-Warp. At this point, we need
     to see if the user has selected the "randomize" option. If the user
     has selected randomize, then we will load the anchor point with a
     random entry from the waypoint array.

   NOTE: At this point, we can not assume that the user in question is
     user^ and his number is usernum^. This routine was probably called
     from player-versus-player combat, and the user who is em-warping is
     the person who was attacked, not the one who is attacking.
*)

procedure warpway_emwarp(var u: usertype);
var
 r: integer;
 count: integer; (* Failsafe counter in case no valid waypoints exist *)
 done: boolean;
begin;
 if hasobj(devapp_waypointman,u) & waypoint_getrand(u.number) then
  load_waypoints(u.number);
  count:=0;
  done:=false;
  repeat
   inc(count);
   r:=randi(maxwaypoints);
   if (waypoints[r].x<>0) then
    with w:waypoints[r] do
     u.anchorx:=w.x;
     u.anchory:=w.y;
     u.anchorz:=w.z;
    endwith;
    done:=true;
   endif;
  until done or (count>10);
 endif;
end warpway_emwarp;
