//-----------------------------------------------------------------------------
//
// Copyright (C) 2019 Team Doominati
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Map handling.
//
//-----------------------------------------------------------------------------

#include "p_defs.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>


//----------------------------------------------------------------------------|
// Extern Objects                                                             |
//

P_Map *P_MapCur = NULL;


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// P_TileCreate
//
static unsigned P_TileCreate(int tx, int ty)
{
   DGE_Fixed x = tx * P_TileSize;
   DGE_Fixed y = ty * P_TileSize;

   P_Tile sec = {DGE_Sector_Create(4, 1)};

   DGE_Object_RefAdd(sec.id);

   sec.type = 0;

   sec.gz = -1;

   DGE_Sector_PointSet(sec.id, 0, (DGE_Point2X){x - P_TileSize / 2, y - P_TileSize / 2});
   DGE_Sector_PointSet(sec.id, 1, (DGE_Point2X){x - P_TileSize / 2, y + P_TileSize / 2});
   DGE_Sector_PointSet(sec.id, 2, (DGE_Point2X){x + P_TileSize / 2, y + P_TileSize / 2});
   DGE_Sector_PointSet(sec.id, 3, (DGE_Point2X){x + P_TileSize / 2, y - P_TileSize / 2});

   DGE_Sector_CalcBounds(sec.id);
   DGE_Sector_Block(sec.id);

   return sec.id;
}

//
// P_TileCreate_Exit
//
static unsigned P_TileCreate_Exit(int tx, int ty)
{
   P_Tile sec = {P_TileCreate(tx, ty)};

   sec.type = 'E';

   sec.friction = 0.015625ulr;

   sec.zu   = P_TileSize;
   sec.texf = DGE_Texture_Get(M_Str("@gfx/Tile/Exit.png"));

   ++P_MapCur->exitC;

   return sec.id;
}

//
// P_TileCreate_Half
//
static unsigned P_TileCreate_Half(int tx, int ty)
{
   P_Tile sec = {P_TileCreate(tx, ty)};

   sec.friction = 0.0625ulr;

   sec.zl   = 4;
   sec.zu   = P_TileSize;
   sec.texc = DGE_Texture_Get(M_Str("@gfx/Tile/Half.png"));
   sec.texf = DGE_Texture_Get(M_Str("@gfx/Tile/Open.png"));

   return sec.id;
}

//
// P_TileCreate_Open
//
static unsigned P_TileCreate_Open(int tx, int ty)
{
   P_Tile sec = {P_TileCreate(tx, ty)};

   sec.friction = 0.015625ulr;

   sec.zu   = P_TileSize;
   sec.texf = DGE_Texture_Get(M_Str("@gfx/Tile/Open.png"));

   return sec.id;
}

