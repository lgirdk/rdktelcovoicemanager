/*
 * If not stated otherwise in this file or this component's LICENSE file the
 * following copyright and licenses apply:
 *
 * Copyright 2020 Sky
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "ansc_platform.h"
#include "telcovoicemgr_dml_backendmgr.h"
#include "telcovoicemgr_dml_v2.h"
#include "ccsp_trace.h"
#include "ccsp_syslog.h"
#include "telcovoicemgr_dml_hal_param_v2.h"

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_MGCP_ClientList_IsUpdated(ANSC_HANDLE hInsContext);

    description:

        This function is checking whether the table is updated or not.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE or FALSE.

**********************************************************************/

BOOL TelcoVoiceMgrDml_MGCP_ClientList_IsUpdated(ANSC_HANDLE hInsContext)
{
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG TelcoVoiceMgrDml_MGCP_ClientList_Synchronize(ANSC_HANDLE hInsContext);

    description:

        This function is called to synchronize the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/

ULONG TelcoVoiceMgrDml_MGCP_ClientList_Synchronize(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG TelcoVoiceMgrDml_MGCP_ClientList_GetEntryCount(ANSC_HANDLE hInsContext);

    description:

        This function is called to retrieve the count of the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The count of the table

**********************************************************************/
ULONG TelcoVoiceMgrDml_MGCP_ClientList_GetEntryCount(ANSC_HANDLE hInsContext)
{
    ULONG ret = 0;

    TELCOVOICEMGR_LOCK_OR_EXIT()

    DML_VOICE_SERVICE_CTRL_T* pVoiceService = (DML_VOICE_SERVICE_CTRL_T*) hInsContext;

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = &(pVoiceService->dml);

    PDML_MGCP_CLIENT_LIST_T pMGCPClientList = &(pDmlVoiceService->MGCP_obj.Client);

    if(pMGCPClientList != NULL)
    {
        ret = pMGCPClientList->ulQuantity;
    }
    TELCOVOICEMGR_UNLOCK()

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE TelcoVoiceMgrDml_MGCP_ClientList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

    description:

        This function is called to retrieve the entry specified by the index.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                ULONG                       nIndex,
                The index of this entry;

                ULONG*                      pInsNumber
                The output instance number;

    return:     The handle to identify the entry

**********************************************************************/
ANSC_HANDLE TelcoVoiceMgrDml_MGCP_ClientList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber)
{

    ANSC_HANDLE ret = NULL;

    TELCOVOICEMGR_LOCK_OR_EXIT()

    DML_VOICE_SERVICE_CTRL_T* pVoiceService = (DML_VOICE_SERVICE_CTRL_T*) hInsContext;

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = &(pVoiceService->dml);

    PDML_MGCP_CLIENT_LIST_T pMgcpClientList = &(pDmlVoiceService->MGCP_obj.Client);

    if(pMgcpClientList != NULL)
    {
        if(nIndex < pMgcpClientList->ulQuantity)
        {
            PDML_MGCP_CLIENT_CTRL_T pMgcpClientCtrl = pMgcpClientList->pdata[nIndex];
            if(pMgcpClientCtrl != NULL)
            {
               *pInsNumber = nIndex + 1;
               ret = (ANSC_HANDLE) pMgcpClientCtrl;
            }
        }
    }

    TELCOVOICEMGR_UNLOCK()

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_MGCP_ClientList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

    description:

        This function is called to retrieve ULONG parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG*                      puLong
                The buffer of returned ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/

BOOL TelcoVoiceMgrDml_MGCP_ClientList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uMGCPClientIndex = 0;
    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL || puLong == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_MGCP_CLIENT_CTRL_T pMGCPClientCtrl = (PDML_MGCP_CLIENT_CTRL_T)hInsContext;

    PDML_MGCP_CLIENT pHEAD = &(pMGCPClientCtrl->dml);

    if(pHEAD != NULL)
    {
        pDmlVoiceService = (PTELCOVOICEMGR_DML_VOICESERVICE)pHEAD->pParentVoiceService;
    }

    if (pHEAD == NULL  || pDmlVoiceService == NULL)
    {
        TELCOVOICEMGR_UNLOCK()

        CcspTraceError(("%s:%d:: pHEAD or pDmlVoiceService NULL\n", __FUNCTION__, __LINE__));

        return ret;
    }

    uVsIndex = pDmlVoiceService->InstanceNumber;

    uMGCPClientIndex = pHEAD->uInstanceNumber;

    if (strcmp(ParamName, "Status") == 0)
    {
        //Fetch status from voice stack
        hal_param_t req_param;
        memset(&req_param, 0, sizeof(req_param));
        snprintf(req_param.name, sizeof(req_param.name), DML_VOICESERVICE_MGCP_CLIENT_PARAM_NAME"%s", uVsIndex, uMGCPClientIndex, "Status");
        if (ANSC_STATUS_SUCCESS == TelcoVoiceHal_GetSingleParameter(&req_param))
        {
            *puLong = strtoul(req_param.value,NULL,10);
            ret = TRUE;
        }
        else
        {
            CcspTraceError(("%s:%d:: Status:get failed \n", __FUNCTION__, __LINE__));
            ret = FALSE;
        }
    }
    else if (strcmp(ParamName, "RegisterMode") == 0)
    {
        *puLong = pHEAD->RegisterMode;
        ret = TRUE;
    }
    else if (strcmp(ParamName, "MaxSessions") == 0)
    {
        *puLong = pHEAD->MaxSessions;
        ret = TRUE;
    }
    else if (strcmp(ParamName, "LocalPort") == 0)
    {
        *puLong = pHEAD->LocalPort;
        ret = TRUE;
    }
    else
    {
        CcspTraceWarning(("%s: Unsupported parameter '%s'\n", __func__,ParamName));
    }


    TELCOVOICEMGR_UNLOCK()

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_MGCP_ClientList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

    description:

        This function is called to set ULONG parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG                       uValue
                The updated ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/

BOOL TelcoVoiceMgrDml_MGCP_ClientList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uClientIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_MGCP_CLIENT_CTRL_T pMGCPClientCtrl = (PDML_MGCP_CLIENT_CTRL_T)hInsContext;

    PDML_MGCP_CLIENT pHEAD = &(pMGCPClientCtrl->dml);

    if(pHEAD != NULL)
    {
        pDmlVoiceService = (PTELCOVOICEMGR_DML_VOICESERVICE)pHEAD->pParentVoiceService;
    }

    if (pHEAD == NULL  || pDmlVoiceService == NULL)
    {
        TELCOVOICEMGR_UNLOCK()

        CcspTraceError(("%s:%d:: pHEAD or pDmlVoiceService NULL\n", __FUNCTION__, __LINE__));

        return ret;
    }

    uVsIndex = pDmlVoiceService->InstanceNumber;

    uClientIndex = pHEAD->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if (strcmp(ParamName, "RegisterMode") == 0)
    {
        char enumValue[][STR_LEN_32]={"Wildcard","Individual"};

        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.MGCP.Client.%d.RegisterMode",uVsIndex,uClientIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,enumValue[uValue]) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->RegisterMode = uValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "MaxSessions") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.MGCP.Client.%d.MaxSessions",uVsIndex,uClientIndex);

        if (TelcoVoiceMgrHal_SetParamULong(HalName,uValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->MaxSessions = uValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "LocalPort") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.MGCP.Client.%d.LocalPort",uVsIndex,uClientIndex);

        if (TelcoVoiceMgrHal_SetParamULong(HalName,uValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->LocalPort = uValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else
    {
        CcspTraceWarning(("%s: Unsupported parameter '%s'\n", __func__,ParamName));
    }

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG TelcoVoiceMgrDml_MGCP_ClientList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

    description:

        This function is called to retrieve string parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pValue,
                The string value buffer;

                ULONG*                      pUlSize
                The buffer of length of string value;
                Usually size of 1023 will be used.
                If it's not big enough, put required size here and return 1;

    return:     0 if succeeded;
                1 if short of buffer size; (*pUlSize = required size)
                -1 if not supported.

**********************************************************************/

ULONG TelcoVoiceMgrDml_MGCP_ClientList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize)
{
    ULONG ret = 1;

    if(ParamName == NULL || pValue == NULL || pulSize == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_MGCP_CLIENT_CTRL_T pMGCPClientCtrl = (PDML_MGCP_CLIENT_CTRL_T)hInsContext;

    PDML_MGCP_CLIENT pHEAD = &(pMGCPClientCtrl->dml);

    if (strcmp(ParamName, "User") == 0)
    {
        AnscCopyString(pValue,pHEAD->User);
        ret = 0;
    }
    else if (strcmp(ParamName, "Network") == 0)
    {
        AnscCopyString(pValue,pHEAD->Network);
        ret = 0;
    }
    else if (strcmp(ParamName, "Domain") == 0)
    {
        AnscCopyString(pValue,pHEAD->Domain);
        ret = 0;
    }
    else if (strcmp(ParamName, "Alias") == 0)
    {
        AnscCopyString(pValue,pHEAD->Alias);
        ret = 0;
    }
    else
    {
        CcspTraceWarning(("%s: Unsupported parameter '%s'\n", __func__,ParamName));
    }


    TELCOVOICEMGR_UNLOCK()

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_MGCP_ClientList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

    description:

        This function is called to set string parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pString
                The updated string value;

    return:     TRUE if succeeded.

**********************************************************************/

BOOL TelcoVoiceMgrDml_MGCP_ClientList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uClientIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_MGCP_CLIENT_CTRL_T pMGCPClientCtrl = (PDML_MGCP_CLIENT_CTRL_T)hInsContext;

    PDML_MGCP_CLIENT pHEAD = &(pMGCPClientCtrl->dml);

    if(pHEAD != NULL)
    {
        pDmlVoiceService = (PTELCOVOICEMGR_DML_VOICESERVICE)pHEAD->pParentVoiceService;
    }

    if (pHEAD == NULL  || pDmlVoiceService == NULL)
    {
        TELCOVOICEMGR_UNLOCK()

        CcspTraceError(("%s:%d:: pHEAD or pDmlVoiceService NULL\n", __FUNCTION__, __LINE__));

        return ret;
    }

    uVsIndex = pDmlVoiceService->InstanceNumber;

    uClientIndex = pHEAD->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if (strcmp(ParamName, "User") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.MGCP.Client.%d.User",uVsIndex,uClientIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,pString) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            AnscCopyString(pHEAD->User,pString);

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "Network") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.MGCP.Client.%d.Network",uVsIndex,uClientIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,pString) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            AnscCopyString(pHEAD->Network,pString);

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "Domain") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.MGCP.Client.%d.Domain",uVsIndex,uClientIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,pString) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            AnscCopyString(pHEAD->Domain,pString);

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "Alias") == 0)
    {
        TELCOVOICEMGR_LOCK_OR_EXIT()

        AnscCopyString(pHEAD->Alias,pString);

        TELCOVOICEMGR_UNLOCK()

        ret = TRUE;
    }
    else
    {
        CcspTraceWarning(("%s: Unsupported parameter '%s'\n", __func__,ParamName));
    }

    return ret;

}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_MGCP_ClientList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

    description:

        This function is called to retrieve Boolean parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                BOOL*                       pBool
                The buffer of returned boolean value;

    return:     TRUE if succeeded.

**********************************************************************/

BOOL TelcoVoiceMgrDml_MGCP_ClientList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || pBool == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_MGCP_CLIENT_CTRL_T pMGCPClientCtrl = (PDML_MGCP_CLIENT_CTRL_T)hInsContext;

    PDML_MGCP_CLIENT pHEAD = &(pMGCPClientCtrl->dml);

    if (strcmp(ParamName, "QuiescentMode") == 0)
    {
        *pBool = pHEAD->QuiescentMode;
        ret = TRUE;
    }
    else if (strcmp(ParamName, "Enable") == 0)
    {
        *pBool = pHEAD->Enable;
        ret = TRUE;
    }
    else
    {
        CcspTraceWarning(("%s: Unsupported parameter '%s'\n", __func__,ParamName));
    }

    TELCOVOICEMGR_UNLOCK()

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_MGCP_ClientList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

    description:

        This function is called to set BOOL parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                BOOL                        bValue
                The updated BOOL value;

    return:     TRUE if succeeded.

**********************************************************************/

BOOL TelcoVoiceMgrDml_MGCP_ClientList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uClientIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_MGCP_CLIENT_CTRL_T pMGCPClientCtrl = (PDML_MGCP_CLIENT_CTRL_T)hInsContext;

    PDML_MGCP_CLIENT pHEAD = &(pMGCPClientCtrl->dml);

    if(pHEAD != NULL)
    {
        pDmlVoiceService = (PTELCOVOICEMGR_DML_VOICESERVICE)pHEAD->pParentVoiceService;
    }

    if (pHEAD == NULL  || pDmlVoiceService == NULL)
    {
        TELCOVOICEMGR_UNLOCK()

        CcspTraceError(("%s:%d:: pHEAD or pDmlVoiceService NULL\n", __FUNCTION__, __LINE__));

        return ret;
    }

    uVsIndex = pDmlVoiceService->InstanceNumber;

    uClientIndex = pHEAD->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if (strcmp(ParamName, "QuiescentMode") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.MGCP.Client.%d.QuiescentMode",uVsIndex,uClientIndex);

        if (TelcoVoiceMgrHal_SetParamBool(HalName,bValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->QuiescentMode = bValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "Enable") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.MGCP.Client.%d.Enable",uVsIndex,uClientIndex);

        if (TelcoVoiceMgrHal_SetParamBool(HalName,bValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->Enable = bValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else
    {
        CcspTraceWarning(("%s: Unsupported parameter '%s'\n", __func__,ParamName));
    }

    TELCOVOICEMGR_UNLOCK()

    return ret;

}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_MGCP_ClientList_Validate(ANSC_HANDLE hInsContext);

    description:

        This function is called to validate the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_MGCP_ClientList_Validate(ANSC_HANDLE hInsContext)
{
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_MGCP_ClientList_Commit(ANSC_HANDLE hInsContext);

    description:

        This function is called to commit the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_MGCP_ClientList_Commit(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_MGCP_ClientList_Rollback(ANSC_HANDLE hInsContext);

    description:

        This function is called to rollback the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_MGCP_ClientList_Rollback(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_MGCP_NetworkList_IsUpdated(ANSC_HANDLE hInsContext);

    description:

        This function is checking whether the table is updated or not.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE or FALSE.

**********************************************************************/

BOOL TelcoVoiceMgrDml_MGCP_NetworkList_IsUpdated(ANSC_HANDLE hInsContext)
{
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG TelcoVoiceMgrDml_MGCP_NetworkList_Synchronize(ANSC_HANDLE hInsContext);

    description:

        This function is called to synchronize the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/

ULONG TelcoVoiceMgrDml_MGCP_NetworkList_Synchronize(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG TelcoVoiceMgrDml_MGCP_NetworkList_GetEntryCount(ANSC_HANDLE hInsContext);

    description:

        This function is called to retrieve the count of the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The count of the table

**********************************************************************/
ULONG TelcoVoiceMgrDml_MGCP_NetworkList_GetEntryCount(ANSC_HANDLE hInsContext)
{
    ULONG ret = 0;

    TELCOVOICEMGR_LOCK_OR_EXIT()

    DML_VOICE_SERVICE_CTRL_T* pVoiceService = (DML_VOICE_SERVICE_CTRL_T*) hInsContext;

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = &(pVoiceService->dml);

    PDML_MGCP_NETWORK_LIST_T pMGCPNetworkList = &(pDmlVoiceService->MGCP_obj.Network);

    if(pMGCPNetworkList != NULL)
    {
        ret = pMGCPNetworkList->ulQuantity;
    }

    TELCOVOICEMGR_UNLOCK()

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE TelcoVoiceMgrDml_MGCP_NetworkList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

    description:

        This function is called to retrieve the entry specified by the index.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                ULONG                       nIndex,
                The index of this entry;

                ULONG*                      pInsNumber
                The output instance number;

    return:     The handle to identify the entry

**********************************************************************/
ANSC_HANDLE TelcoVoiceMgrDml_MGCP_NetworkList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber)
{

    ANSC_HANDLE ret = NULL;

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_VOICE_SERVICE_CTRL_T pVoiceService = (PDML_VOICE_SERVICE_CTRL_T) hInsContext;

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = &(pVoiceService->dml);

    PDML_MGCP_NETWORK_LIST_T pMgcpNetworkList = &(pDmlVoiceService->MGCP_obj.Network);

    if(pMgcpNetworkList != NULL)
    {
        if(nIndex < pMgcpNetworkList->ulQuantity)
        {
            PDML_MGCP_NETWORK_CTRL_T pMgcpNetworCtrl = pMgcpNetworkList->pdata[nIndex];
            if(pMgcpNetworCtrl != NULL)
            {
               *pInsNumber = nIndex + 1;
               ret = (ANSC_HANDLE) pMgcpNetworCtrl;
            }
        }
    }

    TELCOVOICEMGR_UNLOCK()

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_MGCP_NetworkList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

    description:

        This function is called to retrieve ULONG parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG*                      puLong
                The buffer of returned ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/

BOOL TelcoVoiceMgrDml_MGCP_NetworkList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uMGCPNetworkIndex = 0;
    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL || puLong == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_MGCP_NETWORK_CTRL_T pMGCPNetworkCtrl = (PDML_MGCP_NETWORK_CTRL_T)hInsContext;

    PDML_MGCP_NETWORK pHEAD = &(pMGCPNetworkCtrl->dml);

    if(pHEAD != NULL)
    {
        pDmlVoiceService = (PTELCOVOICEMGR_DML_VOICESERVICE)pHEAD->pParentVoiceService;
    }

    if (pHEAD == NULL  || pDmlVoiceService == NULL)
    {
        TELCOVOICEMGR_UNLOCK()

        CcspTraceError(("%s:%d:: pHEAD or pDmlVoiceService NULL\n", __FUNCTION__, __LINE__));

        return ret;
    }

    uVsIndex = pDmlVoiceService->InstanceNumber;

    uMGCPNetworkIndex = pHEAD->uInstanceNumber;

    if (strcmp(ParamName, "Status") == 0)
    {
        //Fetch status from voice stack
        hal_param_t req_param;
        memset(&req_param, 0, sizeof(req_param));
        snprintf(req_param.name, sizeof(req_param.name), DML_VOICESERVICE_MGCP_NETWORK_PARAM_NAME"%s", uVsIndex, uMGCPNetworkIndex, "Status");
        if (ANSC_STATUS_SUCCESS == TelcoVoiceHal_GetSingleParameter(&req_param))
        {
            *puLong = strtoul(req_param.value,NULL,10);
            ret = TRUE;
        }
        else
        {
            CcspTraceError(("%s:%d:: Status:get failed \n", __FUNCTION__, __LINE__));
            ret = FALSE;
        }
    }
    else if (strcmp(ParamName, "RetranIntervalTimer") == 0)
    {
        *puLong = pHEAD->RetranIntervalTimer;
        ret = TRUE;
    }
    else if (strcmp(ParamName, "NonVoiceBandwidthReservedUpstream") == 0)
    {
        *puLong = pHEAD->NonVoiceBandwidthReservedUpstream;
        ret = TRUE;
    }
    else if (strcmp(ParamName, "NonVoiceBandwidthReservedDownstream") == 0)
    {
        *puLong = pHEAD->NonVoiceBandwidthReservedDownstream;
        ret = TRUE;
    }
    else if (strcmp(ParamName, "MaxSessions") == 0)
    {
        *puLong = pHEAD->MaxSessions;
        ret = TRUE;
    }
    else if (strcmp(ParamName, "MaxRetranCount") == 0)
    {
        *puLong = pHEAD->MaxRetranCount;
        ret = TRUE;
    }
    else if (strcmp(ParamName, "DSCPMark") == 0)
    {
        *puLong = pHEAD->DSCPMark;
        ret = TRUE;
    }
    else if (strcmp(ParamName, "CallAgentPort2") == 0)
    {
        *puLong = pHEAD->CallAgentPort2;
        ret = TRUE;
    }
    else if (strcmp(ParamName, "CallAgentPort1") == 0)
    {
        *puLong = pHEAD->CallAgentPort1;
        ret = TRUE;
    }
    else
    {
        CcspTraceWarning(("%s: Unsupported parameter '%s'\n", __func__,ParamName));
    }

    TELCOVOICEMGR_UNLOCK()

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_MGCP_NetworkList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

    description:

        This function is called to set ULONG parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                ULONG                       uValue
                The updated ULONG value;

    return:     TRUE if succeeded.

**********************************************************************/

BOOL TelcoVoiceMgrDml_MGCP_NetworkList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uNetworkIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_MGCP_NETWORK_CTRL_T pMGCPNetworkCtrl = (PDML_MGCP_NETWORK_CTRL_T)hInsContext;

    PDML_MGCP_NETWORK pHEAD = &(pMGCPNetworkCtrl->dml);

    if(pHEAD != NULL)
    {
        pDmlVoiceService = (PTELCOVOICEMGR_DML_VOICESERVICE)pHEAD->pParentVoiceService;
    }

    if (pHEAD == NULL  || pDmlVoiceService == NULL)
    {
        TELCOVOICEMGR_UNLOCK()

        CcspTraceError(("%s:%d:: pHEAD or pDmlVoiceService NULL\n", __FUNCTION__, __LINE__));

        return ret;
    }

    uVsIndex = pDmlVoiceService->InstanceNumber;

    uNetworkIndex = pHEAD->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if (strcmp(ParamName, "RetranIntervalTimer") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.MGCP.Network.%d.RetranIntervalTimer",uVsIndex,uNetworkIndex);

        if (TelcoVoiceMgrHal_SetParamULong(HalName,uValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->RetranIntervalTimer = uValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "NonVoiceBandwidthReservedUpstream") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.MGCP.Network.%d.NonVoiceBandwidthReservedUpstream",uVsIndex,uNetworkIndex);

        if (TelcoVoiceMgrHal_SetParamULong(HalName,uValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->NonVoiceBandwidthReservedUpstream = uValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "NonVoiceBandwidthReservedDownstream") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.MGCP.Network.%d.NonVoiceBandwidthReservedDownstream",uVsIndex,uNetworkIndex);

        if (TelcoVoiceMgrHal_SetParamULong(HalName,uValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->NonVoiceBandwidthReservedDownstream = uValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "MaxSessions") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.MGCP.Network.%d.MaxSessions",uVsIndex,uNetworkIndex);

        if (TelcoVoiceMgrHal_SetParamULong(HalName,uValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->MaxSessions = uValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "MaxRetranCount") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.MGCP.Network.%d.MaxRetranCount",uVsIndex,uNetworkIndex);

        if (TelcoVoiceMgrHal_SetParamULong(HalName,uValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->MaxRetranCount = uValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "DSCPMark") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.MGCP.Network.%d.DSCPMark",uVsIndex,uNetworkIndex);

        if (TelcoVoiceMgrHal_SetParamULong(HalName,uValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->DSCPMark = uValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "CallAgentPort2") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.MGCP.Network.%d.CallAgentPort2",uVsIndex,uNetworkIndex);

        if (TelcoVoiceMgrHal_SetParamULong(HalName,uValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->CallAgentPort2 = uValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "CallAgentPort1") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.MGCP.Network.%d.CallAgentPort1",uVsIndex,uNetworkIndex);

        if (TelcoVoiceMgrHal_SetParamULong(HalName,uValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->CallAgentPort1 = uValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else
    {
        CcspTraceWarning(("%s: Unsupported parameter '%s'\n", __func__,ParamName));
    }

    return ret;

}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG TelcoVoiceMgrDml_MGCP_NetworkList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

    description:

        This function is called to retrieve string parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pValue,
                The string value buffer;

                ULONG*                      pUlSize
                The buffer of length of string value;
                Usually size of 1023 will be used.
                If it's not big enough, put required size here and return 1;

    return:     0 if succeeded;
                1 if short of buffer size; (*pUlSize = required size)
                -1 if not supported.

**********************************************************************/

ULONG TelcoVoiceMgrDml_MGCP_NetworkList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize)
{
    ULONG ret = 1;

    if(ParamName == NULL || pValue == NULL || pulSize == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_MGCP_NETWORK_CTRL_T pMGCPNetworkCtrl = (PDML_MGCP_NETWORK_CTRL_T)hInsContext;

    PDML_MGCP_NETWORK pHEAD = &(pMGCPNetworkCtrl->dml);

    if (strcmp(ParamName, "VoIPProfile") == 0)
    {
        AnscCopyString(pValue,pHEAD->VoIPProfile);
        ret = 0;
    }
    else if (strcmp(ParamName, "STUNServer") == 0)
    {
        AnscCopyString(pValue,pHEAD->STUNServer);
        ret = 0;
    }
    else if (strcmp(ParamName, "CodecList") == 0)
    {
        AnscCopyString(pValue,pHEAD->CodecList);
        ret = 0;
    }
    else if (strcmp(ParamName, "CallAgent2") == 0)
    {
        AnscCopyString(pValue,pHEAD->CallAgent2);
        ret = 0;
    }
    else if (strcmp(ParamName, "CallAgent1") == 0)
    {
        AnscCopyString(pValue,pHEAD->CallAgent1);
        ret = 0;
    }
    else if (strcmp(ParamName, "Alias") == 0)
    {
        AnscCopyString(pValue,pHEAD->Alias);
        ret = 0;
    }
    else
    {
        CcspTraceWarning(("%s: Unsupported parameter '%s'\n", __func__,ParamName));
    }

    TELCOVOICEMGR_UNLOCK()

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_MGCP_NetworkList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

    description:

        This function is called to set string parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                char*                       pString
                The updated string value;

    return:     TRUE if succeeded.

**********************************************************************/

BOOL TelcoVoiceMgrDml_MGCP_NetworkList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uNetworkIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_MGCP_NETWORK_CTRL_T pMGCPNetworkCtrl = (PDML_MGCP_NETWORK_CTRL_T)hInsContext;

    PDML_MGCP_NETWORK pHEAD = &(pMGCPNetworkCtrl->dml);

    if(pHEAD != NULL)
    {
        pDmlVoiceService = (PTELCOVOICEMGR_DML_VOICESERVICE)pHEAD->pParentVoiceService;
    }

    if (pHEAD == NULL  || pDmlVoiceService == NULL)
    {
        TELCOVOICEMGR_UNLOCK()

        CcspTraceError(("%s:%d:: pHEAD or pDmlVoiceService NULL\n", __FUNCTION__, __LINE__));

        return ret;
    }

    uVsIndex = pDmlVoiceService->InstanceNumber;

    uNetworkIndex = pHEAD->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if (strcmp(ParamName, "VoIPProfile") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.MGCP.Network.%d.VoIPProfile",uVsIndex,uNetworkIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,pString) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            AnscCopyString(pHEAD->VoIPProfile,pString);

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "STUNServer") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.MGCP.Network.%d.STUNServer",uVsIndex,uNetworkIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,pString) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            AnscCopyString(pHEAD->STUNServer,pString);

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "CodecList") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.MGCP.Network.%d.CodecList",uVsIndex,uNetworkIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,pString) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            AnscCopyString(pHEAD->CodecList,pString);

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "CallAgent2") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.MGCP.Network.%d.CallAgent2",uVsIndex,uNetworkIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,pString) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            AnscCopyString(pHEAD->CallAgent2,pString);

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "CallAgent1") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.MGCP.Network.%d.CallAgent1",uVsIndex,uNetworkIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,pString) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            AnscCopyString(pHEAD->CallAgent1,pString);

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "Alias") == 0)
    {
        TELCOVOICEMGR_LOCK_OR_EXIT()

        AnscCopyString(pHEAD->Alias,pString);

        TELCOVOICEMGR_UNLOCK()

        ret = TRUE;
    }
    else
    {
        CcspTraceWarning(("%s: Unsupported parameter '%s'\n", __func__,ParamName));
    }

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_MGCP_NetworkList_GetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int* pInt);

    description:

        This function is called to retrieve integer parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                int*                        pInt
                The buffer of returned integer value;

    return:     TRUE if succeeded.

**********************************************************************/

BOOL TelcoVoiceMgrDml_MGCP_NetworkList_GetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int* pInt)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || pInt == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_MGCP_NETWORK_CTRL_T pMGCPNetworkCtrl = (PDML_MGCP_NETWORK_CTRL_T)hInsContext;

    PDML_MGCP_NETWORK pHEAD = &(pMGCPNetworkCtrl->dml);

    if (strcmp(ParamName, "VLANIDMark") == 0)
    {
        *pInt = pHEAD->VLANIDMark;
        ret = TRUE;
    }
    else if (strcmp(ParamName, "EthernetPriorityMark") == 0)
    {
        *pInt = pHEAD->EthernetPriorityMark;
        ret = TRUE;
    }
    else
    {
        CcspTraceWarning(("%s: Unsupported parameter '%s'\n", __func__,ParamName));
    }

    TELCOVOICEMGR_UNLOCK()

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_MGCP_NetworkList_SetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int iValue);

    description:

        This function is called to set integer parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                int                         iValue
                The updated integer value;

    return:     TRUE if succeeded.

**********************************************************************/

BOOL TelcoVoiceMgrDml_MGCP_NetworkList_SetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int iValue)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uNetworkIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_MGCP_NETWORK_CTRL_T pMGCPNetworkCtrl = (PDML_MGCP_NETWORK_CTRL_T)hInsContext;

    PDML_MGCP_NETWORK pHEAD = &(pMGCPNetworkCtrl->dml);

    if(pHEAD != NULL)
    {
        pDmlVoiceService = (PTELCOVOICEMGR_DML_VOICESERVICE)pHEAD->pParentVoiceService;
    }

    if (pHEAD == NULL  || pDmlVoiceService == NULL)
    {
        TELCOVOICEMGR_UNLOCK()

        CcspTraceError(("%s:%d:: pHEAD or pDmlVoiceService NULL\n", __FUNCTION__, __LINE__));

        return ret;
    }

    uVsIndex = pDmlVoiceService->InstanceNumber;

    uNetworkIndex = pHEAD->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if (strcmp(ParamName, "VLANIDMark") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.MGCP.Network.%d.VLANIDMark",uVsIndex,uNetworkIndex);

        if (TelcoVoiceMgrHal_SetParamInt(HalName,iValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->VLANIDMark = iValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "EthernetPriorityMark") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.MGCP.Network.%d.EthernetPriorityMark",uVsIndex,uNetworkIndex);

        if (TelcoVoiceMgrHal_SetParamInt(HalName,iValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->EthernetPriorityMark = iValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else
    {
        CcspTraceWarning(("%s: Unsupported parameter '%s'\n", __func__,ParamName));
    }

    return ret;

}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_MGCP_NetworkList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

    description:

        This function is called to retrieve Boolean parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                BOOL*                       pBool
                The buffer of returned boolean value;

    return:     TRUE if succeeded.

**********************************************************************/

BOOL TelcoVoiceMgrDml_MGCP_NetworkList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || pBool == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_MGCP_NETWORK_CTRL_T pMGCPNetworkCtrl = (PDML_MGCP_NETWORK_CTRL_T)hInsContext;

    PDML_MGCP_NETWORK pHEAD = &(pMGCPNetworkCtrl->dml);

    if (strcmp(ParamName, "STUNEnable") == 0)
    {
        *pBool = pHEAD->STUNEnable;
        ret = TRUE;
    }
    else if (strcmp(ParamName, "SendRSIPImmediately") == 0)
    {
        *pBool = pHEAD->SendRSIPImmediately;
        ret = TRUE;
    }
    else if (strcmp(ParamName, "QuiescentMode") == 0)
    {
        *pBool = pHEAD->QuiescentMode;
        ret = TRUE;
    }
    else if (strcmp(ParamName, "Enable") == 0)
    {
        *pBool = pHEAD->Enable;
        ret = TRUE;
    }
    else if (strcmp(ParamName, "AllowPiggybackEvents") == 0)
    {
        *pBool = pHEAD->AllowPiggybackEvents;
        ret = TRUE;
    }
    else
    {
        CcspTraceWarning(("%s: Unsupported parameter '%s'\n", __func__,ParamName));
    }


    TELCOVOICEMGR_UNLOCK()

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_MGCP_NetworkList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

    description:

        This function is called to set BOOL parameter value;

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

                char*                       ParamName,
                The parameter name;

                BOOL                        bValue
                The updated BOOL value;

    return:     TRUE if succeeded.

**********************************************************************/

BOOL TelcoVoiceMgrDml_MGCP_NetworkList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uNetworkIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_MGCP_NETWORK_CTRL_T pMGCPNetworkCtrl = (PDML_MGCP_NETWORK_CTRL_T)hInsContext;

    PDML_MGCP_NETWORK pHEAD = &(pMGCPNetworkCtrl->dml);

    if(pHEAD != NULL)
    {
        pDmlVoiceService = (PTELCOVOICEMGR_DML_VOICESERVICE)pHEAD->pParentVoiceService;
    }

    if (pHEAD == NULL  || pDmlVoiceService == NULL)
    {
        TELCOVOICEMGR_UNLOCK()

        CcspTraceError(("%s:%d:: pHEAD or pDmlVoiceService NULL\n", __FUNCTION__, __LINE__));

        return ret;
    }

    uVsIndex = pDmlVoiceService->InstanceNumber;

    uNetworkIndex = pHEAD->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if (strcmp(ParamName, "STUNEnable") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.MGCP.Network.%d.STUNEnable",uVsIndex,uNetworkIndex);

        if (TelcoVoiceMgrHal_SetParamBool(HalName,bValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->STUNEnable = bValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "SendRSIPImmediately") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.MGCP.Network.%d.SendRSIPImmediately",uVsIndex,uNetworkIndex);

        if (TelcoVoiceMgrHal_SetParamBool(HalName,bValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->SendRSIPImmediately = bValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "QuiescentMode") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.MGCP.Network.%d.QuiescentMode",uVsIndex,uNetworkIndex);

        if (TelcoVoiceMgrHal_SetParamBool(HalName,bValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->QuiescentMode = bValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "Enable") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.MGCP.Network.%d.Enable",uVsIndex,uNetworkIndex);

        if (TelcoVoiceMgrHal_SetParamBool(HalName,bValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->Enable = bValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "AllowPiggybackEvents") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.MGCP.Network.%d.AllowPiggybackEvents",uVsIndex,uNetworkIndex);

        if (TelcoVoiceMgrHal_SetParamBool(HalName,bValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->AllowPiggybackEvents = bValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else
    {
        CcspTraceWarning(("%s: Unsupported parameter '%s'\n", __func__,ParamName));
    }

    return ret;

}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_MGCP_NetworkList_Validate(ANSC_HANDLE hInsContext);

    description:

        This function is called to validate the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_MGCP_NetworkList_Validate(ANSC_HANDLE hInsContext)
{
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_MGCP_NetworkList_Commit(ANSC_HANDLE hInsContext);

    description:

        This function is called to commit the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_MGCP_NetworkList_Commit(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_MGCP_NetworkList_Rollback(ANSC_HANDLE hInsContext);

    description:

        This function is called to rollback the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_MGCP_NetworkList_Rollback(ANSC_HANDLE hInsContext)
{
    return 0;
}
