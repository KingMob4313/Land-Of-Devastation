procedure PylonSpecial(Town_Number: word): boolean;
var
 b: integer;
begin;
 switch Town_number of

   case 2: if ~checkquest(13) then
            b:=findobj(devapp_cyceye,user^);
            if b=-1 then
             writeln("&CA scrawny guy appears: \"Hey you! The boss says you have to prove your bravery");
             writeln("before you may enter the city. Bring us the eye of a Cyclops and you may");
             writeln("enter.\"");
             return FALSE;
            else
             user^.devices[b].num:=0;
             writeln("&CA scrawny guy appears: \"I see you have the cyclops eye as requested. Enter,");
             writeln("great warrior of Sacre Base. Freedom City awaits you.\"");
             writeln;
             specialquest(13);
             return TRUE;
            endif;
           else
            writeln("&CA scrawny guy appears: \"Welcome again to our fair Freedom City, Sacre Base");
            writeln("warrior.\"");
            return TRUE;
           endif;
           break;  (* not actually needed since all branches above return! *)

  case 3:  if ~checkquest(3) then
            b:=findobj(devapp_deathbadge,user^);
            if b=-1 then
             writeln("&C\"Hey You! You're not a Death Warrior... I bet you're some of that Sacre Base");
             writeln("scum... GUARDS! ... GUARDS! ...\" (Time to get out of here!!!)");
             return FALSE;
            else
             writeln("&C\"Hey You! Only... Oh wait.. Sorry... Didn't see your badge. Pass through into");
             writeln("our camp, brother.\"");
             writeln;
             specialquest(3);
             return TRUE;
            end;
           else
            writeln("&C\"Hmmm... You sure don't look like a death warrior.... Ah what the heck, I know");
            writeln("you've been here before... Open the gate!\"");
            return TRUE;
           endif;
           break; (* not actually needed since all branches above return! *)

  default: return TRUE;
           break; (* not actually needed due to return statement *)
 end; (* switch *)
end PylonSpecial;

procedure dopylon(pylonnum: integer);
var
 p: tpylonentry;
begin;
 getpylon(pylonnum,p);
 if (p.special=0) or PylonSpecial(p.special) then
  Enter_Pylon(pylonnum);
 endif;
end dopylon;

export procedure enter_perform;
var
 z,x,y: word;
 maptile: word;
 ter: terraintype;
 entertype: word;
 entervar: word;
 b: boolean;
begin;
 getenterparams(z,x,y);
 maptile:=getmap(z,x,y);
 getterrain(maptile,ter);
 getenterselection(entertype,entervar);

 if (entertype=eTer) then
  entertype:=ter.ent;
  entervar:=ter.evr;
 endif;

 switch entertype of
  case eHortCent:   ExecMenu(menu_hort);
                    break;

  case ePylon:      DoPylon(entervar);
                    break;

  case eKirk:       Enter_kirk;
                    break;

  case eDraku:      Enter_draku;
                    break;

  case eXeboc:      Enter_xeboc;
                    break;

  case eNukeSilo:   ExecMenu(menu_nukesilo);
                    break;

  case eVayderr:    Enter_Vayderr;
                    break;

  case eSnowcave:   Enter_snowcave;
                    break;

  case eLoki:       ExecMenu(menu_loki);
                    break;

  case eSerpine:    Enter_serpine;
                    break;

  case eCloneCent:  ExecMenu(menu_clonecenter);
                    break;

(*  case eTown:       b:=townok(entervar);
                    if b then
                      move_to_town(entervar);
                    endif;
                    break; *)

  case eBase:       Enter_Base(entervar);
                    break;

  case eCamp:       setcampnum(entervar);
                    ExecMenu(menu_camp);
                    break;

  case eEEEE:       ExecMenu(menu_eeee);
                    break;

  case eKill:       ExecMenu(menu_killstore);
                    break;

  case eAcme:       ExecMenu(menu_acme);
                    break;

  case eCTower:     writeln("&EThe Communications Towers link various zones of the wasteland into the");
                    writeln("&Eelectronic mail system. They''re really rather unimportant.");
                    break;

  case eSetDome:    writeln("&EThe Settler Domes are occasionally constructed by pioneering communities who");
                    writeln("&Ewish to settle outside of the city. They''re really rather unimportant.");
                    break;

  case eHut:        Dialog(300+entervar);
                    break;

  case eStore:      ExecMenu(entervar+100);
                    break;

  case ePtron:      ExecMenu(menu_puritron);
                    break;

  case eTquart:     ExecMenu(menu_barracks);
                    break;

  case eMail:       ExecMenu(menu_comcent);
                    break;

  case eDoc:        ExecMenu(menu_doccent);
                    break;

  case eTavern:     enter_tavern(entervar);
                    break;

  case eHolostore:  holostore;
                    break;

  case eSSSS:       go_samsurplus(z,x,y,entervar);
                    break;

  case eTrivia:     trivia(entervar);
                    break;

  case eSlot:       play_slots(entervar,false);
                    break;

  case eNumGuess:   guess_number;
                    break;

  case eCoinFlip:   coin_flip;
                    break;

  case eBank:       ExecMenu(menu_finance);
                    break;

  case eMegaSlot:   play_slots(entervar,true);
                    break;

  case eFountain:   go_fountain(entervar);
                    break;

  case eRoulette:   play_roulette;
                    break;

  case eCasInfo:    CasinoStatistics;
                    break;

  case eCasStorage: Go_casstorage;
                    break;

  case ePoker:      Play_poker;
                    break;

  case eDouble:     Play_double;
                    break;

  case eBunkBed:    writeln("&EThe bunkbeds are a good place to camp-out. Other than that, they are");
                    writeln("insignificant.");
                    break;
 end; (* switch *)
end enter_perform;

export procedure enter_string;
var
 x,y,z: word;
 maptile: word;
 ter: terraintype;
begin;
 getenterparams(z,x,y);
 maptile:=getmap(z,x,y);
 getterrain(maptile,ter);

 switch ter.ent of
   case eNone:      break;

   default:         setenterstring(ter.typ);
                    break;
 end; (* switch *)
end enter_string;
