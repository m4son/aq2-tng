//-----------------------------------------------------------------------------
// g_save.c
//
// $Id: g_save.c,v 1.17 2001/06/06 18:57:14 slicerdw Exp $
//
//-----------------------------------------------------------------------------
// $Log: g_save.c,v $
// Revision 1.17  2001/06/06 18:57:14  slicerdw
// Some tweaks on Ctf and related things
//
// Revision 1.14  2001/06/01 19:18:42  slicerdw
// Added Matchmode Code
//
// Revision 1.13  2001/05/31 16:58:14  igor_rock
// conflicts resolved
//
// Revision 1.12.2.5  2001/05/27 13:44:07  igor_rock
// corredted the bug with gi.cvar and ctf_dropflag (mixed paremeters)
//
// Revision 1.12.2.4  2001/05/27 13:41:57  igor_rock
// added ctf_dropflag (default: 1)
//
// Revision 1.12.2.3  2001/05/25 18:59:52  igor_rock
// Added CTF Mode completly :)
// Support for .flg files is still missing, but with "real" CTF maps like
// tq2gtd1 the ctf works fine.
// (I hope that all other modes still work, just tested DM and teamplay)
//
// Revision 1.12.2.2  2001/05/20 18:54:19  igor_rock
// added original ctf code snippets from zoid. lib compilesand runs but
// doesn't function the right way.
// Jsut committing these to have a base to return to if something wents
// awfully wrong.
//
// Revision 1.12.2.1  2001/05/20 15:17:31  igor_rock
// removed the old ctf code completly
//
// Revision 1.12  2001/05/19 19:33:19  igor_rock
// changed itm_flags and wp_flags to Non-CVAR_LATCH (so you can change them without restart
//
// Revision 1.11  2001/05/17 14:54:47  igor_rock
// added itm_flags for teamplay and ctf
//
// Revision 1.10  2001/05/16 13:26:38  slicerdw
// Too Many Userinfo Cvars( commented some) & Enabled death messages on CTF
//
// Revision 1.9  2001/05/15 15:49:14  igor_rock
// added itm_flags for deathmatch
//
// Revision 1.8  2001/05/14 21:10:16  igor_rock
// added wp_flags support (and itm_flags skeleton - doesn't disturb in the moment)
//
// Revision 1.7  2001/05/13 01:23:01  deathwatch
// Added Single Barreled Handcannon mode, made the menus and scoreboards
// look nicer and made the voice command a bit less loud.
//
// Revision 1.6  2001/05/12 21:19:51  ra
//
//
// Added punishkills.
//
// Revision 1.5  2001/05/12 20:58:22  ra
//
//
// Adding public mapvoting and kickvoting. Its controlable via cvar's mv_public
// and vk_public (both default off)
//
// Revision 1.4  2001/05/12 00:37:03  ra
//
//
// Fixing various compilerwarnings.
//
// Revision 1.3  2001/05/07 21:18:35  slicerdw
// Added Video Checking System
//
// Revision 1.2  2001/05/07 08:32:17  mort
// Basic CTF code
// No spawns etc
// Just the cvars and flag entity
//
// Revision 1.1.1.1  2001/05/06 17:29:57  igor_rock
// This is the PG Bund Edition V1.25 with all stuff laying around here...
//
//-----------------------------------------------------------------------------

#include "g_local.h"
#include "cgf_sfx_glass.h"

