/*
 * HtmlView -- 
 * Copyright (C) 2012 HAYASHI Kentaro
 *
 */

#include "config.h"
#include <glib.h>
#include <glib/gprintf.h>
#include <gtk/gtk.h>
#include <libintl.h>
#if defined(USE_WEBKITGTK)
#include <webkit/webkit.h>
#include <webkit/webkitwebview.h>
#elif defined(USE_GTKHTML)
#include <gtkhtml/gtkhtml.h>
#else
#error "Not Implemented any HTML features"
#endif

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
#include "sylplugin_factory.h"
#include "copying.h"

static SylPluginInfo info = {
  N_(PLUGIN_NAME),
  "0.4.0",
  "HAYASHI Kentaro",
  N_(PLUGIN_DESC)
};

HtmlViewOption SYLPF_OPTION;

static void init_done_cb(GObject *obj, gpointer data);
static void app_exit_cb(GObject *obj, gpointer data);

static void exec_htmlview_menu_cb(void);
static void load_option_from_rcfile(void);

static GtkWidget *create_config_about_page(GtkWidget *notebook, GKeyFile *pkey);
static GtkWidget *create_config_main_page(GtkWidget *notebook, GKeyFile *pkey);
static GtkWidget *create_preference_dialog(HtmlViewOption *option);

static void save_htmlview_preference(HtmlViewOption *option);

gulong app_exit_handler_id = 0;

#if defined(USE_WEBKITGTK)
#elif defined(USE_GTKHTML)
#else
#error "use WebKitGTK or GtkHTML"
#endif

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

  load_option_from_rcfile();

  SYLPF_OPTION.is_show_attach_tab = SYLPF_GET_RC_SHOW_ATTACH_TAB;

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
  /* return SYL_PLUGIN_INTERFACE_VERSION; */
  /* Sylpheed 3.3.0Beta1 or later */
  return 0x0109;
}

static void init_done_cb(GObject *obj, gpointer data)
{
}

static void app_exit_cb(GObject *obj, gpointer data)
{
}


void load_option_rcfile(const gchar *rcname)
{
  SYLPF_OPTION.rcpath = g_strconcat(get_rc_dir(), G_DIR_SEPARATOR_S, rcname, NULL);
  SYLPF_OPTION.rcfile = g_key_file_new();
  g_key_file_load_from_file(SYLPF_OPTION.rcfile, SYLPF_OPTION.rcpath, G_KEY_FILE_KEEP_COMMENTS, NULL);
}

void save_option_rcfile(void)
{
  gsize sz;
  gchar *buf = g_key_file_to_data(SYLPF_OPTION.rcfile, &sz, NULL);
  g_file_set_contents(SYLPF_OPTION.rcpath, buf, sz, NULL);

  g_key_file_free(SYLPF_OPTION.rcfile);
}

static void load_option_from_rcfile(void)
{
  gchar *font_name = NULL;
  gchar **tokens = NULL;
  gint index = 0;
  
  load_option_rcfile(HTMLVIEWRC);

#if USE_WEBKITGTK
  SYLPF_OPTION.private_flag = SYLPF_GET_RC_BOOLEAN(ENABLE_PRIVATE_BROWSING);
  SYLPF_OPTION.image_flag = SYLPF_GET_RC_BOOLEAN(ENABLE_IMAGES);
  SYLPF_OPTION.script_flag = SYLPF_GET_RC_BOOLEAN(ENABLE_SCRIPTS);
#endif
  SYLPF_OPTION.switch_tab_flag = SYLPF_GET_RC_BOOLEAN(ENABLE_SWITCH_TAB);

  font_name = SYLPF_GET_RC_MESSAGE_FONT_NAME;

  tokens = g_strsplit(font_name, " ", 0);
  for (index = 0; tokens[index]; index++) {
    if (tokens[index+1] == NULL) {
      SYLPF_OPTION.font_size = atoi(tokens[index]);
    }
  }
  g_strfreev(tokens);
  g_free(font_name);

  save_option_rcfile();
}


