/*
 * HtmlView -- 
 * Copyright (C) 2012 HAYASHI Kentaro
 *
 */

#include <glib.h>
#include <gtk/gtk.h>

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
#include "ui.h"

extern HtmlViewOption SYLPF_OPTION;

static gchar* g_copyright = N_("HtmlView is distributed under 2-Clause BSD license.\n"
"\n"
"Copyright (C) 2012 HAYASHI Kentaro <kenhys@gmail.com>"
			       "\n");

GtkWidget *SYLPF_FUNC(pack_widget_with_aligned_frame)(GtkWidget *widget,
                                          const gchar *frame_title)
{
  GtkWidget *align = gtk_alignment_new(0, 0, 1, 1);
  gtk_alignment_set_padding(GTK_ALIGNMENT(align),
                            ALIGN_TOP, ALIGN_BOTTOM, ALIGN_LEFT, ALIGN_RIGHT);

  GtkWidget *frame = gtk_frame_new(frame_title);
  GtkWidget *frame_align = gtk_alignment_new(0, 0, 1, 1);
  gtk_alignment_set_padding(GTK_ALIGNMENT(frame_align),
                            ALIGN_TOP, ALIGN_BOTTOM, ALIGN_LEFT, ALIGN_RIGHT);

  gtk_container_add(GTK_CONTAINER(frame_align), widget);
  gtk_container_add(GTK_CONTAINER(frame), frame_align);
  gtk_container_add(GTK_CONTAINER(align), frame);

  gtk_widget_show(widget);

  return align;
}

GtkWidget *SYLPF_FUNC(create_config_main_page)(GtkWidget *notebook, GKeyFile *pkey)
{
  GtkWidget *vbox;
  GtkWidget *private, *image, *scripts, *switch_tab;
  
  debug_print("create_config_main_page\n");
  if (notebook == NULL){
    return NULL;
  }
  /* startup */
  if (pkey!=NULL){
  }
  vbox = gtk_vbox_new(FALSE, 6);

  SYLPF_OPTION.private_browsing = gtk_check_button_new_with_label(_("Enable private browsing."));
  private = SYLPF_FUNC(pack_widget_with_aligned_frame)(SYLPF_OPTION.private_browsing, _("Privacy"));

  SYLPF_OPTION.load_image = gtk_check_button_new_with_label(_("Enable auto load image."));
  image = SYLPF_FUNC(pack_widget_with_aligned_frame)(SYLPF_OPTION.load_image, _("Image"));

  SYLPF_OPTION.scripts = gtk_check_button_new_with_label(_("Enable scripts."));
  scripts = SYLPF_FUNC(pack_widget_with_aligned_frame)(SYLPF_OPTION.scripts, _("Scripting"));

  SYLPF_OPTION.switch_tab = gtk_check_button_new_with_label(_("Show HTML tab as default."));
  switch_tab = SYLPF_FUNC(pack_widget_with_aligned_frame)(SYLPF_OPTION.switch_tab, _("HTML Tab"));

  gtk_box_pack_start(GTK_BOX(vbox), private, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), image, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), scripts, FALSE, FALSE, 0);
  gtk_box_pack_start(GTK_BOX(vbox), switch_tab, FALSE, FALSE, 0);

  load_option_rcfile(HTMLVIEWRC);

  SYLPF_OPTION.private_flag = SYLPF_GET_RC_BOOLEAN("enable-private-browsing");
  SYLPF_OPTION.image_flag = SYLPF_GET_RC_BOOLEAN("auto-load-images");
  SYLPF_OPTION.script_flag = SYLPF_GET_RC_BOOLEAN("enable-scripts");
  SYLPF_OPTION.switch_tab_flag = SYLPF_GET_RC_BOOLEAN(ENABLE_SWITCH_TAB);

#define TOGGLE_STATE(widget, state) \
  gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(SYLPF_OPTION.widget), SYLPF_OPTION.state)

  TOGGLE_STATE(private_browsing, private_flag);
  TOGGLE_STATE(load_image, image_flag);
  TOGGLE_STATE(scripts, script_flag);
  TOGGLE_STATE(switch_tab, switch_tab_flag);

  save_option_rcfile();

  GtkWidget *general_lbl = gtk_label_new(_("General"));
  gtk_notebook_append_page(GTK_NOTEBOOK(notebook), vbox, general_lbl);
  gtk_widget_show_all(notebook);

  return vbox;
}

/* about, copyright tab */
GtkWidget *SYLPF_FUNC(create_config_about_page)(GtkWidget *notebook, GKeyFile *pkey)
{
  debug_print("create_config_about_page\n");
  if (notebook == NULL){
    return NULL;
  }
  GtkWidget *hbox = gtk_hbox_new(TRUE, 6);
  GtkWidget *vbox = gtk_vbox_new(FALSE, 6);
  gtk_box_pack_start(GTK_BOX(hbox), vbox, TRUE, TRUE, 6);

  GtkWidget *misc = gtk_label_new(HTMLVIEW);
  gtk_box_pack_start(GTK_BOX(vbox), misc, FALSE, TRUE, 6);

  misc = gtk_label_new(PLUGIN_DESC);
  gtk_box_pack_start(GTK_BOX(vbox), misc, FALSE, TRUE, 6);

  /* copyright */
  GtkWidget *scrolled = gtk_scrolled_window_new(NULL, NULL);

  GtkTextBuffer *tbuffer = gtk_text_buffer_new(NULL);
  gtk_text_buffer_set_text(tbuffer, _(g_copyright), strlen(g_copyright));
  GtkWidget *tview = gtk_text_view_new_with_buffer(tbuffer);
  gtk_text_view_set_editable(GTK_TEXT_VIEW(tview), FALSE);
  gtk_container_add(GTK_CONTAINER(scrolled), tview);

  gtk_box_pack_start(GTK_BOX(vbox), scrolled, TRUE, TRUE, 6);

  GtkWidget *general_lbl = gtk_label_new(_("About"));
  gtk_notebook_append_page(GTK_NOTEBOOK(notebook), hbox, general_lbl);
  gtk_widget_show_all(notebook);
  return NULL;
}
