/*
 * Copyright (C) 2026 Bharadwaj Raju <bharadwaj.raju@machinesoul.in>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Author: Bharadwaj Raju <bharadwaj.raju@machinesoul.in>
 */

#include "cui-config.h"

#include "cui-audio-handler-callaudiod.h"
#include "cui-audio-handler.h"
#include "cui-enums.h"

#include <libcallaudio.h>

struct _CuiAudioHandlerCallaudiod {
  GObject parent_instance;
};


/* Just print an error, the main point is that libcallaudio uses async DBus calls */
static void
on_libcallaudio_async_finished (gboolean success, GError *error, gpointer data)
{
  if (!success) {
    g_return_if_fail (error && error->message);
    g_warning ("Failed to select audio mode: %s", error->message);
    g_error_free (error);
  }
}


static void
cui_audio_handler_callaudiod_iface_init (CuiAudioHandlerInterface *iface);

G_DEFINE_TYPE_WITH_CODE (CuiAudioHandlerCallaudiod, cui_audio_handler_callaudiod, G_TYPE_OBJECT,
                         G_IMPLEMENT_INTERFACE (CUI_TYPE_AUDIO_HANDLER,
                                                cui_audio_handler_callaudiod_iface_init))


static void
cui_audio_handler_callaudiod_finalize (GObject *object)
{
G_GNUC_BEGIN_IGNORE_DEPRECATIONS
  call_audio_deinit ();
G_GNUC_END_IGNORE_DEPRECATIONS
  G_OBJECT_CLASS (cui_audio_handler_callaudiod_parent_class)->finalize (object);
}

static void
cui_audio_handler_callaudiod_class_init (CuiAudioHandlerCallaudiodClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);
  object_class->finalize = cui_audio_handler_callaudiod_finalize;
}


static void
cui_audio_handler_callaudiod_init (CuiAudioHandlerCallaudiod *self)
{
G_GNUC_BEGIN_IGNORE_DEPRECATIONS
  call_audio_init (NULL);
G_GNUC_END_IGNORE_DEPRECATIONS
}


static void
cui_audio_handler_callaudiod_mute_mic (CuiAudioHandler *self, gboolean want_mute)
{
G_GNUC_BEGIN_IGNORE_DEPRECATIONS
  call_audio_mute_mic_async (want_mute, on_libcallaudio_async_finished, NULL);
G_GNUC_END_IGNORE_DEPRECATIONS

}


static void
cui_audio_handler_callaudiod_enable_speaker (CuiAudioHandler *self, gboolean want_speaker)
{
G_GNUC_BEGIN_IGNORE_DEPRECATIONS
  call_audio_enable_speaker_async (want_speaker, on_libcallaudio_async_finished, NULL);
G_GNUC_END_IGNORE_DEPRECATIONS

}


static void
cui_audio_handler_callaudiod_enable_call_mode (CuiAudioHandler *self, gboolean want_call_mode)
{
G_GNUC_BEGIN_IGNORE_DEPRECATIONS
    call_audio_select_mode_async (want_call_mode ? CALL_AUDIO_MODE_CALL : CALL_AUDIO_MODE_DEFAULT,
                                  on_libcallaudio_async_finished,
                                  NULL);
G_GNUC_BEGIN_IGNORE_DEPRECATIONS
}


static void
cui_audio_handler_callaudiod_iface_init (CuiAudioHandlerInterface *iface)
{
  iface->mute_mic = cui_audio_handler_callaudiod_mute_mic;
  iface->enable_speaker = cui_audio_handler_callaudiod_enable_speaker;
  iface->enable_call_mode = cui_audio_handler_callaudiod_enable_call_mode;
}


CuiAudioHandlerCallaudiod *
cui_audio_handler_callaudiod_new (void)
{
  return g_object_new (CUI_TYPE_AUDIO_HANDLER_CALLAUDIOD, NULL);
}
