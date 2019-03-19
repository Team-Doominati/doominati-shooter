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

#include <math.h>
#include <stdlib.h>


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// P_SpawnBase
//
static unsigned P_SpawnBase(int x, int y)
{
   DGE_Entity ent = {DGE_Entity_Create(0)};

   ent.friction = 0.5ulr;
   ent.mass     = 8;
   ent.gx       = 1;
   ent.gy       = 1;
   ent.gz       = 1;

   ent.x = x;
   ent.y = y;
   ent.z = 16;

   ent.sx = 16;
   ent.sy = 16;
   ent.sz = 16;

   ent.rsx = 16;
   ent.rsy = 16;

   return ent.id;
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

//
// P_SpawnEnemy
//
unsigned P_SpawnEnemy(int x, int y)
{
   DGE_Entity ent = {P_SpawnBase(x, y)};

   ent.cr = 1.0ulr;
   ent.cg = 0.0ulr;
   ent.cb = 0.0ulr;

   ent.health = 40;
   ent.team   = P_TeamEnemy.id;

   ent.sprite = DGE_Texture_Get(M_Str("@gfx/Entity/Mobj.png"));

   DGE_PhysicsThinker_Block(ent.id);

   DGE_Task_Create(0, (DGE_CallbackType)P_Think_Enemy, ent.id);

   ++P_MapCur->mobjC;

   return ent.id;
}

//
// P_SpawnMissile
//
unsigned P_SpawnMissile(unsigned owner_, int damage, float angle, DGE_Fixed speed)
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

   float x, y;
   sincosf(angle, &y, &x);
   ent.vx = owner.vx + x * speed;
   ent.vy = owner.vy + y * speed;

   ent.sprite = DGE_Texture_Get(M_Str("@gfx/Entity/Missile.png"));

   DGE_PhysicsThinker_Block(ent.id);

   return ent.id;
}

//
// P_SpawnPlayer
//
unsigned P_SpawnPlayer(int x, int y)
{
   DGE_Entity ent = {P_SpawnBase(x, y)};

   ent.cr = 0.0ulr;
   ent.cg = 1.0ulr;
   ent.cb = 0.0ulr;

   ent.health = 100;
   ent.team   = P_TeamPlayer.id;

   ent.sprite = DGE_Texture_Get(M_Str("@gfx/Entity/Mobj.png"));

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

// EOF

