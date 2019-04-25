//-----------------------------------------------------------------------------
//
// Copyright (C) 2019 Team Doominati
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Attack functions.
//
//-----------------------------------------------------------------------------

#include "p_defs.h"

#include <stdlib.h>


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

//
// P_AreaDamage
//
void P_AreaDamage(P_Entity src, DGE_Fixed r, int dmg)
{
   // Damage every entity in range.
   DGE_Fixed xl = src.x - src.sx - r, xu = src.x + src.sx + r;
   DGE_Fixed yl = src.y - src.sy - r, yu = src.y + src.sy + r;
   unsigned find = DGE_BlockMap_Find(xl, yl, xu, yu);
   for(unsigned thC = DGE_BlockMap_FindCountThinker(find); thC--;)
   {
      P_Entity ent = {DGE_Object_Cast(DGE_BlockMap_FindGetThinker(find, thC), DGE_OT_Entity)};
      if(!ent.id || src.team == ent.team ||
         ent.x + ent.sx < xl || xu < ent.x - ent.sx ||
         ent.y + ent.sy < yl || yu < ent.y - ent.sy ||
         !P_Entity_Sight(src, ent))
         continue;

      ent.health = ent.health - dmg;
   }
   DGE_BlockMap_FindFree(find);
}

//
// P_Attack_Bolt
//
unsigned P_Attack_Bolt(P_Entity ent, float angle)
{
   int cost = ent.magBolt + 5;
   if(ent.mana < cost) return 0;
   ent.mana = ent.mana - cost;

   float error = rand() / ((float)RAND_MAX * 32) - 1/64.0f;
   P_SpawnBullet(ent.id, ent.magBolt * 5 + 30, angle + error, 12);

   return 16;
}

//
// P_Attack_Fast
//
unsigned P_Attack_Fast(P_Entity ent, float angle)
{
   int cost = ent.gunFast / 10 + 1;
   if(ent.ammo < cost) return 0;
   ent.ammo = ent.ammo - cost;

   float error = rand() / ((float)RAND_MAX * 8) - 1/16.0f;
   P_SpawnBullet(ent.id, ent.gunFast + 10, angle + error, 12);

   return 6;
}

//
// P_Attack_Hard
//
unsigned P_Attack_Hard(P_Entity ent, float angle)
{
   int cost = ent.gunHard / 2 + 1;
   if(ent.ammo < cost) return 0;
   ent.ammo = ent.ammo - cost;

   float error = rand() / ((float)RAND_MAX * 32) - 1/64.0f;
   P_SpawnBullet(ent.id, ent.gunHard * 5 + 30, angle + error, 12);

   return 26;
}

//
// P_Attack_Rock
//
unsigned P_Attack_Rock(P_Entity ent, float angle)
{
   int cost = ent.gunRock + 5;
   if(ent.ammo < cost) return 0;
   ent.ammo = ent.ammo - cost;

   P_SpawnRocket(ent.id, ent.gunRock * 5 + 30, angle, 16);

   return 36;
}

//
// P_Attack_Slow
//
unsigned P_Attack_Slow(P_Entity ent, float angle)
{
   int costAP = ent.gunFast / 2 + 1;
   int costSP = ent.magBolt     + 5;

   if(ent.ammo < costAP) costAP = ent.ammo;
   if(ent.mana < costSP) costSP = 0;

   if(!costAP && !costSP) return 0;

   ent.ammo = ent.ammo - costAP;
   ent.mana = ent.mana - costSP;

   int damage = 1;
   if(costAP) damage += ent.gunFast + 4;
   if(costSP) damage += ent.magBolt + 4;

   float error = rand() / ((float)RAND_MAX * 32) - 1/64.0f;
   P_SpawnBullet(ent.id, damage, angle + error, 4);

   return 26;
}

//
// P_Attack_Wide
//
unsigned P_Attack_Wide(P_Entity ent, float angle)
{
   int cost = ent.gunWide + 3;
   if(ent.ammo < cost) return 0;
   ent.ammo = ent.ammo - cost;

   while(cost--)
   {
      float error = rand() / ((float)RAND_MAX * 4) - 1/8.0f;
      P_SpawnBullet(ent.id, 10, angle + error, 12);
   }

   return 16;
}

// EOF

