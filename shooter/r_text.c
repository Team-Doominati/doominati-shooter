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
// R_DrawDigitsU
//
void R_DrawDigitsU(int x, int y, int w, unsigned i)
{
   for(int xu = x + w * 20; xu != x; xu -= 20)
   {
      div_t d = div(i, 10);
      DGE_Texture_Bind(R_DigitTab[d.rem]);
      DGE_Draw_Rectangle(xu - 20, y, xu, y + 32);
      i = d.quot;
   }
}

// EOF

