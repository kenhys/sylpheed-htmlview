HtmlView - HTML mail support plug-in for Sylpheed
=================================================

Requirement
-----------

Ubuntu 12.04 or later

Install
-------

1. checkout sylpheed source code.
2. git clone https://github.com/kenhys/sylpheed-htmlview.git into plugin directory.
3. modify configure.ac for generating htmlview/Makefile and htmlview/src/Makefile.
4. sh autogen.sh in sylpheed directory.
5. execute configure.
6. build plugins.

copy compiled htmlview.so into plugin directory.

Usage
-----

Browse html mail.
htmlview plugin automatically renders HTML mail in messageview.

