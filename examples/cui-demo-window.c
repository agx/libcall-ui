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


struct _CuiDemoWindow
{
  HdyApplicationWindow parent_instance;

  GtkImage            *theme_variant_image;
  HdyLeaflet          *content_box;

  CuiCallDisplay      *call_display;
  CuiDemoCall         *call1;
};

G_DEFINE_TYPE (CuiDemoWindow, cui_demo_window, HDY_TYPE_APPLICATION_WINDOW)


static void
theme_variant_button_clicked_cb (CuiDemoWindow *self)
{
  GtkSettings *settings = gtk_settings_get_default ();
  gboolean prefer_dark_theme;

  g_object_get (settings, "gtk-application-prefer-dark-theme", &prefer_dark_theme, NULL);
  g_object_set (settings, "gtk-application-prefer-dark-theme", !prefer_dark_theme, NULL);
}


static gboolean
prefer_dark_theme_to_icon_name_cb (GBinding     *binding,
                                   const GValue *from_value,
                                   GValue       *to_value,
                                   gpointer      user_data)
{
  g_value_set_string (to_value,
                      g_value_get_boolean (from_value) ? "light-mode-symbolic" :
                                                         "dark-mode-symbolic");

  return TRUE;
}


static void
back_clicked_cb (GtkWidget     *sender,
                 CuiDemoWindow *self)
{
  hdy_leaflet_navigate (self->content_box, HDY_NAVIGATION_DIRECTION_BACK);
}


static void
on_incoming_call_clicked (GtkWidget     *sender,
			  CuiDemoWindow *self)
{
  if (!self->call1) {
    self->call1 = cui_demo_call_new ();
    cui_call_display_set_call (self->call_display, CUI_CALL (self->call1));
  }
}


static gboolean
key_pressed_cb (GtkWidget     *sender,
                GdkEvent      *event,
                CuiDemoWindow *self)
{
  GdkModifierType default_modifiers = gtk_accelerator_get_default_mod_mask ();
  guint keyval;
  GdkModifierType state;
  GdkKeymap *keymap;
  GdkEventKey *key_event = (GdkEventKey *) event;

  gdk_event_get_state (event, &state);

  keymap = gdk_keymap_get_for_display (gtk_widget_get_display (sender));

  gdk_keymap_translate_keyboard_state (keymap,
                                       key_event->hardware_keycode,
                                       state,
                                       key_event->group,
                                       &keyval, NULL, NULL, NULL);

  if ((keyval == GDK_KEY_q || keyval == GDK_KEY_Q) &&
      (state & default_modifiers) == GDK_CONTROL_MASK) {
    gtk_widget_destroy (GTK_WIDGET (self));

    return TRUE;
  }

  return FALSE;
}


static void
cui_demo_window_class_init (CuiDemoWindowClass *klass)
{
  GtkWidgetClass *widget_class = GTK_WIDGET_CLASS (klass);

  gtk_widget_class_set_template_from_resource (widget_class, "/org/gnome/CallUI/Demo/ui/cui-demo-window.ui");
  gtk_widget_class_bind_template_child (widget_class, CuiDemoWindow, call_display);
  gtk_widget_class_bind_template_child (widget_class, CuiDemoWindow, content_box);
  gtk_widget_class_bind_template_child (widget_class, CuiDemoWindow, theme_variant_image);
  gtk_widget_class_bind_template_callback (widget_class, back_clicked_cb);
  gtk_widget_class_bind_template_callback (widget_class, key_pressed_cb);
  gtk_widget_class_bind_template_callback (widget_class, theme_variant_button_clicked_cb);
  gtk_widget_class_bind_template_callback (widget_class, on_incoming_call_clicked);
}


static void
cui_demo_window_init (CuiDemoWindow *self)
{
  GtkSettings *settings = gtk_settings_get_default ();

  gtk_widget_init_template (GTK_WIDGET (self));

  g_object_bind_property_full (settings, "gtk-application-prefer-dark-theme",
                               self->theme_variant_image, "icon-name",
                               G_BINDING_SYNC_CREATE,
                               prefer_dark_theme_to_icon_name_cb,
                               NULL,
                               NULL,
                               NULL);
}

CuiDemoWindow *
cui_demo_window_new (GtkApplication *application)
{
  return g_object_new (CUI_TYPE_DEMO_WINDOW, "application", application, NULL);
}
