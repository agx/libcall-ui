/*
 * Copyright (C) 2021 Purism SPC
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */
#include "cui-config.h"

#include "call-ui.h"
#include "cui-audio-handler.h"
#include "cui-audio-handler-dummy.h"
#include "cui-audio-handler-callaudiod.h"
#include "cui-encryption-indicator-priv.h"
#include "cui-resources.h"

#include <libcallaudio.h>
#include <gio/gio.h>
#include <glib/gi18n-lib.h>
#include <gtk/gtk.h>

static gboolean cui_initialized = FALSE;
static CuiAudioHandler *cui_audio_handler = NULL;

/**
 * SECTION:cui-main
 * @short_description: Library initialization.
 * @Title: cui-main
 *
 * Before using the call-ui library you must initialize it by calling the
 * cui_init() function.
 * This makes sure translations, types, themes, and icons for the library
 * are set up properly.
 */

static void
cui_init_types (void)
{
  g_type_ensure (CUI_TYPE_CALL_DISPLAY);
  g_type_ensure (CUI_TYPE_DIALPAD);
  g_type_ensure (CUI_TYPE_KEYPAD);
  g_type_ensure (CUI_TYPE_ENCRYPTION_INDICATOR);
}

static void
cui_init_css (void)
{
  GtkCssProvider *css_provider = gtk_css_provider_new ();

  gtk_css_provider_load_from_resource (css_provider, "/org/gnome/CallUI/style.css");
  gtk_style_context_add_provider_for_display (gdk_display_get_default (),
                                              GTK_STYLE_PROVIDER (css_provider),
                                              GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
  g_object_unref (css_provider);
}

static void
cui_init_icons (void)
{
  static gsize guard = 0;

  if (!g_once_init_enter (&guard))
    return;

  gtk_icon_theme_add_resource_path (gtk_icon_theme_get_for_display (gdk_display_get_default ()),
                                    "/org/gnome/CallUI/icons");

  g_once_init_leave (&guard, 1);
}

/**
 * cui_init:
 * @init_callaudio: Whether to initialize libcallaudio
 *
 * Call this function just after initializing GTK, if you are using
 * #GtkApplication it means it must be called when the #GApplication::startup
 * signal is emitted. If call-ui has already been initialized, the function
 * will simply return.
 *
 * This makes sure translations and types for the call-ui library are
 * set up properly.
 */
void
cui_init (gboolean init_callaudio)
{
  if (cui_initialized)
    return;

  bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
  bindtextdomain (GETTEXT_PACKAGE, LOCALEDIR);

  /*
   * libcall-ui is meant as static library so register resources explicitly.
   * otherwise they get dropped during static linking
   */
  cui_register_resource ();

  cui_init_types ();
  cui_init_icons ();
  cui_init_css ();

  if (cui_audio_handler == NULL) {
    if (init_callaudio) {
      cui_audio_handler = CUI_AUDIO_HANDLER (cui_audio_handler_callaudiod_new ());
    } else {
      cui_audio_handler = CUI_AUDIO_HANDLER (cui_audio_handler_dummy_new ());
    }
  }

  cui_initialized = TRUE;
}


/**
 * cui_uninit:
 *
 * Free up resources.
 */
void
cui_uninit (void)
{
  g_clear_object (&cui_audio_handler);
}

/**
 * cui_set_default_audio_handler:
 * @handler: (transfer full): The audio handler instance
 *
 * Set the default call audio handler.
 */
void
cui_set_default_audio_handler (CuiAudioHandler *handler)
{
  cui_audio_handler = g_object_ref (handler);
}

/**
 * cui_get_default_audio_handler:
 *
 * Get the default call audio handler.
 */
CuiAudioHandler *
cui_get_default_audio_handler (void)
{
  return cui_audio_handler;
}