field_t fields[] = {
  {"classname", FOFS(classname), F_LSTRING},
  {"origin", FOFS(s.origin), F_VECTOR},
  {"model", FOFS(model), F_LSTRING},
  {"spawnflags", FOFS(spawnflags), F_INT},
  {"speed", FOFS(speed), F_FLOAT},
  {"accel", FOFS(accel), F_FLOAT},
  {"decel", FOFS(decel), F_FLOAT},
  {"target", FOFS(target), F_LSTRING},
  {"targetname", FOFS(targetname), F_LSTRING},
  {"pathtarget", FOFS(pathtarget), F_LSTRING},
  {"deathtarget", FOFS(deathtarget), F_LSTRING},
  {"killtarget", FOFS(killtarget), F_LSTRING},
  {"combattarget", FOFS(combattarget), F_LSTRING},
  {"message", FOFS(message), F_LSTRING},
  {"team", FOFS(team), F_LSTRING},
  {"wait", FOFS(wait), F_FLOAT},
  {"delay", FOFS(delay), F_FLOAT},
  {"random", FOFS(random), F_FLOAT},
  {"move_origin", FOFS(move_origin), F_VECTOR},
  {"move_angles", FOFS(move_angles), F_VECTOR},
  {"style", FOFS(style), F_INT},
  {"count", FOFS(count), F_INT},
  {"health", FOFS(health), F_INT},
  {"sounds", FOFS(sounds), F_INT},
  {"light", 0, F_IGNORE},
  {"dmg", FOFS(dmg), F_INT},
  {"angles", FOFS(s.angles), F_VECTOR},
  {"angle", FOFS(s.angles), F_ANGLEHACK},
  {"mass", FOFS(mass), F_INT},
  {"volume", FOFS(volume), F_FLOAT},
  {"attenuation", FOFS(attenuation), F_FLOAT},
  {"map", FOFS(map), F_LSTRING},
  
  // temp spawn vars -- only valid when the spawn function is called
  {"lip", STOFS(lip), F_INT, FFL_SPAWNTEMP},
  {"distance", STOFS(distance), F_INT, FFL_SPAWNTEMP},
  {"height", STOFS(height), F_INT, FFL_SPAWNTEMP},
  {"noise", STOFS(noise), F_LSTRING, FFL_SPAWNTEMP},
  {"pausetime", STOFS(pausetime), F_FLOAT, FFL_SPAWNTEMP},
  {"item", STOFS(item), F_LSTRING, FFL_SPAWNTEMP},
  {"gravity", STOFS(gravity), F_LSTRING, FFL_SPAWNTEMP},
  {"sky", STOFS(sky), F_LSTRING, FFL_SPAWNTEMP},
  {"skyrotate", STOFS(skyrotate), F_FLOAT, FFL_SPAWNTEMP},
  {"skyaxis", STOFS(skyaxis), F_VECTOR, FFL_SPAWNTEMP},
  {"minyaw", STOFS(minyaw), F_FLOAT, FFL_SPAWNTEMP},
  {"maxyaw", STOFS(maxyaw), F_FLOAT, FFL_SPAWNTEMP},
  {"minpitch", STOFS(minpitch), F_FLOAT, FFL_SPAWNTEMP},
  {"maxpitch", STOFS(maxpitch), F_FLOAT, FFL_SPAWNTEMP},
  {"nextmap", STOFS(nextmap), F_LSTRING, FFL_SPAWNTEMP}
};

// -------- just for savegames ----------
// all pointer fields should be listed here, or savegames
// won't work properly (they will crash and burn).
// this wasn't just tacked on to the fields array, because
// these don't need names, we wouldn't want map fields using
// some of these, and if one were accidentally present twice
// it would double swizzle (fuck) the pointer.

field_t         savefields[] =
{
  {"", FOFS(classname), F_LSTRING},
  {"", FOFS(target), F_LSTRING},
  {"", FOFS(targetname), F_LSTRING},
  {"", FOFS(killtarget), F_LSTRING},
  {"", FOFS(team), F_LSTRING},
  {"", FOFS(pathtarget), F_LSTRING},
  {"", FOFS(deathtarget), F_LSTRING},
  {"", FOFS(combattarget), F_LSTRING},
  {"", FOFS(model), F_LSTRING},
  {"", FOFS(map), F_LSTRING},
  {"", FOFS(message), F_LSTRING},
  
  {"", FOFS(client), F_CLIENT},
  {"", FOFS(item), F_ITEM},
  
  {"", FOFS(goalentity), F_EDICT},
  {"", FOFS(movetarget), F_EDICT},
  {"", FOFS(enemy), F_EDICT},
  {"", FOFS(oldenemy), F_EDICT},
  {"", FOFS(activator), F_EDICT},
  {"", FOFS(groundentity), F_EDICT},
  {"", FOFS(teamchain), F_EDICT},
  {"", FOFS(teammaster), F_EDICT},
  {"", FOFS(owner), F_EDICT},
  {"", FOFS(mynoise), F_EDICT},
  {"", FOFS(mynoise2), F_EDICT},
  {"", FOFS(target_ent), F_EDICT},
  {"", FOFS(chain), F_EDICT},
  
  {NULL, 0, F_INT}
};

