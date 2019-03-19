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

//
// P_State
//
typedef enum P_State
{
   P_State_Stop,
   P_State_Quit,

   P_State_Live,
   P_State_Init,
   P_State_Halt,
} P_State;

//
// P_Map
//
typedef struct P_Map
{
   char  *data;
   size_t mobjC;
   size_t nextT;
   size_t respT;
   int    w, h;
   char   name[32];
   char   next[32];
} P_Map;


//----------------------------------------------------------------------------|
// Extern Objects                                                             |
//

extern P_Map *P_MapCur;

extern DGE_Entity P_Player;

extern unsigned P_Score;

extern P_State P_StateCur;

extern DGE_Team P_TeamEnemy;
extern DGE_Team P_TeamPlayer;


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

void P_Map_Init(P_Map *map);

void P_Map_LoadName(P_Map *map, char const *name);
void P_Map_LoadPath(P_Map *map, char const *path);

void P_Map_Quit(P_Map *map);

void P_Map_Read(P_Map *map, FILE *in);
void P_Map_ReadData(P_Map *map, char *data, FILE *in);
bool P_Map_ReadHead(P_Map *map, FILE *in);

void P_Score_Add(unsigned score);
void P_Score_Sub(unsigned score);

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

