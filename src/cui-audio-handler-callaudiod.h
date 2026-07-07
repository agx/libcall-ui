/*
 * Copyright (C) 2026 Bharadwaj Raju <bharadwaj.raju@machinesoul.in>
 *
 * SPDX-License-Identifier: LGPL-2.1-or-later
 *
 * Author: Bharadwaj Raju <bharadwaj.raju@machinesoul.in>
 */

#pragma once

#include "cui-config.h"

#include <glib-object.h>

G_BEGIN_DECLS

#define CUI_TYPE_AUDIO_HANDLER_CALLAUDIOD (cui_audio_handler_callaudiod_get_type ())

G_DECLARE_FINAL_TYPE (CuiAudioHandlerCallaudiod, cui_audio_handler_callaudiod, CUI, AUDIO_HANDLER_CALLAUDIOD, GObject)

CuiAudioHandlerCallaudiod *cui_audio_handler_callaudiod_new (void);

G_END_DECLS
