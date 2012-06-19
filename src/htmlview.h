/*
 * HtmlView -- 
 * Copyright (C) 2012 HAYASHI Kentaro
 *
 */

#ifndef __HTMLVIEW_H__
#define __HTMLVIEW_H__

#define HTMLVIEW "htmlview"
#define HTMLVIEWRC "htmlviewrc"
#define _(String)   dgettext(HTMLVIEW, String)
#define N_(String)  gettext_noop(String)
#define gettext_noop(String) (String)

#define PLUGIN_NAME N_("HtmlView Plug-in")
#define PLUGIN_DESC N_("Show HTML Mail")

#define GET_RC_BOOLEAN(keyarg) g_key_file_get_boolean(g_opt.rcfile, HTMLVIEW, keyarg, NULL)

typedef struct _HtmlViewOption HtmlViewOption;

struct _HtmlViewOption {
  /* General section */

  /* full path to ghostbiffrc*/
  gchar *rcpath;
  /* rcfile */
  GKeyFile *rcfile;
  
  GtkWidget *folder_entry;
  gchar *folder_path;
};
    
static void messageview_show_cb(GObject *obj, gpointer msgview,
                                MsgInfo *msginfo, gboolean all_headers);

#endif /* __HTMLVIEW_H__ */
