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
unsigned R_TexEntity_Rank;

unsigned R_TexGUI_EdgeBL;
unsigned R_TexGUI_EdgeBR;
unsigned R_TexGUI_EdgeBT;
unsigned R_TexGUI_EdgeLB;
unsigned R_TexGUI_EdgeLR;
unsigned R_TexGUI_EdgeLT;
unsigned R_TexGUI_EdgeRB;
unsigned R_TexGUI_EdgeRL;
unsigned R_TexGUI_EdgeRT;
unsigned R_TexGUI_EdgeTB;
unsigned R_TexGUI_EdgeTL;
unsigned R_TexGUI_EdgeTR;

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
// Extern Functions                                                           |
//

//
// R_Init
//
void R_Init(void)
{
   R_CharTabL[' '] = DGE_Texture_Get(M_Str("@gfx/GUI/DigLarge/20.png"));

   R_CharTabL['-'] = DGE_Texture_Get(M_Str("@gfx/GUI/DigLarge/-.png"));

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
   R_CharTabS['E'] = DGE_Texture_Get(M_Str("@gfx/GUI/DigSmall/E.png"));
   R_CharTabS['I'] = R_CharTabS['1'];
   R_CharTabS['L'] = DGE_Texture_Get(M_Str("@gfx/GUI/DigSmall/L.png"));
   R_CharTabS['O'] = R_CharTabS['0'];
   R_CharTabS['S'] = R_CharTabS['5'];

   R_TexEntity_Missile = DGE_Texture_Get(M_Str("@gfx/Entity/Missile.png"));
   R_TexEntity_Mobj    = DGE_Texture_Get(M_Str("@gfx/Entity/Mobj.png"));
   R_TexEntity_Rank    = DGE_Texture_Get(M_Str("@gfx/Entity/Rank.png"));

   R_TexGUI_EdgeBL = DGE_Texture_Get(M_Str("@gfx/GUI/Edge/BL.png"));
   R_TexGUI_EdgeBR = DGE_Texture_Get(M_Str("@gfx/GUI/Edge/BR.png"));
   R_TexGUI_EdgeBT = DGE_Texture_Get(M_Str("@gfx/GUI/Edge/BT.png"));
   R_TexGUI_EdgeLB = DGE_Texture_Get(M_Str("@gfx/GUI/Edge/LB.png"));
   R_TexGUI_EdgeLR = DGE_Texture_Get(M_Str("@gfx/GUI/Edge/LR.png"));
   R_TexGUI_EdgeLT = DGE_Texture_Get(M_Str("@gfx/GUI/Edge/LT.png"));
   R_TexGUI_EdgeRB = DGE_Texture_Get(M_Str("@gfx/GUI/Edge/RB.png"));
   R_TexGUI_EdgeRL = DGE_Texture_Get(M_Str("@gfx/GUI/Edge/RL.png"));
   R_TexGUI_EdgeRT = DGE_Texture_Get(M_Str("@gfx/GUI/Edge/RT.png"));
   R_TexGUI_EdgeTB = DGE_Texture_Get(M_Str("@gfx/GUI/Edge/TB.png"));
   R_TexGUI_EdgeTL = DGE_Texture_Get(M_Str("@gfx/GUI/Edge/TL.png"));
   R_TexGUI_EdgeTR = DGE_Texture_Get(M_Str("@gfx/GUI/Edge/TR.png"));

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

