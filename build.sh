
NAME=htmlview
TARGET=src/$NAME.dll
SRC="src/$NAME.c"
OBJS="src/$NAME.o src/version.o src/sylplugin_factory.o"
PKG=sylpheed-$NAME
LIBSYLPH=./lib/libsylph-0-1.a
LIBSYLPHEED=./lib/libsylpheed-plugin-0-1.a
LIBSYLFILTER=./lib/libsylfilter.a
#LIBS=" -lglib-2.0-0  -lintl"
LIBS=" `pkg-config --libs glib-2.0 gobject-2.0 gtk+-2.0 gthread-2.0 libgtkhtml-3.14`"
INC=" -I. -I../../ -I../../libsylph -I../../src -I../sylplugin_factory/src -I/mingw/local `pkg-config --cflags glib-2.0 cairo gdk-2.0 gtk+-2.0 gthread-2.0 libgtkhtml-3.14`"

DEF=" -DHAVE_CONFIG_H -DUNICODE -D_UNICODE -DRELEASE_3_1"
DEBUG=0

MAJOR=0
MINOR=1
SUBMINOR=0

DCOMPILE=src/.compile
PBUILDH=src/private_build.h

function compile ()
{
    if [ ! -f "$PBUILDH" ]; then
        echo "1" > $DCOMPILE
        echo "#define PRIVATE_BUILD 1" > $PBUILDH
    else
        ret=`cat $DCOMPILE | gawk '{print $i+1}'`
        echo $ret | tee $DCOMPILE
        echo "#define PRIVATE_BUILD \"build $ret\\0\"" > $PBUILDH
        echo "#define NAME \"SylNotify\\0\"" >> $PBUILDH
        echo "#define VERSION \"$MAJOR, $MINOR, $SUBMINOR, 0\\0\"" >> $PBUILDH
        echo "#define NAMEVERSION \"SylNotify $MAJOR.$MINOR.$SUBMINOR\\0\"" >> $PBUILDH
        echo "#define QVERSION \"$MAJOR,$MINOR,$SUBMINOR,0\"" >> $PBUILDH
    fi
    com="windres -i res/version.rc -o src/version.o"
    echo $com
    eval $com

    for f in $SRC; do
	BASENAME=`basename $f .c`
	com="gcc -Wall -c -o src/$BASENAME.o $DEF $INC $f"
	echo $com
	eval $com
	if [ $? != 0 ]; then
            echo "compile error"
            exit
	fi
    done

    com="gcc -shared -o $TARGET $OBJS -L./lib $LIBSYLPH $LIBSYLPHEED $LIBS -lws2_32 -liconv"
    echo $com
    eval $com
    if [ $? != 0 ]; then
        echo "done"
    else
        if [ -d "$SYLPLUGINDIR" ]; then
            com="cp $TARGET \"$SYLPLUGINDIR/$NAME.dll\""
            echo $com
            eval $com
        else
            :
        fi
    fi

}

if [ -z "$1" ]; then
    compile
else
    while [  $# -ne 0 ]; do
        case "$1" in
            -debug|--debug)
                DEF=" -ggdb $DEF -DDEBUG"
                shift
                ;;
            pot)
                mkdir -p po
                com="xgettext $NAME.h $NAME.c -k_ -kN_ -o po/$NAME.pot"
                echo $com
                eval $com
                shift
                ;;
            po)
                com="msgmerge po/ja.po po/$NAME.pot -o po/ja.po"
                echo $com
                eval $com
                shift
                ;;
            mo)
                com="msgfmt po/ja.po -o po/$NAME.mo"
                echo $com
                eval $com
                if [ -d "$SYLLOCALEDIR" ]; then
                    com="cp po/$NAME.mo \"$SYLLOCALEDIR/$NAME.mo\""
                    echo $com
                    eval $com
                fi
                exit
                ;;
            res)
                com="windres -i version.rc -o version.o"
                echo $com
                eval $com
                shift
                ;;
            -r|release)
                shift
                if [ ! -z "$1" ]; then
                    r=$1
                    shift
                    zip sylpheed-$NAME-$r.zip $NAME.dll
                    #zip -r sylpheed-$NAME-$r.zip README
                    zip -r sylpheed-$NAME-$r.zip README.md
                    zip -r sylpheed-$NAME-$r.zip src/$NAME.[ch]
                    zip -r sylpheed-$NAME-$r.zip res/version.rc
                    zip -r sylpheed-$NAME-$r.zip NEWS
                    #zip -r sylpheed-$NAME-$r.zip po/$NAME.mo
                    #zip -r sylpheed-$NAME-$r.zip *.xpm
                    sha1sum sylpheed-$NAME-$r.zip > sylpheed-$NAME-$r.zip.sha1sum
                fi
                ;;
            -c|-compile)
                shift
                if [ ! -z "$1" ]; then
                    if [ "$1" = "stable" ]; then
                        DEF="$DEF -DSTABLE_RELEASE";
                        shift
                    fi
                fi
                compile
                ;;
            def)
                shift
                PKG=libsylph-0-1
                com="(cd lib;pexports $PKG.dll > $PKG.dll.def)"
                echo $com
                eval $com
                com="(cd lib;dlltool --dllname $PKG.dll --input-def $PKG.dll.def --output-lib $PKG.a)"
                echo $com
                eval $com
                PKG=libsylpheed-plugin-0-1
                com="(cd lib;pexports $PKG.dll > $PKG.dll.def)"
                echo $com
                eval $com
                com="(cd lib;dlltool --dllname $PKG.dll --input-def $PKG.dll.def --output-lib $PKG.a)"
                echo $com
                eval $com
                exit
                ;;
            clean)
                rm -f *.o *.lo *.la *.bak *~
                shift
                ;;
            cleanall|distclean)
                rm -f *.o *.lo *.la *.bak *.dll *.zip
                shift
                ;;
            *)
                shift
                ;;
        esac
    done
fi