field_t         levelfields[] =
{
  {"", LLOFS(changemap), F_LSTRING},
  
  {"", LLOFS(sight_client), F_EDICT},
  {"", LLOFS(sight_entity), F_EDICT},
  {"", LLOFS(sound_entity), F_EDICT},
  {"", LLOFS(sound2_entity), F_EDICT},
  
  {NULL, 0, F_INT}
};

field_t         clientfields[] =
{
  {"", CLOFS(pers.weapon), F_ITEM},
  {"", CLOFS(pers.lastweapon), F_ITEM},
  {"", CLOFS(newweapon), F_ITEM},
  
  {NULL, 0, F_INT}
};

/*
  ============
  InitGame
  
  This will be called when the dll is first loaded, which
  only happens when a new game is started or a save game
  is loaded.
  ============
*/
void InitGame (void)
{
  gi.dprintf ("==== InitGame ====\n");
  
  ReadConfigFile();
  ReadMOTDFile();
  
  gun_x = gi.cvar ("gun_x", "0", 0);
  gun_y = gi.cvar ("gun_y", "0", 0);
  gun_z = gi.cvar ("gun_z", "0", 0);
  
  //FIXME: sv_ prefix is wrong for these
  sv_rollspeed = gi.cvar ("sv_rollspeed", "200", 0);
  sv_rollangle = gi.cvar ("sv_rollangle", "2", 0);
  sv_maxvelocity = gi.cvar ("sv_maxvelocity", "2000", 0);
  sv_gravity = gi.cvar ("sv_gravity", "800", 0);
  
  // noset vars
  dedicated = gi.cvar ("dedicated", "0", CVAR_NOSET);
  
  // latched vars
  sv_cheats = gi.cvar ("cheats", "0", CVAR_SERVERINFO|CVAR_LATCH);
  gi.cvar ("gamename", GAMEVERSION , CVAR_SERVERINFO | CVAR_LATCH);
  gi.cvar ("gamedate", __DATE__ , CVAR_SERVERINFO | CVAR_LATCH);
  
  maxclients = gi.cvar ("maxclients", "8", CVAR_SERVERINFO | CVAR_LATCH);
  deathmatch = gi.cvar ("deathmatch", "1", CVAR_LATCH);
  coop = gi.cvar ("coop", "0", CVAR_LATCH);
  skill = gi.cvar ("skill", "1", CVAR_LATCH);
  maxentities = gi.cvar ("maxentities", "1024", CVAR_LATCH);
  
  //FIREBLADE
  if (!deathmatch->value)
    {
      gi.dprintf("Turning deathmatch on.\n");
      gi.cvar_set("deathmatch", "1");
    }
  if (coop->value)
    {
      gi.dprintf("Turning coop off.\n");
      gi.cvar_set("coop", "0");
    }
  //FIREBLADE
  
  // change anytime vars
  dmflags = gi.cvar ("dmflags", "0", CVAR_SERVERINFO);
  fraglimit = gi.cvar ("fraglimit", "0", CVAR_SERVERINFO);
  timelimit = gi.cvar ("timelimit", "0", CVAR_SERVERINFO);
  capturelimit = gi.cvar ("capturelimit", "0", CVAR_SERVERINFO);
  password = gi.cvar ("password", "", CVAR_USERINFO);
  filterban = gi.cvar("filterban", "1", 0);
  //FIREBLADE
  needpass = gi.cvar("needpass", "0", CVAR_SERVERINFO);
  radiolog = gi.cvar("radiolog", "0", 0);
  teamplay = gi.cvar ("teamplay", "0", CVAR_SERVERINFO|CVAR_LATCH);
  motd_time = gi.cvar("motd_time", "2", 0);
  hostname = gi.cvar("hostname", "unnamed", CVAR_SERVERINFO);
  strtwpn = gi.cvar("dmweapon", MK23_NAME, 0);
  actionmaps = gi.cvar ("actionmaps", "1", 0);
  if (actionmaps->value && num_maps < 1)
    {
      gi.dprintf("No maps were read from the config file, \"actionmaps\" won't be used.\n");
      gi.cvar_set("actionmaps", "0");
    }
  nohud = gi.cvar("nohud", "0", CVAR_LATCH);
  roundlimit = gi.cvar("roundlimit", "0", CVAR_SERVERINFO);
  limchasecam = gi.cvar("limchasecam", "0", CVAR_SERVERINFO|CVAR_LATCH);
  skipmotd = gi.cvar("skipmotd", "0", 0);
  roundtimelimit = gi.cvar("roundtimelimit", "0", CVAR_SERVERINFO);
  maxteamkills = gi.cvar("maxteamkills", "0", 0);
  twbanrounds = gi.cvar("twbanrounds", "2", 0);
  tkbanrounds = gi.cvar("tkbanrounds", "2", 0);
  noscore = gi.cvar("noscore", "0", CVAR_SERVERINFO|CVAR_LATCH);
  actionversion = gi.cvar("actionversion", "none set", CVAR_SERVERINFO|CVAR_LATCH);
  gi.cvar_set("actionversion", ACTION_VERSION);
  //AQ2:TNG - Slicer Too many SERVERINFO CVARS !!!! commenting some.

  //PG BUND - BEGIN
  use_voice = gi.cvar("use_voice", "1", CVAR_LATCH); //slicer
  //use_friendlyfire = gi.cvar("use_friendlyfire", "0", CVAR_SERVERINFO);
  //ff_maxkills = gi.cvar("ff_maxkills", "6", 0);
  //ff_kickat = gi.cvar("ff_kickat", "10", 0);
  // ff_tempban = gi.cvar("ff_tempban", "1", 0);		//commented out for AQ1.52 FF compat. -TempFile 7/25/99
  
  ppl_idletime = gi.cvar("ppl_idletime","15", CVAR_LATCH);
  use_tourney = gi.cvar("use_tourney", "0", CVAR_SERVERINFO|CVAR_LATCH);
  use_3teams = gi.cvar("use_3teams", "0", CVAR_SERVERINFO|CVAR_LATCH);
  use_kickvote = gi.cvar("use_kickvote", "1",0);//slicer
  use_mapvote = gi.cvar("use_mapvote", "1", 0);  //slicer
  ctf = gi.cvar("ctf", "0", CVAR_SERVERINFO|CVAR_LATCH);
  ctf_forcejoin = gi.cvar("ctf_forcejoin", "", 0);
  ctf_mode = gi.cvar("ctf_mode", "", 0);
  ctf_dropflag = gi.cvar("ctf_dropflag", "1", 0);
  //PG BUND - END

// AQ:TNG - JBravo adding public voting and punishkills.
  mv_public = gi.cvar("mv_public", "0", 0);//slicer 
  vk_public = gi.cvar("vk_public", "0", 0);//slicer
  punishkills = gi.cvar("punishkills","1", 0);//slicer
// JBravo
  //AQ2:TNG SLICER END
  //tempfile
  sv_gib = gi.cvar("sv_gib", "0", CVAR_LATCH);
  sv_crlf = gi.cvar("sv_crlf", "1", CVAR_LATCH);
  //tempfile
  
  //Igor[Rock] BEGIN
  vrot = gi.cvar("vrot", "0", CVAR_LATCH);
  rrot = gi.cvar("rrot", "0", CVAR_LATCH);
  llsound = gi.cvar("llsound", "1", CVAR_LATCH);
  cvote = gi.cvar("cvote", "0", CVAR_SERVERINFO);  
  //Igor[Rock] END
	//AQ2:TNG - Slicer
	check_time = gi.cvar("check_time", "3", 0);
	video_check = gi.cvar("video_check", "0", CVAR_LATCH);
	video_max_3dfx = gi.cvar("video_max_3dfx", "1.5", 0);
	video_max_3dfxam = gi.cvar("video_max_3dfxam", "1.5", 0);
	video_max_opengl = gi.cvar("video_max_opengl", "3.0", 0);
	video_force_restart = gi.cvar("video_force_restart", "0", CVAR_LATCH);
	video_check_lockpvs = gi.cvar("video_check_lockpvs", "0",CVAR_LATCH);
	video_checktime = gi.cvar("video_checktime", "20", 0);
// AQ2:TNG Deathwatch - Single Barreled HC
	hc_single = gi.cvar("hc_single", "1", CVAR_LATCH); //default ON
// AQ2:TNG End
	//AQ2:TNG - Igor adding wp_flags/itm_flags
	// 511 = WPF_MK23 | WPF_MP5 | WPF_M4 | WPF_M3 | WPF_HC | WPF_SNIPER | WPF_DUAL | WPF_KNIFE | WPF_GRENADE
	wp_flags  = gi.cvar("wp_flags", "511", 0);
	// 63 = ITF_SIL | ITF_SLIP | ITF_BAND | ITF_KEV | ITF_LASER | ITF_HELM
	itm_flags = gi.cvar("itm_flags", "63", 0);
	//AQ2:TNG End adding flags
	//AQ2:TNG Slicer - Matchmode 
	matchmode = gi.cvar("matchmode", "0",CVAR_SERVERINFO|CVAR_LATCH);
//AQ2:TNG END
  
  //FIREBLADE
  
  //zucc get server variables
  unique_weapons = gi.cvar("weapons", 
			   //FIREBLADE
			   // zucc changed teamplay to 1
			   teamplay->value ? "1" : "1", CVAR_SERVERINFO|CVAR_LATCH);
  //FIREBLADE
  unique_items = gi.cvar("items", "1", CVAR_SERVERINFO|CVAR_LATCH);
  // zucc changed ir to 1, enabled        
  
  ir = gi.cvar("ir", "1", CVAR_SERVERINFO);
  knifelimit = gi.cvar ("knifelimit", "40", 0);
  allweapon = gi.cvar ("allweapon", "0", CVAR_SERVERINFO);
  allitem = gi.cvar ("allitem", "0", CVAR_SERVERINFO);
  tgren = gi.cvar ("tgren", "0", CVAR_SERVERINFO);
  // zucc from action
  sv_shelloff = gi.cvar ("shelloff", "1", 0); 
  bholelimit = gi.cvar ("bholelimit", "0", 0);
  splatlimit = gi.cvar ("splatlimit", "0", 0);
  
  // william for CGF (glass fx)
  CGF_SFX_InstallGlassSupport();
  
  g_select_empty = gi.cvar ("g_select_empty", "0", CVAR_ARCHIVE);
  
  run_pitch = gi.cvar ("run_pitch", "0.002", 0);
  run_roll = gi.cvar ("run_roll", "0.005", 0);
  bob_up  = gi.cvar ("bob_up", "0.005", 0);
  bob_pitch = gi.cvar ("bob_pitch", "0.002", 0);
  bob_roll = gi.cvar ("bob_roll", "0.002", 0);
  
  // flood control
  flood_msgs = gi.cvar ("flood_msgs", "4", 0);
  flood_persecond = gi.cvar ("flood_persecond", "4", 0);
  flood_waitdelay = gi.cvar ("flood_waitdelay", "10", 0);
  
  // items
  InitItems ();
  
  Com_sprintf (game.helpmessage1, sizeof(game.helpmessage1), "");
  
  Com_sprintf (game.helpmessage2, sizeof(game.helpmessage2), "");
  
  // initialize all entities for this game
  game.maxentities = maxentities->value;
  g_edicts =  gi.TagMalloc (game.maxentities * sizeof(g_edicts[0]), TAG_GAME);
  globals.edicts = g_edicts;
  globals.max_edicts = game.maxentities;
  
  // initialize all clients for this game
  game.maxclients = maxclients->value;
  game.clients = gi.TagMalloc (game.maxclients * sizeof(game.clients[0]), TAG_GAME);
  globals.num_edicts = game.maxclients+1;

  if (ctf->value)
    CTFInit();

  //PG BUND - must be at end of gameinit:
  vInitGame();
}

