/*
 * Copyright (C) 2021 Purism SPC
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 */
#pragma once

#include "cui-audio-handler.h"

#include <glib.h>

G_BEGIN_DECLS

void cui_init (gboolean init_callaudio);
void cui_uninit (void);

void cui_set_default_audio_handler (CuiAudioHandler *handler);
CuiAudioHandler * cui_get_default_audio_handler (void);

G_END_DECLS