//
// P_TileCreate_Wall
//
static unsigned P_TileCreate_Wall(int tx, int ty)
{
   P_Tile sec = {P_TileCreate(tx, ty)};

   sec.friction = 0.25ulr;

   sec.texf = DGE_Texture_Get(M_Str("@gfx/Tile/Wall.png"));

   return sec.id;
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

//
// P_Map_InExit
//
bool P_Map_InExit(P_Map *map)
{
   if(!map->exitC || !P_Player.id) return false;

   bool     found = false;
   unsigned find  = DGE_BlockMap_Find(
      P_Player.x - P_Player.sx, P_Player.y - P_Player.sy,
      P_Player.x + P_Player.sx, P_Player.y + P_Player.sy);

   // Look for an exit tile.
   unsigned secC = DGE_BlockMap_FindCountSector(find);
   for(unsigned i = 0; i != secC; ++i)
   {
      P_Tile sec = {DGE_BlockMap_FindGetSector(find, i)};

      if(sec.type != 'E')
         continue;

      if(sec.xl > P_Player.x || P_Player.x > sec.xu ||
         sec.yl > P_Player.y || P_Player.y > sec.yu)
         continue;

      found = true;
      break;
   }

   DGE_BlockMap_FindFree(find);

   return found;
}

//
// P_Map_Init
//
void P_Map_Init(P_Map *map)
{
   P_StateCur = P_State_Init;
   P_MapCur   = map;

   int   w = map->w;
   int   h = map->h;
   char *tiles = map->data;
   char *mobjs = map->data + w * h;

   map->exitC = 0;
   map->mobjC = 0;

   for(int ty = 0; ty != h; ++ty) for(int tx = 0; tx != w; ++tx)
   {
      switch(tiles[ty * w + tx])
      {
      case ' ': break;
      case 'E': P_TileCreate_Exit(tx, ty); break;
      case 'H': P_TileCreate_Half(tx, ty); break;
      case 'O': P_TileCreate_Open(tx, ty); break;
      case 'W': P_TileCreate_Wall(tx, ty); break;

      default:
         printf("Unknown tile '%c' (%02X) at (%u, %u)\n",
            tiles[ty * w + tx], tiles[ty * w + tx], tx, ty);
         break;
      }

      switch(mobjs[ty * w + tx])
      {
      case ' ': break;
      case 'E': P_SpawnEnemy(tx * P_TileSize, ty * P_TileSize); break;
      case 'P': break; // Spawn player later.

      default:
         printf("Unknown mobj '%c' (%02X) at (%u, %u)\n",
            mobjs[ty * w + tx], mobjs[ty * w + tx], tx, ty);
         break;
      }
   }

   P_SpawnPlayerStart();

   DGE_BlockMap_Split(P_TileSize, 1);

   P_StateCur = P_State_Live;
}

//
// P_Map_LoadName
//
void P_Map_LoadName(P_Map *map, char const *name)
{
   size_t nameLen = strlen(name);
   char  *path    = malloc(nameLen + 7);
   memcpy(path+0, "/maps/", 6);
   memcpy(path+6, name, nameLen);
   path[nameLen + 6] = '\0';

   P_Map_LoadPath(map, path);

   free(path);
}

//
// P_Map_LoadPath
//
void P_Map_LoadPath(P_Map *map, char const *path)
{
   FILE *in = fopen(path, "r");
   if(in)
   {
      P_Map_Read(map, in);

      if(map->name[0])
         printf("Now entering %s\n", map->name);

      P_Map_Init(map);

      // Print map message.
      for(int c; (c = fgetc(in)) != EOF;)
         putchar(c);
      putchar('\n');

      fclose(in);
   }
   else
      printf("Failed to open map: '%s'\n", path);
}

//
// P_Map_Quit
//
void P_Map_Quit(P_Map *map)
{
   P_StateCur = P_State_Quit;

   DGE_Renderer_SetViewpoint(0);

   // Give an execution tick for all control tasks to see state change.
   DGE_Task_Sleep(0, 1);

   unsigned find = DGE_BlockMap_FindAll();

   unsigned secC = DGE_BlockMap_FindCountSector(find);
   for(unsigned i = 0; i != secC; ++i)
   {
      unsigned sec = DGE_BlockMap_FindGetSector(find, i);
      DGE_Sector_Unblock(sec);
      DGE_Object_RefSub(sec);
   }

   unsigned thC = DGE_BlockMap_FindCountThinker(find);
   for(unsigned i = 0; i != thC; ++i)
   {
      unsigned th = DGE_BlockMap_FindGetThinker(find, i);
      DGE_PhysicsThinker_Unblock(th);
      DGE_Thinker_Unlink(th);
   }

   DGE_BlockMap_FindFree(find);

   P_Player.id = 0;

   if(map->data)
      free(map->data), map->data = NULL;

   P_StateCur = P_State_Stop;
}

//
// P_Map_Read
//
void P_Map_Read(P_Map *map, FILE *in)
{
   map->nextT = P_Map_NextDelay;
   map->respT = P_Map_RespDelay;

   map->w = 0;
   map->h = 0;

   map->name[0] = '\0';
   map->next[0] = '\0';

   while(P_Map_ReadHead(map, in)) {}

   map->data = malloc(map->w * map->h * 2);

   for(int i = 0, e = map->h * 2; i != e; ++i)
      P_Map_ReadData(map, map->data + i * map->w, in);
}

//
// P_Map_ReadData
//
void P_Map_ReadData(P_Map *map, char *data, FILE *in)
{
   int c;
   char *end = data + map->w;
   while(data != end)
   {
      if((c = fgetc(in)) == '\n' || c == EOF) break;
      *data++ = c;
   }

   while(data != end)
      *data++ = ' ';

   while(c != '\n' && c != EOF)
      c = fgetc(in);
}

//
// P_Map_ReadHead
//
bool P_Map_ReadHead(P_Map *map, FILE *in)
{
   char head[8], *itr = head, *end = head + sizeof(head) - 1;

   {
      int c = fgetc(in);
      if(c == '\n' || c == EOF)
         return false;
      ungetc(c, in);
   }

   fscanf(in, "%7s", head);

   if(head[0] == '#')
   {
      // Ignore as comment.
   }
   else if(strcmp(head, "name") == 0)
   {
      fscanf(in, " %31s", map->name);
   }
   else if(strcmp(head, "next") == 0)
   {
      fscanf(in, " %31s", map->next);
   }
   else if(strcmp(head, "size") == 0)
   {
      fscanf(in, " %i %i", &map->w, &map->h);
   }
   else
      fprintf(stderr, "unknown map header: '%s'\n", head);

   // Skip rest of line.
   for(int c; (c = fgetc(in)) != '\n' && c != EOF;) {}

   return true;
}

//
// KillAll
//
M_ShellDefn(KillAll)
{
   unsigned find = DGE_BlockMap_FindAll();

   unsigned thC = DGE_BlockMap_FindCountThinker(find);
   for(unsigned i = 0; i != thC; ++i)
   {
      DGE_Entity ent = {DGE_Object_Cast(DGE_BlockMap_FindGetThinker(find, i), DGE_OT_Entity)};
      if(!ent.id || ent.id == P_Player.id) continue;
      ent.health = 0;
   }

   DGE_BlockMap_FindFree(find);

   return 0;
}

//
// LoadMap
//
M_ShellDefn(LoadMap)
{
   if(argc == 2)
   {
      if(P_StateCur >= P_State_Live)
         P_Map_Quit(P_MapCur);

      printf("Loading map: '%s'\n", argv[1]);

      if(!P_MapCur)
         P_MapCur = malloc(sizeof(P_Map));

      if(argv[1][0] == '/')
         P_Map_LoadPath(P_MapCur, argv[1]);
      else
         P_Map_LoadName(P_MapCur, argv[1]);

      return 0;
   }
   else
   {
      fprintf(stderr, "Usage: LoadMap name\n");
      return 1;
   }
}

// EOF

