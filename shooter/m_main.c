//-----------------------------------------------------------------------------
//
// Copyright (C) 2019 Team Doominati
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Miscellaneous main functions.
//
//-----------------------------------------------------------------------------

#include "m_defs.h"

#include "p_defs.h"

#include <stdio.h>
#include <stdlib.h>


//----------------------------------------------------------------------------|
// Extern Objects                                                             |
//

int M_ScreenH;
int M_ScreenW;


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// DrawHudCB
//
M_Callback("DrawPost") static void DrawHudCB(void)
{
   if(!P_Player.id) return;

   static const DGE_String digs[] =
   {
      M_Str("@gfx/GUI/0.png"),
      M_Str("@gfx/GUI/1.png"),
      M_Str("@gfx/GUI/2.png"),
      M_Str("@gfx/GUI/3.png"),
      M_Str("@gfx/GUI/4.png"),
      M_Str("@gfx/GUI/5.png"),
      M_Str("@gfx/GUI/6.png"),
      M_Str("@gfx/GUI/7.png"),
      M_Str("@gfx/GUI/8.png"),
      M_Str("@gfx/GUI/9.png"),
   };

   DGE_Draw_SetColor(0.0r, 1.0r, 0.0r);

   DGE_Texture_Bind(DGE_Texture_Get(M_Str("@gfx/GUI/HP.png")));
   DGE_Draw_Rectangle(0, 0, 40, 32);

   int health = P_Player.health;
   if(health <   0) health =   0;
   if(health > 999) health = 999;

   for(int x = 80; x != 20; x -= 20)
   {
      div_t d = div(health, 10);
      DGE_Texture_Bind(DGE_Texture_Get(digs[d.rem]));
      DGE_Draw_Rectangle(x, 0, x + 20, 32);
      health = d.quot;
   }
}

//
// ResizeCB
//
M_Callback("Resize") static void Resize(int w, int h)
{
   DGE_Renderer_SetVirtualRes(M_ScreenW = w, M_ScreenH = h);
}


//-----------------------------------------------------------------------------|
// Extern Functions                                                            |
//

//
// main
//
M_EntryMain int main(void)
{
   printf("\nDoominati: Shoot-Bang\n\n");

   DGE_Renderer_SetVirtualRes(M_ScreenW = 1280, M_ScreenH = 720);

   DGE_Input_SetBindMouse(M_Bind_Alt, DGE_Mouse_Right);
   DGE_Input_SetBindMouse(M_Bind_Atk, DGE_Mouse_Left);

   DGE_Input_SetBindKey(M_Bind_Dn, 's');
   DGE_Input_SetBindKey(M_Bind_Lt, 'a');
   DGE_Input_SetBindKey(M_Bind_Rt, 'd');
   DGE_Input_SetBindKey(M_Bind_Up, 'w');

   P_Init();

   while(true)
   {
      P_Task();

      DGE_Task_Sleep(0, 1);
   }

   P_Quit();

   return 0;
}

// EOF

