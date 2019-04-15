//-----------------------------------------------------------------------------
//
// Copyright (C) 2019 Team Doominati
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Shop functions.
//
//-----------------------------------------------------------------------------

#include "p_defs.h"

#include "r_defs.h"


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// P_ShopItem_ActionDef
//
#define P_ShopItem_ActionDef(name, mem, inc, max) \
   static void P_Action_##name(P_Entity ent) \
   { \
      unsigned cost = P_Cost_##name(ent); \
      \
      if(P_Score < cost || ent.mem >= (max)) return; \
      \
      P_Score   -= cost; \
      ent.mem = ent.mem + (inc); \
   }

//
// P_ShopItem_CostDef
//
#define P_ShopItem_CostDef(name, mem, mul, pct) \
static unsigned P_Cost_##name(P_Entity ent) \
   { \
      return P_CostMul(M_Fib(ent.mem) * (mul), pct, ent.statCHA);\
   }

//
// P_ShopItem_Decl
//
#define P_ShopItem_Decl(name) \
   static void P_Action_##name(P_Entity ent); \
   static bool P_Cond_##name(P_Entity ent); \
   static unsigned P_Cost_##name(P_Entity ent); \
   static unsigned P_Level_##name(P_Entity ent)


//----------------------------------------------------------------------------|
// Types                                                                      |
//

//
// P_ShopItem
//
typedef struct P_ShopItem
{
   void       (*action)(P_Entity ent);
   P_AttackFunc attack;
   P_CondFunc   cond;
   unsigned   (*cost)(P_Entity ent);
   unsigned   (*level)(P_Entity ent);
   DGE_Unsig    icon;
} P_ShopItem;


//----------------------------------------------------------------------------|
// Static Objects                                                             |
//

P_ShopItem_Decl(BuyAmmo);
P_ShopItem_Decl(BuyHeal);
P_ShopItem_Decl(GunFast);
P_ShopItem_Decl(GunHard);
P_ShopItem_Decl(GunWide);
P_ShopItem_Decl(MagBolt);
P_ShopItem_Decl(StatCHA);
P_ShopItem_Decl(StatEND);
P_ShopItem_Decl(StatINT);
P_ShopItem_Decl(StatSTR);
P_ShopItem_Decl(StatVIT);
P_ShopItem_Decl(StatWIS);

static P_ShopItem P_ShopItems[] =
{
   // Guns.
   {.cond=P_Cond_Always, .attack=P_Attack_Fast,
      .action=P_Action_GunFast, .cost=P_Cost_GunFast, .level=P_Level_GunFast},
   {.cond=P_Cond_Always, .attack=P_Attack_Hard,
      .action=P_Action_GunHard, .cost=P_Cost_GunHard, .level=P_Level_GunHard},
   {.cond=P_Cond_GunWide, .attack=P_Attack_Wide,
      .action=P_Action_GunWide, .cost=P_Cost_GunWide, .level=P_Level_GunWide},
   {.cond=P_Cond_Never},
   {.cond=P_Cond_Never},
   {.cond=P_Cond_Never},
   {.cond=P_Cond_Never},
   {.cond=P_Cond_Never},
   {.cond=P_Cond_Never},
   {.cond=P_Cond_Never},
   {.cond=P_Cond_Never},
   {.cond=P_Cond_Never},

   // Spells.
   {.cond=P_Cond_Always, .attack=P_Attack_Bolt,
      .action=P_Action_MagBolt, .cost=P_Cost_MagBolt, .level=P_Level_MagBolt},
   {.cond=P_Cond_Never},
   {.cond=P_Cond_Never},
   {.cond=P_Cond_Never},
   {.cond=P_Cond_Never},
   {.cond=P_Cond_Never},
   {.cond=P_Cond_Never},
   {.cond=P_Cond_Never},
   {.cond=P_Cond_Never},
   {.cond=P_Cond_Never},
   {.cond=P_Cond_Never},
   {.cond=P_Cond_Never},

   // Supplies.
   {.cond=P_Cond_Always,
      .action=P_Action_BuyHeal, .cost=P_Cost_BuyHeal, .level=P_Level_BuyHeal},
   {.cond=P_Cond_Always,
      .action=P_Action_BuyAmmo, .cost=P_Cost_BuyAmmo, .level=P_Level_BuyAmmo},
   {.cond=P_Cond_Never},
   {.cond=P_Cond_Never},
   {.cond=P_Cond_Never},
   {.cond=P_Cond_Never},
   {.cond=P_Cond_Always,
      .action=P_Action_StatEND, .cost=P_Cost_StatEND, .level=P_Level_StatEND},
   {.cond=P_Cond_Always,
      .action=P_Action_StatVIT, .cost=P_Cost_StatVIT, .level=P_Level_StatVIT},
   {.cond=P_Cond_Always,
      .action=P_Action_StatSTR, .cost=P_Cost_StatSTR, .level=P_Level_StatSTR},
   {.cond=P_Cond_Always,
      .action=P_Action_StatCHA, .cost=P_Cost_StatCHA, .level=P_Level_StatCHA},
   {.cond=P_Cond_Always,
      .action=P_Action_StatWIS, .cost=P_Cost_StatWIS, .level=P_Level_StatWIS},
   {.cond=P_Cond_Always,
      .action=P_Action_StatINT, .cost=P_Cost_StatINT, .level=P_Level_StatINT},
};


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

