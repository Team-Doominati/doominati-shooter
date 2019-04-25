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

#include <math.h>


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

//
// M_AngleToIndex
//
unsigned M_AngleToIndex(float angle, unsigned steps)
{
   float steprad = M_PIf / steps;

   if(angle < -steprad)
      return (int)(-angle * (steps / 2) / M_PIf + 0.5f);
   else if(angle > steprad)
      return steps - (int)(angle * (steps / 2) / M_PIf + 0.5f);
   else
      return 0;
}

//
// M_Fib
//
unsigned long M_Fib(unsigned i)
{
   if(i <= 1) return 1;

   unsigned long prev = 1, curr = 1, next;

   while(--i)
   {
      next = prev + curr;
      prev = curr;
      curr = next;
   }

   return curr;
}

//
// M_LineBoxCollide2D
//
bool M_LineBoxCollide2D(DGE_Fixed xl, DGE_Fixed yl, DGE_Fixed xu, DGE_Fixed yu,
   DGE_Fixed x, DGE_Fixed y, DGE_Fixed dirx, DGE_Fixed diry)
{
   bool midx, midy;
   DGE_Fixed cx, cy;

        if(x < xl) {midx = false; cx = xl;}
   else if(x > xu) {midx = false; cx = xu;}
   else             midx = true;
        if(y < yl) {midy = false; cy = yl;}
   else if(y > yu) {midy = false; cy = yu;}
   else             midy = true;

   // Origin is inside box.
   if(midx && midy)
      return true;

   DGE_Fixed tx = !midx && dirx ? (cx - x) / dirx : -1;
   DGE_Fixed ty = !midy && diry ? (cy - y) / diry : -1;

   DGE_Fixed max = tx > ty ? tx : ty;

   if(max < 0) return false;

   DGE_Fixed hitx = x + max * dirx;
   DGE_Fixed hity = y + max * diry;

   if(hitx < xl || hitx > xu || hity < yl || hity > yu)
      return false;

   return true;
}

// EOF

