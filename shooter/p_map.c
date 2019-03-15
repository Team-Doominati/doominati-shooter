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


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// P_TileCreate
//
static unsigned P_TileCreate(int tx, int ty)
{
   DGE_Fixed x = tx * 64.0hk;
   DGE_Fixed y = ty * 64.0hk;

   DGE_Sector sec = {DGE_Sector_Create(4, 0)};

   DGE_Object_RefAdd(sec.id);

   sec.gz       = -1;
   sec.friction =  0.5r;

   DGE_Sector_PointSet(sec.id, 0, (DGE_Point2X){x - 32, y - 32});
   DGE_Sector_PointSet(sec.id, 1, (DGE_Point2X){x - 32, y + 32});
   DGE_Sector_PointSet(sec.id, 2, (DGE_Point2X){x + 32, y + 32});
   DGE_Sector_PointSet(sec.id, 3, (DGE_Point2X){x + 32, y - 32});

   DGE_Sector_CalcBounds(sec.id);
   DGE_Sector_Block(sec.id);

   return sec.id;
}

//
// P_TileCreate_Open
//
static unsigned P_TileCreate_Open(int tx, int ty)
{
   DGE_Sector sec = {P_TileCreate(tx, ty)};

   sec.zu   = 64;
   sec.texf = DGE_Texture_Get(M_Str("@gfx/Tile/Open.png"));

   return sec.id;
}

//
// P_TileCreate_Wall
//
static unsigned P_TileCreate_Wall(int tx, int ty)
{
   DGE_Sector sec = {P_TileCreate(tx, ty)};

   sec.texf = DGE_Texture_Get(M_Str("@gfx/Tile/Wall.png"));

   return sec.id;
}

//
// P_EntityCreate
//
static unsigned P_EntityCreate(int x, int y)
{
   DGE_Entity ent = {DGE_Entity_Create(0)};

   ent.friction = 0.5ulr;
   ent.mass     = 8;
   ent.gx       = 1;
   ent.gy       = 1;
   ent.gz       = 1;

   ent.x = x;
   ent.y = y;
   ent.z = 16;

   ent.sx = 16;
   ent.sy = 16;
   ent.sz = 16;

   ent.rsx = 16;
   ent.rsy = 16;

   return ent.id;
}

//
// P_EntityCreate_Enemy
//
static unsigned P_EntityCreate_Enemy(int x, int y)
{
   DGE_Entity ent = {P_EntityCreate(x, y)};

   ent.cr = 1.0ulr;
   ent.cg = 0.0ulr;
   ent.cb = 0.0ulr;

   ent.health = 40;
   ent.team   = P_TeamEnemy.id;

   ent.sprite = DGE_Texture_Get(M_Str("@gfx/Entity/Mobj.png"));

   DGE_PhysicsThinker_Block(ent.id);

   DGE_Task_Create(0, (DGE_CallbackType)P_Think_Enemy, ent.id);

   return ent.id;
}

//
// P_EntityCreate_Player
//
static unsigned P_EntityCreate_Player(int x, int y)
{
   DGE_Entity ent = {P_EntityCreate(x, y)};

   ent.cr = 0.0ulr;
   ent.cg = 1.0ulr;
   ent.cb = 0.0ulr;

   ent.health = 100;
   ent.team   = P_TeamPlayer.id;

   ent.sprite = DGE_Texture_Get(M_Str("@gfx/Entity/Mobj.png"));

   DGE_PhysicsThinker_Block(ent.id);

   DGE_Task_Create(0, (DGE_CallbackType)P_Think_Player, ent.id);

   DGE_Renderer_SetViewpoint(ent.id);

   P_Player.id = ent.id;

   return ent.id;
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

//
// P_Map_Init
//
void P_Map_Init(int w, int h, char const *tiles, char const *mobjs)
{
   P_StateCur = P_State_Init;

   for(int ty = 0; ty != h; ++ty) for(int tx = 0; tx != w; ++tx)
   {
      switch(tiles[ty * w + tx])
      {
      case ' ': P_TileCreate_Open(tx, ty); break;
      case 'W': P_TileCreate_Wall(tx, ty); break;

      default:
         printf("Unknown tile '%c' (%02X) at (%u, %u)\n",
            tiles[ty * w + tx], tiles[ty * w + tx], tx, ty);
         break;
      }

      switch(mobjs[ty * w + tx])
      {
      case ' ': break;
      case 'E': P_EntityCreate_Enemy (tx * 64, ty * 64); break;
      case 'P': P_EntityCreate_Player(tx * 64, ty * 64); break;

      default:
         printf("Unknown mobj '%c' (%02X) at (%u, %u)\n",
            mobjs[ty * w + tx], mobjs[ty * w + tx], tx, ty);
         break;
      }
   }

   DGE_BlockMap_Split(64, 1);

   P_StateCur = P_State_Live;
}

//
// P_Map_Quit
//
void P_Map_Quit(void)
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

   P_Player.id = 0;

   P_StateCur = P_State_Stop;
}

//
// LoadMap
//
M_Shell int LoadMap(int sh)
{
   M_ShellInit();

   if(P_StateCur >= P_State_Live)
      P_Map_Quit();

   if(argc <= 1)
   {
      printf("Loading fallback map...\n");
      P_Map_Init(11, 11,
         "WWWWWWWWWWW"
         "W         W"
         "W      W  W"
         "W  WW  W  W"
         "W      W  W"
         "W      W  W"
         "W      W  W"
         "W  WW  W  W"
         "W      W  W"
         "W         W"
         "WWWWWWWWWWW"
         ,
         "           "
         "           "
         "           "
         "         E "
         "           "
         "     P   E "
         "           "
         "         E "
         "           "
         "           "
         "           "
      );
   }
   else
   {
      printf("Loading map '%s'...\n", argv[1]);
      FILE *map = fopen(argv[1], "r");
      if(map)
      {
         int c;
         size_t w, h;
         fscanf(map, "%zu %zu", &w, &h);

         for(c; (c = fgetc(map)) != '\n' && c != EOF;) {}

         // Load tile and mobj data.
         char *tiles = malloc(w * h * 2), *mobjs = tiles + w * h, *itr = tiles;
         for(size_t i = w * h * 2; i;)
            if((c = fgetc(map)) != '\n') *itr++ = c, --i;

         for(c; (c = fgetc(map)) != '\n' && c != EOF;) {}

         P_Map_Init(w, h, tiles, mobjs);
         free(tiles);

         // Print map message.
         for(c; (c = fgetc(map)) != EOF;)
            putchar(c);
         putchar('\n');

         fclose(map);
      }
      else
         printf("Failed to open map.\n");
   }

   M_ShellFree();
   return 0;
}

// EOF

