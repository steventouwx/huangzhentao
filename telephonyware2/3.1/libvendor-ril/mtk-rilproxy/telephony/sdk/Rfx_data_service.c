// SPDX-License-Identifier: MediaTekProprietary
/* Copyright Statement:
 *
 * This software/firmware and related documentation ("MediaTek Software") are
 * protected under relevant copyright laws. The information contained herein
 * is confidential and proprietary to MediaTek Inc. and/or its licensors.
 * Without the prior written permission of MediaTek inc. and/or its licensors,
 * any reproduction, modification, use or disclosure of MediaTek Software,
 * and information contained herein, in whole or in part, shall be strictly prohibited.
 *
 * MediaTek Inc. (C) 2010. All rights reserved.
 *
 * BY OPENING THIS FILE, RECEIVER HEREBY UNEQUIVOCALLY ACKNOWLEDGES AND AGREES
 * THAT THE SOFTWARE/FIRMWARE AND ITS DOCUMENTATIONS ("MEDIATEK SOFTWARE")
 * RECEIVED FROM MEDIATEK AND/OR ITS REPRESENTATIVES ARE PROVIDED TO RECEIVER ON
 * AN "AS-IS" BASIS ONLY. MEDIATEK EXPRESSLY DISCLAIMS ANY AND ALL WARRANTIES,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE OR NONINFRINGEMENT.
 * NEITHER DOES MEDIATEK PROVIDE ANY WARRANTY WHATSOEVER WITH RESPECT TO THE
 * SOFTWARE OF ANY THIRD PARTY WHICH MAY BE USED BY, INCORPORATED IN, OR
 * SUPPLIED WITH THE MEDIATEK SOFTWARE, AND RECEIVER AGREES TO LOOK ONLY TO SUCH
 * THIRD PARTY FOR ANY WARRANTY CLAIM RELATING THERETO. RECEIVER EXPRESSLY ACKNOWLEDGES
 * THAT IT IS RECEIVER'S SOLE RESPONSIBILITY TO OBTAIN FROM ANY THIRD PARTY ALL PROPER LICENSES
 * CONTAINED IN MEDIATEK SOFTWARE. MEDIATEK SHALL ALSO NOT BE RESPONSIBLE FOR ANY MEDIATEK
 * SOFTWARE RELEASES MADE TO RECEIVER'S SPECIFICATION OR TO CONFORM TO A PARTICULAR
 * STANDARD OR OPEN FORUM. RECEIVER'S SOLE AND EXCLUSIVE REMEDY AND MEDIATEK'S ENTIRE AND
 * CUMULATIVE LIABILITY WITH RESPECT TO THE MEDIATEK SOFTWARE RELEASED HEREUNDER WILL BE,
 * AT MEDIATEK'S OPTION, TO REVISE OR REPLACE THE MEDIATEK SOFTWARE AT ISSUE,
 * OR REFUND ANY SOFTWARE LICENSE FEES OR SERVICE CHARGE PAID BY RECEIVER TO
 * MEDIATEK FOR SUCH MEDIATEK SOFTWARE AT ISSUE.
 *
 * The following software/firmware and/or related documentation ("MediaTek Software")
 * have been modified by MediaTek Inc. All revisions are subject to any receiver's
 * applicable license agreements with MediaTek Inc.
 */

#ifndef MTK_RILPROXY_TELEPHONY_SDK_RFX_DATA_SERVICE_H_
#define MTK_RILPROXY_TELEPHONY_SDK_RFX_DATA_SERVICE_H_

#include <mtk_log.h>
#include <gio/gio.h>

#include "data.h"

#include "Rfx_data_service.h"
#include "Wrapper.h"



#define LOG_TAG "Rfx_data_service"

extern int rfx_get_data_allowed_slotid(void);

static gint sToken = 0;
static GMutex g_mutex;
static GHashTable *hashTable = NULL;
static GAsyncQueue *queue;

typedef struct {
    gint token;
    int requestNumber;
    GDBusMethodInvocation *invocation;
    GVariant *value;
} RequestInfo;

