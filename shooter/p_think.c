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
// P_MissileCreate
//
unsigned P_MissileCreate(unsigned owner_, int damage)
{
   DGE_Entity owner = {owner_};
   DGE_MissileEntity ent = {DGE_MissileEntity_Create(0)};

   ent.damage = damage;
   ent.health = 10;
   ent.owner  = owner.id;
   ent.team   = owner.team;

   ent.cr = owner.cr;
   ent.cg = owner.cr;
   ent.cb = owner.cg;

   ent.x = owner.x;
   ent.y = owner.y;
   ent.z = owner.z;

   ent.sx = 4;
   ent.sy = 4;
   ent.sz = 4;

   ent.rsx = 4;
   ent.rsy = 4;

   ent.sprite = DGE_Texture_Get(M_Str("@gfx/Entity/Missile.png"));

   DGE_PhysicsThinker_Block(ent.id);

   return ent.id;
}

//
// P_Think_Enemy
//
M_Entry void P_Think_Enemy(unsigned id)
{
   DGE_Entity ent = {id};

   DGE_Object_RefAdd(ent.id);

   float x, y;

   unsigned count = 0;

   while(P_StateCur >= P_State_Live)
   {
      if(ent.health <= 0)
      {
         P_Score_Add(100);
         break;
      }

      sincosf(atan2f(P_Player.y - ent.y, P_Player.x - ent.x), &y, &x);

      ent.vx = ent.vx + x;
      ent.vy = ent.vy + y;

      if(!(++count % 20) && (rand() & 1))
      {
         DGE_MissileEntity missile = {P_MissileCreate(ent.id, 10)};

         missile.vx = ent.vx + x * 4;
         missile.vy = ent.vy + y * 4;
      }

      DGE_Task_Sleep(0, 1);
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

   while(P_StateCur >= P_State_Live)
   {
      if(ent.health <= 0)
      {
         P_Score_Sub(1000);
         break;
      }

      unsigned btnAttack = DGE_Input_GetButton(0, M_Bind_Atk);
      unsigned btnAlt    = DGE_Input_GetButton(0, M_Bind_Alt);

      DGE_Point2I cursor = DGE_Input_GetCursor(0);

      if(btnAttack == DGE_Button_Press || btnAlt == DGE_Button_Press)
      {
         float x, y;

         DGE_MissileEntity missile = {P_MissileCreate(ent.id, 25)};

         sincosf(atan2f(cursor.y - M_ScreenH / 2, cursor.x - M_ScreenW / 2), &y, &x);
         missile.vx = ent.vx + x * 8;
         missile.vy = ent.vy + y * 8;
      }

      if(DGE_Input_GetButton(0, M_Bind_Up) & DGE_Button_Down) ent.vy = ent.vy - 2;
      if(DGE_Input_GetButton(0, M_Bind_Dn) & DGE_Button_Down) ent.vy = ent.vy + 2;
      if(DGE_Input_GetButton(0, M_Bind_Lt) & DGE_Button_Down) ent.vx = ent.vx - 2;
      if(DGE_Input_GetButton(0, M_Bind_Rt) & DGE_Button_Down) ent.vx = ent.vx + 2;

      DGE_Task_Sleep(0, 1);
   }

   DGE_Object_RefSub(ent.id);
}

// EOF

