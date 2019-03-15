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

unsigned R_CharTab[36];


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// DrawHudCB
//
M_Callback("DrawPost") static void DrawHudCB(void)
{
   if(!P_Player.id) return;

   DGE_Draw_SetColor(0.0ulr, 1.0ulr, 0.0ulr);

   DGE_Texture_Bind(R_AlphaTab['H' - 'A']);
   DGE_Draw_Rectangle( 0, 0, 20, 32);
   DGE_Texture_Bind(R_AlphaTab['P' - 'A']);
   DGE_Draw_Rectangle(20, 0, 40, 32);

   int health = P_Player.health;
   if(health <   0) health =   0;
   if(health > 999) health = 999;

   R_DrawDigitsU(40, 0, 3, health);

   DGE_Draw_SetColor(1.0ulr, 1.0ulr, 1.0ulr);
   R_DrawDigitsU(M_ScreenW - 180, 0, 9, P_Score);
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

//
// R_Init
//
void R_Init(void)
{
   R_DigitTab[0] = DGE_Texture_Get(M_Str("@gfx/GUI/0.png"));
   R_DigitTab[1] = DGE_Texture_Get(M_Str("@gfx/GUI/1.png"));
   R_DigitTab[2] = DGE_Texture_Get(M_Str("@gfx/GUI/2.png"));
   R_DigitTab[3] = DGE_Texture_Get(M_Str("@gfx/GUI/3.png"));
   R_DigitTab[4] = DGE_Texture_Get(M_Str("@gfx/GUI/4.png"));
   R_DigitTab[5] = DGE_Texture_Get(M_Str("@gfx/GUI/5.png"));
   R_DigitTab[6] = DGE_Texture_Get(M_Str("@gfx/GUI/6.png"));
   R_DigitTab[7] = DGE_Texture_Get(M_Str("@gfx/GUI/7.png"));
   R_DigitTab[8] = DGE_Texture_Get(M_Str("@gfx/GUI/8.png"));
   R_DigitTab[9] = DGE_Texture_Get(M_Str("@gfx/GUI/9.png"));

   R_AlphaTab['H' - 'A'] = DGE_Texture_Get(M_Str("@gfx/GUI/H.png"));
   R_AlphaTab['I' - 'A'] = R_DigitTab[0];
   R_AlphaTab['O' - 'A'] = R_DigitTab[0];
   R_AlphaTab['P' - 'A'] = DGE_Texture_Get(M_Str("@gfx/GUI/P.png"));
   R_AlphaTab['S' - 'A'] = R_DigitTab[5];
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