static GDBusNodeInfo *introspection_data = NULL;
static const gchar introspection_xml[] =
        "<node>"
        "  <interface name='mtk.telephony.Data'>"
        "    <method name='enableData'>"
        "      <arg name='enable' type='b' direction='in'/>"
        "      <arg name='apn_type' type='s' direction='in'/>"
        "      <arg name='datacallresponse' type='(iiiisssssssi)' direction='out'/>"
        "    </method>"
        "    <signal name='InternetStatusChanged'>"
        "      <arg name='datacallresponse' type='(iiiisssssssi)'/>"
        "    </signal>"
        "    <signal name='SuplStatusChanged'>"
        "      <arg name='datacallresponse' type='(iiiisssssssi)'/>"
        "    </signal>"
        "    <signal name='IOTDefaultStatusChanged'>"
        "      <arg name='datacallresponse' type='(iiiisssssssi)'/>"
        "    </signal>"
        "    <signal name='IOTNet0StatusChanged'>"
        "      <arg name='datacallresponse' type='(iiiisssssssi)'/>"
        "    </signal>"
        "    <signal name='IOTNet1StatusChanged'>"
        "      <arg name='datacallresponse' type='(iiiisssssssi)'/>"
        "    </signal>"
        "    <signal name='IOTNet2StatusChanged'>"
        "      <arg name='datacallresponse' type='(iiiisssssssi)'/>"
        "    </signal>"
        "    <signal name='IOTNet3StatusChanged'>"
        "      <arg name='datacallresponse' type='(iiiisssssssi)'/>"
        "    </signal>"
        "    <signal name='IOTNet4StatusChanged'>"
        "      <arg name='datacallresponse' type='(iiiisssssssi)'/>"
        "    </signal>"
        "    <signal name='IOTNet5StatusChanged'>"
        "      <arg name='datacallresponse' type='(iiiisssssssi)'/>"
        "    </signal>"
        "    <signal name='IOTNet6StatusChanged'>"
        "      <arg name='datacallresponse' type='(iiiisssssssi)'/>"
        "    </signal>"
        "  </interface>"
        "</node>";

extern void sendRequestString(int request, int slotId, int token, const char *data);


#define SUPPORTED_APN_NUM 12
const gchar * SUPPORTED_APN[12] = {
        TEXT_APN_TYPE_DEFAULT,
        TEXT_APN_TYPE_SUPL,
        TEXT_APN_TYPE_WAP,
        TEXT_APN_TYPE_RCS,
        TEXT_IOT_APN_TYPE_DEFAULT,
        TEXT_IOT_APN_TYPE_NET_0,
        TEXT_IOT_APN_TYPE_NET_1,
        TEXT_IOT_APN_TYPE_NET_2,
        TEXT_IOT_APN_TYPE_NET_3,
        TEXT_IOT_APN_TYPE_NET_4,
        TEXT_IOT_APN_TYPE_NET_5,
        TEXT_IOT_APN_TYPE_NET_6};

static gboolean isValidArg(const gchar *arg_apn_type){
    for (int i = 0; i < SUPPORTED_APN_NUM; i++) {
        RLOGD("isValidArg compare arg_apn_type %s, %s", arg_apn_type, SUPPORTED_APN[i]);
        if (g_strcmp0(arg_apn_type, SUPPORTED_APN[i]) == 0) {
            return TRUE;
        }
    }

    return FALSE;
}



static gboolean send_complete(gpointer user_data)
{
    RLOGD("send_complete, start blocking");
    RequestInfo *requestInfo = (RequestInfo *)g_async_queue_try_pop(queue);
    if(requestInfo != NULL)
    {
        g_dbus_method_invocation_return_value(requestInfo->invocation,g_variant_new ("(@(iiiisssssssi))",
                requestInfo->value));
        g_object_unref(requestInfo->invocation);
        int token = requestInfo->token;
        if(!g_hash_table_remove(hashTable, &token))
        {
            RLOGD("remove fail token: %d", token);
        }
    } else {
        RLOGD("no requestinfo");
    }
    return G_SOURCE_REMOVE;
}

