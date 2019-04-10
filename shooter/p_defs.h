//-----------------------------------------------------------------------------
//
// Copyright (C) 2019 Team Doominati
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Playsim definitions.
//
//-----------------------------------------------------------------------------

#ifndef P_DEFS_H__
#define P_DEFS_H__

#include "m_defs.h"

#include <stdio.h>


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

#define P_Map_NextDelay 150
#define P_Map_RespDelay 100

#define P_ScoreMax 999999999

#define P_TileSize 64


//----------------------------------------------------------------------------|
// Types                                                                      |
//

typedef unsigned (*P_AttackFunc)(unsigned id, float angle);
typedef bool (*P_CondFunc)(unsigned id);

//
// P_State
//
typedef enum P_State
{
   P_State_Stop,
   P_State_Quit,

   P_State_Edit,

   P_State_Live,
   P_State_Shop,
   P_State_Halt,
   P_State_Init,
} P_State;

//
// P_Entity
//
typedef struct P_Entity
{
   DGE_Unsig id;

   DGE_EntityProps()

   DGE_PropMem(P_AttackFunc, attack1, DGE_OME_Entity+0)
   DGE_PropMem(P_AttackFunc, attack2, DGE_OME_Entity+1)

   DGE_PropMem(unsigned, ammo, DGE_OME_Entity+2)

   DGE_PropMem(unsigned, gunFast, DGE_OME_Entity+3)
   DGE_PropMem(unsigned, gunHard, DGE_OME_Entity+4)
   DGE_PropMem(unsigned, gunWide, DGE_OME_Entity+5)
} P_Entity;

//
// P_EntityStore
//
typedef struct P_EntityStore
{
   P_AttackFunc attack1;
   P_AttackFunc attack2;
   unsigned     ammo;
   unsigned     gunFast;
   unsigned     gunHard;
   unsigned     gunWide;
   unsigned     health;
} P_EntityStore;

//
// P_Map
//
typedef struct P_Map
{
   char  *data;
   size_t exitC;
   size_t mobjC;
   size_t nextT;
   size_t respT;
   int    w, h;
   char   name[32];
   char   next[32];
} P_Map;

//
// P_Tile
//
typedef struct P_Tile
{
   DGE_Unsig id;

   DGE_SectorProps()

   DGE_PropMem(DGE_Unsig, type, DGE_OME_Sector)
} P_Tile;


//----------------------------------------------------------------------------|
// Extern Objects                                                             |
//

extern P_Map *P_MapCur;

extern P_Entity      P_Player;
extern P_EntityStore P_PlayerStore;

extern unsigned P_Score;

extern P_State P_StateCur;

extern DGE_Team P_TeamEnemy;
extern DGE_Team P_TeamPlayer;


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

unsigned P_Attack_Fast(unsigned id, float angle);
unsigned P_Attack_Hard(unsigned id, float angle);
unsigned P_Attack_Slow(unsigned id, float angle);
unsigned P_Attack_Wide(unsigned id, float angle);

bool P_Cond_Always(unsigned id);
bool P_Cond_Never(unsigned id);

void P_EditInit(unsigned w, unsigned h);
void P_EditQuit(void);
M_Entry void P_EditTask(void);

bool P_Map_InExit(P_Map *map);

void P_Map_Init(P_Map *map);

void P_Map_LoadName(P_Map *map, char const *name);
void P_Map_LoadPath(P_Map *map, char const *path);

void P_Map_Quit(P_Map *map);

void P_Map_Read(P_Map *map, FILE *in);
void P_Map_ReadData(P_Map *map, char *data, FILE *in);
bool P_Map_ReadHead(P_Map *map, FILE *in);

void P_Score_Add(unsigned score);
void P_Score_Sub(unsigned score);

void P_ShopInit(void);
void P_ShopQuit(void);
void P_ShopTask(void);

unsigned P_SpawnEnemy(int x, int y);

unsigned P_SpawnMissile(unsigned owner, int damage, float angle, DGE_Fixed speed);

unsigned P_SpawnPlayer(int x, int y);
unsigned P_SpawnPlayerStart(void);

M_Entry void P_Think_Enemy(unsigned id);
M_Entry void P_Think_Player(unsigned id);

void P_Init(void);
void P_Quit(void);
void P_Task(void);

#endif//P_DEFS_H__

