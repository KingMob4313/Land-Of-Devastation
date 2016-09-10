
const
 color_black        = 0;
 color_blue         = 1;
 color_green        = 2;
 color_cyan         = 3;
 color_red          = 4;
 color_magenta      = 5;
 color_brown        = 6;
 color_lightgray    = 7;
 color_darkgray     = 8;
 color_lightblue    = 9;
 color_lightgreen   = 10;
 color_lightcyan    = 11;
 color_lightred     = 12;
 color_lightmagenta = 13;
 color_yellow	    = 14;
 color_white	    = 15;

const
 combat_pchar       = 1;
 combat_mchar       = 2;
 combat_dchar       = 3;

 querylod_numdev    = 1;
 querylod_startstr  = 2;
 querylod_startdex  = 3;
 querylod_startagl  = 4;
 querylod_startmoney= 5;
 querylod_starthp   = 6;
 querylod_caslimit  = 7;
 querylod_triviacount = 8;
 querylod_triviapay = 9;
 querylod_startdev1 = 10;
 querylod_startdev2 = 11;
 querylod_startdev3 = 12;
 querylod_startdev4 = 13;
 querylod_startdev5 = 14;
 querylod_startdev6 = 15;
 querylod_startdev7 = 16;
 querylod_startdev8 = 17;
 querylod_numkeys   = 18;
 querylod_keymap    = 19;
 querylod_maxmaps   = 68;
 querylod_numstrdef = 69;

 querylod_Strdefbytes   = 70;
 querylod_mapbytes      = 71;
 querylod_maxmapx       = 72;
 querylod_maxmapy       = 73;
 querylod_objbytes      = 74;
 querylod_numolist      = 75;
 querylod_monindexbytes = 76;
 querylod_nummonindex   = 77;
 querylod_sizeofpchar   = 78;
 querylod_sizeofmchar   = 79;
 querylod_ovrbufsize    = 80;
 querylod_memavail      = 81;
 querylod_maxavail      = 82;
 querylod_devgood       = 83;
 querylod_devfill       = 84;
 querylod_devnil        = 85;
 querylod_heapused      = 86;
 querylod_emsused       = 87;
 querylod_menucallback  = 88;
 querylod_vortexbattmax = 89;
 querylod_maxcasgames   = 90;
 querylod_moneybag      = 91;
 querylod_gmenubasenum  = 92;
 querylod_maxbaseenergy = 93;
 querylod_levpts        = 94;
 querylod_xebocmax      = 95;
 querylod_moneyperlevel = 96;

 querylod_colorchg      = 97; (* returns 0 if vernum < 5.02 *)
 querylod_vermaj        = 98; (* returns 0 if vernum < 5.02 *)
 querylod_vermin        = 99; (* returns 0 if vernum < 5.02 *)

 querylod_gmenuplayernum = 100; (* returns 0 if vernum < 5.02 *)
 querylod_maxfortattack = 101; (* returns 0 if vernum < 5.02 *)

 querylod_sizetpylonentry = 102;
 querylod_sizetmapentry = 103;
 querylod_sizedevdeftype = 105;
 querylod_sizedevicetype = 106;
 querylod_sizeusertype = 107;
 querylod_sizeclonetype = 108;
 querylod_sizetmonindex = 109;
 querylod_sizeterraintype = 110;
 querylod_sizebaserec = 111;

external 102 procedure linkuserrec(var user: pusertype);
external 103 procedure linkdevdef(var devdef: pdevdefarray);
external 130 procedure linkusernum(var usernum: pbyte);
external 131 procedure linkcombatplayer(n: integer; var u: pusertype);

