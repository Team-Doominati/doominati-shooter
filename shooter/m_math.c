//-----------------------------------------------------------------------------
//
// Copyright (C) 2019 Team Doominati
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Mathematic functions.
//
//-----------------------------------------------------------------------------

#include "m_defs.h"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

//
// M_Fib
//
unsigned M_Fib(unsigned i)
{
   if(i <= 1) return 1;

   unsigned prev = 1;
   unsigned curr = 1;
   unsigned next;

   while(--i)
   {
      next = prev + curr;
      prev = curr;
      curr = next;
   }

   return curr;
}

// EOF
