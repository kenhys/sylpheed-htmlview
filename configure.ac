#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([htmlview], [0.4.0], [kenhys@gmail.com])
AC_CONFIG_SRCDIR([src/htmlview.c])
AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_AUX_DIR([config])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE($PACKAGE_NAME, $PACKAGE_VERSION)

# Checks for programs.
AC_PROG_AWK
AC_PROG_CC
AC_PROG_MAKE_SET
AC_PROG_LIBTOOL

# Checks for libraries.
AM_PATH_GLIB_2_0(2.8.0,, AC_MSG_ERROR(Test for GLib failed. See the 'INSTALL' for help.))
AM_PATH_GTK_2_0(2.8.0,, AC_MSG_ERROR(Test for Gtk failed. See the 'INSTALL' for help.))

AC_SUBST(GTK_CFLAGS)
AC_SUBST(GTK_LIBS)

AC_ARG_ENABLE([webkitgtk],
	      [AS_HELP_STRING([--enable-webkitgtk],
                              [use WebKitGTK as HTML rendering engine. [default=auto]])],
              [enable_webkitgtk="$enableval"],
              [enable_webkitgtk=no]
)

AC_MSG_CHECKING([whether enable WebKitGTK])
if test "x$enable_webkitgtk" != "xyes"; then
  enable_webkitgtk="no"
fi
AC_MSG_RESULT($enable_webkitgtk)

AC_ARG_ENABLE([gtkhtml],
	      [AS_HELP_STRING([--enable-gtkhtml],
                              [use GtkHTML as HTML rendering engine. [default=no]])],
              [enable_gtkhtml="$enableval"],
              [enable_gtkhtml=no]
)
AC_MSG_CHECKING([whether enable GtkHTML])
if test "x$enable_gtkhtml" != "xyes"; then
  enable_gtkhtml="no"
fi
AC_MSG_RESULT($enable_gtkhtml)

AS_IF([test "x$enable_webkitgtk" != xno && test "x$enable_gtkhtml" != xno],
      [AC_DEFINE([USE_GTKHTML], [1], [GtkHTML support])],
      [test "x$enable_webkitgtk" != xno && test "x$enable_gtkhtml" != xyes],
      [AC_DEFINE([USE_WEBKITGTK], [1], [WebKitGTK support])],
      [test "x$enable_webkitgtk" != xyes && test "x$enable_gtkhtml" != xno],
      [AC_DEFINE([USE_GTKHTML], [1], [GtkHTML support])],
      [AC_MSG_ERROR(use WebKitGTK or GtkHTML exclusively.)]
)

# Checks for header files.
m4_include(../sylplugin_factory/m4/sylplugin_factory.m4)
# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
echo ""
echo "  For HTML Preview..."
echo ""
echo "    WebKitGTK: ${enable_webkitgtk}"
echo "      GtkHTML: ${enable_gtkhtml}"
echo ""
