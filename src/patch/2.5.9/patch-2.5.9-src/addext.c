/* addext.c -- add an extension to a file name

   Copyright (C) 1990, 1997, 1998, 1999, 2001, 2003 Free Software
   Foundation, Inc.

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2, or (at your option)
   any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; see the file COPYING.
   If not, write to the Free Software Foundation,
   59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.  */

/* Written by David MacKenzie <djm@gnu.ai.mit.edu> and Paul Eggert */

#if HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef HAVE_DOS_FILE_NAMES
# define HAVE_DOS_FILE_NAMES 0
#endif
#ifndef HAVE_LONG_FILE_NAMES
# define HAVE_LONG_FILE_NAMES 0
#endif

#if HAVE_LIMITS_H
# include <limits.h>
#endif
#ifndef _POSIX_NAME_MAX
# define _POSIX_NAME_MAX 14
#endif

#include <sys/types.h>
#if HAVE_STRING_H
# include <string.h>
#else
# include <strings.h>
#endif

#if HAVE_UNISTD_H
# include <unistd.h>
#endif

#include <errno.h>
#ifndef errno
extern int errno;
#endif

#include "backupfile.h"
#include "dirname.h"

/* Append to FILENAME the extension EXT, unless the result would be too long,
   in which case just append the character E.  */

void
addext (char *filename, char const *ext, int e)
{
  char *s = base_name (filename);
  size_t slen = base_len (s);
  size_t extlen = strlen (ext);
  size_t slen_max = HAVE_LONG_FILE_NAMES ? 255 : _POSIX_NAME_MAX;

#if HAVE_PATHCONF && defined _PC_NAME_MAX
  if (_POSIX_NAME_MAX < slen + extlen || HAVE_DOS_FILE_NAMES)
    {
      /* The new base name is long enough to require a pathconf check.  */
      long name_max;
      errno = 0;
      if (s == filename)
	name_max = pathconf (".", _PC_NAME_MAX);
      else
	{
	  char c = *s;
	  if (! ISSLASH (c))
	    *s = 0;
	  name_max = pathconf (filename, _PC_NAME_MAX);
	  *s = c;
	}
      if (0 <= name_max || errno == 0)
	{
	  long size = slen_max = name_max;
	  if (name_max != size)
	    slen_max = -1;
	}
    }
#endif

  if (HAVE_DOS_FILE_NAMES && slen_max <= 12)
    {
      /* Live within DOS's 8.3 limit.  */
      char *dot = strchr (s, '.');
      size_t s_ext_len = 0;	/* length of existing extension in `s' */

      if (dot)
	s_ext_len = slen - (dot + 1 - s);
      else if (ext[0] == '.')
	{
	  s[slen++] = '.';
	  s[slen] = '\0';
	  dot = s + slen - 1;
	}

      /* DOS doesn't allow more than a single dot.  */
      if (ext[0] == '.')
	{
	  ext++;
	  extlen--;
	}

      /* If EXT is "~N~" and there's not enough space to append it,
	 lose the leading `~' so that we could salvage more of the
	 original name ("foo.c" + ".~9~" -> "foo.c9~").  */
      if (s_ext_len + extlen > 3 && ext[0] == '~' && extlen > 1)
	{
	  ext++;
	  extlen--;
	}
      /* If there isn't enough space in the extension to append
	 EXT, use the base part ("foo.c" + ".~99~" -> "foo99~").  */
      if (!dot || extlen > 3)
	{
	  slen_max = 8;
	  if (dot)
	    slen = dot - s;
	  if (extlen < slen_max)
	    {
	      if (slen + extlen > slen_max)
		slen = slen_max - extlen;
	    }
	  /* Else give up and force using E.  We don't allow EXT to
	     eat up all of the original filename's characters.  That
	     is, "foo" + ".~3456789~" should NOT yield "3456789~".  */
	  else if (dot)
	    {
	      slen_max = 3;
	      extlen = 4;
	      s = dot + 1;
	      slen = s_ext_len;
	    }
	}
      else
	{
	  /* Extension has enough space to append EXT.  */
	  slen_max = 3;
	  slen = s_ext_len;
	  s = dot + 1;
	  if (slen + extlen > slen_max)
	    slen = slen_max - extlen;
	}
    }

  if (slen + extlen <= slen_max)
    strcpy (s + slen, ext);
  else
    {
      if (slen_max <= slen)
	slen = slen_max - 1;
      s[slen] = e;
      s[slen + 1] = 0;
    }
}
