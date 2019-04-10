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
   static void P_Action_##name(unsigned id) \
   { \
      P_Entity ent = {id}; \
      unsigned cost = P_Cost_##name(id); \
      \
      if(P_Score < cost || ent.mem >= (max)) return; \
      \
      P_Score   -= cost; \
      ent.mem = ent.mem + (inc); \
   }


//----------------------------------------------------------------------------|
// Types                                                                      |
//

//
// P_ShopItem
//
typedef struct P_ShopItem
{
   void       (*action)(unsigned id);
   P_AttackFunc attack;
   P_CondFunc   cond;
   unsigned   (*cost)(unsigned id);
   unsigned   (*level)(unsigned id);
   DGE_Unsig    icon;
} P_ShopItem;


//----------------------------------------------------------------------------|
// Static Objects                                                             |
//

static void P_Action_BuyAmmo(unsigned id);
static unsigned P_Cost_BuyAmmo(unsigned id);
static unsigned P_Level_BuyAmmo(unsigned id);

static void P_Action_BuyHeal(unsigned id);
static unsigned P_Cost_BuyHeal(unsigned id);
static unsigned P_Level_BuyHeal(unsigned id);

static void P_Action_GunFast(unsigned id);
static unsigned P_Cost_GunFast(unsigned id);
static unsigned P_Level_GunFast(unsigned id);

static void P_Action_GunHard(unsigned id);
static unsigned P_Cost_GunHard(unsigned id);
static unsigned P_Level_GunHard(unsigned id);

static void P_Action_GunWide(unsigned id);
static bool P_Cond_GunWide(unsigned id);
static unsigned P_Cost_GunWide(unsigned id);
static unsigned P_Level_GunWide(unsigned id);

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

   // Supplies.
   {.cond=P_Cond_Always,
      .action=P_Action_BuyHeal, .cost=P_Cost_BuyHeal, .level=P_Level_BuyHeal},
   {.cond=P_Cond_Always,
      .action=P_Action_BuyAmmo, .cost=P_Cost_BuyAmmo, .level=P_Level_BuyAmmo},
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
};


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// P_Action_*
//
P_ShopItem_ActionDef(BuyAmmo, ammo,  100, 1000)
P_ShopItem_ActionDef(BuyHeal, health, 10,  100)
P_ShopItem_ActionDef(GunFast, gunFast, 1,  100)
P_ShopItem_ActionDef(GunHard, gunHard, 1,  100)
P_ShopItem_ActionDef(GunWide, gunWide, 1,  100)

//
// P_Cond_*
//
static bool P_Cond_GunWide(unsigned id)
   {P_Entity ent = {id}; return ent.gunWide;}

//
// P_Cost_*
//
static unsigned P_Cost_BuyAmmo(unsigned id)
   {return 100;}
static unsigned P_Cost_BuyHeal(unsigned id)
   {return 50;}
static unsigned P_Cost_GunFast(unsigned id)
   {P_Entity ent = {id}; return M_Fib(ent.gunFast) * 1000;}
static unsigned P_Cost_GunHard(unsigned id)
   {P_Entity ent = {id}; return M_Fib(ent.gunHard) * 1000;}
static unsigned P_Cost_GunWide(unsigned id)
   {P_Entity ent = {id}; return M_Fib(ent.gunWide) * 1000;}

//
// P_Level_*
//
static unsigned P_Level_BuyAmmo(unsigned id)
   {P_Entity ent = {id}; return ent.ammo;}
static unsigned P_Level_BuyHeal(unsigned id)
   {P_Entity ent = {id}; return ent.health > 0 ? ent.health : 0;}
static unsigned P_Level_GunFast(unsigned id)
   {P_Entity ent = {id}; return ent.gunFast;}
static unsigned P_Level_GunHard(unsigned id)
   {P_Entity ent = {id}; return ent.gunHard;}
static unsigned P_Level_GunWide(unsigned id)
   {P_Entity ent = {id}; return ent.gunWide;}

//
// P_ShopHudCB
//
M_Callback("DrawPost") static void P_ShopHudCB(void)
{
   if(P_StateCur != P_State_Shop) return;

   int xl = M_ScreenW / 2 - 590;
   int xu = xl + 1200;
   int yl = 80;
   int yu = yl + 240;

   P_ShopItem *item = P_ShopItems;

   for(int y = yl; y != yu; y += 120)
      for(int x = xl; x != xu; x += 100, ++item)
   {
      if(item->cond(P_Player.id))
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
         unsigned level = item->level(P_Player.id);

         DGE_Draw_SetColor(1.0ulr, 1.0ulr, 1.0ulr);
         R_DrawDigitsS_U(x + 40, y + 80, 4, level);
      }

      if(item->cost)
      {
         unsigned cost = item->cost(P_Player.id);

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
   items[ 0].icon = R_TexGUI_Icon_ShopHeal;
   items[ 1].icon = R_TexGUI_Icon_ShopAmmo;
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
   else if(cursor.y < 160) item = P_ShopItems + 0;
   else if(cursor.y < 200) item = NULL;
   else if(cursor.y < 280) item = P_ShopItems + 12;
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

   if(item->attack && item->cond(P_Player.id))
   {
      if(btn1) P_Player.attack1 = item->attack;
      if(btn2) P_Player.attack2 = item->attack;
   }

   if(item->action)
      if(btnU) item->action(P_Player.id);
}

// EOF

