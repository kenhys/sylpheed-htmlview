/*
 * HtmlView -- 
 * Copyright (C) 2012 HAYASHI Kentaro
 *
 */

#ifndef __HTMLVIEW_H__
#define __HTMLVIEW_H__

#include <webkit/webkitwebview.h>

#define HTMLVIEW "htmlview"
#define HTMLVIEWRC "htmlviewrc"
#define SYLPHEEDRC "sylpheedrc"
#define _(String)   dgettext(HTMLVIEW, String)
#define N_(String)  gettext_noop(String)
#define gettext_noop(String) (String)

#define PLUGIN_NAME N_("HtmlView Plug-in")
#define PLUGIN_DESC N_("Show HTML Mail")

#define ENABLE_PRIVATE_BROWSING "enable-private-browsing"
#define ENABLE_IMAGES "auto-load-images"
#define ENABLE_SCRIPTS "enable-scripts"

#define ENABLE_SWITCH_TAB "switch-tab"
#define DEFAULT_FONT_SIZE "default-font-size"

#define SYLPF_ID HTMLVIEW
#define SYLPF_OPTION htmlview_option

#define SYLPF_FUNC(arg) htmlview ## _ ## arg

#define BOOL_TOSTRING(value) (value) ? "TRUE" : "FALSE"

typedef struct _HtmlViewOption HtmlViewOption;

struct _HtmlViewOption {
  /* General section */

  /* full path to ghostbiffrc*/
  gchar *rcpath;
  /* rcfile */
  GKeyFile *rcfile;
  
  WebKitWebView *html_view;

  GtkWidget *private_browsing;
  gboolean private_flag;

  GtkWidget *load_image;
  gboolean image_flag;

  GtkWidget *scripts;
  gboolean script_flag;

  GtkWidget *switch_tab;
  gboolean switch_tab_flag;

  gint is_show_attach_tab;

  gint font_size;
};
    
static void messageview_show_cb(GObject *obj, gpointer msgview,
                                MsgInfo *msginfo, gboolean all_headers);

#endif /* __HTMLVIEW_H__ */
