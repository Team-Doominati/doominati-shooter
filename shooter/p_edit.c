//-----------------------------------------------------------------------------
//
// Copyright (C) 2019 Team Doominati
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Map editor.
//
//-----------------------------------------------------------------------------

#include "p_defs.h"

#include "r_defs.h"

#include <stdlib.h>


//----------------------------------------------------------------------------|
// Types                                                                      |
//

//
// P_EditMobj
//
typedef struct P_EditMobj
{
   DGE_Unsig id;

   DGE_RenderThinkerProps()

   DGE_PropMem(DGE_Unsig, type, DGE_OME_RenderThinker)
} P_EditMobj;

//
// P_EditTile
//
typedef struct P_EditTile
{
   DGE_Unsig id;

   DGE_SectorProps()

   DGE_PropMem(DGE_Unsig, type, DGE_OME_Sector)
} P_EditTile;


//----------------------------------------------------------------------------|
// Static Objects                                                             |
//

static unsigned P_EditW, P_EditH;
static P_EditMobj *P_EditMobjs;
static P_EditTile *P_EditTiles;

static DGE_PointThinker P_EditView;

static unsigned  P_EditMobj_Type;
static unsigned  P_EditMobj_Tex;
static DGE_Fract P_EditMobj_TexR;
static DGE_Fract P_EditMobj_TexG;
static DGE_Fract P_EditMobj_TexB;

static unsigned P_EditTile_Type;
static unsigned P_EditTile_TexC;
static unsigned P_EditTile_TexF;


//----------------------------------------------------------------------------|
// Static Functions                                                           |
//

//
// P_EditHudCB
//
M_Callback("DrawPost") static void P_EditHudCB(void)
{
   if(P_StateCur != P_State_Edit) return;

   DGE_Draw_SetColor(P_EditMobj_TexR, P_EditMobj_TexG, P_EditMobj_TexB);

   if(P_EditMobj_Tex)
   {
      DGE_Texture_Bind(P_EditMobj_Tex);
      DGE_Draw_Rectangle(P_TileSize, 0, P_TileSize * 2, P_TileSize);
   }

   DGE_Draw_SetColor(1.0ulr, 1.0ulr, 1.0ulr);

   if(P_EditTile_TexF)
   {
      DGE_Texture_Bind(P_EditTile_TexF);
      DGE_Draw_Rectangle(0, 0, P_TileSize, P_TileSize);
   }

   if(P_EditTile_TexC)
   {
      DGE_Texture_Bind(P_EditTile_TexC);
      DGE_Draw_Rectangle(0, 0, P_TileSize, P_TileSize);
   }

   DGE_Texture_Bind(R_TexTile_Edit);
   DGE_Draw_Rectangle(0, 0, P_TileSize, P_TileSize);
   DGE_Draw_Rectangle(P_TileSize, 0, P_TileSize * 2, P_TileSize);
}

//
// P_EditMobj_Create
//
static unsigned P_EditMobj_Create(int x, int y)
{
   P_EditMobj mobj = {DGE_RenderThinker_Create(1)};
   DGE_Object_RefAdd(mobj.id);

   mobj.rsx  = 16;
   mobj.rsy  = 16;
   mobj.type = P_EditMobj_Type;
   mobj.x    = x;
   mobj.y    = y;

   return mobj.id;
}

//
// P_EditMobj_TypeSet
//
static void P_EditMobj_TypeSet(unsigned type, unsigned id)
{
   if(type) switch((P_EditMobj_Type = type))
   {
   case ' ':
      P_EditMobj_Tex  = 0;
      P_EditMobj_TexR = 1.0ulr;
      P_EditMobj_TexG = 1.0ulr;
      P_EditMobj_TexB = 1.0ulr;
      break;

   case 'E':
      P_EditMobj_Tex  = R_TexEntity_Mobj;
      P_EditMobj_TexR = 1.0ulr;
      P_EditMobj_TexG = 0.0ulr;
      P_EditMobj_TexB = 0.0ulr;
      break;

   case 'P':
      P_EditMobj_Tex  = R_TexEntity_Mobj;
      P_EditMobj_TexR = 0.0ulr;
      P_EditMobj_TexG = 1.0ulr;
      P_EditMobj_TexB = 0.0ulr;
      break;
   }

   if(id)
   {
      P_EditMobj mobj = {id};

      mobj.type   = P_EditMobj_Type;
      mobj.sprite = P_EditMobj_Tex;
      mobj.cr     = P_EditMobj_TexR;
      mobj.cg     = P_EditMobj_TexG;
      mobj.cb     = P_EditMobj_TexB;
   }
}

