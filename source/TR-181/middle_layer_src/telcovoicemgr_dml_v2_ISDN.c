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

        BOOL TelcoVoiceMgrDml_ISDN_BRIList_IsUpdated(ANSC_HANDLE hInsContext);

    description:

        This function is checking whether the table is updated or not.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE or FALSE.

**********************************************************************/

BOOL TelcoVoiceMgrDml_ISDN_BRIList_IsUpdated(ANSC_HANDLE hInsContext)
{
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG TelcoVoiceMgrDml_ISDN_BRIList_Synchronize(ANSC_HANDLE hInsContext);

    description:

        This function is called to synchronize the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/

ULONG TelcoVoiceMgrDml_ISDN_BRIList_Synchronize(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG TelcoVoiceMgrDml_ISDN_BRIList_GetEntryCount(ANSC_HANDLE hInsContext);

    description:

        This function is called to retrieve the count of the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The count of the table

**********************************************************************/
ULONG TelcoVoiceMgrDml_ISDN_BRIList_GetEntryCount(ANSC_HANDLE hInsContext)
{
    ULONG ret = 0;

    TELCOVOICEMGR_LOCK_OR_EXIT()

    DML_VOICE_SERVICE_CTRL_T* pVoiceService = (DML_VOICE_SERVICE_CTRL_T*) hInsContext;

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = &(pVoiceService->dml);

    PDML_ISDN_BRI_LIST_T pIsdnBriList = &(pDmlVoiceService->ISDN_obj.BRI);

    if(pIsdnBriList != NULL)
    {
        ret = pIsdnBriList->ulQuantity;
    }

    TELCOVOICEMGR_UNLOCK()

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE TelcoVoiceMgrDml_ISDN_BRIList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

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
ANSC_HANDLE TelcoVoiceMgrDml_ISDN_BRIList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber)
{
    ANSC_HANDLE ret = NULL;

    TELCOVOICEMGR_LOCK_OR_EXIT()

    DML_VOICE_SERVICE_CTRL_T* pVoiceService = (DML_VOICE_SERVICE_CTRL_T*) hInsContext;

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = &(pVoiceService->dml);

    PDML_ISDN_BRI_LIST_T pBRIList = &(pDmlVoiceService->ISDN_obj.BRI);

    if(pBRIList != NULL)
    {
        if(nIndex < pBRIList->ulQuantity)
        {
            PDML_ISDN_BRI_CTRL_T pBRICtrl = pBRIList->pdata[nIndex];
            if(pBRICtrl != NULL)
            {
               *pInsNumber = nIndex + 1;
               ret = (ANSC_HANDLE) pBRICtrl;
            }
        }
    }

    TELCOVOICEMGR_UNLOCK()

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_ISDN_BRIList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

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

BOOL TelcoVoiceMgrDml_ISDN_BRIList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uISDNBriIndex = 0;
    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;


    if(ParamName == NULL || puLong == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_ISDN_BRI_CTRL_T pIsdnBriCtrl = (PDML_ISDN_BRI_CTRL_T)hInsContext;

    PDML_ISDN_BRI pHEAD = &(pIsdnBriCtrl->dml);

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

    uISDNBriIndex = pHEAD->uInstanceNumber;

    if (strcmp(ParamName, "TEINegotiation") == 0)
    {
        *puLong = pHEAD->TEINegotiation;
        ret = TRUE;
    }
    else if (strcmp(ParamName, "Status") == 0)
    {
        //Fetch status from voice stack
        hal_param_t req_param;
        memset(&req_param, 0, sizeof(req_param));
        snprintf(req_param.name, sizeof(req_param.name), DML_VOICESERVICE_ISDN_BRI_PARAM_NAME"%s", uVsIndex, uISDNBriIndex, "Status");
        if (ANSC_STATUS_SUCCESS == TelcoVoiceHal_GetSingleParameter(&req_param))
        {
            *puLong = strtoul(req_param.value,NULL,10);
            ret = TRUE;
        }
        else
        {
            CcspTraceError(("%s:%d:: Status:get failed \n", __FUNCTION__, __LINE__));
            *puLong = STATUS_DISABLED;
            ret = FALSE;
        }
    }
    else if (strcmp(ParamName, "StaticTEI") == 0)
    {
        *puLong = pHEAD->StaticTEI;
        ret = TRUE;
    }
    else if (strcmp(ParamName, "ProtocolEmulation") == 0)
    {
        *puLong = pHEAD->ProtocolEmulation;
        ret = TRUE;
    }
    else if (strcmp(ParamName, "Protocol") == 0)
    {
        *puLong = pHEAD->Protocol;
        ret = TRUE;
    }
    else if (strcmp(ParamName, "MaxNumBChannels") == 0)
    {
        *puLong = pHEAD->MaxNumBChannels;
        ret = TRUE;
    }
    else if (strcmp(ParamName, "ApplicationInterface") == 0)
    {
        *puLong = pHEAD->ApplicationInterface;
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

        BOOL TelcoVoiceMgrDml_ISDN_BRIList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

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

BOOL TelcoVoiceMgrDml_ISDN_BRIList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uBriIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_ISDN_BRI_CTRL_T pIsdnBriCtrl = (PDML_ISDN_BRI_CTRL_T)hInsContext;

    PDML_ISDN_BRI pHEAD = &(pIsdnBriCtrl->dml);

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

    uBriIndex = pHEAD->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if (strcmp(ParamName, "TEINegotiation") == 0)
    {
        char enumValue[][STR_LEN_16]= {"Static","Dynamic"};

        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.ISDN.BRI.%d.TEINegotiation",uVsIndex,uBriIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,enumValue[uValue]) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->TEINegotiation = uValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "StaticTEI") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.ISDN.BRI.%d.StaticTEI",uVsIndex,uBriIndex);

        if (TelcoVoiceMgrHal_SetParamULong(HalName,uValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->StaticTEI = uValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "ProtocolEmulation") == 0)
    {
        char enumValue[][STR_LEN_16]= {"TE","NT"};

        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.ISDN.BRI.%d.ProtocolEmulation",uVsIndex,uBriIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,enumValue[uValue]) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->ProtocolEmulation = uValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "Protocol") == 0)
    {
        char enumValue[][STR_LEN_16]= {"EuroISDN","NI-1","NI-2","5ESS","NTT","VN3","VN4","Q.SIG"};

        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.ISDN.BRI.%d.Protocol",uVsIndex,uBriIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,enumValue[uValue]) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->Protocol = uValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "MaxNumBChannels") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.ISDN.BRI.%d.MaxNumBChannels",uVsIndex,uBriIndex);

        if (TelcoVoiceMgrHal_SetParamULong(HalName,uValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->MaxNumBChannels = uValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "ApplicationInterface") == 0)
    {
        char enumValue[][STR_LEN_16]= {"VoIP","VoATM","Data"};

        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.ISDN.BRI.%d.ApplicationInterface",uVsIndex,uBriIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,enumValue[uValue]) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->ApplicationInterface = uValue;

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

        ULONG TelcoVoiceMgrDml_ISDN_BRIList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

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

ULONG TelcoVoiceMgrDml_ISDN_BRIList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize)
{
    ULONG ret = 1;

    if(ParamName == NULL || pValue == NULL || pulSize == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_ISDN_BRI_CTRL_T pIsdnBriCtrl = (PDML_ISDN_BRI_CTRL_T)hInsContext;

    PDML_ISDN_BRI pHEAD = &(pIsdnBriCtrl->dml);

    if (strcmp(ParamName, "ToneEventProfile") == 0)
    {
        AnscCopyString(pValue,pHEAD->ToneEventProfile);
        ret = 0;
    }
    else if (strcmp(ParamName, "OutboundOnlyBChannels") == 0)
    {
        AnscCopyString(pValue,pHEAD->OutboundOnlyBChannels);
        ret = 0;
    }
    else if (strcmp(ParamName, "Name") == 0)
    {
        AnscCopyString(pValue,pHEAD->Name);
        ret = 0;
    }
    else if (strcmp(ParamName, "InboundOnlyBChannels") == 0)
    {
        AnscCopyString(pValue,pHEAD->InboundOnlyBChannels);
        ret = 0;
    }
    else if (strcmp(ParamName, "BidirectionalBChannels") == 0)
    {
        AnscCopyString(pValue,pHEAD->BidirectionalBChannels);
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

        BOOL TelcoVoiceMgrDml_ISDN_BRIList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

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

BOOL TelcoVoiceMgrDml_ISDN_BRIList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uBriIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL || pString == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_ISDN_BRI_CTRL_T pIsdnBriCtrl = (PDML_ISDN_BRI_CTRL_T)hInsContext;

    PDML_ISDN_BRI pHEAD = &(pIsdnBriCtrl->dml);

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

    uBriIndex = pHEAD->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if (strcmp(ParamName, "ToneEventProfile") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.ISDN.BRI.%d.ToneEventProfile",uVsIndex,uBriIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,pString) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            AnscCopyString(pHEAD->ToneEventProfile, pString);

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "OutboundOnlyBChannels") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.ISDN.BRI.%d.OutboundOnlyBChannels",uVsIndex,uBriIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,pString) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            AnscCopyString(pHEAD->OutboundOnlyBChannels, pString);

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "InboundOnlyBChannels") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.ISDN.BRI.%d.InboundOnlyBChannels",uVsIndex,uBriIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,pString) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            AnscCopyString(pHEAD->InboundOnlyBChannels, pString);

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "BidirectionalBChannels") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.ISDN.BRI.%d.InboundOnlyBChannels",uVsIndex,uBriIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,pString) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            AnscCopyString(pHEAD->InboundOnlyBChannels, pString);

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "Alias") == 0)
    {
        TELCOVOICEMGR_LOCK_OR_EXIT()

        AnscCopyString(pHEAD->Alias, pString);

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

        BOOL TelcoVoiceMgrDml_ISDN_BRIList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

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

BOOL TelcoVoiceMgrDml_ISDN_BRIList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || pBool == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_ISDN_BRI_CTRL_T pIsdnBriCtrl = (PDML_ISDN_BRI_CTRL_T)hInsContext;

    PDML_ISDN_BRI pHEAD = &(pIsdnBriCtrl->dml);

    if (strcmp(ParamName, "PowerSourceOne") == 0)
    {
        *pBool = pHEAD->PowerSourceOne;
        ret = TRUE;
    }
    else if (strcmp(ParamName, "PermanentLayer2") == 0)
    {
        *pBool = pHEAD->PermanentLayer2;
        ret = TRUE;
    }
    else if (strcmp(ParamName, "PermanentLayer1") == 0)
    {
        *pBool = pHEAD->PermanentLayer1;
        ret = TRUE;
    }
    else if (strcmp(ParamName, "LifeLineHold") == 0)
    {
        *pBool = pHEAD->LifeLineHold;
        ret = TRUE;
    }
    else if (strcmp(ParamName, "LapdDisconnectionTimeout") == 0)
    {
        *pBool = pHEAD->LapdDisconnectionTimeout;
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

        BOOL TelcoVoiceMgrDml_ISDN_BRIList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

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

BOOL TelcoVoiceMgrDml_ISDN_BRIList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uBriIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_ISDN_BRI_CTRL_T pIsdnBriCtrl = (PDML_ISDN_BRI_CTRL_T)hInsContext;

    PDML_ISDN_BRI pHEAD = &(pIsdnBriCtrl->dml);

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

    uBriIndex = pHEAD->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if (strcmp(ParamName, "PowerSourceOne") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.ISDN.BRI.%d.PowerSourceOne",uVsIndex,uBriIndex);

        if (TelcoVoiceMgrHal_SetParamBool(HalName,bValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->PowerSourceOne = bValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "PermanentLayer2") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.ISDN.BRI.%d.PermanentLayer2",uVsIndex,uBriIndex);

        if (TelcoVoiceMgrHal_SetParamBool(HalName,bValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->PermanentLayer2 = bValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "PermanentLayer1") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.ISDN.BRI.%d.PermanentLayer1",uVsIndex,uBriIndex);

        if (TelcoVoiceMgrHal_SetParamBool(HalName,bValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->PermanentLayer1 = bValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "LifeLineHold") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.ISDN.BRI.%d.LifeLineHold",uVsIndex,uBriIndex);

        if (TelcoVoiceMgrHal_SetParamBool(HalName,bValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->LifeLineHold = bValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "LapdDisconnectionTimeout") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.ISDN.BRI.%d.LapdDisconnectionTimeout",uVsIndex,uBriIndex);

        if (TelcoVoiceMgrHal_SetParamBool(HalName,bValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->LapdDisconnectionTimeout = bValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "Enable") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.ISDN.BRI.%d.Enable",uVsIndex,uBriIndex);

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

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_ISDN_BRIList_Validate(ANSC_HANDLE hInsContext);

    description:

        This function is called to validate the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_ISDN_BRIList_Validate(ANSC_HANDLE hInsContext)
{
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_ISDN_BRIList_Commit(ANSC_HANDLE hInsContext);

    description:

        This function is called to commit the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_ISDN_BRIList_Commit(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_ISDN_BRIList_Rollback(ANSC_HANDLE hInsContext);

    description:

        This function is called to rollback the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_ISDN_BRIList_Rollback(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_ISDN_PRIList_IsUpdated(ANSC_HANDLE hInsContext);

    description:

        This function is checking whether the table is updated or not.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE or FALSE.

**********************************************************************/

BOOL TelcoVoiceMgrDml_ISDN_PRIList_IsUpdated(ANSC_HANDLE hInsContext)
{
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG TelcoVoiceMgrDml_ISDN_PRIList_Synchronize(ANSC_HANDLE hInsContext);

    description:

        This function is called to synchronize the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/

ULONG TelcoVoiceMgrDml_ISDN_PRIList_Synchronize(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG TelcoVoiceMgrDml_ISDN_PRIList_GetEntryCount(ANSC_HANDLE hInsContext);

    description:

        This function is called to retrieve the count of the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The count of the table

**********************************************************************/
ULONG TelcoVoiceMgrDml_ISDN_PRIList_GetEntryCount(ANSC_HANDLE hInsContext)
{
    ULONG ret = 0;

    TELCOVOICEMGR_LOCK_OR_EXIT()

    DML_VOICE_SERVICE_CTRL_T* pVoiceService = (DML_VOICE_SERVICE_CTRL_T*) hInsContext;

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = &(pVoiceService->dml);

    PDML_ISDN_PRI_LIST_T pIsdnPriList = &(pDmlVoiceService->ISDN_obj.PRI);

    if(pIsdnPriList != NULL)
    {
        ret = pIsdnPriList->ulQuantity;
    }

    TELCOVOICEMGR_UNLOCK()

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE TelcoVoiceMgrDml_ISDN_PRIList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

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
ANSC_HANDLE TelcoVoiceMgrDml_ISDN_PRIList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber)
{
    ANSC_HANDLE ret = NULL;

    TELCOVOICEMGR_LOCK_OR_EXIT()

    DML_VOICE_SERVICE_CTRL_T* pVoiceService = (DML_VOICE_SERVICE_CTRL_T*) hInsContext;

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = &(pVoiceService->dml);

    PDML_ISDN_BRI_LIST_T pPRIList = &(pDmlVoiceService->ISDN_obj.PRI);

    if(pPRIList != NULL)
    {
        if(nIndex < pPRIList->ulQuantity)
        {
            PDML_ISDN_PRI_CTRL_T pPRICtrl = pPRIList->pdata[nIndex];
            if(pPRICtrl != NULL)
            {
               *pInsNumber = nIndex + 1;
               ret = (ANSC_HANDLE) pPRICtrl;
            }
        }
    }

    TELCOVOICEMGR_UNLOCK()

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_ISDN_PRIList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

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

BOOL TelcoVoiceMgrDml_ISDN_PRIList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uISDNPriIndex = 0;
    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;


    if(ParamName == NULL || puLong == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_ISDN_PRI_CTRL_T pIsdnPriCtrl = (PDML_ISDN_PRI_CTRL_T)hInsContext;

    PDML_ISDN_PRI pHEAD = &(pIsdnPriCtrl->dml);

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

    uISDNPriIndex = pHEAD->uInstanceNumber;

    if (strcmp(ParamName, "Status") == 0)
    {
        //Fetch status from voice stack
        hal_param_t req_param;
        memset(&req_param, 0, sizeof(req_param));
        snprintf(req_param.name, sizeof(req_param.name), DML_VOICESERVICE_ISDN_PRI_PARAM_NAME"%s", uVsIndex, uISDNPriIndex, "Status");
        if (ANSC_STATUS_SUCCESS == TelcoVoiceHal_GetSingleParameter(&req_param))
        {
            *puLong = strtoul(req_param.value,NULL,10);
            ret = TRUE;
        }
        else
        {
            CcspTraceError(("%s:%d:: Status:get failed \n", __FUNCTION__, __LINE__));
            *puLong = STATUS_DISABLED;
            ret = FALSE;
        }
    }
    else if (strcmp(ParamName, "ProtocolEmulation") == 0)
    {
        *puLong = pHEAD->ProtocolEmulation;
        ret = TRUE;
    }
    else if (strcmp(ParamName, "Protocol") == 0)
    {
        *puLong = pHEAD->Protocol;
        ret = TRUE;
    }
    else if (strcmp(ParamName, "PhysicalInterfaceType") == 0)
    {
        *puLong = pHEAD->PhysicalInterfaceType;
        ret = TRUE;
    }
    else if (strcmp(ParamName, "MaxNumBChannels") == 0)
    {
        *puLong = pHEAD->MaxNumBChannels;
        ret = TRUE;
    }
    else if (strcmp(ParamName, "Linecode") == 0)
    {
        *puLong = pHEAD->Linecode;
        ret = TRUE;
    }
    else if (strcmp(ParamName, "ESF") == 0)
    {
        *puLong = pHEAD->ESF;
        ret = TRUE;
    }
    else if (strcmp(ParamName, "ClockMode") == 0)
    {
        *puLong = pHEAD->ClockMode;
        ret = TRUE;
    }
    else if (strcmp(ParamName, "ApplicationInterface") == 0)
    {
        *puLong = pHEAD->ApplicationInterface;
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

        BOOL TelcoVoiceMgrDml_ISDN_PRIList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

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

BOOL TelcoVoiceMgrDml_ISDN_PRIList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uPriIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_ISDN_PRI_CTRL_T pIsdnPriCtrl = (PDML_ISDN_PRI_CTRL_T)hInsContext;

    PDML_ISDN_PRI pHEAD = &(pIsdnPriCtrl->dml);

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

    uPriIndex = pHEAD->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if (strcmp(ParamName, "ProtocolEmulation") == 0)
    {
        char enumValue[][STR_LEN_16]= {"TE","NT"};

        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.ISDN.PRI.%d.ProtocolEmulation",uVsIndex,uPriIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,enumValue[uValue]) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->ProtocolEmulation = uValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "Protocol") == 0)
    {
        char enumValue[][STR_LEN_16]= {"EuroISDN","NI-1","NI-2","4ESS","5ESS","NTT","Q.SIG"};

        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.ISDN.PRI.%d.Protocol",uVsIndex,uPriIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,enumValue[uValue]) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->Protocol = uValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "PhysicalInterfaceType") == 0)
    {
        char enumValue[][STR_LEN_16]= {"T1","E1"};

        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.ISDN.PRI.%d.PhysicalInterfaceType",uVsIndex,uPriIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,enumValue[uValue]) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->PhysicalInterfaceType = uValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "MaxNumBChannels") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.ISDN.PRI.%d.MaxNumBChannels",uVsIndex,uPriIndex);

        if (TelcoVoiceMgrHal_SetParamULong(HalName,uValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->MaxNumBChannels = uValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "Linecode") == 0)
    {
        char enumValue[][STR_LEN_16]= {"ami","hdb3","b8zs"};

        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.ISDN.PRI.%d.Linecode",uVsIndex,uPriIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,enumValue[uValue]) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->Linecode = uValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "ESF") == 0)
    {
        char enumValue[][STR_LEN_16]= {"None","DF","MF","EMF","SF","ESF","AutoDetect"};

        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.ISDN.PRI.%d.ESF",uVsIndex,uPriIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,enumValue[uValue]) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->ESF = uValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "ClockMode") == 0)
    {
        char enumValue[][STR_LEN_16]= {"Master","Slave"};

        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.ISDN.PRI.%d.ClockMode",uVsIndex,uPriIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,enumValue[uValue]) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->ClockMode = uValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "ApplicationInterface") == 0)
    {
        char enumValue[][STR_LEN_16]= {"VoIP","VoATM","Data"};

        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.ISDN.PRI.%d.ApplicationInterface",uVsIndex,uPriIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,enumValue[uValue]) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->ApplicationInterface = uValue;

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

        ULONG TelcoVoiceMgrDml_ISDN_PRIList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

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

ULONG TelcoVoiceMgrDml_ISDN_PRIList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize)
{
    ULONG ret = 1;

    if(ParamName == NULL || pValue == NULL || pulSize == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_ISDN_PRI_CTRL_T pIsdnPriCtrl = (PDML_ISDN_PRI_CTRL_T)hInsContext;

    PDML_ISDN_PRI pHEAD = &(pIsdnPriCtrl->dml);

    if (strcmp(ParamName, "ToneEventProfile") == 0)
    {
        AnscCopyString(pValue,pHEAD->ToneEventProfile);
        ret = 0;
    }
    else if (strcmp(ParamName, "OutboundOnlyBChannels") == 0)
    {
        AnscCopyString(pValue,pHEAD->OutboundOnlyBChannels);
        ret = 0;
    }
    else if (strcmp(ParamName, "Name") == 0)
    {
        AnscCopyString(pValue,pHEAD->Name);
        ret = 0;
    }
    else if (strcmp(ParamName, "InboundOnlyBChannels") == 0)
    {
        AnscCopyString(pValue,pHEAD->InboundOnlyBChannels);
        ret = 0;
    }
    else if (strcmp(ParamName, "BidirectionalBChannels") == 0)
    {
        AnscCopyString(pValue,pHEAD->BidirectionalBChannels);
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

        BOOL TelcoVoiceMgrDml_ISDN_PRIList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

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

BOOL TelcoVoiceMgrDml_ISDN_PRIList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uPriIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL || pString == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_ISDN_PRI_CTRL_T pIsdnPriCtrl = (PDML_ISDN_PRI_CTRL_T)hInsContext;

    PDML_ISDN_PRI pHEAD = &(pIsdnPriCtrl->dml);

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

    uPriIndex = pHEAD->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if (strcmp(ParamName, "ToneEventProfile") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.ISDN.PRI.%d.ToneEventProfile",uVsIndex,uPriIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,pString) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            AnscCopyString(pHEAD->ToneEventProfile,pString);

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "OutboundOnlyBChannels") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.ISDN.PRI.%d.OutboundOnlyBChannels",uVsIndex,uPriIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,pString) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            AnscCopyString(pHEAD->OutboundOnlyBChannels,pString);

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "InboundOnlyBChannels") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.ISDN.PRI.%d.InboundOnlyBChannels",uVsIndex,uPriIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,pString) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            AnscCopyString(pHEAD->InboundOnlyBChannels,pString);

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "BidirectionalBChannels") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.ISDN.PRI.%d.BidirectionalBChannels",uVsIndex,uPriIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,pString) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            AnscCopyString(pHEAD->BidirectionalBChannels,pString);

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

        BOOL TelcoVoiceMgrDml_ISDN_PRIList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

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

BOOL TelcoVoiceMgrDml_ISDN_PRIList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || pBool == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_ISDN_PRI_CTRL_T pIsdnPriCtrl = (PDML_ISDN_PRI_CTRL_T)hInsContext;

    PDML_ISDN_PRI pHEAD = &(pIsdnPriCtrl->dml);

    if (strcmp(ParamName, "PermanentLayer2") == 0)
    {
        *pBool = pHEAD->PermanentLayer2;
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

        BOOL TelcoVoiceMgrDml_ISDN_PRIList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

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

BOOL TelcoVoiceMgrDml_ISDN_PRIList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uPriIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_ISDN_PRI_CTRL_T pIsdnPriCtrl = (PDML_ISDN_PRI_CTRL_T)hInsContext;

    PDML_ISDN_PRI pHEAD = &(pIsdnPriCtrl->dml);

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

    uPriIndex = pHEAD->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if (strcmp(ParamName, "PermanentLayer2") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.ISDN.PRI.%d.PermanentLayer2",uVsIndex,uPriIndex);

        if (TelcoVoiceMgrHal_SetParamBool(HalName,bValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->PermanentLayer2 = bValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "Enable") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.ISDN.PRI.%d.Enable",uVsIndex,uPriIndex);

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

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_ISDN_PRIList_Validate(ANSC_HANDLE hInsContext);

    description:

        This function is called to validate the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_ISDN_PRIList_Validate(ANSC_HANDLE hInsContext)
{
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_ISDN_PRIList_Commit(ANSC_HANDLE hInsContext);

    description:

        This function is called to commit the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_ISDN_PRIList_Commit(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_ISDN_PRIList_Rollback(ANSC_HANDLE hInsContext);

    description:

        This function is called to rollback the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_ISDN_PRIList_Rollback(ANSC_HANDLE hInsContext)
{
    return 0;
}
