/*
 * Copyright (C) 2021 Purism SPC
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Author: Guido Günther <agx@sigxcpu.org>
 */

#include "cui-demo-call.h"
#include "cui-demo-window.h"

#include <glib/gi18n.h>


struct _CuiDemoWindow {
  AdwApplicationWindow parent_instance;

  GtkImage            *theme_variant_image;
  AdwLeaflet          *content_box;
  GtkButton           *incoming_call;
  GtkButton           *outgoing_call;

  CuiCallDisplay      *call_display;
  CuiDialpad          *dialpad;
  CuiKeypad           *keypad;
  CuiDemoCall         *call1;
};

G_DEFINE_TYPE (CuiDemoWindow, cui_demo_window, ADW_TYPE_APPLICATION_WINDOW)


static void
theme_variant_button_clicked_cb (CuiDemoWindow *self)
{
  AdwStyleManager *style_manager;
  gboolean is_dark;

  style_manager = adw_style_manager_get_default ();
  is_dark = adw_style_manager_get_dark (style_manager);

  g_debug ("let there be %s", is_dark ? "light" : "darkness");

  adw_style_manager_set_color_scheme (style_manager,
                                      is_dark ?
                                      ADW_COLOR_SCHEME_FORCE_LIGHT :
                                      ADW_COLOR_SCHEME_FORCE_DARK);
}


static gboolean
prefer_dark_theme_to_icon_name_cb (GBinding     *binding,
                                   const GValue *from_value,
                                   GValue       *to_value,
                                   gpointer      user_data)
{
  g_value_set_string (to_value,
                      g_value_get_boolean (from_value) ?
                      "light-mode-symbolic" :
                      "dark-mode-symbolic");

  return TRUE;
}


static void
back_clicked_cb (CuiDemoWindow *self)
{
  adw_leaflet_navigate (self->content_box, ADW_NAVIGATION_DIRECTION_BACK);
}


static gboolean
clear_call (CuiDemoWindow *self)
{
  g_assert (CUI_IS_DEMO_WINDOW (self));

  g_clear_object (&self->call1);
  gtk_widget_set_sensitive (GTK_WIDGET (self->incoming_call), TRUE);
  gtk_widget_set_sensitive (GTK_WIDGET (self->outgoing_call), TRUE);

  return G_SOURCE_REMOVE;
}


static void
on_call_state_changed (CuiDemoCall *call, GParamSpec *pspec, gpointer user_data)
{
  CuiDemoWindow *self = CUI_DEMO_WINDOW (user_data);
  CuiCallState state = cui_call_get_state (CUI_CALL (call));

  g_return_if_fail (call == self->call1);

  if (state == CUI_CALL_STATE_DISCONNECTED)
    g_timeout_add_seconds (3, G_SOURCE_FUNC (clear_call), self);
}


static void
on_new_call_clicked (GtkButton     *button,
                     CuiDemoWindow *self)
{
  g_assert (CUI_IS_DEMO_WINDOW (self));
  g_assert (button == self->incoming_call ||
            button == self->outgoing_call);

  if (!self->call1) {
    gboolean incoming = button == self->incoming_call;

    self->call1 = cui_demo_call_new (incoming);
    cui_demo_call_set_encrypted (self->call1, TRUE);

    g_signal_connect (self->call1,
                      "notify::state",
                      G_CALLBACK (on_call_state_changed),
                      self);
    on_call_state_changed (self->call1, NULL, self);

    gtk_widget_set_sensitive (GTK_WIDGET (self->incoming_call), FALSE);
    gtk_widget_set_sensitive (GTK_WIDGET (self->outgoing_call), FALSE);

    cui_call_display_set_call (self->call_display, CUI_CALL (self->call1));
  }
}


static gboolean
key_pressed_cb (CuiDemoWindow *self,
                guint keyval,
                guint keycode,
                GdkModifierType state,
                GtkEventControllerKey* controller)
{
  GdkModifierType default_modifiers = gtk_accelerator_get_default_mod_mask ();

  if ((keyval == GDK_KEY_q || keyval == GDK_KEY_Q) &&
      (state & default_modifiers) == GDK_CONTROL_MASK) {
    gtk_window_destroy (GTK_WINDOW (self));

    return TRUE;
  }

  return FALSE;
}

static void
on_dial (CuiDemoWindow *self, const char number[], GtkWidget *sender)
{
  GtkDialogFlags flags;
  GtkWidget *dialog;

  g_debug ("Dialling %s", number);

  flags = GTK_DIALOG_MODAL | GTK_DIALOG_DESTROY_WITH_PARENT | GTK_DIALOG_USE_HEADER_BAR;
  dialog = gtk_message_dialog_new (GTK_WINDOW (self),
                                   flags,
                                   GTK_MESSAGE_INFO,
                                   GTK_BUTTONS_OK,
                                   "Dialling %s", number);

  g_signal_connect_swapped (dialog, "response",
                            G_CALLBACK (gtk_window_destroy),
                            dialog);

  gtk_window_present (GTK_WINDOW (dialog));
}


static void
on_visible_child_changed (GObject       *sender,
                          GParamSpec    *pspec,
                          CuiDemoWindow *self)
{
  adw_leaflet_navigate (self->content_box, ADW_NAVIGATION_DIRECTION_FORWARD);
}


static void
cui_demo_window_class_init (CuiDemoWindowClass *klass)
{
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  gtk_widget_class_set_template_from_resource (widget_class, "/org/gnome/CallUI/Demo/ui/cui-demo-window.ui");
  gtk_widget_class_bind_template_child (widget_class, CuiDemoWindow, call_display);
  gtk_widget_class_bind_template_child (widget_class, CuiDemoWindow, dialpad);
  gtk_widget_class_bind_template_child (widget_class, CuiDemoWindow, keypad);
  gtk_widget_class_bind_template_child (widget_class, CuiDemoWindow, content_box);
  gtk_widget_class_bind_template_child (widget_class, CuiDemoWindow, incoming_call);
  gtk_widget_class_bind_template_child (widget_class, CuiDemoWindow, outgoing_call);
  gtk_widget_class_bind_template_child (widget_class, CuiDemoWindow, theme_variant_image);
  gtk_widget_class_bind_template_callback (widget_class, back_clicked_cb);
  gtk_widget_class_bind_template_callback (widget_class, key_pressed_cb);
  gtk_widget_class_bind_template_callback (widget_class, theme_variant_button_clicked_cb);
  gtk_widget_class_bind_template_callback (widget_class, on_new_call_clicked);
  gtk_widget_class_bind_template_callback (widget_class, on_dial);
  gtk_widget_class_bind_template_callback (widget_class, on_visible_child_changed);
}


static void
cui_demo_window_init (CuiDemoWindow *self)
{
  AdwStyleManager *style_manager = adw_style_manager_get_default();

  GtkEventController *controller = gtk_event_controller_key_new ();
  g_signal_connect_swapped (controller, "key-pressed", G_CALLBACK (key_pressed_cb), self);

  gtk_widget_init_template (GTK_WIDGET (self));

  g_object_bind_property_full (style_manager, "dark",
                               self->theme_variant_image, "icon-name",
                               G_BINDING_SYNC_CREATE,
                               prefer_dark_theme_to_icon_name_cb,
                               NULL,
                               NULL,
                               NULL);

  gtk_widget_add_controller (GTK_WIDGET (self), controller);
}

CuiDemoWindow *
cui_demo_window_new (GtkApplication *application)
{
  return g_object_new (CUI_TYPE_DEMO_WINDOW, "application", application, NULL);
}
