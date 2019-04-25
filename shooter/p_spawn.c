//-----------------------------------------------------------------------------
//
// Copyright (C) 2019 Team Doominati
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Entity spawning.
//
//-----------------------------------------------------------------------------

#include "p_defs.h"

#include "r_defs.h"

#include <math.h>
#include <stdlib.h>


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// P_SpawnBase
//
static P_Entity P_SpawnBase(int x, int y)
{
   P_Entity ent = {DGE_Entity_Create(P_EntityMemMax)};

   ent.friction = 0.5ulr;
   ent.mass     = 8;
   ent.gx       = 1;
   ent.gy       = 1;
   ent.gz       = 1;

   ent.bvx = 8;
   ent.bvy = 8;

   ent.x = x;
   ent.y = y;
   ent.z = 16;

   ent.sx = 16;
   ent.sy = 16;
   ent.sz = 16;

   ent.rsx = 16;
   ent.rsy = 16;

   return ent;
}

//
// P_Rocket_CollideF
//
DGE_Callback
static unsigned P_Rocket_CollideF(unsigned self, unsigned id)
{
   return false;
}

//
// P_Rocket_CollideI
//
DGE_Callback
static unsigned P_Rocket_CollideI(unsigned self_, unsigned id)
{
   P_Rocket self = {self_};
   P_Entity ent  = {DGE_Object_Cast(id, DGE_OT_Entity)};

   // Don't collide if already dead.
   if(self.health <= 0) return false;

   if(ent.id)
   {
      if(self.team == ent.team)
         return false;
   }

   self.health = 0;
   return true;
}

//
// P_Rocket_Think
//
DGE_Callback
static void P_Rocket_Think(unsigned self_)
{
   P_Rocket self = {self_};

   if(self.health <= 0)
   {
      P_AreaDamage((P_Entity){self.id}, self.damage, self.damage);

      DGE_PhysicsThinker_Unblock(self.id);
      DGE_Thinker_Unlink(self.id);

      return;
   }

   if(abshk(self.vx) < self.speed && abshk(self.vy) < self.speed)
   {
      self.vx = self.vx + self.ax;
      self.vy = self.vy + self.ay;
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

//
// P_SpawnBullet
//
unsigned P_SpawnBullet(unsigned owner_, int damage, float angle, DGE_Fixed speed)
{
   P_Entity owner = {owner_};
   DGE_MissileEntity ent = {DGE_MissileEntity_Create(0)};

   ent.damage = damage;
   ent.health = 10;
   ent.owner  = owner.id;
   ent.team   = owner.team;

   ent.cr = owner.cr;
   ent.cg = 1.0ulr;
   ent.cb = owner.cg;

   ent.x = owner.x;
   ent.y = owner.y;
   ent.z = owner.z;

   ent.sx = 8;
   ent.sy = 8;
   ent.sz = 8;

   ent.rsx = 8;
   ent.rsy = 8;

   float x, y;
   sincosf(angle, &y, &x);
   ent.vx = owner.vx + x * speed;
   ent.vy = owner.vy + y * speed;

   ent.sprite = R_TexEntity_Bullet;

   DGE_PhysicsThinker_Block(ent.id);

   return ent.id;
}

//
// P_SpawnEnemy
//
unsigned P_SpawnEnemy(int x, int y)
{
   P_Entity ent = P_SpawnBase(x, y);

   ent.cr = 1.0ulr;
   ent.cg = 0.0ulr;
   ent.cb = 0.0ulr;

   ent.team = P_TeamEnemy.id;

   ent.sprite = R_TexEntity_Mobj;

   ent.attack1 = P_Attack_Slow;
   ent.attack2 = P_Attack_Slow;

   // Allocate stats.
   unsigned level = P_MapCur->level;
   for(int i = 8; i-- && !M_Random(0, 3);) ++level;
   ent.statEND = M_Random(0, level / 3); level -= ent.statEND;
   ent.gunFast = M_Random(0, level / 2); level -= ent.gunFast;
   ent.statSTR = M_Random(0, level / 2); level -= ent.statSTR;
   ent.statVIT = M_Random(0, level / 2); level -= ent.statVIT;
   ent.magBolt = M_Random(0, level / 1); level -= ent.magBolt;
   ent.statWIS = M_Random(0, level / 1); level -= ent.statWIS;
   ent.statINT = M_Random(0, level / 1); level -= ent.statINT;
   ent.statCHA = level; /* Dump stat. */ level -= ent.statCHA;

   // Calculate stat-based values.
   ent.ammo   = P_Entity_AmmoMax(ent);
   ent.health = P_Entity_HealthMax(ent);
   ent.mana   = P_Entity_ManaMax(ent);

   DGE_PhysicsThinker_Block(ent.id);

   DGE_Task_Create(0, (DGE_CallbackType)P_Think_Enemy, ent.id);

   ++P_MapCur->mobjC;

   return ent.id;
}

//
// P_SpawnPlayer
//
unsigned P_SpawnPlayer(int x, int y)
{
   P_Entity ent = P_SpawnBase(x, y);

   ent.cr = 0.0ulr;
   ent.cg = 1.0ulr;
   ent.cb = 0.0ulr;

   ent.team = P_TeamPlayer.id;

   ent.sprite = R_TexEntity_Mobj;

   P_Entity_StoreLoad(ent, &P_PlayerStore);

   DGE_PhysicsThinker_Block(ent.id);

   DGE_Task_Create(0, (DGE_CallbackType)P_Think_Player, ent.id);

   DGE_Renderer_SetViewpoint(ent.id);

   P_Player.id = ent.id;

   return ent.id;
}

//
// P_SpawnPlayerStart
//
unsigned P_SpawnPlayerStart(void)
{
   int found = 0, x, y;

   int   w = P_MapCur->w, h = P_MapCur->h;
   char *mobjs = P_MapCur->data + w * h;

   for(int ty = 0; ty != h; ++ty) for(int tx = 0; tx != w; ++tx)
   {
      if(mobjs[ty * w + tx] != 'P')
         continue;

      if(found && rand() > RAND_MAX / ++found)
         continue;

      x = tx * P_TileSize;
      y = ty * P_TileSize;
   }

   return P_SpawnPlayer(x, y);
}

//
// P_SpawnRocket
//
unsigned P_SpawnRocket(unsigned owner_, int damage, float angle, DGE_Fixed speed)
{
   P_Entity owner = {owner_};
   P_Rocket ent = {DGE_ScriptedEntity_Create(4)};

   ent.collideF = P_Rocket_CollideF;
   ent.collideI = P_Rocket_CollideI;
   ent.think    = P_Rocket_Think;

   ent.damage = damage;
   ent.health = 10;
   ent.team   = owner.team;

   ent.cr = owner.cr;
   ent.cg = 1.0ulr;
   ent.cb = owner.cg;

   ent.x = owner.x;
   ent.y = owner.y;
   ent.z = owner.z;

   ent.sx = 8;
   ent.sy = 8;
   ent.sz = 8;

   ent.rsx = 8;
   ent.rsy = 8;

   float x, y;
   sincosf(angle, &y, &x);
   ent.vx = owner.vx;
   ent.vy = owner.vy;

   ent.ax = x;
   ent.ay = y;

   ent.speed = speed;

   ent.sprite = R_TexEntity_Rocket[M_AngleToIndex(angle, 8)];

   DGE_PhysicsThinker_Block(ent.id);

   return ent.id;
}

// EOF

