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
// P_Attack_Fast
//
unsigned P_Attack_Fast(unsigned id, float angle)
{
   P_Entity ent = {id};

   if(ent.ammo < 1) return 0;
   ent.ammo = ent.ammo - 1;

   float error = rand() / ((float)RAND_MAX * 8) - 1/16.0f;
   P_SpawnMissile(id, ent.gunFast + 9, angle + error, 12);

   return 6;
}

//
// P_Attack_Hard
//
unsigned P_Attack_Hard(unsigned id, float angle)
{
   P_Entity ent = {id};

   if(ent.ammo < 1) return 0;
   ent.ammo = ent.ammo - 1;

   float error = rand() / ((float)RAND_MAX * 32) - 1/64.0f;
   P_SpawnMissile(id, ent.gunHard * 10  + 30, angle + error, 12);

   return 26;
}

//
// P_Attack_Slow
//
unsigned P_Attack_Slow(unsigned id, float angle)
{
   P_Entity ent = {id};

   if(ent.ammo < 1) return 0;
   ent.ammo = ent.ammo - 1;

   float error = rand() / ((float)RAND_MAX * 32) - 1/64.0f;
   P_SpawnMissile(id, 10, angle + error, 4);

   return 26;
}

//
// P_Attack_Wide
//
unsigned P_Attack_Wide(unsigned id, float angle)
{
   P_Entity ent = {id};

   int i = ent.gunWide + 3;

   if(ent.ammo < i) return 0;
   ent.ammo = ent.ammo - i;

   while(i--)
   {
      float error = rand() / ((float)RAND_MAX * 4) - 1/8.0f;
      P_SpawnMissile(id, 10, angle + error, 12);
   }

   return 16;
}

// EOF