static void
on_enable_data (GDBusConnection       *connection,
                    const gchar           *sender,
                    const gchar           *object_path,
                    const gchar           *interface_name,
                    const gchar           *method_name,
                    GVariant              *parameters,
                    GDBusMethodInvocation *invocation,
                    gpointer               user_data)
{
    int data_slotId=rfx_get_data_allowed_slotid();

    if (g_strcmp0 (method_name, "enableData") == 0)
    {
        g_mutex_lock(&g_mutex);
        gboolean arg_enable;
        gchar *arg_apn_type;
        g_variant_get (parameters, "(bs)", &arg_enable, &arg_apn_type);
        RLOGD("APN type is %s, %s data", arg_apn_type, (arg_enable ? "enable" : "disable"));
        if (isValidArg(arg_apn_type))
        {
            if(sToken == G_MAXINT32) sToken = 0; // prevent overflow.
            g_atomic_int_inc(&sToken);
            RLOGD("token: %d", sToken);
            int request = arg_enable ? RIL_REQUEST_SETUP_DATA_CALL : RIL_REQUEST_DEACTIVATE_DATA_CALL;
            RequestInfo *requestInfo = (RequestInfo *)g_try_malloc0(sizeof(RequestInfo));
            if (requestInfo == NULL) {
                RLOGD("allocation memory for the requestInfo fail.");
                g_dbus_method_invocation_return_dbus_error(invocation,
                                                                   "mtk.telephony.data.memory.allocation.fail",
                                                                   "allocation memory for the requestInfo fail.");
            } else {
                requestInfo->token = sToken;
                requestInfo->requestNumber = request;
                requestInfo->invocation = g_object_ref(invocation);
                g_hash_table_insert(hashTable, &sToken, requestInfo);

                sendRequestString(request, data_slotId, sToken, arg_apn_type);
            }
        } else {
            RLOGD("the APN type(%s) don't support", arg_apn_type);
            g_dbus_method_invocation_return_dbus_error(invocation,
                                                       "mtk.telephony.data.apntype.wrong",
                                                       "the parameters is invalid!");
        }
        g_free(arg_apn_type);
        g_mutex_unlock(&g_mutex);
    }
}

/* for now */
static const GDBusInterfaceVTable interface_vtable =
{
  on_enable_data,
  NULL,
  NULL
};

static void
on_bus_acquired (GDBusConnection *connection,
                                      const gchar     *name,
                                      gpointer         user_data)
{

    RLOGD("Acquired a message bus connection");

    guint registration_id;

    registration_id = g_dbus_connection_register_object (connection,
                                                         TELEPHONY_DATA_PATH,
                                                         introspection_data->interfaces[0],
                                                         &interface_vtable,
                                                         NULL,  /* user_data */
                                                         NULL,  /* user_data_free_func */
                                                         NULL); /* GError** */
    g_assert (registration_id > 0);
}

static void
on_name_acquired (GDBusConnection *connection,
                                          const gchar     *name,
                                          gpointer         user_data)
{
    RLOGD("Acquired the name %s", name);
}

static void
on_name_lost(GDBusConnection *connection,
                                         const gchar     *name,
                                         gpointer user_data)
{
    RLOGD("Lost the name %s", name);
}

static void
on_name_appeared (GDBusConnection *connection,
                  const gchar     *name,
                  const gchar     *name_owner,
                  gpointer         user_data)
{
    RLOGD("Name %s on %s is owned by %s\n",name,"the system bus",name_owner);
}

//handle connman destroy.
static void
on_name_vanished (GDBusConnection *connection,
                  const gchar     *name,
                  gpointer         user_data)
{
    int data_slotId=rfx_get_data_allowed_slotid();
    RLOGD("Name %s does not exist on %s\n, disable data",name,"the system bus");
    if (sToken == G_MAXINT32)
        sToken = 0; // prevent overflow.
    g_atomic_int_inc(&sToken);
    RLOGD("on_name_vanished() token: %d", sToken);
    int request = RIL_REQUEST_DEACTIVATE_DATA_CALL;
    RequestInfo *requestInfo = (RequestInfo *) g_try_malloc0(sizeof(RequestInfo));
    if (requestInfo == NULL) {
        RLOGD("allocation memory for the requestInfo fail.");
    } else {
        requestInfo->token = sToken;
        requestInfo->requestNumber = request;
        requestInfo->invocation = NULL;
        g_hash_table_insert(hashTable, &sToken, requestInfo);

        sendRequestString(request, data_slotId, sToken, TEXT_APN_TYPE_DEFAULT);
    }
}

static gpointer
data_dbus_service_cb (gpointer data)
{
    RLOGD("data_dbus_service_cb");
    GMainLoop *loop;
    guint id;
    guint watcher_id;
    sleep(5);
    RLOGD("sleep 5");
    introspection_data = g_dbus_node_info_new_for_xml (introspection_xml, NULL);
    g_assert (introspection_data != NULL);

    hashTable = g_hash_table_new_full(g_int_hash, g_int_equal, NULL, g_free);
    queue = g_async_queue_new();
    loop = g_main_loop_new(NULL, FALSE);
    id = g_bus_own_name(G_BUS_TYPE_SYSTEM,
                           TELEPHONY_SERVICE,
                           G_BUS_NAME_OWNER_FLAGS_ALLOW_REPLACEMENT |
                           G_BUS_NAME_OWNER_FLAGS_REPLACE,
                           on_bus_acquired,
                           on_name_acquired,
                           on_name_lost,
                           loop,
                           NULL);
    watcher_id = g_bus_watch_name (G_BUS_TYPE_SYSTEM,
                                   "net.connman",
                                   G_BUS_NAME_WATCHER_FLAGS_NONE,
                                   on_name_appeared,
                                   on_name_vanished,
                                   NULL,
                                   NULL);

    g_main_loop_run(loop);

    g_bus_unown_name(id);
    g_bus_unwatch_name(watcher_id);
    RLOGD("unref loop");
    g_main_loop_unref(loop);
    g_async_queue_unref(queue);
    g_dbus_node_info_unref (introspection_data);
    return NULL;
}