//=========================================================

void WriteField1 (FILE *f, field_t *field, byte *base)
{
  void            *p;
  int                     len;
  int                     index;
  
  p = (void *)(base + field->ofs);
  switch (field->type)
    {
    case F_INT:
    case F_FLOAT:
    case F_ANGLEHACK:
    case F_VECTOR:
    case F_IGNORE:
      break;
      
    case F_LSTRING:
    case F_GSTRING:
      if ( *(char **)p )
	len = strlen(*(char **)p) + 1;
      else
	len = 0;
      *(int *)p = len;
      break;
    case F_EDICT:
      if ( *(edict_t **)p == NULL)
	index = -1;
      else
	index = *(edict_t **)p - g_edicts;
      *(int *)p = index;
      break;
    case F_CLIENT:
      if ( *(gclient_t **)p == NULL)
	index = -1;
      else
	index = *(gclient_t **)p - game.clients;
      *(int *)p = index;
      break;
    case F_ITEM:
      if ( *(edict_t **)p == NULL)
	index = -1;
      else
	index = *(gitem_t **)p - itemlist;
      *(int *)p = index;
      break;
      
    default:
      gi.error ("WriteEdict: unknown field type");
    }
}

void WriteField2 (FILE *f, field_t *field, byte *base)
{
  int                     len;
  void            *p;
  
  p = (void *)(base + field->ofs);
  switch (field->type)
    {
    case F_LSTRING:
    case F_GSTRING:
      if ( *(char **)p )
	{
	  len = strlen(*(char **)p) + 1;
	  fwrite (*(char **)p, len, 1, f);
	}
      break;
// AQ:TNG JBravo fixing Compiler warning. Im not entirely sure here...
    default:
	return;
// End Compiler warning fix
    }
}

