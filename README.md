HtmlView - HTML mail support plug-in for Sylpheed
=================================================

Requirement
-----------

Sylpheed 3.2.0 or later

recommend to use Ubuntu 12.04 or later.

Install
-------

```sh
% svn checkout svn://sylpheed.sraoss.jp/sylpheed/trunk sylpheed
% cd sylpheed/plugin
% git clone https://github.com/kenhys/sylpheed-htmlview.git htmlview
% git clone https://github.com/kenhys/sylpheed-plugin-factory.git sylplugin_factory
% cd htmlview
% sh ./autogen.sh
% sh ./configure
% make
% make copy
```

**make copy** copies the compiled htmlview shared object into plugin directory.

Usage
-----

Browse html mail.
htmlview plugin automatically renders HTML mail in messageview.

