
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
	$(SYLPHEED_CFLAGS)
	$(SYLPF_CFLAGS)

htmlview_la_LIBADD = \
	$(GTK_LIBS) \
	$(WEBKITGTK_LIBS) \
	$(GTKHTML_LIBS) \
	$(SYLPHEED_LIBS) \
	$(SYLPF_LIBS)

