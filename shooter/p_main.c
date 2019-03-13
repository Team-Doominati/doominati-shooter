//-----------------------------------------------------------------------------
//
// Copyright (C) 2019 Team Doominati
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Playsim main functions.
//
//-----------------------------------------------------------------------------

#include "p_defs.h"


//----------------------------------------------------------------------------|
// Extern Objects                                                             |
//

DGE_Entity P_Player;

P_State P_StateCur = P_State_Stop;

DGE_Team P_TeamEnemy;
DGE_Team P_TeamPlayer;


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

//
// P_Init
//
void P_Init(void)
{
   // Create teams.
   P_TeamEnemy.id = DGE_Team_Create(0);
   DGE_Object_RefAdd(P_TeamEnemy.id);

   P_TeamPlayer.id = DGE_Team_Create(0);
   DGE_Object_RefAdd(P_TeamPlayer.id);
}

//
// P_Quit
//
void P_Quit(void)
{
   // Release reference to player, if any.
   if(P_Player.id)
      DGE_Object_RefSub(P_Player.id);

   // Release references to teams.
   DGE_Object_RefSub(P_TeamEnemy.id);
   DGE_Object_RefSub(P_TeamPlayer.id);
}

//
// P_Task
//
void P_Task(void)
{
}

// EOF

