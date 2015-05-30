AC_DEFUN([ag_CHECK_NANOSECOND_STAT],
  [AC_CACHE_CHECK([for nanosecond timestamps in struct stat],
     [ac_cv_stat_nsec],
     [AC_TRY_COMPILE(
	[
	  #include <sys/types.h>
	  #include <sys/stat.h>
	  #include <unistd.h>
	  struct stat st;
	],
	[ return st.st_atimensec + st.st_mtimensec + st.st_ctimensec; ],
	[ac_cv_stat_nsec=yes],
	[ac_cv_stat_nsec=no])
     ])
   if test $ac_cv_stat_nsec = yes; then
     AC_DEFINE(HAVE_STAT_NSEC, 1, [Define to 1 if struct stat has nanosecond timestamps.])
   fi

   AC_CACHE_CHECK([for nanosecond timestamps in struct stat],
     [ac_cv_stat_timeval],
     [AC_TRY_COMPILE(
	[
	  #include <time.h>
	  #include <sys/types.h>
	  #include <sys/stat.h>
	  #include <unistd.h>
	  struct stat st;
	],
	[ return st.st_atim.tv_nsec + st.st_mtim.tv_nsec + st.st_ctim.tv_nsec; ],
	[ac_cv_stat_timeval=yes],
	[ac_cv_stat_timeval=no])
     ])
   if test $ac_cv_stat_timeval = yes; then
     AC_DEFINE(HAVE_STAT_TIMEVAL, 1, [Define to 1 if struct stat comtains struct timeval's.])
   fi])
