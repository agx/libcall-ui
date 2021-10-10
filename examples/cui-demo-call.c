/*
 * Copyright (C) 2021 Purism SPC
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 * Author: Guido Günther <agx@sigxcpu.org>
 */

#include "cui-demo-call.h"

#include <glib/gi18n.h>

enum {
  PROP_0,
  PROP_DISPLAY_NAME,
  PROP_ID,
  PROP_STATE,
  PROP_ENCRYPTED,
  PROP_CAN_DTMF,
  PROP_LAST_PROP,
};

struct _CuiDemoCall
{
  GObject       parent_instance;

  gchar        *id;
  gchar        *display_name;
  CuiCallState  state;
  gboolean      encrypted;
  gboolean      can_dtmf;
};

static void cui_demo_cui_call_interface_init (CuiCallInterface *iface);
G_DEFINE_TYPE_WITH_CODE (CuiDemoCall, cui_demo_call, G_TYPE_OBJECT,
                         G_IMPLEMENT_INTERFACE (CUI_TYPE_CALL,
                                                cui_demo_cui_call_interface_init))


static void
cui_demo_call_get_property (GObject    *object,
			    guint       prop_id,
			    GValue     *value,
			    GParamSpec *pspec)
{
  CuiDemoCall *self = CUI_DEMO_CALL (object);

  switch (prop_id) {
  case PROP_ID:
    g_value_set_string (value, self->id);
    break;
  case PROP_DISPLAY_NAME:
    g_value_set_string (value, self->display_name);
    break;
  case PROP_STATE:
    g_value_set_enum (value, self->state);
    break;
  case PROP_ENCRYPTED:
    g_value_set_boolean (value, self->encrypted);
    break;
  case PROP_CAN_DTMF:
    g_value_set_boolean (value, self->can_dtmf);
    break;
  default:
    G_OBJECT_WARN_INVALID_PROPERTY_ID (object, prop_id, pspec);
  }
}


static void
cui_demo_call_class_init (CuiDemoCallClass *klass)
{
  GObjectClass *object_class = G_OBJECT_CLASS (klass);

  object_class->get_property = cui_demo_call_get_property;

  g_object_class_override_property (object_class,
                                    PROP_DISPLAY_NAME,
                                    "id");

  g_object_class_override_property (object_class,
                                    PROP_ID,
                                    "display-name");

  g_object_class_override_property (object_class,
                                    PROP_STATE,
                                    "state");

  g_object_class_override_property (object_class,
                                    PROP_ENCRYPTED,
                                    "encrypted");

  g_object_class_override_property (object_class,
                                    PROP_CAN_DTMF,
                                    "can-dtmf");
}


static const char *
cui_demo_call_get_id (CuiCall *call)
{
  g_return_val_if_fail (CUI_IS_DEMO_CALL (call), NULL);

  return CUI_DEMO_CALL (call)->id;
}


static const char *
cui_demo_call_get_display_name (CuiCall *call)
{
  g_return_val_if_fail (CUI_IS_DEMO_CALL (call), NULL);

  return CUI_DEMO_CALL (call)->display_name;
}


static CuiCallState
cui_demo_call_get_state (CuiCall *call)
{
  g_return_val_if_fail (CUI_IS_DEMO_CALL (call), CUI_CALL_STATE_UNKNOWN);

  return CUI_DEMO_CALL (call)->state;
}


static gboolean
cui_demo_call_get_encrypted (CuiCall *call)
{
  g_return_val_if_fail (CUI_IS_DEMO_CALL (call), CUI_CALL_STATE_UNKNOWN);

  return CUI_DEMO_CALL (call)->encrypted;
}


static gboolean
cui_demo_call_get_can_dtmf (CuiCall *call)
{
  g_return_val_if_fail (CUI_IS_DEMO_CALL (call), FALSE);

  return CUI_DEMO_CALL (call)->can_dtmf;
}


static gboolean
on_accept_timeout (gpointer data)
{
  CuiDemoCall *self = CUI_DEMO_CALL (data);

  self->state = CUI_CALL_STATE_ACTIVE;
  g_object_notify (G_OBJECT (self), "state");

  return G_SOURCE_REMOVE;
}


static gboolean
on_hang_up_timeout (gpointer data)
{
  CuiDemoCall *self = CUI_DEMO_CALL (data);

  self->state = CUI_CALL_STATE_DISCONNECTED;
  g_object_notify (G_OBJECT (self), "state");

  return G_SOURCE_REMOVE;
}


static void
cui_demo_call_accept (CuiCall *call)
{
  g_return_if_fail (CUI_IS_DEMO_CALL (call));

  g_timeout_add_seconds (1, on_accept_timeout, call);
}


static void
cui_demo_call_hang_up (CuiCall *call)
{
  g_return_if_fail (CUI_IS_DEMO_CALL (call));

  g_timeout_add (250, on_hang_up_timeout, call);
}


static void
cui_demo_call_send_dtmf (CuiCall *call, const gchar *dtmf)
{
  g_return_if_fail (CUI_IS_DEMO_CALL (call));

  g_message ("DTMF: %s", dtmf);
}


static void
cui_demo_cui_call_interface_init (CuiCallInterface *iface)
{
  iface->get_id = cui_demo_call_get_id;
  iface->get_display_name = cui_demo_call_get_display_name;
  iface->get_state = cui_demo_call_get_state;
  iface->get_encrypted = cui_demo_call_get_encrypted;
  iface->get_can_dtmf = cui_demo_call_get_can_dtmf;

  iface->accept = cui_demo_call_accept;
  iface->hang_up = cui_demo_call_hang_up;
  iface->send_dtmf = cui_demo_call_send_dtmf;
}


static void
cui_demo_call_init (CuiDemoCall *self)
{
  self->display_name = g_strdup ("John Doe");
  self->id = "0800 1234";
  self->state = CUI_CALL_STATE_INCOMING;
  self->can_dtmf = TRUE;
}


CuiDemoCall *
cui_demo_call_new (void)
{
   return g_object_new (CUI_TYPE_DEMO_CALL, NULL);
}
