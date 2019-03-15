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
// Macros                                                                     |
//

#define R_AlphaTab (R_CharTab + 10)
#define R_DigitTab (R_CharTab +  0)


//----------------------------------------------------------------------------|
// Extern Objects                                                             |
//

extern unsigned R_CharTab[36];


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

void R_DrawDigitsU(int x, int y, int w, unsigned i);

void R_Init(void);
void R_Task(void);
void R_Quit(void);

#endif//R_DEFS_H__