void
initGDbus()
{
    RLOGD("initGDBus\n");
    GError *error = NULL;
    GThread *thead = g_thread_try_new("sdk_data_dbus_service", data_dbus_service_cb, NULL, &error);
    g_assert_no_error(error);
    if(error!=NULL)
    {
        RLOGD("initGDbus free error pointer %P", error);
        g_error_free(error);
    }
}

static gchar* sdk_requestToString(int request)
{
    RLOGD("the request values is %d", request);
    switch(request){
        case RIL_REQUEST_SETUP_DATA_CALL : return "RIL_REQUEST_SETUP_DATA_CALL";
        case RIL_REQUEST_DEACTIVATE_DATA_CALL : return "RIL_REQUEST_DEACTIVATE_DATA_CALL";
        case RIL_UNSOL_LOCAL_UPDATE_PDN_INFO_CHANGED : return "RIL_UNSOL_LOCAL_UPDATE_PDN_INFO_CHANGED";
        case RIL_UNSOL_SETUP_PDN_STATE_CHANGED: return "RIL_UNSOL_SETUP_PDN_STATE_CHANGED";
        case RIL_UNSOL_TEAR_DOWN_PDN_STATE_CHANGED: return "RIL_UNSOL_TEAR_DOWN_PDN_STATE_CHANGED";
        default: return "<unknown state>";
    }
}

void
sdk_onRequestComplete(int token, RIL_Errno e, void *response, size_t responselen)
{
  RLOGD("requestComplete token: %d, size: %d", token,
      g_hash_table_size(hashTable));
    RequestInfo *requestInfo = (RequestInfo*) g_hash_table_lookup(hashTable, &token);
    if(requestInfo == NULL)
    {
        RLOGE("cann't find requestInfo object with token(%d)", token);
        return;
    }
    if (e != RIL_E_SUCCESS)
    {
        RLOGW("%s fail(%d)", sdk_requestToString(requestInfo->requestNumber),e);
        gchar* msg = g_strdup_printf("%s fail", sdk_requestToString(requestInfo->requestNumber));
        g_dbus_method_invocation_return_dbus_error(requestInfo->invocation,
                                                   "mtk.telephony.data.service.fail",
                                                   msg);
        g_free(msg);

        goto done;
    }

    if (response == NULL)
    {
        RLOGD("no response value");
        g_dbus_method_invocation_return_dbus_error(requestInfo->invocation,
                                                         "mtk.telephony.data.service.fail",
                                                         "no response value");
        goto done;
    }

    if (responselen == 0)
    {
        RLOGD("responselen value is invalid(0)");
        g_dbus_method_invocation_return_dbus_error(requestInfo->invocation,
                                                         "mtk.telephony.data.service.fail",
                                                         "responselen value is invalid(0)");
        goto done;
    }

    MTK_Data_Call_Response_v1 rsp;
    parseDataCallResponse(&rsp, response, responselen);
    GVariant *value;
    value = g_variant_new("(iiiisssssssi)", rsp.netId,
                rsp.pdnState, rsp.status, rsp.cId, rsp.apnType,
                rsp.type, rsp.ifname, rsp.addresses, rsp.dnses,
                rsp.gateways, rsp.pcscf, rsp.mtu);
    requestInfo->value = value;
    g_async_queue_push(queue, requestInfo);
    g_main_context_invoke(NULL, send_complete, NULL);
    goto success;
done:
    if(!g_hash_table_remove(hashTable, &token))
    {
        RLOGD("remove fail token: %d", token);
    }
success:
    g_free(rsp.apnType);
    g_free(rsp.type);
    g_free(rsp.ifname);
    g_free(rsp.addresses);
    g_free(rsp.dnses);
    g_free(rsp.gateways);
    g_free(rsp.pcscf);
    RLOGD("complete return,token: %d", token);
}

