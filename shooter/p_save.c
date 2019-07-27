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

   else if(strcmp(field, "score") == 0) fscanf(in, " %ld", &P_Score);

   else if(strcmp(field, "ammo")    == 0) fscanf(in, " %d", &P_PlayerStore.ammo);
   else if(strcmp(field, "gunFast") == 0) fscanf(in, " %d", &P_PlayerStore.gunFast);
   else if(strcmp(field, "gunHard") == 0) fscanf(in, " %d", &P_PlayerStore.gunHard);
   else if(strcmp(field, "gunRock") == 0) fscanf(in, " %d", &P_PlayerStore.gunRock);
   else if(strcmp(field, "gunWide") == 0) fscanf(in, " %d", &P_PlayerStore.gunWide);
   else if(strcmp(field, "magBolt") == 0) fscanf(in, " %d", &P_PlayerStore.magBolt);
   else if(strcmp(field, "health")  == 0) fscanf(in, " %d", &P_PlayerStore.health);
   else if(strcmp(field, "mana")    == 0) fscanf(in, " %d", &P_PlayerStore.mana);
   else if(strcmp(field, "statCHA") == 0) fscanf(in, " %d", &P_PlayerStore.statCHA);
   else if(strcmp(field, "statEND") == 0) fscanf(in, " %d", &P_PlayerStore.statEND);
   else if(strcmp(field, "statINT") == 0) fscanf(in, " %d", &P_PlayerStore.statINT);
   else if(strcmp(field, "statSTR") == 0) fscanf(in, " %d", &P_PlayerStore.statSTR);
   else if(strcmp(field, "statVIT") == 0) fscanf(in, " %d", &P_PlayerStore.statVIT);
   else if(strcmp(field, "statWIS") == 0) fscanf(in, " %d", &P_PlayerStore.statWIS);

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

   FILE *out = fopen("/var/shooter/save", "w");
   if(!out)
   {
      fprintf(stderr, "failed to write '%s'\n", "/var/shooter/save");
      return;
   }

   fprintf(out, "score %ld\n", P_Score);

   fprintf(out, "ammo %d\n",    P_PlayerStore.ammo);
   fprintf(out, "gunFast %d\n", P_PlayerStore.gunFast);
   fprintf(out, "gunHard %d\n", P_PlayerStore.gunHard);
   fprintf(out, "gunRock %d\n", P_PlayerStore.gunRock);
   fprintf(out, "gunWide %d\n", P_PlayerStore.gunWide);
   fprintf(out, "magBolt %d\n", P_PlayerStore.magBolt);
   fprintf(out, "health %d\n",  P_PlayerStore.health);
   fprintf(out, "mana %d\n",    P_PlayerStore.mana);
   fprintf(out, "statCHA %d\n", P_PlayerStore.statCHA);
   fprintf(out, "statEND %d\n", P_PlayerStore.statEND);
   fprintf(out, "statINT %d\n", P_PlayerStore.statINT);
   fprintf(out, "statSTR %d\n", P_PlayerStore.statSTR);
   fprintf(out, "statVIT %d\n", P_PlayerStore.statVIT);
   fprintf(out, "statWIS %d\n", P_PlayerStore.statWIS);

   fclose(out);
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

