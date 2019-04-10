//-----------------------------------------------------------------------------
//
// Copyright (C) 2019 Team Doominati
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Score handling.
//
//-----------------------------------------------------------------------------

#include "p_defs.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>


//----------------------------------------------------------------------------|
// Extern Objects                                                             |
//

unsigned P_Score = 0;


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// ScoreAdd
//
static void ScoreAdd(unsigned score)
{
   if(P_Score + score > P_ScoreMax)
      P_Score = P_ScoreMax;
   else
      P_Score += score;
}

//
// ScoreAdder
//
DGE_Callback static void ScoreAdder(unsigned score)
{
   for(; score && P_StateCur >= P_State_Live; DGE_Task_Sleep(0, 1))
   {
      if(P_StateCur > P_State_Live) continue;

      unsigned add = score > 100 ? score / 100 : 1;
      ScoreAdd(add);
      score -= add;
   }

   if(score)
      ScoreAdd(score);
}

//
// ScoreSub
//
static void ScoreSub(unsigned score)
{
   if(P_Score < score)
      P_Score = 0;
   else
      P_Score -= score;
}

//
// ScoreSubber
//
DGE_Callback static void ScoreSubber(unsigned score)
{
   for(; score && P_StateCur >= P_State_Live; DGE_Task_Sleep(0, 1))
   {
      if(P_StateCur > P_State_Live) continue;

      unsigned sub = score > 100 ? score / 100 : 1;
      ScoreSub(sub);
      score -= sub;
   }

   if(score)
      ScoreSub(score);
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

//
// P_Score_Add
//
void P_Score_Add(unsigned score)
{
   DGE_Task_Create(0, (DGE_CallbackType)ScoreAdder, score);
}

//
// P_Score_Sub
//
void P_Score_Sub(unsigned score)
{
   DGE_Task_Create(0, (DGE_CallbackType)ScoreSubber, score);
}

//
// Score
//
M_ShellDefn(Score)
{
   if(argc == 2)
   {
      long score = strtol(argv[1], NULL, 10);
      if(score < 0)
         P_Score_Sub(-score);
      else
         P_Score_Add(score);

      return 0;
   }
   else
   {
      fprintf(stderr, "Usage: Score score\n");

      return 1;
   }
}

// EOF

