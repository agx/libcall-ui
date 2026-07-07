/*
 * Copyright (C) 2025 Bharadwaj Raju <bharadwaj.raju@machinesoul.in>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */
#pragma once

#include <glib-object.h>

G_BEGIN_DECLS

#define CUI_TYPE_AUDIO_HANDLER (cui_audio_handler_get_type ())
G_DECLARE_INTERFACE (CuiAudioHandler, cui_audio_handler, CUI, AUDIO_HANDLER, GObject)

/**
 * CuiAudioHandlerInterface:
 * @parent_iface: The parent interface
 * @mute_mic: Mute or unmute the microphone
 * @enable_speaker: Enable or disable the speaker
 * @enable_call_mode: Enable or disable the "call" audio mode
 *
 * Interface for handling audio management during a call.
 *
 * All interface methods are required to be implemented.
 */
struct _CuiAudioHandlerInterface {
  GTypeInterface parent_iface;

  void           (*mute_mic)             (CuiAudioHandler *self, gboolean want_mute);
  void           (*enable_speaker)       (CuiAudioHandler *self, gboolean want_speaker);
  void           (*enable_call_mode)     (CuiAudioHandler *self, gboolean want_call_mode);
};

void cui_audio_handler_mute_mic (CuiAudioHandler *self, gboolean want_mute);
void cui_audio_handler_enable_speaker (CuiAudioHandler *self, gboolean want_speaker);
void cui_audio_handler_enable_call_mode (CuiAudioHandler *self, gboolean want_call_mode);

G_END_DECLS
