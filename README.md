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

Click [Tools]-[HtmlView [htmlview]]] menu, then customize preference settings about htmlview plugin.

Note that this plugin requires [Toggle attachment list view with tab] option is enabled.

Check required option by follows:

1. Click [Configuration]-[Common preferences]
2. Select [Display] tab
3. Select [Attachment] tab inside [Display] tab
4. Confirm [Toggle attachment list view with tab] is enabled

Browse html mail.
htmlview plugin automatically renders HTML mail in messageview.