static unsigned P_CostMul(unsigned cost, unsigned pct, unsigned n);

//
// P_Action_*
//
P_ShopItem_ActionDef(BuyAmmo, ammo,  100, P_Entity_AmmoMax(ent))
P_ShopItem_ActionDef(BuyHeal, health, 10, P_Entity_HealthMax(ent))
P_ShopItem_ActionDef(GunFast, gunFast, 1, 100)
P_ShopItem_ActionDef(GunHard, gunHard, 1, 100)
P_ShopItem_ActionDef(GunWide, gunWide, 1, 100)
P_ShopItem_ActionDef(MagBolt, magBolt, 1, 100)
P_ShopItem_ActionDef(StatCHA, statCHA, 1, 100)
P_ShopItem_ActionDef(StatEND, statEND, 1, 100)
P_ShopItem_ActionDef(StatINT, statINT, 1, 100)
P_ShopItem_ActionDef(StatSTR, statSTR, 1, 100)
P_ShopItem_ActionDef(StatVIT, statVIT, 1, 100)
P_ShopItem_ActionDef(StatWIS, statWIS, 1, 100)

//
// P_Cond_*
//
static bool P_Cond_GunWide(P_Entity ent) {return ent.gunWide;}

//
// P_Cost_*
//
static unsigned P_Cost_BuyAmmo(P_Entity ent)
   {return P_CostMul(P_CostMul(100, 105, P_Entity_Level(ent)), 90, ent.statCHA);}
static unsigned P_Cost_BuyHeal(P_Entity ent)
   {return P_CostMul(P_CostMul(100, 101, P_Entity_Level(ent)), 98, ent.statCHA);}
P_ShopItem_CostDef(GunFast, gunFast, 1000, 99)
P_ShopItem_CostDef(GunHard, gunHard, 1000, 99)
P_ShopItem_CostDef(GunWide, gunWide, 1000, 99)
P_ShopItem_CostDef(MagBolt, magBolt, 1000, 99)
P_ShopItem_CostDef(StatCHA, statCHA, 1000, 99)
P_ShopItem_CostDef(StatEND, statEND, 1000, 99)
P_ShopItem_CostDef(StatINT, statINT, 1000, 99)
P_ShopItem_CostDef(StatSTR, statSTR, 1000, 99)
P_ShopItem_CostDef(StatVIT, statVIT, 1000, 99)
P_ShopItem_CostDef(StatWIS, statWIS, 1000, 99)