external 101 procedure listuserinv(var user: usertype; useonly: boolean);
external 106 procedure EnterMessage(replyname,replysub: string);
external 107 procedure NewMail;
external 108 procedure ReadMail;
external 111 procedure displr(z,x,y,r: integer);
external 112 procedure findmaptile(tilenum: byte; var z,x,y: byte);
external 113 procedure checklaptop(var user: usertype; prognum: integer): boolean;
external 116 procedure gtermnomapcheck;
external 122 procedure testcap(var o: devicetype; cap: integer): boolean;
external 123 procedure setcap(var o: devicetype; cap: integer);
external 124 procedure showchar(user: pusertype);
external 125 procedure findmonster(z,x: word; var n: word): boolean;
external 126 procedure loadcombatdata(pc,mc: word);
external 127 procedure cdist(x1,y1,x2,y2: integer): integer;
external 128 procedure sqrt(x: integer): integer;
external 129 procedure dropobj(var d: devicetype);
external 132 procedure vbatt_numammo(): integer;
external 133 procedure vbatt_ammoname(n: integer): string;
external 134 procedure vbatt_ammoeff(n: integer): integer;
external 136 procedure addobject(z,x,y: integer; var d: devicetype): integer;
external 137 procedure peekobject(n: integer; var d: devicetype);
external 138 procedure retrieveobject(n: integer; var d: devicetype);
external 139 procedure enumobjects(start,z,x,y: integer): integer;
external 140 procedure enumplayers(start,z,x,y: integer): integer;
external 141 procedure enumforts(start,z,x,y: integer): integer;
external 142 procedure loaduser(n: word; var u: usertype);
external 143 procedure saveuser(n: word; var u: usertype);
external 144 procedure checkdupename(name: string): boolean;
external 145 procedure namestr(name: string): string;
external 146 procedure ansimapper(x1,y1,xtodo,ytodo,xhi,yhi: integer);
external 148 procedure flagredraw;
external 149 procedure querygtermactive(): boolean;
external 150 procedure getmoveparams(var z,x,y: word; var quiet: boolean);
external 151 procedure getmap(z,x,y: word): word;
external 152 procedure getterrain(maptile: word; var ter: terraintype);
external 153 procedure setpassable(b: boolean);
external 154 procedure hasobj(devapp: word; var u: usertype): boolean;
external 155 procedure checkquest(n: word): boolean;
external 156 procedure specialquest(n: word);
external 157 procedure rentcheck(): boolean;
external 158 procedure findobj(devapp: word; var u: usertype): integer;
external 159 procedure getenterparams(var z,x,y: word);
external 160 procedure getenterselection(var entertype,entervar: word);
external 161 procedure laptop_builtin(var d: devicetype; n: integer; s: string);
external 166 procedure gtermaddbigpic(s: string);
external 169 procedure gtermdecbigpic;
external 171 varargs procedure writelog;
external 177 procedure move_to_town(entervar: word);
external 178 procedure dialog(person: word);
external 179 procedure setenterstring(s: string);
external 180 procedure keycheck(keynum: word; munch: boolean): boolean;
external 181 procedure findmedkit(var u: usertype): integer;
external 182 procedure resetcap(var o: devicetype; cap: integer);
external 189 procedure checkanchorpoint(var u: usertype): boolean;
external 190 procedure setwarpincombatsuccess(success: boolean);
external 195 procedure devname(var d: devicetype): string;
external 196 procedure querymapsize(z: integer; var x,y: integer);
external 197 procedure getemwarpparams(var u: pusertype);
external 198 procedure setemwarpsuccess(b: boolean);
external 199 procedure getbase(n: integer): pbaserec;
external 200 procedure getpylon(pylonnum: integer; var p: tpylonentry);
external 201 procedure getmaptableentry(n: integer; var m: tmapentry);
external 202 procedure createdevice(n: integer): devicetype;
external 203 procedure itemval(var d: devicetype): integer;
external 204 procedure querylodint(n: integer): integer;
external 205 procedure returntoorigin(var user: usertype);
external 206 procedure execmenu(num: integer);
external 184 procedure delay(num: integer);
external 186 procedure resetneedpause;
external 187 procedure enumterrain(var z,x,y: byte; ternum: byte): boolean;
external 207 procedure ckdevstr(s: string): integer;
external 208 procedure delparam_global(s: string);
external 210 procedure submatch(s,s2: string): boolean;
external 211 procedure outstr(n: integer);
external 212 procedure lgplstats;
external 213 procedure svgastats;
external 214 procedure sethunt(n: integer);
external 215 procedure ansistats;
external 216 procedure devsort_key(var d: devicetype): integer;
external 217 procedure killuser(num: integer; var u: usertype; reason: integer; data: string);
external 218 procedure delappcode(appcode: string);
external 219 procedure showbasespec(basenum: integer);
external 220 procedure teststore(var d: devdeftype; n: word): boolean;
external 221 procedure complev(n: integer): comp;
external 222 procedure setlodint(n: integer; l: integer);
external 223 procedure doplayercombat(un: word): boolean;
external 224 procedure dobasecombat(basenum: word): boolean;
external 225 procedure basecombat(basenum: word): boolean;
external 226 procedure dowastecombat(): boolean;

