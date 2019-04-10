//-----------------------------------------------------------------------------
//
// Copyright (C) 2019 Team Doominati
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Condition functions.
//
//-----------------------------------------------------------------------------

#include "p_defs.h"


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

//
// P_Cond_Always
//
bool P_Cond_Always(unsigned id)
{
   return true;
}

//
// P_Cond_Never
//
bool P_Cond_Never(unsigned id)
{
   return false;
}

// EOF

