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

#include <stdlib.h>


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
   if(P_StateCur == P_State_Live)
   {
      DGE_Thinker_ThinkAll();

      if(!P_Player.id)
      {
         if(!P_MapCur->respT)
            P_SpawnPlayerStart();
         else
            --P_MapCur->respT;
      }
      else
         P_MapCur->respT = P_Map_RespDelay;

      if(P_Map_InExit(P_MapCur) && P_MapCur->next[0])
      {
         if(!P_MapCur->nextT)
         {
            P_Map *old = P_MapCur;
            P_Map_Quit(old);
            P_Map_LoadName(malloc(sizeof(P_Map)), old->next);
            free(old);
         }
         else
            --P_MapCur->nextT;
      }
      else if(P_MapCur->nextT < P_Map_NextDelay)
         ++P_MapCur->nextT;
   }
}

// EOF

