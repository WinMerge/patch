$a\
/* WIN32-specific definitions */\
\
#define chdir chdir_safer\
int chdir_safer (char const *);\
\
#define FILESYSTEM_PREFIX_LEN(f) ((f)[0] && (f)[1] == ':' ? 2 : 0)\
#define ISSLASH(c) ((c) == '/'  ||  (c) == '\\\\')\
\
#define HAVE_DOS_FILE_NAMES 1\
\
#define HAVE_SETMODE 1\
#ifdef WIN32\
# define setmode _setmode\
#endif\
\
/* #define TMPDIR "c:" */
