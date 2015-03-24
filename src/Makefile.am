
if USE_WEBKITGTK
HTML_CFLAGS=`pkg-config --cflags webkit-1.0`
HTML_LIBS=`pkg-config --libs webkit-1.0`
endif

if USE_GTKHTML
HTML_CFLAGS=`pkg-config --cflags libgtkhtml-3.14`
HTML_LIBS=`pkg-config --libs libgtkhtml-3.14`
endif

libdir = $(sylplugin_dir)

lib_LTLIBRARIES = htmlview.la

htmlview_la_SOURCES = \
	htmlview.c \
	htmlview.h
htmlview_la_LDFLAGS = -module -no-undefined -avoid-version $(HTML_LIBS)


AM_CPPFLAGS = \
	$(GTK_CFLAGS) \
        $(HTML_CFLAGS) \
	-I$(includedir) \
	-I$(sylpheed_build_dir) \
	-I$(sylpheed_build_dir)/libsylph \
	-I$(sylpheed_build_dir)/src \
	-I$(SYLPF_CFLAGS)

htmlview_la_LIBADD = \
	$(GTK_LIBS) \
        $(sylpheed_build_dir)/src/libsylpheed-plugin-0.la \
	$(sylpheed_build_dir)/libsylph/libsylph-0.la \
	$(SYLPF_LIBS)

install:
install-plugin: install-am
