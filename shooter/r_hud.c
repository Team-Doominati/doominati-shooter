//-----------------------------------------------------------------------------
//
// Copyright (C) 2019 Team Doominati
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Renderer main functions.
//
//-----------------------------------------------------------------------------

#include "r_defs.h"


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// R_DrawHudMain_AP
//
static void R_DrawHudMain_AP(int x, int y)
{
   unsigned ammo = P_Player.id ? P_Player.ammo : 0;

   if(ammo > 9999) ammo = 9999;

   DGE_Draw_SetColor(1.0ulr, 1.0ulr, 0.0ulr);
   R_DrawFormatL(x, y, "AP%04u", ammo);
}

//
// R_DrawHudMain_EC
//
static void R_DrawHudMain_EC(int x, int y)
{
   size_t mobjC = P_MapCur->mobjC;

   if(mobjC > 9999) mobjC = 9999;

   DGE_Draw_SetColor(1.0ulr, 0.0ulr, 0.0ulr);
   R_DrawFormatL(x, y, "EC%04u", mobjC);
}

//
// R_DrawHudMain_EL
//
static void R_DrawHudMain_EL(int x, int y)
{
   unsigned level = P_MapCur->level;

   if(level > 9999) level = 9999;

   DGE_Draw_SetColor(1.0ulr, 0.0ulr, 0.0ulr);
   R_DrawFormatL(x, y, "EL%04u", level);
}

//
// R_DrawHudMain_ET
//
static void R_DrawHudMain_ET(int x, int y)
{
   DGE_Draw_SetColor(0.0ulr, 0.0ulr, 1.0ulr);
   if(P_MapCur->nextT < P_Map_NextDelay)
      R_DrawFormatL(x, y, "%06u", P_MapCur->nextT);
   else
      R_DrawCharsL(x, y, "      ");
}

//
// R_DrawHudMain_GS
//
static void R_DrawHudMain_GS(int x, int y)
{
   DGE_Draw_SetColor(0.0ulr, 1.0ulr, 0.0ulr);
   switch(P_StateCur)
   {
   case P_State_Halt: R_DrawCharsL(x, y, "  PAUS  "); break;
   case P_State_Shop: R_DrawCharsL(x, y, "  SHOP  "); break;
   default:           R_DrawCharsL(x, y, "        "); break;
   }
}

//
// R_DrawHudMain_HP
//
static void R_DrawHudMain_HP(int x, int y)
{
   int health = P_Player.id ? P_Player.health : 0;

   if(health <    0) health =    0;
   if(health > 9999) health = 9999;

   DGE_Draw_SetColor(0.0ulr, 1.0ulr, 0.0ulr);
   R_DrawFormatL(x, y, "HP%04u", health);
}

//
// R_DrawHudMain_PL
//
static void R_DrawHudMain_PL(int x, int y)
{
   unsigned level = P_Player.id
      ? P_Entity_Level(P_Player)
      : P_EntityStore_Level(&P_PlayerStore);

   if(level > 9999) level = 9999;

   DGE_Draw_SetColor(1.0ulr, 1.0ulr, 1.0ulr);
   R_DrawFormatL(x, y, "PL%04u", level);
}

//
// R_DrawHudMain_RT
//
static void R_DrawHudMain_RT(int x, int y)
{
   DGE_Draw_SetColor(1.0ulr, 0.0ulr, 0.0ulr);
   if(P_MapCur->respT < P_Map_RespDelay)
      R_DrawFormatL(x, y, "%06u", P_MapCur->respT);
   else
      R_DrawCharsL(x, y, "      ");
}

//
// R_DrawHudMain_SC
//
static void R_DrawHudMain_SC(int x, int y)
{
   DGE_Draw_SetColor(1.0ulr, 1.0ulr, 1.0ulr);
   R_DrawFormatL(x, y, "SC%018ld", (long)P_Score);
}

//
// R_DrawHudMain_SP
//
static void R_DrawHudMain_SP(int x, int y)
{
   unsigned mana = P_Player.id ? P_Player.mana : 0;

   if(mana > 9999) mana = 9999;

   DGE_Draw_SetColor(0.0ulr, 1.0ulr, 1.0ulr);
   R_DrawFormatL(x, y, "SP%04u", mana);
}