static void save_htmlview_preference(HtmlViewOption *option)
{
  SYLPF_START_FUNC;

#define TOGGLE_STATE(widget) gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(widget))

#if USE_WEBKITGTK
  option->private_flag = TOGGLE_STATE(option->private_browsing);
  option->image_flag = TOGGLE_STATE(option->load_image);
  option->script_flag = TOGGLE_STATE(option->scripts);
#endif
  option->switch_tab_flag = TOGGLE_STATE(option->switch_tab);

#if USE_WEBKITGTK
  g_print("%s:%s\n", ENABLE_PRIVATE_BROWSING, BOOL_TOSTRING(option->private_flag));
  g_print("%s:%s\n", ENABLE_IMAGES, BOOL_TOSTRING(option->image_flag));
  g_print("%s:%s\n", ENABLE_SCRIPTS, BOOL_TOSTRING(option->script_flag));
#endif
  g_print("%s:%s\n", ENABLE_SWITCH_TAB, BOOL_TOSTRING(option->switch_tab_flag));

#undef TOGGLE_STATE
  
  load_option_rcfile(HTMLVIEWRC);
  
#if USE_WEBKITGTK
  SYLPF_SET_RC_BOOLEAN(ENABLE_PRIVATE_BROWSING, SYLPF_OPTION.private_flag);
  SYLPF_SET_RC_BOOLEAN(ENABLE_IMAGES, SYLPF_OPTION.image_flag);
  SYLPF_SET_RC_BOOLEAN(ENABLE_SCRIPTS, SYLPF_OPTION.script_flag);
#endif
  SYLPF_SET_RC_BOOLEAN(ENABLE_SWITCH_TAB, SYLPF_OPTION.switch_tab_flag);

  save_option_rcfile();

}

static void exec_htmlview_menu_cb(void)
{
  GtkWidget *dialog;
  gint response;
  
  SYLPF_START_FUNC;

  dialog = create_preference_dialog(&SYLPF_OPTION);
  
  gtk_widget_show_all(dialog);
  response = gtk_dialog_run(GTK_DIALOG(dialog));

  switch (response) {
  case GTK_RESPONSE_OK:
    save_htmlview_preference(&SYLPF_OPTION);
    break;
  case GTK_RESPONSE_CANCEL:
  default:
    break;
  }

  gtk_widget_destroy(dialog);
  
  SYLPF_END_FUNC;
}

static GtkWidget *create_preference_dialog(HtmlViewOption *option)
{
  GtkWidget *vbox, *hbox;
  GtkWidget *dialog;
  gint width, height;
  gpointer mainwin;
  GtkWidget *window;
  GtkWidget *notebook;

  SYLPF_START_FUNC;

  mainwin = syl_plugin_main_window_get();
  window = ((MainWindow*)mainwin)->window;
  
  gtk_window_get_size(GTK_WINDOW(window),
                      &width,
                      &height);

  dialog = gtk_dialog_new_with_buttons(_("HtmlView"),
                                       GTK_WINDOW(window),
                                       GTK_DIALOG_MODAL,
                                       GTK_STOCK_CANCEL, GTK_RESPONSE_CANCEL,
                                       GTK_STOCK_OK, GTK_RESPONSE_OK,
                                       NULL);

  height *= 0.8;
  if (width * 0.8 > 400) {
    width = 400;
  }

  gtk_widget_set_size_request(dialog,
                              width, height * 0.8);

  vbox = gtk_vbox_new(FALSE, SYLPF_BOX_SPACE);
  hbox = gtk_hbox_new(TRUE, SYLPF_BOX_SPACE);

  gtk_container_add(GTK_CONTAINER(hbox), vbox);
  gtk_container_add(GTK_CONTAINER(GTK_DIALOG(dialog)->vbox), hbox);

  notebook = gtk_notebook_new();
  create_config_main_page(notebook, SYLPF_OPTION.rcfile);
  create_config_about_page(notebook, SYLPF_OPTION.rcfile);

  gtk_widget_show(notebook);
  gtk_box_pack_start(GTK_BOX(vbox), notebook, TRUE, TRUE, 0);

  gtk_box_set_spacing(GTK_BOX(GTK_DIALOG(dialog)->vbox), SYLPF_BOX_SPACE);

  SYLPF_RETURN_VALUE(dialog);
}

