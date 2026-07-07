/*
 * Copyright (C) 2026 Bharadwaj Raju <bharadwaj.raju@machinesoul.in>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Author: Bharadwaj Raju <bharadwaj.raju@machinesoul.in>
 */

#include "cui-config.h"

#include "cui-audio-handler-dummy.h"
#include "cui-audio-handler.h"
#include "cui-enums.h"


struct _CuiAudioHandlerDummy {
  GObject parent_instance;
};


static void
cui_audio_handler_dummy_iface_init (CuiAudioHandlerInterface *iface);

G_DEFINE_TYPE_WITH_CODE (CuiAudioHandlerDummy, cui_audio_handler_dummy, G_TYPE_OBJECT,
                         G_IMPLEMENT_INTERFACE (CUI_TYPE_AUDIO_HANDLER,
                                                cui_audio_handler_dummy_iface_init))


static void
cui_audio_handler_dummy_class_init (CuiAudioHandlerDummyClass *klass)
{
}


static void
cui_audio_handler_dummy_init (CuiAudioHandlerDummy *self)
{
}


static void
cui_audio_handler_dummy_mute_mic (CuiAudioHandler *self, gboolean want_mute)
{
}


static void
cui_audio_handler_dummy_enable_speaker (CuiAudioHandler *self, gboolean want_speaker)
{
}


static void
cui_audio_handler_dummy_enable_call_mode (CuiAudioHandler *self, gboolean want_call_mode)
{
}


static void
cui_audio_handler_dummy_iface_init (CuiAudioHandlerInterface *iface)
{
  iface->mute_mic = cui_audio_handler_dummy_mute_mic;
  iface->enable_speaker = cui_audio_handler_dummy_enable_speaker;
  iface->enable_call_mode = cui_audio_handler_dummy_enable_call_mode;
}


CuiAudioHandlerDummy *
cui_audio_handler_dummy_new (void)
{
  return g_object_new (CUI_TYPE_AUDIO_HANDLER_DUMMY, NULL);
}