void ReadField (FILE *f, field_t *field, byte *base)
{
  void            *p;
  int                     len;
  int                     index;
  
  p = (void *)(base + field->ofs);
  switch (field->type)
    {
    case F_INT:
    case F_FLOAT:
    case F_ANGLEHACK:
    case F_VECTOR:
    case F_IGNORE:
      break;
      
    case F_LSTRING:
      len = *(int *)p;
      if (!len)
	*(char **)p = NULL;
      else
	{
	  *(char **)p = gi.TagMalloc (len, TAG_LEVEL);
	  fread (*(char **)p, len, 1, f);
	}
      break;
    case F_GSTRING:
      len = *(int *)p;
      if (!len)
	*(char **)p = NULL;
      else
	{
	  *(char **)p = gi.TagMalloc (len, TAG_GAME);
	  fread (*(char **)p, len, 1, f);
	}
      break;
    case F_EDICT:
      index = *(int *)p;
      if ( index == -1 )
	*(edict_t **)p = NULL;
      else
	*(edict_t **)p = &g_edicts[index];
      break;
    case F_CLIENT:
      index = *(int *)p;
      if ( index == -1 )
	*(gclient_t **)p = NULL;
      else
	*(gclient_t **)p = &game.clients[index];
      break;
    case F_ITEM:
      index = *(int *)p;
      if ( index == -1 )
	*(gitem_t **)p = NULL;
      else
	*(gitem_t **)p = &itemlist[index];
      break;
      
    default:
      gi.error ("ReadEdict: unknown field type");
    }
}

