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
}

static void app_exit_cb(GObject *obj, gpointer data)
{
}

static void prefs_ok_cb(GtkWidget *widget, gpointer data)
{
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
  gtk_window_set_default_size(GTK_WINDOW(window), 300, 300);
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

static WebKitWebView *create_htmlview(GtkNotebook *notebook)
{

  WebKitWebView *html_widget = WEBKIT_WEB_VIEW(webkit_web_view_new());

  GtkWidget *scrolled = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled),
                                 GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_container_add(GTK_CONTAINER(scrolled), GTK_WIDGET(html_widget));
  gtk_widget_show_all(GTK_WIDGET(scrolled));

  gtk_container_add(GTK_CONTAINER(notebook),
                    GTK_WIDGET(scrolled));
  gtk_notebook_set_tab_label_text(GTK_NOTEBOOK(notebook),
                                  GTK_WIDGET(scrolled), _("Html"));
  return html_widget;
}

static void messageview_show_cb(GObject *obj, gpointer msgview,
				MsgInfo *msginfo, gboolean all_headers)
{
  g_return_if_fail(msgview != NULL);

  MessageView *messageview = (MessageView*)msgview;
  g_return_if_fail(messageview != NULL);

  if (option.html_view == NULL) {
    option.html_view = create_htmlview(GTK_NOTEBOOK(messageview->notebook));
  }

  MimeInfo *mimeinfo = procmime_scan_message(msginfo);
  FILE *msg_file = procmsg_open_message(msginfo);

  MimeInfo *partial = mimeinfo;
  while (partial && partial->mime_type != MIME_TEXT_HTML) {
    partial = procmime_mimeinfo_next(partial);
  }

  MimeInfo *next_partial = NULL;

  if (partial && partial->mime_type == MIME_TEXT_HTML) {
    partial->mime_type = MIME_TEXT;

    FILE *input = procmime_get_text_content(partial, msg_file, NULL);

    gchar *html_buf = calloc(partial->size+1, 1);

    fread(html_buf, partial->size, 1, input);
    webkit_web_view_load_string(option.html_view, html_buf, NULL, NULL, "");
    gtk_widget_grab_focus(GTK_WIDGET(option.html_view));
    gtk_notebook_set_current_page(GTK_NOTEBOOK(messageview->notebook), 2);

    fclose(input);
    free(html_buf);
  } else {
    gtk_notebook_set_current_page(GTK_NOTEBOOK(messageview->notebook), 0);
    return;
  }
}

