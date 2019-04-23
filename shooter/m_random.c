//-----------------------------------------------------------------------------
//
// Copyright (C) 2019 Team Doominati
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Random number functions.
//
//-----------------------------------------------------------------------------

#include "m_defs.h"

#include <stdlib.h>


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

//
// M_Random
//
int M_Random(int min, int max)
{
   if(min == max) return min;

   return rand() % (max - min) + min;
}

// EOF

