
libdir = $(sylplugin_dir)

lib_LTLIBRARIES = htmlview.la

htmlview_la_SOURCES = \
	htmlview.c \
	htmlview.h
htmlview_la_LDFLAGS = -module -no-undefined -avoid-version $(HTML_LIBS)


AM_CPPFLAGS = \
	$(GTK_CFLAGS) \
        $(WEBKITGTK_CFLAGS) \
        $(GTKHTML_CFLAGS) \
	-I$(includedir) \
	-I$(sylpheed_build_dir)/libsylph \
	-I$(sylpheed_build_dir)/src \
	$(SYLPF_CFLAGS)

htmlview_la_LIBADD = \
	$(GTK_LIBS) \
	$(GTKHTML_LIBS) \
        $(sylpheed_build_dir)/src/libsylpheed-plugin-0.la \
	$(sylpheed_build_dir)/libsylph/libsylph-0.la \
	$(SYLPF_LIBS)

install:
install-plugin: install-am
