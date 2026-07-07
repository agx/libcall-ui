/*
 * Copyright (C) 2026 Bharadwaj Raju <bharadwaj.raju@machinesoul.in>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Author: Bharadwaj Raju <bharadwaj.raju@machinesoul.in>
 */

#include "cui-config.h"

#include "cui-audio-handler.h"
#include "cui-enums.h"

#include <gio/gio.h>

/**
 * CuiAudioHandler:
 *
 * An interface for managing audio during calls.
 *
 * Objects implementing the `CuiAudioHandler` interface allow managing audio
 * outputs during a call, such as muting the microphone, switching to speaker,
 * or switching audio modes.
 */

G_DEFINE_INTERFACE (CuiAudioHandler, cui_audio_handler, G_TYPE_OBJECT)


void
cui_audio_handler_default_init (CuiAudioHandlerInterface *iface)
{
}


/**
 * cui_audio_handler_mute_mic:
 * @self: The audio handler
 * @want_mute: Whether to mute the microphone
 *
 * Mutes or unmutes the microphone.
 */
void
cui_audio_handler_mute_mic (CuiAudioHandler *self, gboolean want_mute)
{
  CuiAudioHandlerInterface *iface;

  g_return_if_fail (CUI_IS_AUDIO_HANDLER (self));

  iface = CUI_AUDIO_HANDLER_GET_IFACE (self);
  g_return_if_fail (iface->mute_mic);

  iface->mute_mic (self, want_mute);
}


/**
 * cui_audio_handler_enable_speaker:
 * @self: The audio handler
 * @want_speaker: Whether to enable the speaker
 *
 * Enables or disables the loud speaker.
 */
void
cui_audio_handler_enable_speaker (CuiAudioHandler *self, gboolean want_speaker)
{
  CuiAudioHandlerInterface *iface;

  g_return_if_fail (CUI_IS_AUDIO_HANDLER (self));

  iface = CUI_AUDIO_HANDLER_GET_IFACE (self);
  g_return_if_fail (iface->enable_speaker);

  iface->enable_speaker (self, want_speaker);
}


/**
 * cui_audio_handler_enable_call_mode:
 * @self: The audio handler
 * @want_call_mode: Whether to enter "call" audio mode or a "default" one.
 *
 * Selects the audio mode.
 */
void
cui_audio_handler_enable_call_mode (CuiAudioHandler *self, gboolean want_call_mode)
{
  CuiAudioHandlerInterface *iface;

  g_return_if_fail (CUI_IS_AUDIO_HANDLER (self));

  iface = CUI_AUDIO_HANDLER_GET_IFACE (self);
  g_return_if_fail (iface->enable_call_mode);

  iface->enable_call_mode (self, want_call_mode);
}

