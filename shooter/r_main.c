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
// Extern Objects                                                             |
//

unsigned R_CharTabL[256];
unsigned R_CharTabS[256];

unsigned R_TexEntity_Missile;
unsigned R_TexEntity_Mobj;

unsigned R_TexGUI_Icon_Blank;

unsigned R_TexGUI_Icon_GunFast;
unsigned R_TexGUI_Icon_GunSlow;
unsigned R_TexGUI_Icon_GunWide;

unsigned R_TexGUI_Icon_MagBolt;

unsigned R_TexGUI_Icon_Select1;
unsigned R_TexGUI_Icon_Select2;

unsigned R_TexGUI_Icon_ShopAmmo;
unsigned R_TexGUI_Icon_ShopHeal;

unsigned R_TexGUI_Icon_StatCHA;
unsigned R_TexGUI_Icon_StatEND;
unsigned R_TexGUI_Icon_StatINT;
unsigned R_TexGUI_Icon_StatSTR;
unsigned R_TexGUI_Icon_StatVIT;
unsigned R_TexGUI_Icon_StatWIS;

unsigned R_TexTile_Edit;
unsigned R_TexTile_Exit;
unsigned R_TexTile_Half;
unsigned R_TexTile_Open;
unsigned R_TexTile_Wall;


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// DrawHudCB
//
M_Callback("DrawPost") static void DrawHudCB(void)
{
   if(P_StateCur < P_State_Live) return;

   // Draw health.

   int health = P_Player.id ? P_Player.health : 0;
   if(health <   0) health =   0;
   if(health > 999) health = 999;

   DGE_Draw_SetColor(0.0ulr, 1.0ulr, 0.0ulr);

   R_DrawCharL( 0, 0, 'H');
   R_DrawCharL(20, 0, 'P');

   R_DrawDigitsL_U(40, 0, 3, health);

   DGE_Draw_SetColor(1.0ulr, 1.0ulr, 1.0ulr);

   R_DrawCharL(100, 0, ' ');

   // Draw ammo.

   unsigned ammo = P_Player.id ? P_Player.ammo : 0;
   if(ammo > 9999) ammo = 9999;

   DGE_Draw_SetColor(1.0ulr, 1.0ulr, 0.0ulr);

   R_DrawCharL(120, 0, 'A');
   R_DrawCharL(140, 0, 'P');

   R_DrawDigitsL_U(160, 0, 4, ammo);

   // Draw level.

   unsigned level = P_Entity_Level(P_Player);
   if(level > 9999) level = 9999;

   DGE_Draw_SetColor(1.0ulr, 1.0ulr, 1.0ulr);

   R_DrawCharL(0, 32, 'L');

   R_DrawDigitsL_U(20, 32, 4, level);

   DGE_Draw_SetColor(1.0ulr, 1.0ulr, 1.0ulr);

   R_DrawCharL(100, 32, ' ');

   // Draw mana.

   unsigned mana = P_Player.id ? P_Player.mana : 0;
   if(mana > 9999) mana = 9999;

   DGE_Draw_SetColor(0.0ulr, 1.0ulr, 1.0ulr);

   R_DrawCharL(120, 32, 'S');
   R_DrawCharL(140, 32, 'P');

   R_DrawDigitsL_U(160, 32, 4, mana);

   // Draw score.

   DGE_Draw_SetColor(1.0ulr, 1.0ulr, 1.0ulr);
   R_DrawCharL(M_ScreenW - 220, 0, 'S');
   R_DrawCharL(M_ScreenW - 200, 0, 'C');
   R_DrawDigitsL_U(M_ScreenW - 180, 0, 9, P_Score);

   // Draw enemy count.

   size_t mobjC = P_MapCur->mobjC;
   if(mobjC > 9999) mobjC = 9999;

   DGE_Draw_SetColor(1.0ulr, 0.0ulr, 0.0ulr);
   R_DrawCharL(M_ScreenW - 120, 32, 'E');
   R_DrawCharL(M_ScreenW - 100, 32, 'C');
   R_DrawDigitsL_U(M_ScreenW - 80, 32, 4, mobjC);

   // Draw respawn time.

   if(P_MapCur->respT < P_Map_RespDelay)
   {
      DGE_Draw_SetColor(1.0ulr, 0.0ulr, 0.0ulr);
      R_DrawDigitsL_U(M_ScreenW / 2 - 40, 32, 4, P_MapCur->respT);
   }

   // Draw next map time.

   if(P_MapCur->nextT < P_Map_NextDelay)
   {
      DGE_Draw_SetColor(0.0ulr, 1.0ulr, 0.0ulr);
      R_DrawDigitsL_U(M_ScreenW / 2 - 40, 32, 4, P_MapCur->nextT);
   }

   // Draw pause indicator.
   if(P_StateCur == P_State_Halt)
   {
      int x = M_ScreenW / 2 - 50;

      DGE_Draw_SetColor(0.0ulr, 1.0ulr, 0.0ulr);
      R_DrawCharL(x +  0, 0, 'P');
      R_DrawCharL(x + 20, 0, 'A');
      R_DrawCharL(x + 40, 0, 'U');
      R_DrawCharL(x + 60, 0, 'S');
      R_DrawCharL(x + 80, 0, 'E');
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

//
// R_Init
//
void R_Init(void)
{
   R_CharTabL[' '] = DGE_Texture_Get(M_Str("@gfx/GUI/DigLarge/20.png"));

   R_CharTabL['0'] = DGE_Texture_Get(M_Str("@gfx/GUI/DigLarge/0.png"));
   R_CharTabL['1'] = DGE_Texture_Get(M_Str("@gfx/GUI/DigLarge/1.png"));
   R_CharTabL['2'] = DGE_Texture_Get(M_Str("@gfx/GUI/DigLarge/2.png"));
   R_CharTabL['3'] = DGE_Texture_Get(M_Str("@gfx/GUI/DigLarge/3.png"));
   R_CharTabL['4'] = DGE_Texture_Get(M_Str("@gfx/GUI/DigLarge/4.png"));
   R_CharTabL['5'] = DGE_Texture_Get(M_Str("@gfx/GUI/DigLarge/5.png"));
   R_CharTabL['6'] = DGE_Texture_Get(M_Str("@gfx/GUI/DigLarge/6.png"));
   R_CharTabL['7'] = DGE_Texture_Get(M_Str("@gfx/GUI/DigLarge/7.png"));
   R_CharTabL['8'] = DGE_Texture_Get(M_Str("@gfx/GUI/DigLarge/8.png"));
   R_CharTabL['9'] = DGE_Texture_Get(M_Str("@gfx/GUI/DigLarge/9.png"));

   R_CharTabL['A'] = DGE_Texture_Get(M_Str("@gfx/GUI/DigLarge/A.png"));
   R_CharTabL['C'] = DGE_Texture_Get(M_Str("@gfx/GUI/DigLarge/C.png"));
   R_CharTabL['E'] = DGE_Texture_Get(M_Str("@gfx/GUI/DigLarge/E.png"));
   R_CharTabL['H'] = DGE_Texture_Get(M_Str("@gfx/GUI/DigLarge/H.png"));
   R_CharTabL['I'] = R_CharTabL['1'];
   R_CharTabL['L'] = DGE_Texture_Get(M_Str("@gfx/GUI/DigLarge/L.png"));
   R_CharTabL['O'] = R_CharTabL['0'];
   R_CharTabL['P'] = DGE_Texture_Get(M_Str("@gfx/GUI/DigLarge/P.png"));
   R_CharTabL['S'] = R_CharTabL['5'];
   R_CharTabL['U'] = DGE_Texture_Get(M_Str("@gfx/GUI/DigLarge/U.png"));

   R_CharTabS[' '] = DGE_Texture_Get(M_Str("@gfx/GUI/DigSmall/20.png"));

   R_CharTabS['0'] = DGE_Texture_Get(M_Str("@gfx/GUI/DigSmall/0.png"));
   R_CharTabS['1'] = DGE_Texture_Get(M_Str("@gfx/GUI/DigSmall/1.png"));
   R_CharTabS['2'] = DGE_Texture_Get(M_Str("@gfx/GUI/DigSmall/2.png"));
   R_CharTabS['3'] = DGE_Texture_Get(M_Str("@gfx/GUI/DigSmall/3.png"));
   R_CharTabS['4'] = DGE_Texture_Get(M_Str("@gfx/GUI/DigSmall/4.png"));
   R_CharTabS['5'] = DGE_Texture_Get(M_Str("@gfx/GUI/DigSmall/5.png"));
   R_CharTabS['6'] = DGE_Texture_Get(M_Str("@gfx/GUI/DigSmall/6.png"));
   R_CharTabS['7'] = DGE_Texture_Get(M_Str("@gfx/GUI/DigSmall/7.png"));
   R_CharTabS['8'] = DGE_Texture_Get(M_Str("@gfx/GUI/DigSmall/8.png"));
   R_CharTabS['9'] = DGE_Texture_Get(M_Str("@gfx/GUI/DigSmall/9.png"));
   R_CharTabS['C'] = DGE_Texture_Get(M_Str("@gfx/GUI/DigSmall/C.png"));
   R_CharTabS['I'] = R_CharTabS['1'];
   R_CharTabS['L'] = DGE_Texture_Get(M_Str("@gfx/GUI/DigSmall/L.png"));
   R_CharTabS['O'] = R_CharTabS['0'];
   R_CharTabS['S'] = R_CharTabS['5'];

   R_TexEntity_Missile = DGE_Texture_Get(M_Str("@gfx/Entity/Missile.png"));
   R_TexEntity_Mobj    = DGE_Texture_Get(M_Str("@gfx/Entity/Mobj.png"));

   R_TexGUI_Icon_Blank = DGE_Texture_Get(M_Str("@gfx/GUI/Icon/Blank.png"));

   R_TexGUI_Icon_GunFast = DGE_Texture_Get(M_Str("@gfx/GUI/Icon/GunFast.png"));
   R_TexGUI_Icon_GunSlow = DGE_Texture_Get(M_Str("@gfx/GUI/Icon/GunSlow.png"));
   R_TexGUI_Icon_GunWide = DGE_Texture_Get(M_Str("@gfx/GUI/Icon/GunWide.png"));

   R_TexGUI_Icon_MagBolt = DGE_Texture_Get(M_Str("@gfx/GUI/Icon/MagBolt.png"));

   R_TexGUI_Icon_Select1 = DGE_Texture_Get(M_Str("@gfx/GUI/Icon/Select1.png"));
   R_TexGUI_Icon_Select2 = DGE_Texture_Get(M_Str("@gfx/GUI/Icon/Select2.png"));

   R_TexGUI_Icon_ShopAmmo = DGE_Texture_Get(M_Str("@gfx/GUI/Icon/ShopAmmo.png"));
   R_TexGUI_Icon_ShopHeal = DGE_Texture_Get(M_Str("@gfx/GUI/Icon/ShopHeal.png"));

   R_TexGUI_Icon_StatCHA = DGE_Texture_Get(M_Str("@gfx/GUI/Icon/StatCHA.png"));
   R_TexGUI_Icon_StatEND = DGE_Texture_Get(M_Str("@gfx/GUI/Icon/StatEND.png"));
   R_TexGUI_Icon_StatINT = DGE_Texture_Get(M_Str("@gfx/GUI/Icon/StatINT.png"));
   R_TexGUI_Icon_StatVIT = DGE_Texture_Get(M_Str("@gfx/GUI/Icon/StatVIT.png"));
   R_TexGUI_Icon_StatSTR = DGE_Texture_Get(M_Str("@gfx/GUI/Icon/StatSTR.png"));
   R_TexGUI_Icon_StatWIS = DGE_Texture_Get(M_Str("@gfx/GUI/Icon/StatWIS.png"));

   R_TexTile_Edit = DGE_Texture_Get(M_Str("@gfx/Tile/Edit.png"));
   R_TexTile_Exit = DGE_Texture_Get(M_Str("@gfx/Tile/Exit.png"));
   R_TexTile_Half = DGE_Texture_Get(M_Str("@gfx/Tile/Half.png"));
   R_TexTile_Open = DGE_Texture_Get(M_Str("@gfx/Tile/Open.png"));
   R_TexTile_Wall = DGE_Texture_Get(M_Str("@gfx/Tile/Wall.png"));
}

//
// R_Task
//
void R_Task(void)
{
}

//
// R_Quit
//
void R_Quit(void)
{
}

// EOF

