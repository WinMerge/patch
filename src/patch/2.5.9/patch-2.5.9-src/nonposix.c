#ifdef _WIN32

#include <stdio.h>
#include <errno.h>
#include <unistd.h>
#include <windows.h>
#include "nonposix.h"

/* Rename the file OLD to NEW.  */
#ifdef rename
#undef rename
#endif

int unlink (const char *filename)
{
	if (_access (filename, W_OK) < 0) 
		switch (errno) {
			case EINVAL:
				return -1;
				break;
			case ENOENT:
				return -1;
				break;
			case EACCES:
				_chmod (filename, _S_IREAD | _S_IWRITE);
				break;
			default:
				break;
		}
	return _unlink (filename);
}

int
w32_rename (old, new)
	const char *old;
	const char *new;
{
	if (old == NULL || new == NULL) {
		errno = EINVAL;
		return -1;
	}

	if (!_access (new, F_OK))
		unlink (new);
	return rename (old, new);
}

#endif /* _WIN32 */
