s/^OBJS.*=/& patch-rc.o/
s/^SRCS.*=/& patch.rc/
/^.c.o:/i\
# Rule to make compiled resource (Windows)\
%-rc.o: %.rc\
	windres -i $< -o $@
