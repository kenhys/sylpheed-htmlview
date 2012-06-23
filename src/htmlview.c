/*
 * HtmlView -- 
 * Copyright (C) 2012 HAYASHI Kentaro
 *
 */

#include <glib.h>
#include <gtk/gtk.h>
#include <webkit/webkit.h>
#include <webkit/webkitwebview.h>

#include "sylmain.h"
#include "plugin.h"
#include "folder.h"
#include "procmsg.h"
#include "compose.h"
#include "alertpanel.h"
#include "foldersel.h"
#include "headerview.h"
#include "messageview.h"
#include "procheader.h"
#include "htmlview.h"

static SylPluginInfo info = {
  N_(PLUGIN_NAME),
  "0.1.0",
  "HAYASHI Kentaro",
  N_(PLUGIN_DESC)
};

HtmlViewOption option;

static void init_done_cb(GObject *obj, gpointer data);
static void app_exit_cb(GObject *obj, gpointer data);

static void exec_htmlview_menu_cb(void);

gulong app_exit_handler_id = 0;

void plugin_load(void)
{
  gpointer mainwin;

  syl_init_gettext(HTMLVIEW, "lib/locale");
  g_print("HtmlView plug-in loaded!\n");

  syl_plugin_add_menuitem("/Tools", NULL, NULL, NULL);
  syl_plugin_add_menuitem("/Tools", _("HtmlView [htmlview]"), exec_htmlview_menu_cb, NULL);

  mainwin = syl_plugin_main_window_get();
  syl_plugin_main_window_popup(mainwin);

  g_signal_connect_after(syl_app_get(), "init-done", G_CALLBACK(init_done_cb),
                         NULL);
  g_signal_connect(syl_app_get(), "app-exit", G_CALLBACK(app_exit_cb), NULL);

  syl_plugin_signal_connect("messageview-show",
                            G_CALLBACK(messageview_show_cb), NULL);
  g_print("htmlview plug-in loading done\n");

  GList* folder_list = folder_get_list();
  Folder *cur_folder;
  GList *cur;
  gint i;

  for (i = 0, cur = folder_list; cur != NULL; cur = cur->next, i++) {
    cur_folder = FOLDER(cur->data);
    debug_print("[PLUGIN] folder[%d] name %s\n",i, cur_folder->name);
    if (FOLDER_TYPE(cur_folder) == F_MH) {
      if (cur_folder->data!=NULL){
        debug_print("[PLUGIN] folder[%d] data %s\n",i, cur_folder->data);
      }
      if (LOCAL_FOLDER(cur_folder)->rootpath!=NULL){
        debug_print("[PLUGIN] folder[%d] rootpath %s\n",i, LOCAL_FOLDER(cur_folder)->rootpath);
        option.folder_path = g_strdup(LOCAL_FOLDER(cur_folder)->rootpath);
        break;
      }
    }
  }
}

void plugin_unload(void)
{
  g_print("htmlview plug-in unloaded!\n");
}

SylPluginInfo *plugin_info(void)
{
  return &info;
}

gint plugin_interface_version(void)
{
    return 0x0108;
    /* return SYL_PLUGIN_INTERFACE_VERSION; */
}

static void init_done_cb(GObject *obj, gpointer data)
{
  g_print("htmlview: %p: app init done\n", obj);
}

static void app_exit_cb(GObject *obj, gpointer data)
{
  g_print("htmlview: %p: app will exit\n", obj);
}

static void prefs_ok_cb(GtkWidget *widget, gpointer data)
{
  GList* folder_list = folder_get_list();

  Folder *cur_folder;
  GList *cur;
  gint i;
  Folder *mh_folder;

  gint mhn = 0;
  for (i = 0, cur = folder_list; cur != NULL; cur = cur->next, i++) {
    cur_folder = FOLDER(cur->data);
    debug_print("[PLUGIN] folder[%d] name %s\n",i, cur_folder->name);
    if (FOLDER_TYPE(cur_folder) == F_MH) {
      if (cur_folder->data!=NULL){
        debug_print("[PLUGIN] folder[%d] data %s\n",i, cur_folder->data);
      }
      if (LOCAL_FOLDER(cur_folder)->rootpath!=NULL){
        debug_print("[PLUGIN] folder[%d] rootpath %s\n",i, LOCAL_FOLDER(cur_folder)->rootpath);
        if (mhn == 0) {
          mh_folder = cur_folder;
        }
        mhn++;
      }
    }
  }
  if (mhn != 1){
    syl_plugin_alertpanel_message(_("HtmlView"), _("does not support multiple MH folder"), ALERT_ERROR);
  } else {
    if (mh_folder != NULL){
      LOCAL_FOLDER(mh_folder)->rootpath = g_strdup(gtk_entry_get_text(GTK_ENTRY(option.folder_entry)));
    }
    folder_write_list();
  }
#endif
  gtk_widget_destroy(GTK_WIDGET(data));
}