static GtkWidget *create_htmlview(GtkNotebook *notebook)
{

#if defined(USE_WEBKITGTK)
  WebKitWebView *html_widget = WEBKIT_WEB_VIEW(webkit_web_view_new());
#elif defined(USE_GTKHTML)
  GtkWidget *html_widget = gtk_html_new();
#endif
  GtkWidget *scrolled;

  SYLPF_START_FUNC;

  scrolled = gtk_scrolled_window_new(NULL, NULL);
  gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled),
                                 GTK_POLICY_AUTOMATIC, GTK_POLICY_AUTOMATIC);
  gtk_container_add(GTK_CONTAINER(scrolled), GTK_WIDGET(html_widget));
  gtk_widget_show_all(GTK_WIDGET(scrolled));

  gtk_container_add(GTK_CONTAINER(notebook),
                    GTK_WIDGET(scrolled));
  gtk_notebook_set_tab_label_text(GTK_NOTEBOOK(notebook),
                                  GTK_WIDGET(scrolled), _("HTML"));

  SYLPF_RETURN_VALUE(GTK_WIDGET(html_widget));
}

static void messageview_show_cb(GObject *obj, gpointer msgview,
				MsgInfo *msginfo, gboolean all_headers)
{
  MessageView *messageview = NULL;
  MimeInfo *mimeinfo, *partial;
  FILE *msg_file, *input = NULL;
  gchar *html_buf = NULL;
  size_t n_size;
#if defined(USE_WEBKITGTK)
  WebKitWebSettings *settings = NULL;
#elif defined(USE_GTKHTML)
#else
#error "Not Implemented"
#endif

  SYLPF_START_FUNC;

  g_return_if_fail(msgview != NULL);

  messageview = (MessageView*)msgview;

  g_return_if_fail(messageview != NULL);

  SYLPF_OPTION.is_show_attach_tab = SYLPF_GET_RC_SHOW_ATTACH_TAB;

  if (SYLPF_OPTION.is_show_attach_tab == 0) {
    gtk_notebook_set_current_page(GTK_NOTEBOOK(messageview->notebook), 0);
    SYLPF_RETURN;
  }

  if (SYLPF_OPTION.html_view == NULL) {
    SYLPF_OPTION.html_view = create_htmlview(GTK_NOTEBOOK(messageview->notebook));
  }

  mimeinfo = procmime_scan_message(msginfo);
  msg_file = procmsg_open_message(msginfo);

  partial = mimeinfo;
  while (partial && partial->mime_type != MIME_TEXT_HTML) {
    partial = procmime_mimeinfo_next(partial);
  }

  if (partial && partial->mime_type == MIME_TEXT_HTML) {
    
    partial->mime_type = MIME_TEXT;

    input = procmime_get_text_content(partial, msg_file, NULL);

    html_buf = calloc(partial->size+1, 1);

    n_size = fread(html_buf, partial->size, 1, input);

#if defined(USE_WEBKITGTK)
    settings = webkit_web_view_get_settings(SYLPF_OPTION.html_view);

    g_object_set(G_OBJECT(settings), ENABLE_IMAGES, SYLPF_OPTION.image_flag, NULL);
    g_object_set(G_OBJECT(settings), ENABLE_SCRIPTS, SYLPF_OPTION.script_flag, NULL);
    g_object_set(G_OBJECT(settings), ENABLE_PRIVATE_BROWSING, SYLPF_OPTION.private_flag, NULL);

    g_object_set(G_OBJECT(settings), DEFAULT_FONT_SIZE, SYLPF_OPTION.font_size, NULL);

    webkit_web_view_set_settings(SYLPF_OPTION.html_view, settings);

    webkit_web_view_load_string(SYLPF_OPTION.html_view, html_buf, NULL, NULL, "");

#elif defined(USE_GTKHTML)
    gtk_html_load_from_string(GTK_HTML(SYLPF_OPTION.html_view), html_buf, -1);
#endif

    if (SYLPF_OPTION.switch_tab_flag != FALSE) {
      gtk_notebook_set_current_page(GTK_NOTEBOOK(messageview->notebook), 2);
    }

    fclose(input);
    free(html_buf);
  } else {
    gtk_notebook_set_current_page(GTK_NOTEBOOK(messageview->notebook), 0);
  }
  SYLPF_END_FUNC;
}