//
// P_EditTile_Create
//
static unsigned P_EditTile_Create(int x, int y)
{
   P_EditTile tile = {DGE_Sector_Create(4, 1)};
   DGE_Object_RefAdd(tile.id);

   tile.type = P_EditTile_Type;
   tile.texc = P_EditTile_TexC;
   tile.texf = P_EditTile_TexF;

   DGE_Sector_PointSet(tile.id, 0, (DGE_Point2X){x - P_TileSize / 2, y - P_TileSize / 2});
   DGE_Sector_PointSet(tile.id, 1, (DGE_Point2X){x - P_TileSize / 2, y + P_TileSize / 2});
   DGE_Sector_PointSet(tile.id, 2, (DGE_Point2X){x + P_TileSize / 2, y + P_TileSize / 2});
   DGE_Sector_PointSet(tile.id, 3, (DGE_Point2X){x + P_TileSize / 2, y - P_TileSize / 2});

   DGE_Sector_CalcBounds(tile.id);
   DGE_Sector_Block(tile.id);

   return tile.id;
}

//
// P_EditTile_TypeSet
//
static void P_EditTile_TypeSet(unsigned type, unsigned id)
{
   if(type) switch((P_EditTile_Type = type))
   {
   case ' ':
      P_EditTile_TexC = R_TexTile_Edit;
      P_EditTile_TexF = 0;
      break;

   case 'E':
      P_EditTile_TexC = R_TexTile_Edit;
      P_EditTile_TexF = R_TexTile_Exit;
      break;

   case 'H':
      P_EditTile_TexC = R_TexTile_Half;
      P_EditTile_TexF = R_TexTile_Open;
      break;

   case 'O':
      P_EditTile_TexC = R_TexTile_Edit;
      P_EditTile_TexF = R_TexTile_Open;
      break;

   case 'W':
      P_EditTile_TexC = R_TexTile_Edit;
      P_EditTile_TexF = R_TexTile_Wall;
      break;
   }

   if(id)
   {
      P_EditTile tile = {id};
      tile.type = P_EditTile_Type;
      tile.texc = P_EditTile_TexC;
      tile.texf = P_EditTile_TexF;
   }
}


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

//
// P_EditInit
//
void P_EditInit(unsigned w, unsigned h)
{
   P_EditW = w;
   P_EditH = h;

   unsigned size = w * h;

   P_EditMobjs = malloc(sizeof(P_EditMobj) * size);
   P_EditTiles = malloc(sizeof(P_EditTile) * size);

   P_EditMobj_Type = ' ';
   P_EditMobj_Tex  = 0;
   P_EditMobj_TexR = 1.0ulr;
   P_EditMobj_TexG = 1.0ulr;
   P_EditMobj_TexB = 1.0ulr;

   P_EditTile_Type = ' ';
   P_EditTile_TexC = R_TexTile_Edit;
   P_EditTile_TexF = 0;

   for(unsigned ty = 0; ty != h; ++ty) for(unsigned tx = 0; tx != w; ++tx)
   {
      unsigned ti = ty * h + tx;
      unsigned x  = tx * P_TileSize;
      unsigned y  = ty * P_TileSize;

      P_EditMobjs[ti].id = P_EditMobj_Create(x, y);
      P_EditTiles[ti].id = P_EditTile_Create(x, y);
   }

   P_EditView.id = DGE_PointThinker_Create(0);
   DGE_Object_RefAdd(P_EditView.id);
   DGE_Renderer_SetViewpoint(P_EditView.id);

   P_StateCur = P_State_Edit;

   DGE_Task_Create(0, (DGE_CallbackType)P_EditTask);
}

//
// P_EditQuit
//
void P_EditQuit(void)
{
   for(size_t i = 0, e = P_EditW * P_EditH; i != e; ++i)
   {
      DGE_Thinker_Unlink(P_EditMobjs[i].id);
      DGE_Object_RefSub(P_EditMobjs[i].id);
      DGE_Object_RefSub(P_EditTiles[i].id);
   }

   free(P_EditMobjs);
   free(P_EditTiles);

   DGE_Object_RefSub(P_EditView.id);
   P_EditView.id = 0;
   DGE_Renderer_SetViewpoint(0);

   P_StateCur = P_State_Stop;

   // Make sure running tasks see state change.
   DGE_Task_Sleep(0, 1);
}