external 162 procedure enter_pylon(entervar: word);
external 163 procedure enter_kirk;
external 164 procedure enter_draku;
external 165 procedure enter_xeboc;
external 167 procedure enter_vayderr;
external 168 procedure enter_snowcave;
external 170 procedure enter_serpine;
external 172 procedure enter_base(entervar: word);
external 173 procedure SetCampNum(entervar: word);
external 174 procedure LogCasPlay(n: word; money: integer);
external 175 procedure LogCasWin(n: word; money:  integer);
external 176 procedure LogCasLose(n: word; money: integer);
external 183 procedure trytrivia(n: word): boolean;
external 185 procedure getcasinostat(n1,n2: word): integer;
external 188 procedure enter_tavern(entervar: word);

external 118 procedure wmindam(i: integer): integer;
external 119 procedure wmaxdam(i: integer): integer;
external 120 procedure wammo(i: integer): integer;
external 121 procedure getminerobo(var o: devicetype): integer;

external 109 procedure use_genobj(index: integer);
external 110 procedure use_mappack(var device: devicetype);
external 135 procedure use_scanner(var device: devicetype);

external 104 procedure textcolor(color: integer);
external 105 procedure waitkey;
external 117 procedure readkey(): char;
external 147 procedure clrscr;
external 209 procedure readraw(): char;

external 191 varargs  procedure putparam(appcode,tag: string);
external 192 lvarargs procedure getparam(appcode,tag: string): boolean;

external 193 procedure deleteparam(appcode,tag: string);
external 194 procedure verifyparam(appcode,tag: string): boolean;

#define appuser(unum) \
  ("user"+itoa(unum))

procedure strfix(s: string): string;
begin;
 return s;
end strfix;

procedure checkyesno(s: string): boolean;
var
 result: boolean;
begin;
 write(s);
 switch upcase(readkey()) of
   case 'Y': writeln("&FYes");
             result:=true;
	     exit;

   default:  writeln("&FNo");
             result:=false;
             exit;
 end;
 return result;
end checkyesno;

procedure nukedevice(var o: devicetype);
begin;
 o.num:=0;
 o.var1:=0;
 o.var2:=0;
 o.var3:=0;
 o.devapp:=0;
end nukedevice;

#define sexstr(n) Cond(n=sex_male,"male", \
                    Cond(n=sex_female,"female", \
                      Cond(n=sex_other,"other", \
                        "unknown")))

procedure FindFreeInv(var u: usertype; var n: integer): boolean;
var
 loop1: integer;
begin;
 for loop1:=0 to numudev-1 do
  if u.devices[loop1].num=0 then
   n:=loop1;
   return TRUE;
  endif;
 endfor;
 n:=0;
 return FALSE;
end FindFreeInv;

procedure decuseleft(var d: devicetype);
begin;
 dec(d.useleft);
 if d.useleft=0 then
  nukedevice(d);
 endif;
end decuseleft;

procedure baseownername(var base: baserec): string;
var
 u: usertype;
begin;
 if base.owner=usernum^ then
  u:=user^;
 else
  loaduser(base.owner,u);
 endif;
 if (upstr(u.alias)="NIL USER") or (u.alive=false) then
  return strcat("unknown owner","");
 else
  return namestr(u.alias);
 endif;
