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

#include <stdarg.h>
#include <stdlib.h>


//----------------------------------------------------------------------------|
// Static Objects                                                             |
//

static char R_DrawFormatBuf[64];


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

//
// R_DrawCharL
//
void R_DrawCharL(int x, int y, char c)
{
   R_DrawTex(x, y, R_CharTabL[c], 20, 32);
}

//
// R_DrawCharS
//
void R_DrawCharS(int x, int y, char c)
{
   R_DrawTex(x, y, R_CharTabS[c], 10, 16);
}

//
// R_DrawCharsL
//
void R_DrawCharsL(int x, int y, char const *s)
{
   for(; *s; x += 20) R_DrawCharL(x, y, *s++);
}

//
// R_DrawCharsS
//
void R_DrawCharsS(int x, int y, char const *s)
{
   for(; *s; x += 10) R_DrawCharS(x, y, *s++);
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
// R_DrawFormatL
//
void R_DrawFormatL(int x, int y, char const *format, ...)
{
   va_list va;
   va_start(va, format);
   vsnprintf(R_DrawFormatBuf, sizeof(R_DrawFormatBuf), format, va);
   va_end(va);

   R_DrawCharsL(x, y, R_DrawFormatBuf);
}

//
// R_DrawFormatS
//
void R_DrawFormatS(int x, int y, char const *format, ...)
{
   va_list va;
   va_start(va, format);
   vsnprintf(R_DrawFormatBuf, sizeof(R_DrawFormatBuf), format, va);
   va_end(va);

   R_DrawCharsS(x, y, R_DrawFormatBuf);
}

//
// R_DrawTex
//
void R_DrawTex(int x, int y, unsigned tex, int w, int h)
{
   DGE_Texture_Bind(tex);
   DGE_Draw_Rectangle(x, y, x + w, y + h);
}

// EOF