static void prefs_cancel_cb(GtkWidget *widget, gpointer data)
{
  gtk_widget_destroy(GTK_WIDGET(data));
}

static void exec_htmlview_menu_cb(void)
{
  /* show modal dialog */
  GtkWidget *window;
  GtkWidget *vbox;
  GtkWidget *confirm_area;
  GtkWidget *ok_btn;
  GtkWidget *cancel_btn;

  window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
  gtk_container_set_border_width(GTK_CONTAINER(window), 8);
  gtk_window_position(GTK_WINDOW(window), GTK_WIN_POS_CENTER);
  gtk_window_set_modal(GTK_WINDOW(window), TRUE);
  gtk_window_set_policy(GTK_WINDOW(window), FALSE, TRUE, FALSE);
  gtk_window_set_default_size(GTK_WINDOW(window), 300, 100);
  gtk_widget_realize(window);

  vbox = gtk_vbox_new(FALSE, 6);
  gtk_widget_show(vbox);
  gtk_container_add(GTK_CONTAINER(window), vbox);


  /* notebook */ 
  GtkWidget *notebook = gtk_notebook_new();
  /* main tab */
  create_config_main_page(notebook, option.rcfile);
  /* about, copyright tab */
  create_config_about_page(notebook, option.rcfile);

  gtk_widget_show(notebook);
  gtk_box_pack_start(GTK_BOX(vbox), notebook, TRUE, TRUE, 0);

  confirm_area = gtk_hbutton_box_new();
  gtk_button_box_set_layout(GTK_BUTTON_BOX(confirm_area), GTK_BUTTONBOX_END);
  gtk_box_set_spacing(GTK_BOX(confirm_area), 6);


  ok_btn = gtk_button_new_from_stock(GTK_STOCK_OK);
  GTK_WIDGET_SET_FLAGS(ok_btn, GTK_CAN_DEFAULT);
  gtk_box_pack_start(GTK_BOX(confirm_area), ok_btn, FALSE, FALSE, 0);
  gtk_widget_show(ok_btn);

  cancel_btn = gtk_button_new_from_stock(GTK_STOCK_CANCEL);
  GTK_WIDGET_SET_FLAGS(cancel_btn, GTK_CAN_DEFAULT);
  gtk_box_pack_start(GTK_BOX(confirm_area), cancel_btn, FALSE, FALSE, 0);
  gtk_widget_show(cancel_btn);

  gtk_widget_show(confirm_area);

  gtk_box_pack_end(GTK_BOX(vbox), confirm_area, FALSE, FALSE, 0);
  gtk_widget_grab_default(ok_btn);
  gtk_widget_show(vbox);

  gtk_window_set_title(GTK_WINDOW(window), _("HtmlView"));

  g_signal_connect(G_OBJECT(ok_btn), "clicked",
                   G_CALLBACK(prefs_ok_cb), window);
  g_signal_connect(G_OBJECT(cancel_btn), "clicked",
                   G_CALLBACK(prefs_cancel_cb), window);
  gtk_widget_show_all(window);

}

typedef struct _Mailer {
    gchar *head;
    gchar *image;
} Mailer;

static Mailer x_mailer[] = {
    {"Microsoft Office Outlook", "ms_outlook12.png"},
    {"Microsoft Outlook Express", "ms_outlook_express.png"},
    {"Microsoft Outlook", "ms_outlook.png"},
    {"Mew", "mew.png"},
    {"Thunderbird", "thunderbird.png"},
    {"Wanderlust", "wanderlust.png"},
    {"Becky!", "becky.png"},
    {"Sylpheed", "sylpheed.png"},
    {"Claws Mail", "claws-mail.png"},
    {"Mutt", "mutt.png"},
    {"Shuriken", "shuriken_pro.png"},
    {"Eudora", "eudora.png"},
};