//
// P_EditTask
//
M_Entry void P_EditTask(void)
{
   DGE_Object_RefAdd(P_EditView.id);

   for(; P_StateCur == P_State_Edit; DGE_Task_Sleep(0, 1))
   {
      unsigned btnTileT = DGE_Input_GetButton(0, M_Bind_Wiz);
      unsigned btnMobjT = DGE_Input_GetButton(0, M_Bind_Use);
      unsigned btnTile  = DGE_Input_GetButton(0, M_Bind_Atk);
      unsigned btnMobj  = DGE_Input_GetButton(0, M_Bind_Alt);

      DGE_Point2I cursor = DGE_Input_GetCursor(0);
      int x  = cursor.x - M_ScreenW / 2 + (int)P_EditView.x + P_TileSize / 2;
      int y  = cursor.y - M_ScreenH / 2 + (int)P_EditView.y + P_TileSize / 2;
      int tx = x / P_TileSize;
      int ty = y / P_TileSize;

      if(DGE_Input_GetButton(0, M_Bind_Up) & DGE_Button_Down) P_EditView.y = P_EditView.y - 8;
      if(DGE_Input_GetButton(0, M_Bind_Dn) & DGE_Button_Down) P_EditView.y = P_EditView.y + 8;
      if(DGE_Input_GetButton(0, M_Bind_Lt) & DGE_Button_Down) P_EditView.x = P_EditView.x - 8;
      if(DGE_Input_GetButton(0, M_Bind_Rt) & DGE_Button_Down) P_EditView.x = P_EditView.x + 8;

      if(btnMobjT == DGE_Button_Down) switch(P_EditMobj_Type)
      {
      case ' ': P_EditMobj_TypeSet('E', 0); break;
      case 'E': P_EditMobj_TypeSet('P', 0); break;
      case 'P': P_EditMobj_TypeSet(' ', 0); break;
      }

      if(btnTileT == DGE_Button_Down) switch(P_EditTile_Type)
      {
      case ' ': P_EditTile_TypeSet('E', 0); break;
      case 'E': P_EditTile_TypeSet('H', 0); break;
      case 'H': P_EditTile_TypeSet('O', 0); break;
      case 'O': P_EditTile_TypeSet('W', 0); break;
      case 'W': P_EditTile_TypeSet(' ', 0); break;
      }

      if(0 <= x || tx < P_EditW || 0 <= y || ty < P_EditH)
      {
         if(btnMobj & DGE_Button_Down)
            P_EditMobj_TypeSet(0, P_EditMobjs[ty * P_EditW + tx].id);

         if(btnTile & DGE_Button_Down)
            P_EditTile_TypeSet(0, P_EditTiles[ty * P_EditW + tx].id);
      }
   }

   DGE_Object_RefSub(P_EditView.id);

   P_EditView.id = 0;
}

//
// EditNew
//
M_ShellDefn(EditNew)
{
   if(argc == 3)
   {
      if(P_StateCur >= P_State_Live)
         P_Map_Quit(P_MapCur);

      if(P_StateCur == P_State_Edit)
         P_EditQuit();

      unsigned w = strtol(argv[1], NULL, 10);
      unsigned h = strtol(argv[2], NULL, 10);

      P_EditInit(w, h);

      return 0;
   }
   else
   {
      fprintf(stderr, "Usage: EditNew width height\n");
      return 1;
   }
}

//
// EditSave
//
M_ShellDefn(EditSave)
{
   if(P_StateCur != P_State_Edit)
   {
      fprintf(stderr, "Editor not running.\n");
      return 1;
   }

   printf("Map data:\nsize %u %u\n\n", P_EditW, P_EditH);

   for(int ty = 0; ty != P_EditH; ++ty)
   {
      for(int tx = 0; tx != P_EditW; ++tx)
         putchar(P_EditTiles[ty * P_EditW + tx].type);
      putchar('\n');
   }

   for(int ty = 0; ty != P_EditH; ++ty)
   {
      for(int tx = 0; tx != P_EditW; ++tx)
         putchar(P_EditMobjs[ty * P_EditW + tx].type);
      putchar('\n');
   }

   printf("End map data.\n");

   return 0;
}

//
// EditQuit
//
M_ShellDefn(EditQuit)
{
   if(P_StateCur != P_State_Edit)
   {
      fprintf(stderr, "Editor not running.\n");
      return 1;
   }

   P_EditQuit();
}

// EOF