//=========================================================

/*
  ==============
  WriteClient
  
  All pointer variables (except function pointers) must be handled specially.
  ==============
*/
void WriteClient (FILE *f, gclient_t *client)
{
  field_t         *field;
  gclient_t       temp;
  
  // all of the ints, floats, and vectors stay as they are
  temp = *client;
  
  // change the pointers to lengths or indexes
  for (field=clientfields ; field->name ; field++)
    {
      WriteField1 (f, field, (byte *)&temp);
    }
  
  // write the block
  fwrite (&temp, sizeof(temp), 1, f);
  
  // now write any allocated data following the edict
  for (field=clientfields ; field->name ; field++)
    {
      WriteField2 (f, field, (byte *)client);
    }
}

/*
  ==============
  ReadClient
  
  All pointer variables (except function pointers) must be handled specially.
  ==============
*/
void ReadClient (FILE *f, gclient_t *client)
{
  field_t         *field;
  
  fread (client, sizeof(*client), 1, f);
  
  for (field=clientfields ; field->name ; field++)
    {
      ReadField (f, field, (byte *)client);
    }
}

/*
  ============
  WriteGame
  
  This will be called whenever the game goes to a new level,
  and when the user explicitly saves the game.
  
  Game information include cross level data, like multi level
  triggers, help computer info, and all client states.
  
  A single player death will automatically restore from the
  last save position.
  ============
*/
void WriteGame (char *filename, qboolean autosave)
{       
  FILE    *f;
  int             i;
  char    str[16];
  
  if (!autosave)
    SaveClientData ();
  
  f = fopen (filename, "wb");
  if (!f)
    gi.error ("Couldn't open %s", filename);
  
  memset (str, 0, sizeof(str));
  strcpy (str, __DATE__);
  fwrite (str, sizeof(str), 1, f);
  
  game.autosaved = autosave;
  fwrite (&game, sizeof(game), 1, f);
  game.autosaved = false;
  
  for (i=0 ; i<game.maxclients ; i++)
    WriteClient (f, &game.clients[i]);
  
  fclose (f);
}

