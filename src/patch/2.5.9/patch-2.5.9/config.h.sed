/* DJGPP-specific definitions */

#define HAVE_DOS_FILE_NAMES 1

#ifdef WIN32
# define setmode _setmode
#endif

#define filename_char_eq(c1,c2) \
  ({int _c1 = (c1), _c2 = (c2); \
    (_c1 >= 'A' &&  _c1 <= 'Z' ? tolower(_c1) : _c1) == \
    (_c2 >= 'A' &&  _c2 <= 'Z' ? tolower(_c2) : _c2); \
  })

#define DEFAULT_OS_QUOTING_STYLE     dos_nt_quoting()
#define DEFAULT_OS_QUOTING_STYLE_DCL extern enum quoting_style dos_nt_quoting (void);

#define TMPDIR "c:"