//
// DrawHudCB
//
M_Callback("DrawPost") static void DrawHudCB(void)
{
   switch(P_StateCur)
   {
   case P_State_Edit:
      R_DrawHudEdit(0, 0);
      break;

   case P_State_Live:
   case P_State_Halt:
      R_DrawHudLive();
      R_DrawHudInfo(0, M_ScreenH - 64);
      R_DrawHudMain(0, 0, M_ScreenW);
      break;

   case P_State_Shop:
      R_DrawHudLive();
      R_DrawHudMain(0, 0, M_ScreenW);
      R_DrawHudShop();
      break;
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

//
// R_DrawHudInfo
//
void R_DrawHudInfo(int x, int y)
{
   P_Entity ent;
   {
     DGE_Point2I curs = DGE_Input_GetCursor(0);
     DGE_Point2X view = DGE_Renderer_GetViewpoint();
     ent = P_Entity_FindAtPoint(view.x + curs.x, view.y + curs.y);
   }

   DGE_Draw_SetColor(1.0ulr, 0.0ulr, 0.0ulr);

   if(ent.id && ent.health > 0 && ent.team == P_TeamEnemy.id)
   {
      int      health = ent.health;
      unsigned level  = P_Entity_Level(ent);

      if(health > 9999) health = 9999;
      if(level  > 9999) level  = 9999;

      R_DrawFormatL(x, y,      "HP%04u", health);
      R_DrawFormatL(x, y + 32, "EL%04u", level);
   }
   else
   {
      R_DrawCharsL(x, y,      "      ");
      R_DrawCharsL(x, y + 32, "      ");
   }

   DGE_Draw_SetColor(1.0ulr, 1.0ulr, 1.0ulr);
   R_DrawTex(x + 120, y,     R_TexGUI_EdgeBT,   8, 64);
   R_DrawTex(x + 120, y - 8, R_TexGUI_EdgeBL,   8,  8);
   R_DrawTex(x,       y - 8, R_TexGUI_EdgeRL, 120,  8);
}

//
// R_DrawHudLive
//
void R_DrawHudLive(void)
{
   DGE_Point2X view = DGE_Renderer_GetViewpoint();

   unsigned find = DGE_BlockMap_Find(view.x, view.y, view.x + M_ScreenW, view.y + M_ScreenH);

   for(unsigned thC = DGE_BlockMap_FindCountThinker(find); thC--;)
   {
      P_Entity ent = {DGE_Object_Cast(DGE_BlockMap_FindGetThinker(find, thC), DGE_OT_Entity)};
      if(!ent.id || ent.emc != P_EntityMemMax || ent.health <= 0) continue;

      int healthMax = P_Entity_HealthMax(ent);
      if(ent.health < healthMax)
      {
         int w   = ent.rsx * 2;
         int pct = ent.health * w / healthMax;

         int x = ent.x - ent.rsx - view.x;
         int y = ent.y + ent.rsy + 4 - view.y;

         DGE_Texture_Bind(0);
         DGE_Draw_SetColor(0.00ulr, 0.75ulr, 0.00ulr, 0.75ulr);
         DGE_Draw_Rectangle(x, y, x + pct, y + 4);
         DGE_Draw_SetColor(0.75ulr, 0.00ulr, 0.00ulr, 0.75ulr);
         DGE_Draw_Rectangle(x + pct, y, x + w, y + 4);
      }

      if(ent.team == P_TeamEnemy.id)
      {
         int x = ent.x - 6 - view.x + 0.5hk;
         int y = ent.y   -   view.y + 0.5hk;

         DGE_Draw_SetColor(1.00ulr, 0.50ulr, 0.50ulr);
         switch(P_Entity_Rank(ent))
         {
         default:
         case 4: R_DrawTex(x, y - 11, R_TexEntity_Rank, 12, 6);
         case 3: R_DrawTex(x, y -  6, R_TexEntity_Rank, 12, 6);
         case 2: R_DrawTex(x, y -  1, R_TexEntity_Rank, 12, 6);
         case 1: R_DrawTex(x, y +  4, R_TexEntity_Rank, 12, 6);
         case 0: break;
         }
      }
   }

   DGE_BlockMap_FindFree(find);
}

//
// R_DrawHudMain
//
void R_DrawHudMain(int x, int y, int w)
{
   // Draw left.

   R_DrawHudMain_HP(x, y);
   DGE_Draw_SetColor(1.0ulr, 1.0ulr, 1.0ulr);
   R_DrawCharL(x + 120, 0, ' ');
   R_DrawHudMain_AP(x + 140, y);

   R_DrawHudMain_PL(x, y + 32);
   DGE_Draw_SetColor(1.0ulr, 1.0ulr, 1.0ulr);
   R_DrawCharL(x + 120, y + 32, ' ');
   R_DrawHudMain_SP(x + 140, y + 32);

   DGE_Draw_SetColor(1.0ulr, 1.0ulr, 1.0ulr);
   R_DrawTex(x,       y + 64, R_TexGUI_EdgeLR, 260,  8);
   R_DrawTex(x + 260, y + 64, R_TexGUI_EdgeLT,   8,  8);
   R_DrawTex(x + 260, y,      R_TexGUI_EdgeBT,   8, 64);

   // Draw center.

   R_DrawHudMain_SC(x + w / 2 - 200, y);

   R_DrawHudMain_ET(x + w / 2 - 200, y + 32);
   R_DrawHudMain_GS(x + w / 2 -  80, y + 32);
   R_DrawHudMain_RT(x + w / 2 +  80, y + 32);

   DGE_Draw_SetColor(1.0ulr, 1.0ulr, 1.0ulr);
   R_DrawTex(x + w / 2 - 208, y,      R_TexGUI_EdgeTB,   8, 64);
   R_DrawTex(x + w / 2 - 208, y + 64, R_TexGUI_EdgeTR,   8,  8);
   R_DrawTex(x + w / 2 - 200, y + 64, R_TexGUI_EdgeLR, 400,  8);
   R_DrawTex(x + w / 2 + 200, y + 64, R_TexGUI_EdgeLT,   8,  8);
   R_DrawTex(x + w / 2 + 200, y,      R_TexGUI_EdgeBT,   8, 64);

   // Draw right.

   R_DrawHudMain_EC(x + w - 120, y);
   DGE_Draw_SetColor(1.0ulr, 0.0ulr, 0.0ulr);
   R_DrawCharsL(x + w - 260, y, "       ");

   R_DrawHudMain_EL(x + w - 120, y + 32);
   DGE_Draw_SetColor(1.0ulr, 0.0ulr, 0.0ulr);
   R_DrawCharsL(x + w - 260, y + 32, "       ");

   DGE_Draw_SetColor(1.0ulr, 1.0ulr, 1.0ulr);
   R_DrawTex(x + w - 268, y,      R_TexGUI_EdgeTB,   8, 64);
   R_DrawTex(x + w - 268, y + 64, R_TexGUI_EdgeTR,   8,  8);
   R_DrawTex(x + w - 260, y + 64, R_TexGUI_EdgeLR, 260,  8);
}

// EOF