void ReadGame (char *filename)
{
  FILE    *f;
  int             i;
  char    str[16];
  
  gi.FreeTags (TAG_GAME);
  
  f = fopen (filename, "rb");
  if (!f)
    gi.error ("Couldn't open %s", filename);
  
  fread (str, sizeof(str), 1, f);
  if (strcmp (str, __DATE__))
    {
      fclose (f);
      gi.error ("Savegame from an older version.\n");
    }
  
  g_edicts =  gi.TagMalloc (game.maxentities * sizeof(g_edicts[0]), TAG_GAME);
  globals.edicts = g_edicts;
  
  fread (&game, sizeof(game), 1, f);
  game.clients = gi.TagMalloc (game.maxclients * sizeof(game.clients[0]), TAG_GAME);
  for (i=0 ; i<game.maxclients ; i++)
    ReadClient (f, &game.clients[i]);
  
  fclose (f);
}

//==========================================================


/*
  ==============
  WriteEdict
  
  All pointer variables (except function pointers) must be handled specially.
  ==============
*/
void WriteEdict (FILE *f, edict_t *ent)
{
  field_t         *field;
  edict_t         temp;
  
  // all of the ints, floats, and vectors stay as they are
  temp = *ent;
  
  // change the pointers to lengths or indexes
  for (field=savefields ; field->name ; field++)
    {
      WriteField1 (f, field, (byte *)&temp);
    }
  
  // write the block
  fwrite (&temp, sizeof(temp), 1, f);
  
  // now write any allocated data following the edict
  for (field=savefields ; field->name ; field++)
    {
      WriteField2 (f, field, (byte *)ent);
    }
  
}

/*
  ==============
  WriteLevelLocals
  
  All pointer variables (except function pointers) must be handled specially.
  ==============
*/
void WriteLevelLocals (FILE *f)
{
  field_t         *field;
  level_locals_t          temp;
  
  // all of the ints, floats, and vectors stay as they are
  temp = level;
  
  // change the pointers to lengths or indexes
  for (field=levelfields ; field->name ; field++)
    {
      WriteField1 (f, field, (byte *)&temp);
    }
  
  // write the block
  fwrite (&temp, sizeof(temp), 1, f);
  
  // now write any allocated data following the edict
  for (field=levelfields ; field->name ; field++)
    {
      WriteField2 (f, field, (byte *)&level);
    }
}


/*
  ==============
  ReadEdict
  
  All pointer variables (except function pointers) must be handled specially.
  ==============
*/
void ReadEdict (FILE *f, edict_t *ent)
{
  field_t         *field;
  
  fread (ent, sizeof(*ent), 1, f);
  
  for (field=savefields ; field->name ; field++)
    {
      ReadField (f, field, (byte *)ent);
    }
}

