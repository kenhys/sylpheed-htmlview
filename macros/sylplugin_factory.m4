#
# --with-sylpheed-build-dir
#
AC_ARG_WITH(sylpheed-build-dir,
            [AS_HELP_STRING([--with-sylpheed-build-dir=DIR],
            [specify sylpheed build directory.])],
            [syldir="$withval"])
if test "x$syldir" = "x"; then
  # for local build under sylpheed/plugin/PLUGINS
  sylpheed_build_dir=`pwd`/../..
  if test -d "$sylpheed_build_dir/libsylph"; then
    if test -d "$sylpheed_build_dir/src"; then
      syldir_available="yes"
    fi
  fi
  # for Travis-CI
  if test -d "sylpheed/libsylph"; then
     if test -d "sylpheed/src"; then
     	syldir_available="yes"
	sylpheed_build_dir=`pwd`/sylpheed
     fi
  fi
else
  syldir_available="yes"
  sylpheed_build_dir=$syldir
fi
AC_SUBST(sylpheed_build_dir)
AM_CONDITIONAL([WITH_SYLPHEED], [test "$syldir_available" = "yes"])

SYLPHEED_CFLAGS="-I${sylpheed_build_dir}/libsylph -I$sylpheed_build_dir/src"
AC_SUBST(SYLPHEED_CFLAGS)

#
# --with-sylplugin-factory-source-dir
#
AC_ARG_WITH(sylplugin-factory-source-dir,
            [AS_HELP_STRING([--with-sylplugin-factory-source-dir=DIR],
            [specify sylpheed-plugin-factory source directory.])],
            [sylpfdir="$withval"])
if test "x$sylpfdir" = "x"; then
  sylpfdir_available="yes"
  sylplugin_factory_source_dir=`pwd`/lib/sylplugin_factory
else
  if test -d "$sylpfdir"; then
    if test -d "$sylpfdir/src"; then
      sylpf_dir_available="yes"
      sylplugin_factory_source_dir=$sylpfdir
    fi
  fi
fi
AC_SUBST(sylplugin_factory_source_dir)
AM_CONDITIONAL([WITH_SYLPLUGIN_FACTORY], [test "$sylpfdir_available" = "yes"])

#
# --with-sylpheed-plugin-dir
#
AC_ARG_WITH(sylpheed-plugin-dir,
            [AS_HELP_STRING([--with-sypheed-plugin-dir=DIR],
            [specify sylpheed plugin directory.])],
            [sylplugin_dir="$withval"])
if test "x$sylplugin_dir" = "x"; then
  sylplugindir_available="yes"
  sylplugin_dir=$prefix/lib/sylpheed/plugins
else
  if test -d "$sylplugin_dir"; then
    sylplugindir_available="yes"
  fi
fi
AC_SUBST(sylplugin_dir)
AM_CONDITIONAL([WITH_SYLPLUGIN], [test "$syplugindir_available" = "yes"])

#
# --with-libsylph-impl
#
AC_ARG_WITH(libsylph-impl,
            [AS_HELP_STRING([--with-libsylph-impl=LIBRARY],
            [specify libsylph import library(.a)])],
            [libsylph_impl="$withval"])
if test "x$libsylph_impl" = "x"; then
  libsylph_impl_available="no"
else
  if test -f "$libsylph_impl"; then
    libsylph_impl_available="yes"
  fi
fi
AC_SUBST(libsylph_impl)
AM_CONDITIONAL([WITH_LIBSYLPH_IMPL], [test "$libsylph_impl_available" = "yes"])

#
# --with-libsylpheeed-impl
#
AC_ARG_WITH(libsylpheed-impl,
            [AS_HELP_STRING([--with-libsylpheed-impl=LIBRARY],
            [specify libsylpheed import library(.a)])],
            [libsylpheed_impl="$withval"])
if test "x$libsylpheed_impl" = "x"; then
  libsylpheed_impl_available="no"
else
  if test -f "$libsylpheed_impl"; then
    libsylpheed_impl_available="yes"
  fi
fi
AC_SUBST(libsylpheed_impl)
AM_CONDITIONAL([WITH_LIBSYLPHEED_IMPL], [test "$libsylpheed_impl_available" = "yes"])

if test "x$libsylph_impl" != "x"; then
  LIBSYLPH_IMPL="$libsylph_impl"
else
  LIBSYLPH_IMPL="${sylpheed_build_dir}/libsylph/libsylph-0.la"
fi
if test "x$libsylpheed_impl" = "x"; then
  LIBSYLPHEED_IMPL="$libslpheed_impl"
else
  LIBSYLPHEED_IMPL="${sylpheed_build_dir}/src/libsylpheed-plugin-0.la"
fi
SYLPHEED_LIBS="$LIBSYLPH_IMPL $LIBSYLPHEED_IMPL"
AC_SUBST(SYLPHEED_LIBS)
