/* pc_quote.c - quoting functions specific to PC environments
   Copyright (C) 1998 Free Software Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software Foundation,
   Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

#if HAVE_CONFIG_H
# include <config.h>
#endif

#include "nonposix.h"

#if HAVE_STDLIB_H
# include <stdlib.h>
#else
extern char *getenv();
#endif

#if HAVE_STRING_H
# include <string.h>
#endif

#if HAVE_UNISTD_H
# include <unistd.h>
#endif
#ifndef F_OK
# define F_OK 0
#endif

#include <ctype.h>

#include <quotearg.h>
#include <backupfile.h>

struct shell_spec {
  const char *name;
  const size_t name_len;
  const enum quoting_style style;
};

#include <stdio.h>

/* Determine the default quoting style suitable for the user's shell.  */
enum quoting_style
dos_nt_quoting (void)
{
  /* Environment variables used to specify the user shell.  */
  static char const *const shell_vars[] = {
    "BASH", "SHELL", "COMSPEC", "ComSpec", 0
  };

  /* List of shells which don't support Unix-style shell_quoting_style.  */
  static char const command_com[] = "COMMAND.COM";
  static char const fordos_com[] = "4DOS.COM";
  static char const ndos_com[] = "NDOS.COM";
  static char const cmd_exe[] = "CMD.EXE";

  static struct shell_spec const dos_nt_shells[] = {
    {command_com, sizeof(command_com) - 1, dos_shell_quoting_style},
    {fordos_com, sizeof(fordos_com) - 1, dos_shell_quoting_style},
    {ndos_com, sizeof(ndos_com) - 1, dos_shell_quoting_style},
    {cmd_exe, sizeof(cmd_exe) - 1, nt_shell_quoting_style},
    {0, 0, 0}
  };

  const char *const *try_var = shell_vars;
  char *shell = NULL;

  while (*try_var
	 && ((shell = getenv (*try_var)) == NULL
	     /* Make sure it is indeed a name of an existing file.  */
	     || access (shell, F_OK) == -1))
    try_var++;

  if (shell)
    {
      char *shell_base = base_name (shell);
      size_t shell_base_len = strlen (shell_base);
      const struct shell_spec *try_shell;

      for (try_shell = dos_nt_shells; try_shell->name; try_shell++)
	if (shell_base_len == try_shell->name_len)
	  {
	    const char *s = shell_base, *p = try_shell->name;

	    for ( ; *s && *p && filename_char_eq (*s, *p); s++, p++)
	      ;
	    if (*s == '\0' && *p == '\0')
	      return try_shell->style;
	  }
    }

  /* All other shells are assumed to be Unix-like.  */
  return shell_quoting_style;
}
