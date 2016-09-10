(* NOTE: LGPL is NOT Pascal! LGPL integers are 4-bytes, equivelant to what
         Borland Pascal calls a "longint"
*)

const
 nbspec=10;
 numbasestartspec=5;
 numueqp=6;
 numudev=18;
 numdev=350;
 numobj=2048;
 numst=50;
 numprod=3;
 numbase=75;
 numspell=5;
 numbforsale=8;
 numterrain=255;
 hinc=45;

 maxpylongroups=48;
 maxpylonpress=5;
 maxpylonbuttons=8;
 maxusers=255; (* not fully implemented *)

 maxmapx=125;
 maxmapy=125;
 maxmapz=1;
 maxgenobj=255;
 maxtover=64;
 maxquest=32;

 maxrentlevels = 3;

 numca=100;
 maxtav=50;
 numtcode=27;
 maxinclude=10;
 maxtrestrict=4;

 maxpurparts=10;
 numpurparts=9;
 kirkpart=1;
 xebocpart=2;
 drakpart=3;
 xebpart=4;
 tranliuspart=5;
 vayderrpart=6;
 snowguypart=7;
 lokipart=8;
 serpinepart=9;

 z_bgenrobo=254;
 z_purboss=253;
 z_robomine=252;

 ammomax=20;

 maxteams=10;
 maxteamplayers=5;

 maxmon=300;

 maxnab=15;

 maxpmask=10;

 AnsiStandard    = 0;
 AnsiEnh5x5      = 1;
 AnsiEnh3x3      = 2;

 (* terrain passability types *)
 pGenImpass      = 0;
 pPassable       = 1;
 pMntns          = 2;
 pWalls          = 3;
 pWater          = 4;
 pLava           = 5;
 pForrest        = 6;
 pKillInc        = 7;
 pNukeSilo       = 8;
 pFountainDoor   = 9;
 pDWDoor         = 10;
 pHercDoor       = 11;
 pHercTaunt      = 12;
 pCasinoDoor     = 13;
 pCloneTank      = 14;
 pEmpty          = 15;

 eNone           = 0;
 eTown           = 1;
 eBase           = 2;
 eCamp           = 3;
 eHortCent       = 4;
 eCloneCent      = 5;
 ePylon          = 6;
 eKirk           = 7;
 eNukeSilo       = 8;
 eEEEE           = 9;
 eKill           = 10;
 eCTower         = 11;
 eSetDome        = 12;
 eHut            = 13;
 eVayDerr        = 14;
 eSnowCave       = 15;
 eAcme           = 16;
 eLoki           = 17;
 eSerpine        = 18;
 eTer            = 19;
 eDraku          = 20;
 eXeboc          = 21;
 eStore          = 22;
 ePtron          = 23;
 eTquart         = 24;
 eMail           = 25;
 eDoc            = 26;
 eTavern         = 27;
 eHoloStore      = 28;
 eBank           = 29;
 eSSSS           = 30;
 eSlot           = 31;
 eNumGuess       = 32;
 eCoinFlip       = 33;
 eTrivia         = 34;
 eMegaSlot       = 35;
 eFountain       = 36;
 eRoulette       = 37;
 eCasInfo        = 38;
 eCasStorage     = 39;
 ePoker          = 40;
 eDouble         = 41;
 eBunkBed        = 42;

 store_none      = 0;
 store_weap      = 1;
 store_armor     = 2;
 store_misc      = 3;
 store_ammo      = 4;
 store_basereact = 5;
 store_basemisc  = 6;
 store_basedef   = 7;
 store_basemoat  = 8;
 store_baseweap  = 9;
 store_computer  = 10;
 store_weaplr    = 11;
 store_troysteak = 12;
 store_troydisk  = 13;
 store_gsrz      = 14;
 store_basefact  = 15;
 store_EEEE	 = 16;
 store_town2	 = 17;
 store_town3     = 18;
 store_town4     = 19;
 store_town5     = 20;
 store_town6	 = 21;
 store_town7     = 22;
 store_town8	 = 23;
 store_town9	 = 24;
 store_kill	 = 25;
 store_basepack  = 26;
 store_basescav  = 27;
 store_baseene	 = 28;
 store_basemed   = 29;
 store_acmefort  = 30;
 store_acmeweap  = 31;
 store_acmeROM   = 32;
 store_baserobo  = 33;
 store_acmechem  = 34;
 store_basecomp  = 35;

 devapp_radio    = 0;
 devapp_medkit	 = 1;
 devapp_weapon   = 2;
 devapp_rweapon  = 3;
 devapp_armor    = 4;
 devapp_aml	 = 5;
 devapp_bad	 = 6;
 devapp_lrscan	 = 7;
 devapp_ammo     = 8;
 devapp_medline  = 9;
 devapp_rope	 = 10;
 devapp_rations  = 11;
 devapp_scanner  = 12;
 devapp_stealth  = 13;
 devapp_gblaster = 14;
 devapp_armorn   = 15;
 devapp_grenade  = 16;
 devapp_cyceye	 = 17;
 devapp_deathbadge = 18;
 devapp_warper   = 19;
 devapp_communicator = 20;
 devapp_reactor  = 21;
 devapp_comm	 = 22;
 devapp_deffield = 23;
 devapp_moat	 = 24;
 devapp_bweap	 = 25;
 devapp_laptop	 = 26;
 devapp_rom	 = 27;
 devapp_troysteak = 28;
 devapp_tele	 = 29;
 devapp_recharge = 30;
 devapp_pylonkey = 31;
 devapp_vortex	 = 32;
 devapp_unitran  = 33;
 devapp_mine	 = 34;
 devapp_boat     = 35;
 devapp_mapper   = 36;
 devapp_cryo	 = 37;
 devapp_bgen	 = 38;
 devapp_mappack  = 39;
 devapp_genobj   = 40;
 devapp_flashbomb= 41;
 devapp_waypointman = 42;
 devapp_hypervault = 43;
 devapp_tricorder = 44;
 devapp_chem     = 45;
 devapp_labeller = 46;
 devapp_shield   = 47;
 devapp_userman  = 48;
 devapp_keyring  = 49;
 devapp_samlink  = 50;
 devapp_dnasamp  = 51;
 devapp_chest    = 52;
 devapp_boatplans= 53;
 devapp_tkit     = 54;
 devapp_palarm   = 55;

 cap1 = 0;
 cap2 = 1;
 cap3 = 2;
 cap4 = 3;
 cap5 = 4;
 cap6 = 5;
 cap7 = 6;
 cap8 = 7;
 cap9 = 8;
 cap10= 9;
 cap11= 10;
 cap12= 11;
 cap13= 12;
 cap14= 13;
 cap15= 14;
 cap16= 15;
 cap17= 16;
 cap18= 17;
 cap19= 18;
 cap20= 19;
 cap21= 20;
 cap22= 21;
 cap23= 22;
 cap24= 23;
 cap25= 24;
 cap26= 25;
 cap27= 26;
 cap28= 27;
 cap29= 28;
 cap30= 29;
 cap31= 30;
 cap32= 31;
 cap33= 32;
 cap34= 33;
 cap35= 34;
 cap36= 35;
 cap37= 36;
 cap38= 37;
 cap39= 38;
 cap40= 39;
 cap41= 40;
 cap42= 41;
 cap43= 42;
 cap44= 43;
 cap45= 44;
 cap46= 45;
 cap47= 46;
 cap48= 47;

 callback_steal = 2000;

 menu_invmaint = 1;
 menu_equip = 2;
 menu_finance = 3;
 menu_barracks = 4;
 menu_team = 5;
 menu_hort = 6;
 menu_nukesilo = 7;
 menu_loki = 8;
 menu_eeee = 9;
 menu_clonecenter = 10;
 menu_acme = 11;
 menu_comcent = 14;
 menu_puritron = 15;
 menu_doccent = 16;
 menu_basetrade = 17;
 menu_base = 18;
 menu_lrcombat = 24;
 menu_camp = 25;
 menu_killstore = 125;

 rom_dir = cap1;
 rom_exit = cap1;
 rom_chkdsk = cap1;
 rom_copy = cap1;
 rom_del = cap1;
 rom_ver = cap1;
 rom_help = cap1;
 rom_notes = cap2;
 rom_autonav = cap3;
 rom_transfer = cap4;
 rom_rankings = cap5;
 rom_viewlog = cap6;
 rom_emwarp = cap7;
 rom_fortlist = cap8;
 rom_diet = cap9;
 rom_cstats = cap10;
 rom_lrscan = cap11;
 rom_scanner = cap12;
 rom_troyinfo = cap13;
 rom_commlink = cap14;
 rom_tetris = cap15;
 rom_appraise = cap16;
 rom_tcode = cap17;
 rom_history = cap18;
 rom_windows = cap19;
 rom_fortinv = cap20;
 rom_bartalk = cap21;
 rom_spweap = cap22;
 rom_weapsmth = cap23;
 rom_samlist = cap24;
 rom_slothack = cap25;
 rom_ping = cap26;
 rom_antiping = cap27;
 rom_fortspy = cap28;
 rom_minescan = cap29;
 rom_macro = cap30;

 sex_male        = 0;
 sex_female	 = 1;
 sex_other	 = 2;
