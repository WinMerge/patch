s,$[({]top_builddir[)}]/intl/libintl.a,-lintl,
s/^patch_OBJECTS.*=/& patch-rc.o/
/^.cc*.o:/i\
# Rule to make compiled resource (Windows)\
%-rc.o: %.rc\
	windres -i $< -o $@
s,/cygdrive/\([^/]*\)/,\1:/,g
s/^OBJS.*=/& patch-rc.o/
s/^SRCS.*=/& patch.rc/
/^.c.o:/i\
# Rule to make compiled resource (Windows)\
%-rc.o: %.rc\
	windres -i $< -o $@