//
// P_CostMul
//
static unsigned P_CostMul(unsigned cost, unsigned pct, unsigned n)
{
   while(n--) cost = cost * pct/100;
   return cost;
}

//
// P_Level_*
//
static unsigned P_Level_BuyAmmo(P_Entity ent) {return ent.ammo;}
static unsigned P_Level_BuyHeal(P_Entity ent) {return ent.health > 0 ? ent.health : 0;}
static unsigned P_Level_GunFast(P_Entity ent) {return ent.gunFast;}
static unsigned P_Level_GunHard(P_Entity ent) {return ent.gunHard;}
static unsigned P_Level_GunWide(P_Entity ent) {return ent.gunWide;}
static unsigned P_Level_MagBolt(P_Entity ent) {return ent.magBolt;}
static unsigned P_Level_StatCHA(P_Entity ent) {return ent.statCHA;}
static unsigned P_Level_StatEND(P_Entity ent) {return ent.statEND;}
static unsigned P_Level_StatINT(P_Entity ent) {return ent.statINT;}
static unsigned P_Level_StatSTR(P_Entity ent) {return ent.statSTR;}
static unsigned P_Level_StatVIT(P_Entity ent) {return ent.statVIT;}
static unsigned P_Level_StatWIS(P_Entity ent) {return ent.statWIS;}

