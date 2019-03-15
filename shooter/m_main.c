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
#include "r_defs.h"

#include <stdio.h>


//----------------------------------------------------------------------------|
// Extern Objects                                                             |
//

int M_ScreenH;
int M_ScreenW;


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

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

   R_Init();
   P_Init();

   while(true)
   {
      P_Task();
      R_Task();

      DGE_Task_Sleep(0, 1);
   }

   P_Quit();
   R_Quit();

   return 0;
}

// EOF