static void messageview_show_cb(GObject *obj, gpointer msgview,
				MsgInfo *msginfo, gboolean all_headers)
{
#if DEBUG
  g_print("[DEBUG] htmlview: %p: messageview_show (%p), all_headers: %d: %s\n",
	  obj, msgview, all_headers,
	  msginfo && msginfo->subject ? msginfo->subject : "");
#endif
  
  if (!msgview) {
    g_print("[DEBUG] msgview is NULL\n");
    return;
  }

  MessageView *messageview = (MessageView*)msgview;
  if (!messageview) {
    g_print("[DEBUG] messageview is NULL\n");
    return;
  }

  HeaderView *headerview = messageview->headerview;
  if (!headerview) {
    g_print("[DEBUG] headerview is NULL\n");
    return;
  }

  GtkWidget *hbox = headerview->hbox;
  if (!hbox) {
    g_print("[DEBUG] hbox is NULL\n");
    return;
  }

  GList* wl = gtk_container_get_children(GTK_CONTAINER(hbox));

  gpointer gicon = NULL;
  guint iconn = 0;
  gint i=g_list_length(wl)-1;

  /* search recently added GtkImage */
  while (i >= 0) {
    gpointer gdata = g_list_nth_data(wl, i);
    if (GTK_IS_IMAGE(gdata) && gdata != headerview->image) {
      /* remove from hbox */
      g_print("[DEBUG] GTK_IS_IMAGE %p\n", gdata);
#if DEBUG
      g_print("[DEBUG] remove icon: %p\n", gicon);
#endif
      gtk_container_remove(GTK_CONTAINER(hbox), GTK_IMAGE(gdata));
    }
    i--;
  }

  /* check X-Mailer or User-Agent */
  gchar *msg_path = procmsg_get_message_file_path(msginfo);

#if DEBUG
  g_print("[DEBUG] msg_path:%s\n", msg_path);
#endif
  GList* hl = procheader_get_header_list_from_file(msg_path);
  gchar *path = NULL;

  gboolean gface = FALSE;
  for (i=0; i<g_list_length(hl); i++){
    Header *header = g_list_nth_data(hl, i);
    if (header && header->name && header->body) {
      if (strcmp(header->name, "X-Face") == 0) {
	/* skip to display MUA icon */
	gface = TRUE;
	break;
      } else if (strcmp(header->name, "X-Mailer") == 0 ||
		 strcmp(header->name, "User-Agent") == 0) {
#if DEBUG
	g_print("[DEBUG] name:%s body:%s\n", header->name, header->body);
#endif
	guint mindex = 0;
	guint mmax = sizeof(x_mailer)/sizeof(Mailer);
	for (mindex = 0; mindex < mmax; mindex++){
	  if (header->body && x_mailer[mindex].head &&
	      g_strrstr(header->body, x_mailer[mindex].head) != NULL) {
	    path = g_strconcat(get_rc_dir(),
			       G_DIR_SEPARATOR_S,
			       "plugins",
			       G_DIR_SEPARATOR_S,
			       HTMLVIEW,
			       G_DIR_SEPARATOR_S,
			       x_mailer[mindex].image, NULL);
	    break;
	  }
	}
      }
    }
  }

#if 0
  GtkImage *icon = gtk_image_new_from_file(path);
#else
  GError *gerr = NULL;
  if (path && g_file_test(path, G_FILE_TEST_IS_REGULAR | G_FILE_TEST_EXISTS)) {
#if DEBUG
    g_print("[DEBUG] MUA icon path: %s\n", path);
#endif

    GdkPixbuf *pbuf = gdk_pixbuf_new_from_file(path, &gerr);
    if (gerr) {
      g_error(gerr->message);
      return;
    }
    GtkImage *icon = gtk_image_new_from_pixbuf(pbuf);
    gtk_box_pack_end(GTK_BOX(hbox), icon, FALSE, FALSE, 0);
    gtk_widget_show(icon);
#if DEBUG
    g_print("[DEBUG] MUA new icon path: %p\n", icon);
#endif

  } else {
#if DEBUG
    g_print("[DEBUG] MUA icon path 404: %s\n", path ? path : "");
#endif
  }
#endif
}

