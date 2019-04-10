//-----------------------------------------------------------------------------
//
// Copyright (C) 2019 Team Doominati
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Text rendering.
//
//-----------------------------------------------------------------------------

#include "r_defs.h"

#include <stdlib.h>


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

//
// R_DrawCharL
//
void R_DrawCharL(int x, int y, char c)
{
   R_DrawTex(R_CharTabL[c], x, y, 20, 32);
}

//
// R_DrawCharS
//
void R_DrawCharS(int x, int y, char c)
{
   R_DrawTex(R_CharTabS[c], x, y, 10, 16);
}

//
// R_DrawDigitsL_U
//
void R_DrawDigitsL_U(int x, int y, int w, unsigned i)
{
   for(int xu = x + w * 20; xu != x;)
   {
      div_t d = div(i, 10);
      R_DrawCharL(xu -= 20, y, d.rem + '0');
      i = d.quot;
   }
}

//
// R_DrawDigitsS_U
//
void R_DrawDigitsS_U(int x, int y, int w, unsigned i)
{
   for(int xu = x + w * 10; xu != x;)
   {
      div_t d = div(i, 10);
      R_DrawCharS(xu -= 10, y, d.rem + '0');
      i = d.quot;
   }
}

//
// R_DrawTex
//
void R_DrawTex(unsigned tex, int x, int y, int w, int h)
{
   DGE_Texture_Bind(tex);
   DGE_Draw_Rectangle(x, y, x + w, y + h);
}

// EOF