static GtkWidget *create_config_main_page(GtkWidget *notebook, GKeyFile *pkey)
{
  GtkWidget *vbox;
  GtkWidget *private, *image, *scripts, *switch_tab;
  GtkWidget *label;

  SYLPF_START_FUNC;

  if (notebook == NULL){
    return NULL;
  }
  /* startup */
  if (pkey!=NULL){
  }
  vbox = gtk_vbox_new(FALSE, 6);

#if USE_WEBKITGTK
  SYLPF_OPTION.private_browsing = gtk_check_button_new_with_label(_("Enable private browsing."));
  private = sylpf_pack_widget_with_aligned_frame(SYLPF_OPTION.private_browsing, _("Privacy"));

  SYLPF_OPTION.load_image = gtk_check_button_new_with_label(_("Enable auto load image."));
  image = sylpf_pack_widget_with_aligned_frame(SYLPF_OPTION.load_image, _("Image"));

  SYLPF_OPTION.scripts = gtk_check_button_new_with_label(_("Enable scripts."));
  scripts = sylpf_pack_widget_with_aligned_frame(SYLPF_OPTION.scripts, _("Scripting"));

#endif

  SYLPF_OPTION.switch_tab = gtk_check_button_new_with_label(_("Show HTML tab as default."));
  switch_tab = sylpf_pack_widget_with_aligned_frame(SYLPF_OPTION.switch_tab, _("HTML Tab"));

  gtk_box_pack_start(GTK_BOX(vbox), switch_tab, FALSE, FALSE, 0);

#if USE_WEBKITGTK
  gtk_box_pack_start(GTK_BOX(vbox), private, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), image, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), scripts, FALSE, FALSE, 0);
#endif

  load_option_rcfile(HTMLVIEWRC);

#if USE_WEBKITGTK
  SYLPF_OPTION.private_flag = SYLPF_GET_RC_BOOLEAN("enable-private-browsing");
  SYLPF_OPTION.image_flag = SYLPF_GET_RC_BOOLEAN("auto-load-images");
  SYLPF_OPTION.script_flag = SYLPF_GET_RC_BOOLEAN("enable-scripts");
#endif

  SYLPF_OPTION.switch_tab_flag = SYLPF_GET_RC_BOOLEAN(ENABLE_SWITCH_TAB);

#define TOGGLE_STATE(widget, state) \
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(SYLPF_OPTION.widget), SYLPF_OPTION.state)

#if USE_WEBKITGTK
  TOGGLE_STATE(private_browsing, private_flag);
  TOGGLE_STATE(load_image, image_flag);
  TOGGLE_STATE(scripts, script_flag);
#endif
  TOGGLE_STATE(switch_tab, switch_tab_flag);

  save_option_rcfile();

  label = gtk_label_new(_("General"));
  gtk_notebook_append_page(GTK_NOTEBOOK(notebook), vbox, label);
  gtk_widget_show_all(notebook);

  SYLPF_RETURN_VALUE(vbox);
}

/* about, copyright tab */
static GtkWidget *create_config_about_page(GtkWidget *notebook, GKeyFile *pkey)
{
  GtkWidget *hbox, *vbox;
  GtkWidget *misc;
  GtkWidget *scrolled;
  GtkTextBuffer *tbuffer;
  GtkWidget *tview;
  GtkWidget *label;

  SYLPF_START_FUNC;

  if (notebook == NULL){
    return NULL;
  }
  hbox = gtk_hbox_new(TRUE, 6);
  vbox = gtk_vbox_new(FALSE, 6);
  gtk_box_pack_start(GTK_BOX(hbox), vbox, TRUE, TRUE, 6);

  misc = gtk_label_new(HTMLVIEW);
  gtk_box_pack_start(GTK_BOX(vbox), misc, FALSE, TRUE, 6);

  misc = gtk_label_new(PLUGIN_DESC);
  gtk_box_pack_start(GTK_BOX(vbox), misc, FALSE, TRUE, 6);

  /* copyright */
  scrolled = gtk_scrolled_window_new(NULL, NULL);

  tbuffer = gtk_text_buffer_new(NULL);
  gtk_text_buffer_set_text(tbuffer, _(copyright), strlen(copyright));
  tview = gtk_text_view_new_with_buffer(tbuffer);
  gtk_text_view_set_editable(GTK_TEXT_VIEW(tview), FALSE);
  gtk_container_add(GTK_CONTAINER(scrolled), tview);

  gtk_box_pack_start(GTK_BOX(vbox), scrolled, TRUE, TRUE, 6);

  label = gtk_label_new(_("About"));
  gtk_notebook_append_page(GTK_NOTEBOOK(notebook), hbox, label);
  gtk_widget_show_all(notebook);

  SYLPF_RETURN_VALUE(NULL);
}
