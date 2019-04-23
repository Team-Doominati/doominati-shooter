//-----------------------------------------------------------------------------
//
// Copyright (C) 2019 Team Doominati
//
// See COPYING for license information.
//
//-----------------------------------------------------------------------------
//
// Miscellaneous definitions.
//
//-----------------------------------------------------------------------------

#ifndef M_DEFS_H__
#define M_DEFS_H__

#define _GNU_SOURCE

#include <stdbool.h>
#include <stddef.h>

#if __GDCC__
#include <stdfix.h>
#include <Doominati.h>
#else
#include "/usr/share/gdcc/lib/inc/C/Doominati.h"
#endif


//----------------------------------------------------------------------------|
// Macros                                                                     |
//

//
// M_Callback
//
// Declares a function as a callback.
//
#if __GDCC__
#define M_Callback(cb) M_Entry [[script(cb)]]
#else
#define M_Callback(cb) M_Entry
#endif

//
// M_Entry
//
// Declares a function as an entry point.
//
#if __GDCC__
#define M_Entry [[call("StkCall")]]
#else
#define M_Entry
#endif

//
// M_EntryMain
//
// Declares a function as the main entry point.
//
#if __GDCC__
#define M_EntryMain [[call("StkCall"), extern("asm")]]
#else
#define M_EntryMain
#endif

//
// M_Shell
//
// Declares a function as a shell command.
//
#if __GDCC__
#define M_Shell M_Callback("ShellCommand") [[extern("asm")]]
#else
#define M_Shell M_Callback("ShellCommand")
#endif

//
// M_ShellDefn
//
// Defines a function as a shell command, with a arg-loading wrapper.
//
#define M_ShellDefn(cmd) \
   static int (cmd##_)(int sh, char **argv, size_t argc); \
   M_Shell int (cmd)(int sh) \
   { \
      size_t argc = DGE_Shell_GetArgC(sh); \
      char **argv = malloc(sizeof(*argv) * argc + DGE_Shell_GetArgBufLen(sh)); \
      \
      DGE_Shell_GetArgBuf(sh, (char *)(argv + argc)); \
      DGE_Shell_GetArgV(sh, argv, (char *)(argv + argc)); \
      \
      int res = (cmd##_)(sh, argv, argc); \
      \
      free(argv); \
      return res; \
   } \
   static int (cmd##_)(int sh, char **argv, size_t argc)

//
// M_Str
//
// Makes a string literal into an indexed string literal.
//
#if __GDCC__
#define M_Str(s) s"" s
#else
#define M_Str(s) 0
#endif


//----------------------------------------------------------------------------|
// Types                                                                      |
//

//
// M_Bind
//
typedef enum M_Bind
{
   M_Bind_Alt,
   M_Bind_Atk,
   M_Bind_Dn,
   M_Bind_Lt,
   M_Bind_Rt,
   M_Bind_Up,
   M_Bind_Use,
   M_Bind_Wiz,

   M_Bind_Halt,
} M_Bind;


//----------------------------------------------------------------------------|
// Extern Objects                                                             |
//

extern int M_ScreenH;
extern int M_ScreenW;


//----------------------------------------------------------------------------|
// Extern Functions                                                           |
//

unsigned long M_Fib(unsigned i);

bool M_LineBoxCollide2D(DGE_Fixed xl, DGE_Fixed yl, DGE_Fixed xu, DGE_Fixed yu,
   DGE_Fixed x, DGE_Fixed y, DGE_Fixed dirx, DGE_Fixed diry);

int M_Random(int min, int max);


#endif//M_DEFS_H__

