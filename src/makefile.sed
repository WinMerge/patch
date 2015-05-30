s/^LIBOBJS.*=/& stripslash.o chdirsaf.o/
s/^LIBSRCS.*=/& stripslash.c chdirsah.c/
s/^OBJS.*=/& patch-rc.o/
s/^SRCS.*=/& patch.rc/
/^all:/i\
# Rule to make compiled resource (Windows)\
%-rc.o: %.rc\
	windres -i $< -o $@
