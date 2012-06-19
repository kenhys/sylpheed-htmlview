libdir = $(plugindir)

lib_LTLIBRARIES = htmlview.la

htmlview_la_SOURCES = htmlview.c htmlview.h ui.c 
htmlview_la_LDFLAGS = -module -no-undefined -avoid-version

WEBKITGTK_CFLAGS=`pkg-config --cflags webkit-1.0`
WEBKITGTK_LIBS=`pkg-config --libs webkit-1.0`

INCLUDES = \
	$(GTK_CFLAGS) \
        $(WEBKITGTK_CFLAGS) \
	-I$(includedir) \
	-I$(top_srcdir) \
	-I$(top_srcdir)/libsylph \
	-I$(top_srcdir)/src

htmlview_la_LIBADD = \
	$(GTK_LIBS) \
	$(WEBKITGTK_LIBS) \
        $(top_builddir)/src/libsylpheed-plugin-0.la \
	$(top_builddir)/libsylph/libsylph-0.la

install:
install-plugin: install-am
