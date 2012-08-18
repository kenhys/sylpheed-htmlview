SUBDIRS = src

copy:
	nm -u src/.libs/*.so | \
	  gawk '{print $$2}' | \
	  egrep -v '^g_|^gtk_|^webkit|^syl_|^procmime_|^procmsg_|.*@@GLIBC_'
	if [ -f "src/.libs/htmlview.so" ]; then \
	  cp -f src/.libs/*.so ~/.sylpheed-2.0/plugins; \
	fi

