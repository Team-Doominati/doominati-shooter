//-----------------------------------------------------------------------------
//
// Copyright (C) 2019 Team Doominati
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Basic Entity handling.
//
//-----------------------------------------------------------------------------

#include "p_defs.h"


//----------------------------------------------------------------------------|
// Extern Objects                                                             |
//

P_Entity P_Player;

P_EntityStore P_PlayerStore =
{
   .ammo = 200,

   .attack1 = P_Attack_Fast,
   .attack2 = P_Attack_Bolt,

   .gunFast = 1,

   .health = 100,

   .mana = 100,
};


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

//
// P_Entity_AmmoMax
//
unsigned P_Entity_AmmoMax(P_Entity ent)
{
   return 200 + ent.statSTR * 50;
}

//
// P_Entity_FindAtPoint
//
P_Entity P_Entity_FindAtPoint(DGE_Fixed x, DGE_Fixed y)
{
   unsigned find = DGE_BlockMap_Find(x, y, x, y);

   P_Entity ent;
   for(unsigned thC = DGE_BlockMap_FindCountThinker(find); thC--;)
   {
      ent.id = DGE_Object_Cast(DGE_BlockMap_FindGetThinker(find, thC), DGE_OT_Entity);
      if(ent.emc != P_EntityMemMax) {ent.id = 0; continue;}

      if(ent.x - ent.sx > x || x > ent.x + ent.sx ||
         ent.y - ent.sy > y || y > ent.y + ent.sy)
         {ent.id = 0; continue;}

      break;
   }

   DGE_BlockMap_FindFree(find);

   return ent;
}

//
// P_Entity_HealthMax
//
unsigned P_Entity_HealthMax(P_Entity ent)
{
   int max = 100 + ent.statEND * 20;
   if(ent.team == P_TeamEnemy.id)
      max /= 2;
   return max;
}

//
// P_Entity_Level
//
unsigned P_Entity_Level(P_Entity ent)
{
   return
      ent.gunFast +
      ent.gunHard +
      ent.gunWide +

      ent.magBolt +

      ent.statCHA +
      ent.statEND +
      ent.statINT +
      ent.statSTR +
      ent.statVIT +
      ent.statWIS;
}

//
// P_Entity_ManaMax
//
unsigned P_Entity_ManaMax(P_Entity ent)
{
   return 100 + ent.statWIS * 20;
}

//
// P_Entity_Rank
//
unsigned P_Entity_Rank(P_Entity ent)
{
   unsigned level = P_Entity_Level(ent);
   if(level <= P_MapCur->level)
      return 0;

   switch(level - P_MapCur->level)
   {
   case 1: case 2: case 3: case 4: return 1;
   case 5: case 6:                 return 2;
   case 7:                         return 3;
   case 8:                         return 4;
   default:                        return 5;
   }
}

//
// P_Entity_Regen
//
void P_Entity_Regen(P_Entity ent)
{
   if(ent.health < P_Entity_HealthMax(ent))
   {
      ent.healFrac = ent.healFrac + ent.statVIT;

      while(ent.healFrac >= 250)
      {
         ent.healFrac = ent.healFrac - 250;
         ent.health   = ent.health + 1;
      }
   }
   else
      ent.healFrac = 0;

   if(ent.mana < P_Entity_ManaMax(ent))
   {
      ent.manaFrac = ent.manaFrac + ent.statINT + 2;

      while(ent.manaFrac >= 100)
      {
         ent.manaFrac = ent.manaFrac - 100;
         ent.mana     = ent.mana + 1;
      }
   }
   else
      ent.manaFrac = 0;
}

//
// P_Entity_StoreLoad
//
void P_Entity_StoreLoad(P_Entity ent, P_EntityStore const *store)
{
   ent.ammo     = store->ammo;
   ent.attack1  = store->attack1;
   ent.attack2  = store->attack2;
   ent.gunFast  = store->gunFast;
   ent.gunHard  = store->gunHard;
   ent.gunWide  = store->gunWide;
   ent.magBolt  = store->magBolt;
   ent.health   = store->health;
   ent.mana     = store->mana;
   ent.statCHA  = store->statCHA;
   ent.statEND  = store->statEND;
   ent.statINT  = store->statINT;
   ent.statSTR  = store->statSTR;
   ent.statVIT  = store->statVIT;
   ent.statWIS  = store->statWIS;
}

//
// P_Entity_StoreSave
//
void P_Entity_StoreSave(P_Entity ent, P_EntityStore *store)
{
   store->ammo     = ent.ammo;
   store->attack1  = ent.attack1;
   store->attack2  = ent.attack2;
   store->gunFast  = ent.gunFast;
   store->gunHard  = ent.gunHard;
   store->gunWide  = ent.gunWide;
   store->magBolt  = ent.magBolt;
   store->health   = ent.health > 0 ? ent.health : 100;
   store->mana     = ent.mana;
   store->statCHA  = ent.statCHA;
   store->statEND  = ent.statEND;
   store->statINT  = ent.statINT;
   store->statSTR  = ent.statSTR;
   store->statVIT  = ent.statVIT;
   store->statWIS  = ent.statWIS;
}

//
// P_EntityStore_Level
//
unsigned P_EntityStore_Level(P_EntityStore *store)
{
   return
      store->gunFast +
      store->gunHard +
      store->gunWide +

      store->magBolt +

      store->statCHA +
      store->statEND +
      store->statINT +
      store->statSTR +
      store->statVIT +
      store->statWIS;
}

// EOF

