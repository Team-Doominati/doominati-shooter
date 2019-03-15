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


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

#define P_ScoreMax 999999999


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


//----------------------------------------------------------------------------|
// Extern Objects                                                             |
//

extern DGE_Entity P_Player;

extern unsigned P_Score;

extern P_State P_StateCur;

extern DGE_Team P_TeamEnemy;
extern DGE_Team P_TeamPlayer;


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

void P_Map_Init(int w, int h, char const *tiles, char const *mobjs);
void P_Map_Quit();

unsigned P_MissileCreate(unsigned owner, int damage);

void P_Score_Add(unsigned score);
void P_Score_Sub(unsigned score);

M_Entry void P_Think_Enemy(unsigned id);
M_Entry void P_Think_Player(unsigned id);

void P_Init(void);
void P_Quit(void);
void P_Task(void);

#endif//P_DEFS_H__

