SUBDIRS = src

PLUGIN_NAME = htmlview

sylpf_dir = ../sylplugin_factory

update-po:
	xgettext src/*.[ch] -k_ -kN_ -o po/$(PLUGIN_NAME).pot
	msgmerge po/ja.po po/$(PLUGIN_NAME).pot -o po/ja.po
	msgfmt po/ja.po -o po/ja.mo

copy:
	nm -u src/.libs/*.so | \
	  awk '{print $$2}' | \
	  egrep -v '^g_|^gtk_|^webkit|^syl_|^procmime_|^procmsg_|.*@@GLIBC_'
	if [ -f "src/.libs/htmlview.so" ]; then \
	  cp -f src/.libs/*.so ~/.sylpheed-2.0/plugins; \
	fi