end baseownername;

procedure adev(var d: devicetype): string;
var
 s: string[32];
begin;
 s:=devname(d);
 switch upcase(s[1]) of
  case 'A':
  case 'E':
  case 'I':
  case 'O':
  case 'U':
  case 'Y': return strcat("an","");

  default: return strcat("a","");
 end;
end adev;

procedure devname_num(n: integer): string;
var
 d: devicetype;
begin;
 d:=createdevice(n);
 return devname(d);
end devname_num;

procedure ckdevstr_verbal(s: string): integer;
var
 i: integer;
begin;
 i:=ckdevstr(s);
 if i=0 then
  writeln("LGPL programmer error: Unable to find device reference ",s);
 endif;
 return i;
end ckdevstr_verbal;

procedure useronmap(var u: usertype; mapnum: integer): boolean;
var
 b: pbaserec;
begin;
 if (u.z=mapnum) then
  return TRUE;
 endif;

 if (u.z=255) & (u.x>0) & (u.x<=numbase) then
  b:=getbase(u.x);
  if b^.z=mapnum then return TRUE; endif;
 endif;

 return FALSE;
end useronmap;

procedure checkbasebgen(var b: baserec; flag: integer): boolean;
var
 loop1: integer;
begin;
 for loop1:=0 to nbspec-1 do
  with d:b.spec[loop1] do
   if (d.num<>0) & (d.devapp=devapp_bgen) & (testcap(d,flag)) then
    return TRUE;
   endif;
  endwith;
 endfor;
 return FALSE;
end checkbasebgen;

procedure getusername(n: integer): string;
var
 u: usertype;
begin;
 if (n>0) & (n<maxusers) then
  if n=usernum^ then
   return namestr(user^.alias);
  else
   loaduser(n,u);
   return namestr(u.alias);
  endif;
 else
  return strfix("");
 endif;
end getusername;

procedure directstring(x,y,x1,y1: integer): string;
begin;
 if (x<x1) & (y<y1) then
  return strfix("Northwest");
 elsif (x=x1) & (y<y1) then
  return strfix("North");
 elsif (x>x1) & (y<y1) then
  return strfix("Northeast");
 elsif (x<x1) & (y=y1) then
  return strfix("West");
 elsif (x>x1) & (y=y1) then
  return strfix("East");
 elsif (x<x1) & (y>y1) then
  return strfix("Southwest");
 elsif (x=x1) & (y>y1) then
  return strfix("South");
 elsif (x>x1) & (y>y1) then
  return strfix("Southeast");
 else
  return strfix("Current");
 endif;
end directstring;

(* Procedure getusercoords: If the user is C)amped, then returns the coords
     of the camp. If the user is in a base, then returns the coords of the
     base. If the user is neither (shouldn't happen for live players), then
     returns 0,0.
*)

procedure getusercoords(var u: usertype; var x,y: byte);
var
 b: pbaserec;
begin;
 if (u.z>0) & (u.z<=querylodint(querylod_maxmaps)) then
  x:=u.x;
  y:=u.y;
 elsif (u.z=255) & (u.x>0) & (u.x<=numbase) then
  b:=getbase(u.x);
  x:=b^.x;
  y:=b^.y;
 else
  x:=0;
  y:=0;
 endif;
end getusercoords;

procedure killuser_specloc(num: integer;
                           var u: usertype;
                           reason: integer;
                           data: string;
                           z,x,y: integer);
var
 savex,savey,savez: integer;
begin;
 savex:=u.x;
 savey:=u.y;
 savez:=u.z;
 u.x:=x;
 u.y:=y;
 u.z:=z;
 killuser(num,u,reason,data);
 u.x:=savex;
 u.y:=savey;
 u.z:=savez;
end killuser_specloc;

procedure lodversion(): double;
var
 r,r1,r2: double;
begin;
 r1:=querylodint(querylod_vermaj);
 r2:=querylodint(querylod_vermin);
 r2:=r2/100;
 r:=r1+r2;
 return r;
end lodversion;
