# HtmlView - HTML mail support plug-in for Sylpheed

## Requirement

* Sylpheed 3.2.0 or later
* Ubuntu 12.04 or later is recommended
* [sylpheed-plubin-factory](https://github.com/kenhys/sylpheed-plugin-factory)

## Install

In this scenario, we assume that we use master version of Sylpheed.

```sh
% svn checkout svn://sylpheed.sraoss.jp/sylpheed/trunk sylpheed
% cd sylpheed
% ./autogen.sh
% ./configure
% make
% sudo make install
% cd plugin
% git clone https://github.com/kenhys/sylpheed-plugin-factory.git
% cd sylpheed-plugin-factory
% ./autogen.sh
% ./configure --with-sylpheed-build-dir=`pwd`/../../
% make
% sudo make install
% cd -
% git clone https://github.com/kenhys/sylpheed-htmlview.git
% cd sylpheed-htmlview
% ./autogen.sh
% ./configure --with-sylpheed-build-dir=`pwd`/../../ --enable-webkitgtk
% make
% sudo make install
```

**make copy** copies the compiled htmlview shared object into plugin directory.

## Usage

Click [Tools]-[HtmlView [htmlview]]] menu, then customize preference settings about htmlview plugin.

Note that this plugin requires [Toggle attachment list view with tab] option is enabled.

Check required option by follows:

1. Click [Configuration]-[Common preferences]
2. Select [Display] tab
3. Select [Attachment] tab inside [Display] tab
4. Confirm [Toggle attachment list view with tab] is enabled

Browse html mail.
htmlview plugin automatically renders HTML mail in messageview.

## TODO

* Support Windows
