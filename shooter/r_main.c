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
   if(P_StateCur < P_State_Live) return;

   // Draw health.

   int health = P_Player.id ? P_Player.health : 0;
   if(health <   0) health =   0;
   if(health > 999) health = 999;

   DGE_Draw_SetColor(
      health <= 50 ? 1.0ulr : health < 100 ? 1.0hk - (health - 50) / 50.0hk : 0.0ulr,
      health < 50 ? health / 50.0hk : 1.0ulr, 0.0ulr);

   DGE_Texture_Bind(R_AlphaTab['H' - 'A']);
   DGE_Draw_Rectangle( 0, 0, 20, 32);
   DGE_Texture_Bind(R_AlphaTab['P' - 'A']);
   DGE_Draw_Rectangle(20, 0, 40, 32);

   R_DrawDigitsU(40, 0, 3, health);

   // Draw respawn time.

   if(P_MapCur->respT < P_Map_RespDelay)
   {
      DGE_Draw_SetColor(0.0ulr, 0.0ulr, 1.0ulr);
      R_DrawDigitsU(40, 32, 3, P_MapCur->respT);
   }

   // Draw score.

   DGE_Draw_SetColor(1.0ulr, 1.0ulr, 1.0ulr);
   DGE_Texture_Bind(R_AlphaTab['S' - 'A']);
   DGE_Draw_Rectangle(M_ScreenW - 220, 0, M_ScreenW - 200, 32);
   DGE_Texture_Bind(R_AlphaTab['C' - 'A']);
   DGE_Draw_Rectangle(M_ScreenW - 200, 0, M_ScreenW - 180, 32);
   R_DrawDigitsU(M_ScreenW - 180, 0, 9, P_Score);

   // Draw next map time.

   if(P_MapCur->nextT < P_Map_NextDelay)
   {
      DGE_Draw_SetColor(0.0ulr, 0.0ulr, 1.0ulr);
      DGE_Texture_Bind(R_AlphaTab['E' - 'A']);
      DGE_Draw_Rectangle(M_ScreenW - 220, 32, M_ScreenW - 200, 64);
      R_DrawDigitsU(M_ScreenW - 200, 32, 4, P_MapCur->nextT);
   }

   // Draw enemy count.

   size_t mobjC = P_MapCur->mobjC;
   if(mobjC > 9999) mobjC = 9999;

   DGE_Draw_SetColor(1.0ulr, 0.0ulr, 0.0ulr);
   DGE_Texture_Bind(R_AlphaTab['E' - 'A']);
   DGE_Draw_Rectangle(M_ScreenW - 100, 32, M_ScreenW - 80, 64);
   R_DrawDigitsU(M_ScreenW - 80, 32, 4, mobjC);
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

   R_AlphaTab['C' - 'A'] = DGE_Texture_Get(M_Str("@gfx/GUI/C.png"));
   R_AlphaTab['E' - 'A'] = DGE_Texture_Get(M_Str("@gfx/GUI/E.png"));
   R_AlphaTab['H' - 'A'] = DGE_Texture_Get(M_Str("@gfx/GUI/H.png"));
   R_AlphaTab['I' - 'A'] = R_DigitTab[1];
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