//
// P_ShopHudCB
//
M_Callback("DrawPost") static void P_ShopHudCB(void)
{
   if(P_StateCur != P_State_Shop) return;

   int xl = M_ScreenW / 2 - 590;
   int xu = xl + 1200;
   int yl = 80;
   int yu = yl + 360;

   P_ShopItem *item = P_ShopItems;

   DGE_Texture_Bind(0);
   DGE_Draw_SetColor(0.25ulr, 0.25ulr, 0.25ulr);
   DGE_Draw_Rectangle(xl - 8, yl - 8, xu - 12, yu);

   DGE_Draw_SetColor(0.0ulr, 1.0ulr, 0.0ulr);
   R_DrawCharL(M_ScreenW / 2 - 40, 0, 'S');
   R_DrawCharL(M_ScreenW / 2 - 20, 0, 'H');
   R_DrawCharL(M_ScreenW / 2 + 00, 0, 'O');
   R_DrawCharL(M_ScreenW / 2 + 20, 0, 'P');

   for(int y = yl; y != yu; y += 120)
      for(int x = xl; x != xu; x += 100, ++item)
   {
      if(!item->attack || item->cond(P_Player))
         DGE_Draw_SetColor(1.0ulr, 1.0ulr, 1.0ulr);
      else
         DGE_Draw_SetColor(0.5ulr, 0.5ulr, 0.5ulr);

      R_DrawTex(item->icon, x, y, 80, 80);

      if(item->attack)
      {
         if(P_Player.attack1 == item->attack)
            R_DrawTex(R_TexGUI_Icon_Select1, x, y, 80, 80);

         if(P_Player.attack2 == item->attack)
            R_DrawTex(R_TexGUI_Icon_Select2, x, y, 80, 80);
      }

      if(item->level)
      {
         unsigned level = item->level(P_Player);

         DGE_Draw_SetColor(1.0ulr, 1.0ulr, 1.0ulr);
         R_DrawDigitsS_U(x + 40, y + 80, 4, level);
      }

      if(item->cost)
      {
         unsigned cost = item->cost(P_Player);

         if(cost <= P_Score)
            DGE_Draw_SetColor(1.0ulr, 1.0ulr, 1.0ulr);
         else
            DGE_Draw_SetColor(1.0ulr, 0.0ulr, 0.0ulr);

         R_DrawDigitsS_U(x, y + 96, 8, cost);
      }
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

//
// P_ShopInit
//
void P_ShopInit(void)
{
   P_ShopItem *items;

   items = P_ShopItems + 0;
   items[ 0].icon = R_TexGUI_Icon_GunFast;
   items[ 1].icon = R_TexGUI_Icon_GunSlow;
   items[ 2].icon = R_TexGUI_Icon_GunWide;
   items[ 3].icon = R_TexGUI_Icon_Blank;
   items[ 4].icon = R_TexGUI_Icon_Blank;
   items[ 5].icon = R_TexGUI_Icon_Blank;
   items[ 6].icon = R_TexGUI_Icon_Blank;
   items[ 7].icon = R_TexGUI_Icon_Blank;
   items[ 8].icon = R_TexGUI_Icon_Blank;
   items[ 9].icon = R_TexGUI_Icon_Blank;
   items[10].icon = R_TexGUI_Icon_Blank;
   items[11].icon = R_TexGUI_Icon_Blank;

   items = P_ShopItems + 12;
   items[ 0].icon = R_TexGUI_Icon_MagBolt;
   items[ 1].icon = R_TexGUI_Icon_Blank;
   items[ 2].icon = R_TexGUI_Icon_Blank;
   items[ 3].icon = R_TexGUI_Icon_Blank;
   items[ 4].icon = R_TexGUI_Icon_Blank;
   items[ 5].icon = R_TexGUI_Icon_Blank;
   items[ 6].icon = R_TexGUI_Icon_Blank;
   items[ 7].icon = R_TexGUI_Icon_Blank;
   items[ 8].icon = R_TexGUI_Icon_Blank;
   items[ 9].icon = R_TexGUI_Icon_Blank;
   items[10].icon = R_TexGUI_Icon_Blank;
   items[11].icon = R_TexGUI_Icon_Blank;

   items = P_ShopItems + 24;
   items[ 0].icon = R_TexGUI_Icon_ShopHeal;
   items[ 1].icon = R_TexGUI_Icon_ShopAmmo;
   items[ 2].icon = R_TexGUI_Icon_Blank;
   items[ 3].icon = R_TexGUI_Icon_Blank;
   items[ 4].icon = R_TexGUI_Icon_Blank;
   items[ 5].icon = R_TexGUI_Icon_Blank;
   items[ 6].icon = R_TexGUI_Icon_StatEND;
   items[ 7].icon = R_TexGUI_Icon_StatVIT;
   items[ 8].icon = R_TexGUI_Icon_StatSTR;
   items[ 9].icon = R_TexGUI_Icon_StatCHA;
   items[10].icon = R_TexGUI_Icon_StatWIS;
   items[11].icon = R_TexGUI_Icon_StatINT;
}

//
// P_ShopQuit
//
void P_ShopQuit(void)
{
}

//
// P_ShopTask
//
void P_ShopTask(void)
{
   bool btn1 = DGE_Input_GetButton(0, M_Bind_Atk) == DGE_Button_Down;
   bool btn2 = DGE_Input_GetButton(0, M_Bind_Alt) == DGE_Button_Down;
   bool btnU = DGE_Input_GetButton(0, M_Bind_Use) == DGE_Button_Down;

   if(!btn1 && !btn2 && !btnU) return;

   DGE_Point2I cursor = DGE_Input_GetCursor(0);

   // Look for shop item under cursor.
   P_ShopItem *item;

        if(cursor.y <  80) item = NULL;
   else if(cursor.y < 160) item = P_ShopItems +  0;
   else if(cursor.y < 200) item = NULL;
   else if(cursor.y < 280) item = P_ShopItems + 12;
   else if(cursor.y < 320) item = NULL;
   else if(cursor.y < 400) item = P_ShopItems + 24;
   else                    item = NULL;

   if(item)
   {
      int xl = M_ScreenW / 2 - 600;
      int xh = M_ScreenW / 2 + 600;

           if(cursor.x < xl) item = NULL;
      else if(cursor.x < xh) item = &item[(cursor.x - xl) / 100];
      else                   item = NULL;
   }

   if(!item) return;

   if(item->attack && item->cond(P_Player))
   {
      if(btn1) P_Player.attack1 = item->attack;
      if(btn2) P_Player.attack2 = item->attack;
   }

   if(item->action)
      if(btnU) item->action(P_Player);
}

// EOF

