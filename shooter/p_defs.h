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

#define P_ScoreMax 999999999999999999L
#define P_ScoreMin -99999999999999999L

#define P_TileSize 64


//----------------------------------------------------------------------------|
// Types                                                                      |
//

typedef struct P_Entity P_Entity;

typedef unsigned (*P_AttackFunc)(P_Entity ent, float angle);
typedef bool (*P_CondFunc)(P_Entity ent);

typedef long P_ScoreT;

//
// P_EntityMem
//
typedef enum P_EntityMem
{
   P_EntityMem_Ammo,

   P_EntityMem_Attack1,
   P_EntityMem_Attack2,

   P_EntityMem_GunFast,
   P_EntityMem_GunHard,
   P_EntityMem_GunWide,

   P_EntityMem_HealFrac,

   P_EntityMem_MagBolt,

   P_EntityMem_Mana,
   P_EntityMem_ManaFrac,

   P_EntityMem_StatCHA, // AP Cost
   P_EntityMem_StatEND, // HP Max
   P_EntityMem_StatINT, // SP Regen
   P_EntityMem_StatSTR, // AP Max
   P_EntityMem_StatVIT, // HP Regen
   P_EntityMem_StatWIS, // SP Max

   P_EntityMemMax
} P_EntityMem;

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
struct P_Entity
{
   DGE_Unsig id;

   DGE_EntityProps()

   DGE_PropMem(unsigned, ammo, DGE_OME_Entity + P_EntityMem_Ammo)

   DGE_PropMem(P_AttackFunc, attack1, DGE_OME_Entity + P_EntityMem_Attack1)
   DGE_PropMem(P_AttackFunc, attack2, DGE_OME_Entity + P_EntityMem_Attack2)

   DGE_PropMem(unsigned, gunFast, DGE_OME_Entity + P_EntityMem_GunFast)
   DGE_PropMem(unsigned, gunHard, DGE_OME_Entity + P_EntityMem_GunHard)
   DGE_PropMem(unsigned, gunWide, DGE_OME_Entity + P_EntityMem_GunWide)

   DGE_PropMem(unsigned, healFrac, DGE_OME_Entity + P_EntityMem_HealFrac)

   DGE_PropMem(unsigned, magBolt, DGE_OME_Entity + P_EntityMem_MagBolt)

   DGE_PropMem(unsigned, statCHA, DGE_OME_Entity + P_EntityMem_StatCHA)
   DGE_PropMem(unsigned, statEND, DGE_OME_Entity + P_EntityMem_StatEND)
   DGE_PropMem(unsigned, statINT, DGE_OME_Entity + P_EntityMem_StatINT)
   DGE_PropMem(unsigned, statSTR, DGE_OME_Entity + P_EntityMem_StatSTR)
   DGE_PropMem(unsigned, statVIT, DGE_OME_Entity + P_EntityMem_StatVIT)
   DGE_PropMem(unsigned, statWIS, DGE_OME_Entity + P_EntityMem_StatWIS)

   DGE_PropMem(unsigned, mana,     DGE_OME_Entity + P_EntityMem_Mana)
   DGE_PropMem(unsigned, manaFrac, DGE_OME_Entity + P_EntityMem_ManaFrac)
};

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
   unsigned     magBolt;
   unsigned     health;
   unsigned     mana;
   unsigned     statCHA;
   unsigned     statEND;
   unsigned     statINT;
   unsigned     statSTR;
   unsigned     statVIT;
   unsigned     statWIS;
} P_EntityStore;

//
// P_Map
//
typedef struct P_Map
{
   char    *data;
   size_t   exitC;
   size_t   mobjC;
   size_t   nextT;
   size_t   respT;
   unsigned level;
   int      w, h;
   char     name[32];
   char     next[32];
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

extern P_ScoreT P_Score;

extern P_State P_StateCur;

extern DGE_Team P_TeamEnemy;
extern DGE_Team P_TeamPlayer;


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

unsigned P_Attack_Bolt(P_Entity ent, float angle);
unsigned P_Attack_Fast(P_Entity ent, float angle);
unsigned P_Attack_Hard(P_Entity ent, float angle);
unsigned P_Attack_Slow(P_Entity ent, float angle);
unsigned P_Attack_Wide(P_Entity ent, float angle);

bool P_Cond_Always(P_Entity ent);
bool P_Cond_Never(P_Entity ent);

void P_EditInit(unsigned w, unsigned h);
void P_EditQuit(void);
M_Entry void P_EditTask(void);

unsigned P_Entity_AmmoMax(P_Entity ent);

P_Entity P_Entity_FindAtPoint(DGE_Fixed x, DGE_Fixed y);

unsigned P_Entity_HealthMax(P_Entity ent);

unsigned P_Entity_Level(P_Entity ent);

unsigned P_Entity_ManaMax(P_Entity ent);

void P_Entity_Regen(P_Entity ent);

void P_Entity_StoreLoad(P_Entity ent, P_EntityStore const *store);
void P_Entity_StoreSave(P_Entity ent, P_EntityStore *store);

unsigned P_EntityStore_Level(P_EntityStore *store);

bool P_Map_InExit(P_Map *map);

void P_Map_Init(P_Map *map);

void P_Map_LoadName(P_Map *map, char const *name);
void P_Map_LoadPath(P_Map *map, char const *path);

void P_Map_Quit(P_Map *map);

void P_Map_Read(P_Map *map, FILE *in);
void P_Map_ReadData(P_Map *map, char *data, FILE *in);
bool P_Map_ReadHead(P_Map *map, FILE *in);

void P_Score_Add(P_ScoreT score);
void P_Score_Sub(P_ScoreT score);

void P_ShopInit(void);
void P_ShopQuit(void);
void P_ShopTask(void);

unsigned P_SpawnEnemy(int x, int y);

unsigned P_SpawnMissile(unsigned owner, int damage, float angle, DGE_Fixed speed);

unsigned P_SpawnPlayer(int x, int y);
unsigned P_SpawnPlayerStart(void);

M_Entry void P_Think_Enemy(P_Entity ent);
M_Entry void P_Think_Player(P_Entity ent);

void P_Init(void);
void P_Quit(void);
void P_Task(void);

#endif//P_DEFS_H__