type
 pbyte = pointer to byte;

 tpylonentry = record
                name: word;
                tag: byte;
                numbuttons: byte;
                buttons: array maxpylonbuttons of word;
                buttonchars: array maxpylonbuttons of char;
                bogus_start,bogus_end: word;
                intro_start,intro_end: word;
                nokey_start,nokey_end: word;
                tport_start,tport_end: word;
                key: word;
                numpresses: byte;
                yesnoprompt: word;
                pic: string[8];
                animpic: string[8];
                modmusic: string[8];
                flags: word;
                special: word;
               end;

  trentlevel = record
                lvl: word;
                amt: word;
               end;

  tmapentry = record
               valid: boolean;
               fileoffset: integer;
               mapsize: word;
               xsize: word;
               ysize: word;
               zoneorigx: word;
               zoneorigy: word;
               warphomex: word;
               warphomey: word;
               modmusic: string[8];
               flags: word;
               rent: array maxrentlevels of trentlevel;
               minminedist: byte;
               minproxdist: byte;
               mindanger: byte;
               dmult: word;
               scavodds: word;
               cop: word;
               blah: array 32 of byte;
              end;

 puritronunit=record
	       ishere: boolean;
	       userrealname: string[25];
	       reset: boolean;
	       blah: array 24 of byte;
	      end;

 storeset = array 5 of byte;

 devdeftype=record
	     num: word;               (* device number               *)
	     var1,var2: integer;      (* start up variables          *)
             var3: comp;
	     x,y,z: byte;             (* object x,y,z position       *)
	     devapp: byte;            (* application type            *)
	     useleft: word;
	     maxuse: word;
	     decuse: boolean;
	     name: string[18];        (* device name                 *)
	     store: storeset;         (* store it sells at           *)
	     cost: comp;              (* how much it costs           *)
	     usable: boolean;         (* can it be used?             *)
	     pic: string[8];          (* ansi picture identifier     *)
	     picnum: word;
	    end;

 pdevdef     = pointer to devdeftype;
 devdefarray = array numdev of pdevdef;
 pdevdefarray= pointer to devdefarray;

 devicetype=record
	     num: word;               (* Device Numbers              *)
	     var1,var2: integer;      (* generic variables           *)
             var3: comp;
	     x,y,z: byte;             (* object x,y,z position       *)
	     devapp: byte;            (* application type            *)
	     useleft: word;           (* maximum uses and uses left  *)
	    end;

 pdevicetype = pointer to devicetype;

 devarray=  array numudev of devicetype;

 usertype=  record
	     realname: string[35];    (* real name of user           *)
	     alias: string[25];       (* user's alias                *)

	     devices: devarray;
	     weapon1: devicetype;     (* Close combat weapon         *)
	     weapon2: devicetype;     (* Ranged combat weapon        *)
	     auxweapon: devicetype;   (* Auxilary close weapon       *)
	     armor: devicetype;       (* armor                       *)
	     medline: devicetype;     (* Automated medical lining    *)
	     laptop: devicetype;      (* GS-224 Laptop               *)

	     hitpts,hitmax: word;     (* user's hitpoints and hitmax *)
             dnacode: word;
             unused_1: word;
	     str,dex,agl: word;       (* strength, dexterity, agil   *)
	     lvl: word;               (* level                       *)
	     exp: comp;               (* experience                  *)
	     money: comp;             (* money                       *)
	     bank: comp;              (* money in the bank           *)
	     alive: boolean;          (* alive ?                     *)
	     x,y,z: byte;             (* x,y,z coords                *)
	     hunger: word;            (* hunger points               *)
	     expert: boolean;         (* expert on?                  *)
	     combat: integer;         (* battles faught in           *)
	     flee: integer;           (* battles fleed in            *)
	     surrender: integer;      (* battles surrendered in      *)
	     kills: integer;          (* number of monsters killed   *)
	     hits: integer;           (* times he hit somebody       *)
	     misses: integer;         (* times he missed somebody    *)
	     lastread: word;          (* message base lastread ptr   *)
	     ansimode: byte;          (* used enhanced ansi if avail *)
	     clrmap: boolean;         (* color map?                  *)
	     forceansi: boolean;      (* force ansi on?              *)
             number: byte;
	     dietoff: boolean;
	     products: array numprod of comp;
	     buysteak: boolean;
	     getdisk: boolean;
	     didtetris: boolean;
	     rent: integer;
	     rentday: word;
	     lastcity: byte;          (* last city user was at       *)
	     combattext: byte;        (* Entry in monsters.txt w/ combat txt *)
	     timetoday: word;
	     sex: byte;
	     flags: word;
	     quests: integer;
	     janitor: integer;
	     casacct: integer;
	     maintrun: byte;
	     deathtoday: byte;
	     loan: integer;
	     maintdelcount: word;
	     team: byte;
	     fortattack: byte;
	     combatmode: byte;
	     anchorx,anchory,anchorz: byte; (*EMWARP anchor point*)
	     addpts: byte;
	     unused_2: array 12 of byte;
	    end;
 pusertype = pointer to usertype;

 clonetype=record
	    realname: string[35];
	    alias: string[25];
	    hitmax: word;
	    str,dex,agl: word;
	    lvl: word;
	    exp: comp;
	    alive: boolean;
	    blah: array 10 of byte;
	   end;

 tmonindex = record        (* dupe of beginning of tmonster *)
           mindist: byte;
           maxdist: byte;
           origx: byte;
           origy: byte;
           origz: byte;
           ifalive: byte;
           isreal: byte;
           flags: integer;
           file_pos: integer;
           trestrict: array maxtrestrict of byte;
          end;

 terraintype= record
               typ: string[16];   (* ascii name *)
               map: string[2];    (* 2 character symbol *)
               dng: byte;         (* danger - 0 to 100 % *)
               clr: byte;         (* color *)
               pas: byte;         (* passability variant *)
               ent: byte;         (* enter variant *)
               mpr: byte;         (* mapper color code (0-4) *)
               flg: word;         (* misc terrain flags *)
               evr: word;         (* enter code *)
               tilenum: byte;     (* graphic tile number *)
              end;

 baserec = record
	    x,y,z: byte;
	    owner: byte;
	    name: string[20];
	    password: string[8];
	    active: boolean;
	    products: array numprod of integer; (* Products on hand      *)
	    prodfs:   array numprod of integer; (* Products for sale     *)
	    prodsp:   array numprod of word;    (* Prod sell prices      *)
	    prodbp:   array numprod of word;    (* Prod buy prices       *)
	    prodbm:   array numprod of integer; (* Prod buy money        *)
	    med:      integer;                      (* Medkit doses on hand  *)
	    medfs:    integer;                      (* Medkit doses for sale *)
	    medprice: word;                         (* Price of medkit dose  *)
	    energyfs: integer;                      (* Energy for sale       *)
	    energyprice: word;                      (* Energy price          *)
	    energy:  comp;                          (* Energy on hand        *)
	    money:   comp;                          (* money on hand         *)
	    teleprice: integer;                     (* teleportation price   *)
	    spec:    array nbspec of devicetype; (* Specialty Devices     *)

	    forsale: array numbforsale of devicetype;    (* Devices for sale      *)
	    saleprice: array numbforsale of integer;     (* For sale prices       *)

	    basereg: boolean;

	    tollprice: integer;

	    flags: integer;

	    blah: array 8 of byte;
	   end;
 pbaserec = pointer to baserec;

 teamtype=   record
	      players: array maxteamplayers of byte;
	      numplayers: byte;
	      name: string[30];
	      pword: string[8];
	     end;
type
 laptoprec=record
	    mask: integer;
	    cmdname: string[8];
	    cmdext: string[3];
	    filesize: integer;
	    date: string[8];
	    time: string[6];
	   end;
const
 flagmapcheckraise = 1;
 flagmapnomonster = 2;
 flagmapnopvp = 4;
 flagmapnofort = 8;
 flagmapnomine = 16;
 flagmaptroyquest = 32;
 flagmaphunger = 64;
 flagmapnosteal = 128;

 flagpylonkeymunch = 1;
 flagpylonsuggest = 2;

 flagscannercloak=1;

 flagbaseholo=1;

 flaglrscan=1;
 flagroaddata=2;
 flagbigdata=4;
 flaghasmapper=8;
 flagtinyfort=16;

 flagtermineok=1;        (* set if mine can be placed on terrain square *)
 flagterbaseok=2;        (* set if fortress can be created *)
 flagterabsok=4;         (* set if absolute teleport can be used *)

 flagtertrans1=8;
 flagtertrans2=16;
 flagtertrans4=32;
 flagterlink1=64;
 flagterlink2=128;
 flagterlink4=256;
 flagterentlink=512;

 flagterzoneorig=1024;
 flagterwarphome=2048;
 flagtergameorig=4096;
 flagterdoorlink=8192;
 flagterdoorway=16384;

 flaguserdull=1;         (*not using the fancy character sheet*)
 flagusersc=2;           (*store colors disabled*)
 flagusernodel=4;        (*set if user is on No-Deletion status*)
 flagusernodonate=8;     (*set if user is on No-Donations status*)
 flagusernorank=16;      (*set if user should not be ranked*)
 flaguserquick=32;       (*set if user is using QUICK gterm*)
 flaguserrow=64;         (*set if user uses row format inventory*)
 flagusertinyfort=128;   (*use [B] symbol on map instead of fort map tile*)

 flagweapacc1=cap1;
 flagweapacc2=cap2;
 flagweapacc4=cap3;
 flagweapint1=cap4;
 flagweapint2=cap5;
 flagweapint4=cap6;
 flagweapstun=cap7;
 flagweapburst=cap8;
 flagweapuni=cap9;
 flagweapshock=cap10;
 flagweaplaunch=cap11;
 flagweapunacc1=cap12;
 flagweapunacc2=cap13;
 flagweapunacc4=cap14;
 flagweapunint1=cap15;
 flagweapunint2=cap16;
 flagweapunint4=cap17;
 flagweapburn1=cap18;
 flagweapburn2=cap19;
 flagweapburn4=cap20;
 flagweapbroken=cap21;
 flagweapdpulse=cap22;
 flagweapnodpulse=cap23;
 flagweapnoburn=cap24;
 flagweapnolaunch=cap25;
 flagweapnoshock=cap26;
 flagweapnostun=cap27;
 flagweapnoburst=cap28;

 flagarmorrecharge=1;
 flagarmorstr1=2;
 flagarmorstr2=4;
 flagarmorstr3=2+4;
 flagarmordex1=8;
 flagarmordex2=16;
 flagarmordex3=8+16;
 flagarmoragl1=32;
 flagarmoragl2=64;
 flagarmoragl3=32+64;

 flagmonisreal=1;                 (*set if monster uses ISREAL*)
 flagmonwateronly=2;              (*set for water only monsters*)
 flagmonlandwater=4;              (*set for land/water monsters*)
 flagmonnorandom=8;               (*disables random encounters*)

 flagminesmart=cap1;              (*set if smart mine*)
 flagmineenergy=cap2;             (*set if mine is energy drainer*)
 flagmineteleport=cap3;           (*set if mine is a teleporter mine*)
 flagminerobo=cap4;               (*set if mine is a robomine*)
 flagminenowarn=cap5;             (*set if mine has warning DISABLED! *)

 flagratiskelp=1;                 (*set if ration is kelp ration - spq #14*)

 flagbgentele1 = cap1;            (*standard teleporter*)
 flagbgentele2 = cap2;            (*excelsior teleport*)
 flagbgentele3 = cap3;            (*transporter*)
 flagbgencomm  = cap4;            (*communications*)
 flagbgencloak = cap5;            (*cloaking device*)
 flagbgenwander = cap6;           (*wanderer*)
 flagbgenfactory = cap7;          (*factory*)
 flagbgendrainer = cap8;          (*ammo drainer*)

 flagbgenprox1 = cap9;            (*proximity detector bit 0*)
 flagbgenprox2 = cap10;           (*proximity detector bit 1*)
 flagbgenprox4 = cap11;           (*proximity detector bit 2*)

 flagbgentoll1 = cap12;           (*toll station bit 0*)
 flagbgentoll2 = cap13;           (*toll station bit 1*)
 flagbgentoll4 = cap14;           (*toll station bit 2*)

 flagbgenspread1 = cap15;         (*mine spreader bit 0*)
 flagbgenspread2 = cap16;         (*mine spreader bit 1*)
 flagbgenspread4 = cap17;         (*mine spreader bit 2*)

 flagbgenreactor = cap18;         (*reactor*)

 flagbgenscav1 = cap19;           (*scavenger bit 0*)
 flagbgenscav2 = cap20;           (*scavenger bit 1*)
 flagbgenscav4 = cap21;           (*scavenger bit 2*)

 flagbgenscan = cap22;            (*scanner*)

 flagbgendevlock = cap23;         (*device lock*)
 flagbgenantisd = cap24;          (*anti self destruct*)

 flagbgensell1 = cap25;           (*autosellers*)
 flagbgensell2 = cap26;
 flagbgensell3 = cap27;
 flagbgensell4 = cap28;

 flagbgensmartscav = cap29;       (*says scavenger droid is smart*)

 flagbgenrobo1 = cap30;           (*robodefender bit 0*)
 flagbgenrobo2 = cap31;           (*robodefender bit 1*)
 flagbgenrobo4 = cap32;           (*robodefender bit 2*)
 flagbgenrobo8 = cap33;           (*robodefender bit 3*)

 flagbgenagl1  = cap34;           (*shield computers*)
 flagbgenagl2  = cap35;
 flagbgenagl4  = cap36;
 flagbgenagl8  = cap37;

 flagbgendex1  = cap38;           (*targetting computers*)
 flagbgendex2  = cap39;
 flagbgendex4  = cap40;
 flagbgendex8  = cap41;

 flagbgenburn1 = cap42;           (*armor burner upgrade*)
 flagbgenburn2 = cap43;
 flagbgenburn4 = cap44;

 flagbgenvortex = cap45;          (*vortex cell link*)
var
 user: pusertype;
 usernum: pbyte;
 devicedef: pdevdefarray;

 pchar_player,
 mchar_player: pusertype;

#define devdef_decuse(num) (devicedef^[num]^.decuse)
#define devdef_maxuse(num) (devicedef^[num]^.maxuse)
#define devdef_usable(num) (devicedef^[num]^.usable)
#define devdef_name(num)   (devicedef^[num]^.name)

#define onoff(b) (Cond(b,"On","Off"))
#define yesno(b) (Cond(b,"Yes","No"))

