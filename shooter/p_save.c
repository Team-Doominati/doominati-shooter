//-----------------------------------------------------------------------------
//
// Copyright (C) 2019 Team Doominati
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Save/load functions.
//
//-----------------------------------------------------------------------------

#include "p_defs.h"

#include <stdlib.h>
#include <string.h>


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// P_LoadField
//
static bool P_LoadField(FILE *in)
{
   char field[8];

   if(fscanf(in, "%7s", field) != 1)
      return false;

   else if(strcmp(field, "score") == 0) fscanf(in, " %lX", &P_Score);

   else if(strcmp(field, "ammo")    == 0) fscanf(in, " %X", &P_PlayerStore.ammo);
   else if(strcmp(field, "gunFast") == 0) fscanf(in, " %X", &P_PlayerStore.gunFast);
   else if(strcmp(field, "gunHard") == 0) fscanf(in, " %X", &P_PlayerStore.gunHard);
   else if(strcmp(field, "gunRock") == 0) fscanf(in, " %X", &P_PlayerStore.gunRock);
   else if(strcmp(field, "gunWide") == 0) fscanf(in, " %X", &P_PlayerStore.gunWide);
   else if(strcmp(field, "magBolt") == 0) fscanf(in, " %X", &P_PlayerStore.magBolt);
   else if(strcmp(field, "health")  == 0) fscanf(in, " %X", &P_PlayerStore.health);
   else if(strcmp(field, "mana")    == 0) fscanf(in, " %X", &P_PlayerStore.mana);
   else if(strcmp(field, "statCHA") == 0) fscanf(in, " %X", &P_PlayerStore.statCHA);
   else if(strcmp(field, "statEND") == 0) fscanf(in, " %X", &P_PlayerStore.statEND);
   else if(strcmp(field, "statINT") == 0) fscanf(in, " %X", &P_PlayerStore.statINT);
   else if(strcmp(field, "statSTR") == 0) fscanf(in, " %X", &P_PlayerStore.statSTR);
   else if(strcmp(field, "statVIT") == 0) fscanf(in, " %X", &P_PlayerStore.statVIT);
   else if(strcmp(field, "statWIS") == 0) fscanf(in, " %X", &P_PlayerStore.statWIS);

   else
      fprintf(stderr, "unknown save field: '%s'\n", field);

   // Skip rest of line.
   for(int c; (c = fgetc(in)) != '\n' && c != EOF;) {}

   return true;
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

//
// P_Load
//
void P_Load(void)
{
   FILE *in = fopen("/var/shooter/save", "r");

   if(!in) return;

   while(P_LoadField(in)) {}

   fclose(in);
}

//
// P_Save
//
void P_Save(void)
{
   DGE_Dir_Create("/var/shooter");

   char buf[1024], *itr = buf;

   itr += sprintf(itr, "score %lX\n", P_Score);

   itr += sprintf(itr, "ammo %X\n",    P_PlayerStore.ammo);
   itr += sprintf(itr, "gunFast %X\n", P_PlayerStore.gunFast);
   itr += sprintf(itr, "gunHard %X\n", P_PlayerStore.gunHard);
   itr += sprintf(itr, "gunRock %X\n", P_PlayerStore.gunRock);
   itr += sprintf(itr, "gunWide %X\n", P_PlayerStore.gunWide);
   itr += sprintf(itr, "magBolt %X\n", P_PlayerStore.magBolt);
   itr += sprintf(itr, "health %X\n",  P_PlayerStore.health);
   itr += sprintf(itr, "mana %X\n",    P_PlayerStore.mana);
   itr += sprintf(itr, "statCHA %X\n", P_PlayerStore.statCHA);
   itr += sprintf(itr, "statEND %X\n", P_PlayerStore.statEND);
   itr += sprintf(itr, "statINT %X\n", P_PlayerStore.statINT);
   itr += sprintf(itr, "statSTR %X\n", P_PlayerStore.statSTR);
   itr += sprintf(itr, "statVIT %X\n", P_PlayerStore.statVIT);
   itr += sprintf(itr, "statWIS %X\n", P_PlayerStore.statWIS);

   if(DGE_File_Create("/var/shooter/save", buf, itr - buf) < 0)
      fprintf(stderr, "failed to write '%s'\n", "/var/shooter/save");
}

//
// Save
//
M_ShellDefn(Save)
{
   P_Entity_StoreSave(P_Player, &P_PlayerStore);
   P_Save();

   return 0;
}

// EOF