/*
  ==============
  ReadLevelLocals
  
  All pointer variables (except function pointers) must be handled specially.
  ==============
*/
void ReadLevelLocals (FILE *f)
{
  field_t         *field;
  
  fread (&level, sizeof(level), 1, f);
  
  for (field=levelfields ; field->name ; field++)
    {
      ReadField (f, field, (byte *)&level);
    }
}

/*
  =================
  WriteLevel
  
  =================
*/
void WriteLevel (char *filename)
{
  int             i;
  edict_t *ent;
  FILE    *f;
  void    *base;
  
  f = fopen (filename, "wb");
  if (!f)
    gi.error ("Couldn't open %s", filename);
  
  // write out edict size for checking
  i = sizeof(edict_t);
  fwrite (&i, sizeof(i), 1, f);
  
  // write out a function pointer for checking
  base = (void *)InitGame;
  fwrite (&base, sizeof(base), 1, f);
  
  // write out level_locals_t
  WriteLevelLocals (f);
  
  // write out all the entities
  for (i=0 ; i<globals.num_edicts ; i++)
    {
      ent = &g_edicts[i];
      if (!ent->inuse)
	continue;
      fwrite (&i, sizeof(i), 1, f);
      WriteEdict (f, ent);
    }
  i = -1;
  fwrite (&i, sizeof(i), 1, f);
  
  fclose (f);
}


/*
  =================
  ReadLevel
  
  SpawnEntities will already have been called on the
  level the same way it was when the level was saved.
  
  That is necessary to get the baselines
  set up identically.
  
  The server will have cleared all of the world links before
  calling ReadLevel.
  
  No clients are connected yet.
  =================
*/
void ReadLevel (char *filename)
{
  int             entnum;
  FILE    *f;
  int             i;
  void    *base;
  edict_t *ent;
  
  f = fopen (filename, "rb");
  if (!f)
    gi.error ("Couldn't open %s", filename);
  
  // free any dynamic memory allocated by loading the level
  // base state
  gi.FreeTags (TAG_LEVEL);
  
  // wipe all the entities
  memset (g_edicts, 0, game.maxentities*sizeof(g_edicts[0]));
  globals.num_edicts = maxclients->value+1;
  
  // check edict size
  fread (&i, sizeof(i), 1, f);
  if (i != sizeof(edict_t))
    {
      fclose (f);
      gi.error ("ReadLevel: mismatched edict size");
    }
  
  // check function pointer base address
  fread (&base, sizeof(base), 1, f);
  if (base != (void *)InitGame)
    {
      fclose (f);
      gi.error ("ReadLevel: function pointers have moved");
    }
  
  // load the level locals
  ReadLevelLocals (f);
  
  // load all the entities
  while (1)
    {
      if (fread (&entnum, sizeof(entnum), 1, f) != 1)
	{
	  fclose (f);
	  gi.error ("ReadLevel: failed to read entnum");
	}
      if (entnum == -1)
	break;
      if (entnum >= globals.num_edicts)
	globals.num_edicts = entnum+1;
      
      ent = &g_edicts[entnum];
      ReadEdict (f, ent);
      
      // let the server rebuild world links for this ent
      memset (&ent->area, 0, sizeof(ent->area));
      gi.linkentity (ent);
    }
  
  fclose (f);
  
  // mark all clients as unconnected
  for (i=0 ; i<maxclients->value ; i++)
    {
      ent = &g_edicts[i+1];
      ent->client = game.clients + i;
      ent->client->pers.connected = false;
    }
  
  // do any load time things at this point
  for (i=0 ; i<globals.num_edicts ; i++)
    {
      ent = &g_edicts[i];
      
      if (!ent->inuse)
	continue;
      
      // fire any cross-level triggers
      if (ent->classname)
	if (strcmp(ent->classname, "target_crosslevel_target") == 0)
	  ent->nextthink = level.time + ent->delay;
    }
}
