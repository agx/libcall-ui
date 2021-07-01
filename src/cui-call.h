/*
 * Copyright (C) 2021 Purism SPC
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */
#pragma once

#include <glib-object.h>

G_BEGIN_DECLS

#define CUI_TYPE_CALL (cui_call_get_type ())
G_DECLARE_INTERFACE (CuiCall, cui_call, CUI, CALL, GObject)

/**
 * CuiCall
 * @parent_iface: The parent interface
 * @get_dispaly_name: Get current calls's display name
 * @get_id: Get current calls's id
 *
 * Represents a call displayed in the ui
 */

/**
 * CuiCallState:
 *
 * The call state of a #CuiCall
 */
typedef enum
{
  CUI_CALL_STATE_UNKNOWN = 0,
  CUI_CALL_STATE_ACTIVE,
  CUI_CALL_STATE_HELD,
  CUI_CALL_STATE_DIALING,
  CUI_CALL_STATE_ALERTING,
  CUI_CALL_STATE_INCOMING,
  CUI_CALL_STATE_WAITING,
  CUI_CALL_STATE_DISCONNECTED
} CuiCallState;

struct _CuiCallInterface {
  GTypeInterface parent_iface;

  const char    *(*get_display_name)       (CuiCall *self);
  const char    *(*get_id)                 (CuiCall *self);
  CuiCallState   (*get_state)              (CuiCall *self);
  gboolean       (*get_encrypted)          (CuiCall *self);

  void           (*accept)                 (CuiCall *self);
  void           (*hang_up)                (CuiCall *self);
};

const char  *cui_call_get_display_name (CuiCall *self);
const char  *cui_call_get_id           (CuiCall *self);
CuiCallState cui_call_get_state        (CuiCall *self);
gboolean     cui_call_get_encrypted    (CuiCall *self);

void         cui_call_accept           (CuiCall *self);
void         cui_call_hang_up          (CuiCall *self);
