//-----------------------------------------------------------------------------
//
// Copyright (C) 2019 Team Doominati
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Renderer definitions.
//
//-----------------------------------------------------------------------------

#ifndef R_DEFS_H__
#define R_DEFS_H__

#include "p_defs.h"


//----------------------------------------------------------------------------|
// Extern Objects                                                             |
//

extern unsigned R_CharTabL[256];
extern unsigned R_CharTabS[256];

extern unsigned R_TexEntity_Missile;
extern unsigned R_TexEntity_Mobj;

extern unsigned R_TexGUI_Icon_Blank;

extern unsigned R_TexGUI_Icon_GunFast;
extern unsigned R_TexGUI_Icon_GunSlow;
extern unsigned R_TexGUI_Icon_GunWide;

extern unsigned R_TexGUI_Icon_MagBolt;

extern unsigned R_TexGUI_Icon_Select1;
extern unsigned R_TexGUI_Icon_Select2;

extern unsigned R_TexGUI_Icon_ShopAmmo;
extern unsigned R_TexGUI_Icon_ShopHeal;

extern unsigned R_TexGUI_Icon_StatCHA;
extern unsigned R_TexGUI_Icon_StatEND;
extern unsigned R_TexGUI_Icon_StatINT;
extern unsigned R_TexGUI_Icon_StatSTR;
extern unsigned R_TexGUI_Icon_StatVIT;
extern unsigned R_TexGUI_Icon_StatWIS;

extern unsigned R_TexTile_Edit;
extern unsigned R_TexTile_Exit;
extern unsigned R_TexTile_Half;
extern unsigned R_TexTile_Open;
extern unsigned R_TexTile_Wall;


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

void R_DrawCharL(int x, int y, char c);
void R_DrawCharS(int x, int y, char c);
void R_DrawDigitsL_U(int x, int y, int w, unsigned i);
void R_DrawDigitsS_U(int x, int y, int w, unsigned i);

void R_DrawTex(unsigned tex, int x, int y, int w, int h);

void R_Init(void);
void R_Task(void);
void R_Quit(void);

#endif//R_DEFS_H__

