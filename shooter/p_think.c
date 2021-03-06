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
         P_Score_Add(P_Entity_Level(ent) * 25 + 100);
         break;
      }

      P_Entity_Regen(ent);

      if(!ent.target)
      {
         if(!P_Player.id || !P_Entity_Sight(ent, P_Player))
            continue;

         ent.target = P_Player.id;
         DGE_Object_RefAdd(ent.target);
      }

      P_Entity target = {ent.target};

      if(target.health < 0) {DGE_Object_RefSub(ent.target); ent.target = 0; continue;}

      if(abshk(ent.vx) + abshk(ent.vy) < P_Entity_Rank(ent) * 1.5hk + 6)
      {
         DGE_Fixed accel = 0.25hk;
         if(ent.x < target.x) ent.vx = ent.vx + accel;
         if(ent.x > target.x) ent.vx = ent.vx - accel;
         if(ent.y < target.y) ent.vy = ent.vy + accel;
         if(ent.y > target.y) ent.vy = ent.vy - accel;
      }

      if(!cooldown && P_Entity_Sight(ent, target))
         cooldown = ent.attack1(ent, atan2f(target.y - ent.y, target.x - ent.x));

      if(cooldown) --cooldown;
   }

   if(ent.target) DGE_Object_RefSub(ent.target);
   DGE_Object_RefSub(ent.id);
}

//
// P_Think_Player
//
M_Entry void P_Think_Player(P_Entity ent)
{
   DGE_Object_RefAdd(ent.id);

   unsigned cooldown1 = 0;
   unsigned cooldown2 = 0;

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

      if(!cooldown1 && (DGE_Input_GetButton(0, M_Bind_Atk) & DGE_Button_Down))
         cooldown1 = ent.attack1(ent, angle);

      if(!cooldown2 && (DGE_Input_GetButton(0, M_Bind_Alt) & DGE_Button_Down))
         cooldown2 = ent.attack2(ent, angle);

      if(cooldown1) --cooldown1;
      if(cooldown2) --cooldown2;
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