static void
emitTelephonySignal (MTK_Data_Call_Response_v1 *rsp)
{
    if(rsp == NULL || rsp->apnType == NULL)
    {
        RLOGW("rsponse data is (rsp or apnType)NULL, just return");
        return;
    }

    char* signal_name = NULL;
    RLOGD("emit %s PDN status changed", rsp->apnType);
    if (g_str_match_string(TEXT_APN_TYPE_DEFAULT, rsp->apnType, FALSE)) {
        signal_name = "InternetStatusChanged";
    } else if(g_str_match_string(TEXT_APN_TYPE_SUPL, rsp->apnType, FALSE)){
        signal_name = "SuplStatusChanged";
    } else if(g_str_match_string(TEXT_IOT_APN_TYPE_DEFAULT, rsp->apnType, FALSE)){
        signal_name = "IOTDefaultStatusChanged";
    } else if(g_str_match_string(TEXT_IOT_APN_TYPE_NET_0, rsp->apnType, FALSE)){
        signal_name = "IOTNet0StatusChanged";
    } else if(g_str_match_string(TEXT_IOT_APN_TYPE_NET_1, rsp->apnType, FALSE)){
        signal_name = "IOTNet1StatusChanged";
    } else if(g_str_match_string(TEXT_IOT_APN_TYPE_NET_2, rsp->apnType, FALSE)){
        signal_name = "IOTNet2StatusChanged";
    } else if(g_str_match_string(TEXT_IOT_APN_TYPE_NET_3, rsp->apnType, FALSE)){
        signal_name = "IOTNet3StatusChanged";
    } else if(g_str_match_string(TEXT_IOT_APN_TYPE_NET_4, rsp->apnType, FALSE)){
        signal_name = "IOTNet4StatusChanged";
    } else if(g_str_match_string(TEXT_IOT_APN_TYPE_NET_5, rsp->apnType, FALSE)){
        signal_name = "IOTNet5StatusChanged";
    } else if(g_str_match_string(TEXT_IOT_APN_TYPE_NET_6, rsp->apnType, FALSE)){
        signal_name = "IOTNet6StatusChanged";
    } else {
        RLOGD("no match apn type, just return");
        return;
    }
    GVariant *value;
    value = g_variant_new("(iiiisssssssi)", rsp->netId,
            rsp->pdnState, rsp->status, rsp->cId, rsp->apnType,
            rsp->type, rsp->ifname, rsp->addresses, rsp->dnses,
            rsp->gateways, rsp->pcscf, rsp->mtu);
    if (value == NULL) {
        RLOGW("value is NULL");
        return;
    }

    GDBusConnection *connection;
    GError *error = NULL;
    RLOGD("get sync connection begin");
    connection = g_bus_get_sync(G_BUS_TYPE_SYSTEM, NULL, &error);
    if (error) {
        RLOGD("can't get connection, reason: %s", error->message);
        g_error_free(error);
        goto done;
    }
    error = NULL;
    GVariant *tmp = g_variant_new("(@(iiiisssssssi))", value);
    RLOGD("emit signal");
    g_dbus_connection_emit_signal(connection,
                                    NULL,
                                    TELEPHONY_DATA_PATH,
                                    TELEPHONY_DATA_INTERFACE,
                                    signal_name,
                                    tmp,
                                    &error);
    if (error) {
        RLOGD("can't emit %s signal, reason: %s", rsp->apnType, error->message);
        g_error_free(error);
    }
done:
    g_object_unref(connection);
    connection = NULL;
}

void sdk_onUnsolicitedResponse(int slotId, int unsolResponse, const void *data, size_t datalen)
{
    if (datalen == 0)
    {
        //RLOGD("datalen value is invalid(0)");
        return;
    }

    if (unsolResponse == RIL_UNSOL_SETUP_PDN_STATE_CHANGED
            || unsolResponse == RIL_UNSOL_TEAR_DOWN_PDN_STATE_CHANGED
            || unsolResponse == RIL_UNSOL_LOCAL_UPDATE_PDN_INFO_CHANGED)
    {
        RLOGD("unsolResponse %s: ", sdk_requestToString(unsolResponse));
        if(data != NULL) {
            MTK_Data_Call_Response_v1 rsp;
            parseDataCallResponse(&rsp, data, datalen);
            emitTelephonySignal(&rsp);
            g_free(rsp.apnType);
            g_free(rsp.type);
            g_free(rsp.ifname);
            g_free(rsp.addresses);
            g_free(rsp.dnses);
            g_free(rsp.gateways);
            g_free(rsp.pcscf);

        } else {
            RLOGW("data is null");
        }
    } else {
        //RLOGW("don't support this URC");
    }
}
#endif /*MTK_RILPROXY_TELEPHONY_SDK_RFX_DATA_SERVICE_H_*/


