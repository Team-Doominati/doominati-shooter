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
M_Entry void P_Think_Enemy(unsigned id)
{
   DGE_Entity ent = {id};

   DGE_Object_RefAdd(ent.id);

   float x, y;

   unsigned cooldown = 0;

   for(; P_StateCur >= P_State_Live; DGE_Task_Sleep(0, 1))
   {
      if(ent.health <= 0)
      {
         --P_MapCur->mobjC;
         P_Score_Add(100);
         break;
      }

      if(!P_Player.id) continue;

      float angle = atan2f(P_Player.y - ent.y, P_Player.x - ent.x);
      sincosf(angle, &y, &x);

      if(abshk(ent.vx) + abshk(ent.vy) < 12)
      {
         ent.vx = ent.vx + (short accum)x * 0.5hk;
         ent.vy = ent.vy + (short accum)y * 0.5hk;
      }

      if(!cooldown && rand() < RAND_MAX / 16)
      {
         float error = rand() / ((float)RAND_MAX * 32) - 1/64.0f;
         P_SpawnMissile(ent.id, 10, angle + error, 4);
         cooldown = 26;
      }

      if(cooldown) --cooldown;
   }

   DGE_Object_RefSub(ent.id);
}

//
// P_Think_Player
//
M_Entry void P_Think_Player(unsigned id)
{
   DGE_Entity ent = {id};

   DGE_Object_RefAdd(ent.id);

   unsigned cooldown = 0;

   float x, y;

   for(; P_StateCur >= P_State_Live; DGE_Task_Sleep(0, 1))
   {
      if(ent.health <= 0)
      {
         P_Score_Sub(1000);
         break;
      }

      unsigned btnAtk = DGE_Input_GetButton(0, M_Bind_Atk);
      unsigned btnAlt = DGE_Input_GetButton(0, M_Bind_Alt);

      DGE_Point2I cursor = DGE_Input_GetCursor(0);

      if(!cooldown && (btnAtk & DGE_Button_Down))
      {
         float error = rand() / ((float)RAND_MAX * 8) - 1/16.0f;
         float angle = atan2f(cursor.y - M_ScreenH / 2, cursor.x - M_ScreenW / 2);
         P_SpawnMissile(ent.id, 10, angle + error, 12);

         cooldown = 6;
      }

      if(!cooldown && (btnAlt & DGE_Button_Down))
      {
         float angle = atan2f(cursor.y - M_ScreenH / 2, cursor.x - M_ScreenW / 2);
         P_SpawnMissile(ent.id, 40, angle, 8);

         cooldown = 26;
      }

      if(cooldown) --cooldown;

      if(DGE_Input_GetButton(0, M_Bind_Up) & DGE_Button_Down) ent.vy = ent.vy - 0.5hk;
      if(DGE_Input_GetButton(0, M_Bind_Dn) & DGE_Button_Down) ent.vy = ent.vy + 0.5hk;
      if(DGE_Input_GetButton(0, M_Bind_Lt) & DGE_Button_Down) ent.vx = ent.vx - 0.5hk;
      if(DGE_Input_GetButton(0, M_Bind_Rt) & DGE_Button_Down) ent.vx = ent.vx + 0.5hk;
   }

   DGE_Object_RefSub(ent.id);

   if(P_Player.id == id) P_Player.id = 0;
}

// EOF

