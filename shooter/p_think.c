//-----------------------------------------------------------------------------
//
// Copyright (C) 2019 Team Doominati
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Think functions.
//
//-----------------------------------------------------------------------------

#include "p_defs.h"

#include <math.h>
#include <stdlib.h>


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

//
// P_Think_Enemy
//
M_Entry void P_Think_Enemy(P_Entity ent)
{
   DGE_Object_RefAdd(ent.id);

   float x, y;

   unsigned cooldown = 0;

   for(; P_StateCur >= P_State_Live; DGE_Task_Sleep(0, 1))
   {
      if(P_StateCur > P_State_Live) continue;

      if(ent.health <= 0)
      {
         --P_MapCur->mobjC;
         P_Score_Add(100);
         break;
      }

      P_Entity_Regen(ent);

      if(!P_Player.id) continue;

      float angle = atan2f(P_Player.y - ent.y, P_Player.x - ent.x);
      sincosf(angle, &y, &x);

      if(abshk(ent.vx) + abshk(ent.vy) < 12)
      {
         ent.vx = ent.vx + (short accum)x * 0.5hk;
         ent.vy = ent.vy + (short accum)y * 0.5hk;
      }

      ent.ammo = 100;

      if(!cooldown && rand() < RAND_MAX / 16)
         cooldown = ent.attack1(ent, angle);

      if(cooldown) --cooldown;
   }

   DGE_Object_RefSub(ent.id);
}

//
// P_Think_Player
//
M_Entry void P_Think_Player(P_Entity ent)
{
   DGE_Object_RefAdd(ent.id);

   unsigned cooldown = 0;

   float x, y;

   for(; P_StateCur >= P_State_Live; DGE_Task_Sleep(0, 1))
   {
      if(P_StateCur > P_State_Live) continue;

      if(ent.health <= 0)
      {
         P_Score_Sub(500);
         break;
      }

      P_Entity_Regen(ent);

      if(DGE_Input_GetButton(0, M_Bind_Up) & DGE_Button_Down) ent.vy = ent.vy - 0.5hk;
      if(DGE_Input_GetButton(0, M_Bind_Dn) & DGE_Button_Down) ent.vy = ent.vy + 0.5hk;
      if(DGE_Input_GetButton(0, M_Bind_Lt) & DGE_Button_Down) ent.vx = ent.vx - 0.5hk;
      if(DGE_Input_GetButton(0, M_Bind_Rt) & DGE_Button_Down) ent.vx = ent.vx + 0.5hk;

      if(P_StateCur > P_State_Live) continue;

      DGE_Point2I cursor = DGE_Input_GetCursor(0);
      float angle = atan2f(cursor.y - M_ScreenH / 2, cursor.x - M_ScreenW / 2);

      if(!cooldown && (DGE_Input_GetButton(0, M_Bind_Atk) & DGE_Button_Down))
         cooldown = ent.attack1(ent, angle);

      if(!cooldown && (DGE_Input_GetButton(0, M_Bind_Alt) & DGE_Button_Down))
         cooldown = ent.attack2(ent, angle);

      if(cooldown) --cooldown;
   }

   // Sync to player store.
   if(P_Player.id == ent.id)
   {
      P_Entity_StoreSave(ent, &P_PlayerStore);

      P_Player.id = 0;
   }

   DGE_Object_RefSub(ent.id);
}

// EOF

