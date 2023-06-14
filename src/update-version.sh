# (Re)compute version from git repostitory
# Copyright (C) 2009 Free Software Foundation, Inc.
#
# Copying and distribution of this file, with or without modification,
# in any medium, are permitted without royalty provided the copyright
# notice and this notice are preserved.

# The version generated is of the form TAG[-SHORTREF[-dirty]], with
# TAG being the closest tag to the current version and SHORTREF being
# the first 8 hex digits of the the SHA-1 hash of HEAD.  If there are
# local changes in the repository, -dirty is added.

version=$1

if git rev-parse --verify HEAD >/dev/null 2>/dev/null \
   && test -z "`git rev-parse --show-cdup`" ; then
    set -- `git describe --tags HEAD 2> /dev/null || \
	    git rev-parse --short HEAD` \
	   `git update-index --refresh --unmerged > /dev/null
	    if git diff-index --name-only HEAD | read dummy; then
	      echo -dirty
	    fi`
    if test "`expr substr "$1" 1 1`" = v ; then
	set -- "`expr substr "$1" 2 "(" length "$1" - 1 ")"`" "$2"
    fi
    echo $1$2 > .$version.tmp
    if test ! -e $version \
       || ! cmp -s .$version.tmp $version ; then
	mv .$version.tmp $version
    else
	rm -f .$version.tmp
    fi
fi
