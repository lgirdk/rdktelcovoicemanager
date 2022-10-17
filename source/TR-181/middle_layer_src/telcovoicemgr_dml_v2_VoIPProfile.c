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

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_VoipProfileList_IsUpdated(ANSC_HANDLE hInsContext);

    description:

        This function is checking whether the table is updated or not.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE or FALSE.

**********************************************************************/

BOOL TelcoVoiceMgrDml_VoipProfileList_IsUpdated(ANSC_HANDLE hInsContext)
{
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG TelcoVoiceMgrDml_VoipProfileList_Synchronize(ANSC_HANDLE hInsContext);

    description:

        This function is called to synchronize the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The status of the operation.

**********************************************************************/

ULONG TelcoVoiceMgrDml_VoipProfileList_Synchronize(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ULONG TelcoVoiceMgrDml_VoipProfileList_GetEntryCount(ANSC_HANDLE hInsContext);

    description:

        This function is called to retrieve the count of the table.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     The count of the table

**********************************************************************/
ULONG TelcoVoiceMgrDml_VoipProfileList_GetEntryCount(ANSC_HANDLE hInsContext)
{
    ULONG ret = 0;

    TELCOVOICEMGR_LOCK_OR_EXIT()

    DML_VOICE_SERVICE_CTRL_T* pVoiceService = (DML_VOICE_SERVICE_CTRL_T*) hInsContext;

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = &(pVoiceService->dml);

    PDML_VOIPPROFILE_LIST_T pVoipProfileList = pDmlVoiceService->VoIPProfile;

    if(pVoipProfileList != NULL)
    {
        ret = pVoipProfileList->ulQuantity;
    }
    TELCOVOICEMGR_UNLOCK()

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        ANSC_HANDLE TelcoVoiceMgrDml_VoipProfileList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber);

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
ANSC_HANDLE TelcoVoiceMgrDml_VoipProfileList_GetEntry(ANSC_HANDLE hInsContext, ULONG nIndex, ULONG* pInsNumber)
{
    ANSC_HANDLE ret = NULL;

    TELCOVOICEMGR_LOCK_OR_EXIT()

    DML_VOICE_SERVICE_CTRL_T* pVoiceService = (DML_VOICE_SERVICE_CTRL_T*) hInsContext;

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = &(pVoiceService->dml);

    PDML_VOIPPROFILE_LIST_T pVoipProfileList = pDmlVoiceService->VoIPProfile;

    if(pVoipProfileList != NULL)
    {
        if(nIndex < pVoipProfileList->ulQuantity)
        {
            PDML_VOIPPROFILE_CTRL_T pVoipProfileCtrl = pVoipProfileList->pdata[nIndex];
            if(pVoipProfileCtrl != NULL)
            {
               *pInsNumber = nIndex + 1;
               ret = (ANSC_HANDLE) pVoipProfileCtrl;
            }
        }
    }

    TELCOVOICEMGR_UNLOCK()

    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_VoipProfileList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

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

BOOL TelcoVoiceMgrDml_VoipProfileList_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || puLong == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_VOIPPROFILE_CTRL_T pVoipProfileCtrl = (PDML_VOIPPROFILE_CTRL_T)hInsContext;

    PDML_VOIPPROFILE pHEAD = &(pVoipProfileCtrl->dml);

    if (strcmp(ParamName, "X_RDK-Central_COM_ZDigitTimer") == 0)
    {
        *puLong = pHEAD->X_RDK_Central_COM_ZDigitTimer;
        ret = TRUE;
    }
    else if (strcmp(ParamName, "X_RDK-Central_COM_SDigitTimer") == 0)
    {
        *puLong = pHEAD->X_RDK_Central_COM_SDigitTimer;
        ret = TRUE;
    }
    else if (strcmp(ParamName, "QICalculationInterval") == 0)
    {
        *puLong = pHEAD->QICalculationInterval;
        ret = TRUE;
    }
    else if (strcmp(ParamName, "NumberOfWorstQIValues") == 0)
    {
        *puLong = pHEAD->NumberOfWorstQIValues;
        ret = TRUE;
    }
    else if (strcmp(ParamName, "DTMFMethodG711") == 0)
    {
        *puLong = pHEAD->DTMFMethodG711;
        ret = TRUE;
    }
    else if (strcmp(ParamName, "DTMFMethod") == 0)
    {
        *puLong = pHEAD->DTMFMethod;
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

        BOOL TelcoVoiceMgrDml_VoipProfileList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

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

BOOL TelcoVoiceMgrDml_VoipProfileList_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uProfileIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_VOIPPROFILE_CTRL_T pVoipProfileCtrl = (PDML_VOIPPROFILE_CTRL_T)hInsContext;

    PDML_VOIPPROFILE pHEAD = &(pVoipProfileCtrl->dml);

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

    uProfileIndex = pHEAD->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if (strcmp(ParamName, "X_RDK-Central_COM_ZDigitTimer") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.VoIPProfile.%d.X_RDK-Central_COM_ZDigitTimer",uVsIndex,uProfileIndex);

        if (TelcoVoiceMgrHal_SetParamULong(HalName,uValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->X_RDK_Central_COM_ZDigitTimer = uValue;

            TELCOVOICEMGR_UNLOCK()

            (void)storeObjectInteger(uVsIndex, uProfileIndex, 1, 1, "ZDigitTimer", uValue);

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "X_RDK-Central_COM_SDigitTimer") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.VoIPProfile.%d.X_RDK-Central_COM_SDigitTimer",uVsIndex,uProfileIndex);

        if (TelcoVoiceMgrHal_SetParamULong(HalName,uValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->X_RDK_Central_COM_SDigitTimer = uValue;

            TELCOVOICEMGR_UNLOCK()

            (void)storeObjectInteger(uVsIndex, uProfileIndex, 1, 1, "SDigitTimer", uValue);

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "QICalculationInterval") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.VoIPProfile.%d.QICalculationInterval",uVsIndex,uProfileIndex);

        if (TelcoVoiceMgrHal_SetParamULong(HalName,uValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->QICalculationInterval = uValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "NumberOfWorstQIValues") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.VoIPProfile.%d.NumberOfWorstQIValues",uVsIndex,uProfileIndex);

        if (TelcoVoiceMgrHal_SetParamULong(HalName,uValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->NumberOfWorstQIValues = uValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "DTMFMethodG711") == 0)
    {
        char enumValue[][STR_LEN_32]={"InBand","RFC4733","SIPInfo",""};

        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.VoIPProfile.%d.DTMFMethodG711",uVsIndex,uProfileIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,enumValue[uValue]) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->DTMFMethodG711 = uValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "DTMFMethod") == 0)
    {
        char enumValue[][STR_LEN_32]={"InBand","RFC4733","SIPInfo"};

        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.VoIPProfile.%d.DTMFMethod",uVsIndex,uProfileIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,enumValue[uValue]) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->DTMFMethod = uValue;

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

        ULONG TelcoVoiceMgrDml_VoipProfileList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

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

ULONG TelcoVoiceMgrDml_VoipProfileList_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize)
{
    ULONG ret = 1;

    if(ParamName == NULL || pValue == NULL || pulSize == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_VOIPPROFILE_CTRL_T pVoipProfileCtrl = (PDML_VOIPPROFILE_CTRL_T)hInsContext;

    PDML_VOIPPROFILE pHEAD = &(pVoipProfileCtrl->dml);

    if (strcmp(ParamName, "X_RDK-Central_COM_EmergencyDigitMap") == 0)
    {
        if(strlen(pHEAD->X_RDK_Central_COM_EmergencyDigitMap) < *pulSize)
        {
          AnscCopyString(pValue,pHEAD->X_RDK_Central_COM_EmergencyDigitMap);
          ret = 0;
        }
        else
        {
          CcspTraceWarning(("%s: Buffer length insufficient, ParamName:%s\n", __FUNCTION__, ParamName));
          //X_RDK-Central_COM_EmergencyDigitMap suppors maximum 2048 characters
          // But by default, the DML library will allocate 1023 buffer size only
          // If we need more size just put the new size and return 1
          // If return value is 1 ,the library will allocate required buffer space for us if new size is specified
          *pulSize = MAXLENGTH_DIGITMAP;
          ret = 1;
        }
    }
    else if (strcmp(ParamName, "X_RDK-Central_COM_DigitMap") == 0)
    {
        if(strlen(pHEAD->X_RDK_Central_COM_DigitMap) < *pulSize)
        {
          AnscCopyString(pValue,pHEAD->X_RDK_Central_COM_DigitMap);
          ret = 0;
        }
        else
        {
          CcspTraceWarning(("%s: Buffer length insufficient, ParamName:%s\n", __FUNCTION__, ParamName));
          //X_RDK-Central_COM_DigitMap supports maximum 2048 characters
          // But by default,  the DML library will allocate 1023 buffer size only
          // If we need more size just put the new size and return 1
          // If return value is 1 ,the library will allocate required buffer space for us if new size is specified
          *pulSize = MAXLENGTH_DIGITMAP;
          ret = 1;
        }
    }
    else if (strcmp(ParamName, "QIModelUsed") == 0)
    {
        AnscCopyString(pValue,pHEAD->QIModelUsed);
        ret = 0;
    }
    else if (strcmp(ParamName, "Name") == 0)
    {
        AnscCopyString(pValue,pHEAD->Name);
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

        BOOL TelcoVoiceMgrDml_VoipProfileList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

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

BOOL TelcoVoiceMgrDml_VoipProfileList_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uProfileIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_VOIPPROFILE_CTRL_T pVoipProfileCtrl = (PDML_VOIPPROFILE_CTRL_T)hInsContext;

    PDML_VOIPPROFILE pHEAD = &(pVoipProfileCtrl->dml);

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

    uProfileIndex = pHEAD->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if (strcmp(ParamName, "X_RDK-Central_COM_EmergencyDigitMap") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.VoIPProfile.%d.X_RDK-Central_COM_EmergencyDigitMap",uVsIndex,uProfileIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,pString) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            AnscCopyString(pHEAD->X_RDK_Central_COM_EmergencyDigitMap,pString);

            TELCOVOICEMGR_UNLOCK()

            (void)storeObjectString(uVsIndex, uProfileIndex, 1,1, "EmDigitMap", pString);

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "X_RDK-Central_COM_DigitMap") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.VoIPProfile.%d.X_RDK-Central_COM_DigitMap",uVsIndex,uProfileIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,pString) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            AnscCopyString(pHEAD->X_RDK_Central_COM_DigitMap,pString);

            TELCOVOICEMGR_UNLOCK()

            (void)storeObjectString(uVsIndex, uProfileIndex, 1,1, "CustomDigitMap", pString);

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "QIModelUsed") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.VoIPProfile.%d.QIModelUsed",uVsIndex,uProfileIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,pString) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            AnscCopyString(pHEAD->QIModelUsed,pString);

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "Name") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.VoIPProfile.%d.Name",uVsIndex,uProfileIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,pString) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            AnscCopyString(pHEAD->Name,pString);

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

        BOOL TelcoVoiceMgrDml_VoipProfileList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

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

BOOL TelcoVoiceMgrDml_VoipProfileList_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || pBool == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_VOIPPROFILE_CTRL_T pVoipProfileCtrl = (PDML_VOIPPROFILE_CTRL_T)hInsContext;

    PDML_VOIPPROFILE pHEAD = &(pVoipProfileCtrl->dml);

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

        BOOL TelcoVoiceMgrDml_VoipProfileList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

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

BOOL TelcoVoiceMgrDml_VoipProfileList_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uProfileIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_VOIPPROFILE_CTRL_T pVoipProfileCtrl = (PDML_VOIPPROFILE_CTRL_T)hInsContext;

    PDML_VOIPPROFILE pHEAD = &(pVoipProfileCtrl->dml);

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

    uProfileIndex = pHEAD->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if (strcmp(ParamName, "QuiescentMode") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.VoIPProfile.%d.QuiescentMode",uVsIndex,uProfileIndex);

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
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.VoIPProfile.%d.Enable",uVsIndex,uProfileIndex);

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

        BOOL TelcoVoiceMgrDml_VoipProfileList_Validate(ANSC_HANDLE hInsContext);

    description:

        This function is called to validate the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     TRUE if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_VoipProfileList_Validate(ANSC_HANDLE hInsContext)
{
    return TRUE;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_VoipProfileList_Commit(ANSC_HANDLE hInsContext);

    description:

        This function is called to commit the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_VoipProfileList_Commit(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_VoipProfileList_Rollback(ANSC_HANDLE hInsContext);

    description:

        This function is called to rollback the update.

    argument:   ANSC_HANDLE                 hInsContext,
                The instance handle;

    return:     0 if there's no issue.

**********************************************************************/

BOOL TelcoVoiceMgrDml_VoipProfileList_Rollback(ANSC_HANDLE hInsContext)
{
    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_VoipProfileList_RTP_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

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

BOOL TelcoVoiceMgrDml_VoipProfileList_RTP_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || puLong == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_VOIPPROFILE_CTRL_T pVoipProfileCtrl = (PDML_VOIPPROFILE_CTRL_T)hInsContext;

    PDML_VOIPPROFILE pDmlVoipProfile = &(pVoipProfileCtrl->dml);

    PDML_VOIPPROFILE_RTP pHEAD = &(pDmlVoipProfile->RTP);

    if (strcmp(ParamName, "X_RDK_SKBMark") == 0)
    {
        *puLong = pHEAD->X_RDK_SKBMark;
        ret = TRUE;
    }
    else if (strcmp(ParamName, "TelephoneEventPayloadType") == 0)
    {
        *puLong = pHEAD->TelephoneEventPayloadType;
        ret = TRUE;
    }
    else if (strcmp(ParamName, "LocalPortMin") == 0)
    {
        *puLong = pHEAD->LocalPortMin;
        ret = TRUE;
    }
    else if (strcmp(ParamName, "LocalPortMax") == 0)
    {
        *puLong = pHEAD->LocalPortMax;
        ret = TRUE;
    }
    else if (strcmp(ParamName, "JitterBufferType") == 0)
    {
        *puLong = pHEAD->JitterBufferType;
        ret = TRUE;
    }
    else if (strcmp(ParamName, "JitterBufferMinSize") == 0)
    {
        *puLong = pHEAD->JitterBufferMinSize;
        ret = TRUE;
    }
    else if (strcmp(ParamName, "JitterBufferMaxSize") == 0)
    {
        *puLong = pHEAD->JitterBufferMaxSize;
        ret = TRUE;
    }
    else if (strcmp(ParamName, "JitterBufferActualSize") == 0)
    {
        *puLong = pHEAD->JitterBufferActualSize;
        ret = TRUE;
    }
    else if (strcmp(ParamName, "Gmin") == 0)
    {
        *puLong = pHEAD->Gmin;
        ret = TRUE;
    }
    else if (strcmp(ParamName, "DSCPMark") == 0)
    {
        *puLong = pHEAD->DSCPMark;
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

        BOOL TelcoVoiceMgrDml_VoipProfileList_RTP_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

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

BOOL TelcoVoiceMgrDml_VoipProfileList_RTP_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uProfileIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_VOIPPROFILE_CTRL_T pVoipProfileCtrl = (PDML_VOIPPROFILE_CTRL_T)hInsContext;

    PDML_VOIPPROFILE pDmlVoipProfile = &(pVoipProfileCtrl->dml);

    PDML_VOIPPROFILE_RTP pHEAD = &(pDmlVoipProfile->RTP);


    if(pHEAD != NULL)
    {
        pDmlVoiceService = (PTELCOVOICEMGR_DML_VOICESERVICE)pDmlVoipProfile->pParentVoiceService;
    }

    if (pHEAD == NULL  || pDmlVoiceService == NULL)
    {
        TELCOVOICEMGR_UNLOCK()

        CcspTraceError(("%s:%d:: pHEAD or pDmlVoiceService NULL\n", __FUNCTION__, __LINE__));

        return ret;
    }

    uVsIndex = pDmlVoiceService->InstanceNumber;

    uProfileIndex = pDmlVoipProfile->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if (strcmp(ParamName, "X_RDK_SKBMark") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.VoIPProfile.%d.RTP.X_RDK_SKBMark",uVsIndex,uProfileIndex);

        if (TelcoVoiceMgrHal_SetParamULong(HalName,uValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->X_RDK_SKBMark = uValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "TelephoneEventPayloadType") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.VoIPProfile.%d.RTP.TelephoneEventPayloadType",uVsIndex,uProfileIndex);

        if (TelcoVoiceMgrHal_SetParamULong(HalName,uValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->TelephoneEventPayloadType = uValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "LocalPortMin") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.VoIPProfile.%d.RTP.LocalPortMin",uVsIndex,uProfileIndex);

        if (TelcoVoiceMgrHal_SetParamULong(HalName,uValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->LocalPortMin = uValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "LocalPortMax") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.VoIPProfile.%d.RTP.LocalPortMax",uVsIndex,uProfileIndex);

        if (TelcoVoiceMgrHal_SetParamULong(HalName,uValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->LocalPortMax = uValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "JitterBufferType") == 0)
    {
        char enumValue[][STR_LEN_32]={"Static","Dynamic"};

        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.VoIPProfile.%d.RTP.JitterBufferType",uVsIndex,uProfileIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,enumValue[uValue]) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->JitterBufferType = uValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "JitterBufferMinSize") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.VoIPProfile.%d.RTP.JitterBufferMinSize",uVsIndex,uProfileIndex);

        if (TelcoVoiceMgrHal_SetParamULong(HalName,uValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->JitterBufferMinSize = uValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "JitterBufferMaxSize") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.VoIPProfile.%d.RTP.JitterBufferMaxSize",uVsIndex,uProfileIndex);

        if (TelcoVoiceMgrHal_SetParamULong(HalName,uValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->JitterBufferMaxSize = uValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "JitterBufferActualSize") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.VoIPProfile.%d.RTP.JitterBufferActualSize",uVsIndex,uProfileIndex);

        if (TelcoVoiceMgrHal_SetParamULong(HalName,uValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->JitterBufferActualSize = uValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "Gmin") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.VoIPProfile.%d.RTP.Gmin",uVsIndex,uProfileIndex);

        if (TelcoVoiceMgrHal_SetParamULong(HalName,uValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->Gmin = uValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "DSCPMark") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.VoIPProfile.%d.RTP.DSCPMark",uVsIndex,uProfileIndex);

        if (TelcoVoiceMgrHal_SetParamULong(HalName,uValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->DSCPMark = uValue;

            TELCOVOICEMGR_UNLOCK()

            (void)storeObjectInteger(uVsIndex, uProfileIndex, 1, 1, "DSCPMark", uValue);

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

        ULONG TelcoVoiceMgrDml_VoipProfileList_RTP_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

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

ULONG TelcoVoiceMgrDml_VoipProfileList_RTP_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize)
{
    ULONG ret = 0;
    return ret;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_VoipProfileList_RTP_GetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int* pInt);

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

BOOL TelcoVoiceMgrDml_VoipProfileList_RTP_GetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int* pInt)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || pInt == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_VOIPPROFILE_CTRL_T pVoipProfileCtrl = (PDML_VOIPPROFILE_CTRL_T)hInsContext;

    PDML_VOIPPROFILE pDmlVoipProfile = &(pVoipProfileCtrl->dml);

    PDML_VOIPPROFILE_RTP pHEAD = &(pDmlVoipProfile->RTP);

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

        BOOL TelcoVoiceMgrDml_VoipProfileList_RTP_SetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int iValue);

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

BOOL TelcoVoiceMgrDml_VoipProfileList_RTP_SetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int iValue)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uProfileIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_VOIPPROFILE_CTRL_T pVoipProfileCtrl = (PDML_VOIPPROFILE_CTRL_T)hInsContext;

    PDML_VOIPPROFILE pDmlVoipProfile = &(pVoipProfileCtrl->dml);

    PDML_VOIPPROFILE_RTP pHEAD = &(pDmlVoipProfile->RTP);


    if(pHEAD != NULL)
    {
        pDmlVoiceService = (PTELCOVOICEMGR_DML_VOICESERVICE)pDmlVoipProfile->pParentVoiceService;
    }

    if (pHEAD == NULL  || pDmlVoiceService == NULL)
    {
        TELCOVOICEMGR_UNLOCK()

        CcspTraceError(("%s:%d:: pHEAD or pDmlVoiceService NULL\n", __FUNCTION__, __LINE__));

        return ret;
    }

    uVsIndex = pDmlVoiceService->InstanceNumber;

    uProfileIndex = pDmlVoipProfile->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if (strcmp(ParamName, "VLANIDMark") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.VoIPProfile.%d.RTP.VLANIDMark",uVsIndex,uProfileIndex);

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
        int iEthValue = -1;
        if ((TelcoVoiceMgrDmlGetWanEthernetPriorityMark(RTP, &iEthValue) == ANSC_STATUS_SUCCESS) &&
                                                                               (iEthValue != iValue))
        {
            if (TelcoVoiceMgrDmlSetWanEthernetPriorityMark(RTP, iValue) != ANSC_STATUS_SUCCESS)
            {
                CcspTraceInfo(("%s %d: Update Wanmgr: iValue[%d] Failed\n", __func__, __LINE__,iValue));
                return ANSC_STATUS_DISCARD;
            }
        }
        TELCOVOICEMGR_LOCK_OR_EXIT()

        pHEAD->EthernetPriorityMark = iValue;

        TELCOVOICEMGR_UNLOCK()

        (void)storeObjectInteger(uVsIndex, uProfileIndex, 1, 1, "RtpEthernetPriorityMark", iValue);

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

        BOOL TelcoVoiceMgrDml_VoipProfileList_RTP_RTCP_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

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

BOOL TelcoVoiceMgrDml_VoipProfileList_RTP_RTCP_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || puLong == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_VOIPPROFILE_CTRL_T pVoipProfileCtrl = (PDML_VOIPPROFILE_CTRL_T)hInsContext;

    PDML_VOIPPROFILE pDmlVoipProfile = &(pVoipProfileCtrl->dml);

    PDML_VOIPPROFILE_RTP_RTCP pHEAD = &(pDmlVoipProfile->RTP.RTCP);

    if (strcmp(ParamName, "TxRepeatInterval") == 0)
    {
        *puLong = pHEAD->TxRepeatInterval;
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

        BOOL TelcoVoiceMgrDml_VoipProfileList_RTP_RTCP_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

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

BOOL TelcoVoiceMgrDml_VoipProfileList_RTP_RTCP_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uProfileIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_VOIPPROFILE_CTRL_T pVoipProfileCtrl = (PDML_VOIPPROFILE_CTRL_T)hInsContext;

    PDML_VOIPPROFILE pDmlVoipProfile = &(pVoipProfileCtrl->dml);

    PDML_VOIPPROFILE_RTP_RTCP pHEAD = &(pDmlVoipProfile->RTP.RTCP);


    if(pHEAD != NULL)
    {
        pDmlVoiceService = (PTELCOVOICEMGR_DML_VOICESERVICE)pDmlVoipProfile->pParentVoiceService;
    }

    if (pHEAD == NULL  || pDmlVoiceService == NULL)
    {
        TELCOVOICEMGR_UNLOCK()

        CcspTraceError(("%s:%d:: pHEAD or pDmlVoiceService NULL\n", __FUNCTION__, __LINE__));

        return ret;
    }

    uVsIndex = pDmlVoiceService->InstanceNumber;

    uProfileIndex = pDmlVoipProfile->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if (strcmp(ParamName, "TxRepeatInterval") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.VoIPProfile.%d.RTP.RTCP.TxRepeatInterval",uVsIndex,uProfileIndex);

        if (TelcoVoiceMgrHal_SetParamULong(HalName,uValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->TxRepeatInterval = uValue;

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

        ULONG TelcoVoiceMgrDml_VoipProfileList_RTP_RTCP_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

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

ULONG TelcoVoiceMgrDml_VoipProfileList_RTP_RTCP_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize)
{
    ULONG ret = 1;

    if(ParamName == NULL || pValue == NULL || pulSize == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_VOIPPROFILE_CTRL_T pVoipProfileCtrl = (PDML_VOIPPROFILE_CTRL_T)hInsContext;

    PDML_VOIPPROFILE pDmlVoipProfile = &(pVoipProfileCtrl->dml);

    PDML_VOIPPROFILE_RTP_RTCP pHEAD = &(pDmlVoipProfile->RTP.RTCP);

    if (strcmp(ParamName, "LocalCName") == 0)
    {
        AnscCopyString(pValue,pHEAD->LocalCName);
        ret = 0;
    }
    else
    {
        CcspTraceWarning(("%s: Unsupported parameter '%s'\n", __func__,ParamName));
    }
    TELCOVOICEMGR_UNLOCK()

    return 0;
}

/**********************************************************************

    caller:     owner of this object

    prototype:

        BOOL TelcoVoiceMgrDml_VoipProfileList_RTP_RTCP_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

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

BOOL TelcoVoiceMgrDml_VoipProfileList_RTP_RTCP_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uProfileIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_VOIPPROFILE_CTRL_T pVoipProfileCtrl = (PDML_VOIPPROFILE_CTRL_T)hInsContext;

    PDML_VOIPPROFILE pDmlVoipProfile = &(pVoipProfileCtrl->dml);

    PDML_VOIPPROFILE_RTP_RTCP pHEAD = &(pDmlVoipProfile->RTP.RTCP);


    if(pHEAD != NULL)
    {
        pDmlVoiceService = (PTELCOVOICEMGR_DML_VOICESERVICE)pDmlVoipProfile->pParentVoiceService;
    }

    if (pHEAD == NULL  || pDmlVoiceService == NULL)
    {
        TELCOVOICEMGR_UNLOCK()

        CcspTraceError(("%s:%d:: pHEAD or pDmlVoiceService NULL\n", __FUNCTION__, __LINE__));

        return ret;
    }

    uVsIndex = pDmlVoiceService->InstanceNumber;

    uProfileIndex = pDmlVoipProfile->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if (strcmp(ParamName, "LocalCName") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.VoIPProfile.%d.RTP.RTCP.LocalCName",uVsIndex,uProfileIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,pString) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            AnscCopyString(pHEAD->LocalCName,pString);

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

        BOOL TelcoVoiceMgrDml_VoipProfileList_RTP_RTCP_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

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

BOOL TelcoVoiceMgrDml_VoipProfileList_RTP_RTCP_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || pBool == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_VOIPPROFILE_CTRL_T pVoipProfileCtrl = (PDML_VOIPPROFILE_CTRL_T)hInsContext;

    PDML_VOIPPROFILE pDmlVoipProfile = &(pVoipProfileCtrl->dml);

    PDML_VOIPPROFILE_RTP_RTCP pHEAD = &(pDmlVoipProfile->RTP.RTCP);

    if (strcmp(ParamName, "Enable") == 0)
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

        BOOL TelcoVoiceMgrDml_VoipProfileList_RTP_RTCP_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

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

BOOL TelcoVoiceMgrDml_VoipProfileList_RTP_RTCP_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uProfileIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_VOIPPROFILE_CTRL_T pVoipProfileCtrl = (PDML_VOIPPROFILE_CTRL_T)hInsContext;

    PDML_VOIPPROFILE pDmlVoipProfile = &(pVoipProfileCtrl->dml);

    PDML_VOIPPROFILE_RTP_RTCP pHEAD = &(pDmlVoipProfile->RTP.RTCP);


    if(pHEAD != NULL)
    {
        pDmlVoiceService = (PTELCOVOICEMGR_DML_VOICESERVICE)pDmlVoipProfile->pParentVoiceService;
    }

    if (pHEAD == NULL  || pDmlVoiceService == NULL)
    {
        TELCOVOICEMGR_UNLOCK()

        CcspTraceError(("%s:%d:: pHEAD or pDmlVoiceService NULL\n", __FUNCTION__, __LINE__));

        return ret;
    }

    uVsIndex = pDmlVoiceService->InstanceNumber;

    uProfileIndex = pDmlVoipProfile->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if (strcmp(ParamName, "Enable") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.VoIPProfile.%d.RTP.RTCP.Enable",uVsIndex,uProfileIndex);

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

        BOOL TelcoVoiceMgrDml_VoipProfileList_RTP_SRTP_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

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

BOOL TelcoVoiceMgrDml_VoipProfileList_RTP_SRTP_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || puLong == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_VOIPPROFILE_CTRL_T pVoipProfileCtrl = (PDML_VOIPPROFILE_CTRL_T)hInsContext;

    PDML_VOIPPROFILE pDmlVoipProfile = &(pVoipProfileCtrl->dml);

    PDML_VOIPPROFILE_RTP_SRTP pHEAD = &(pDmlVoipProfile->RTP.SRTP);

    if (strcmp(ParamName, "KeyingMethods") == 0)
    {
        *puLong = pHEAD->KeyingMethods;
        ret =TRUE;
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

        BOOL TelcoVoiceMgrDml_VoipProfileList_RTP_SRTP_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

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

BOOL TelcoVoiceMgrDml_VoipProfileList_RTP_SRTP_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uProfileIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_VOIPPROFILE_CTRL_T pVoipProfileCtrl = (PDML_VOIPPROFILE_CTRL_T)hInsContext;

    PDML_VOIPPROFILE pDmlVoipProfile = &(pVoipProfileCtrl->dml);

    PDML_VOIPPROFILE_RTP_SRTP pHEAD = &(pDmlVoipProfile->RTP.SRTP);


    if(pHEAD != NULL)
    {
        pDmlVoiceService = (PTELCOVOICEMGR_DML_VOICESERVICE)pDmlVoipProfile->pParentVoiceService;
    }

    if (pHEAD == NULL  || pDmlVoiceService == NULL)
    {
        TELCOVOICEMGR_UNLOCK()

        CcspTraceError(("%s:%d:: pHEAD or pDmlVoiceService NULL\n", __FUNCTION__, __LINE__));

        return ret;
    }

    uVsIndex = pDmlVoiceService->InstanceNumber;

    uProfileIndex = pDmlVoipProfile->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if (strcmp(ParamName, "KeyingMethods") == 0)
    {
        char enumValue[][STR_LEN_32]={"Null","Static","SDP","IKE"};

        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.VoIPProfile.%d.RTP.SRTP.KeyingMethods",uVsIndex,uProfileIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,enumValue[uValue]) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->KeyingMethods = uValue;

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

        ULONG TelcoVoiceMgrDml_VoipProfileList_RTP_SRTP_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize);

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

ULONG TelcoVoiceMgrDml_VoipProfileList_RTP_SRTP_GetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pValue, ULONG* pulSize)
{
    ULONG ret = 1;

    if(ParamName == NULL || pValue == NULL || pulSize == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_VOIPPROFILE_CTRL_T pVoipProfileCtrl = (PDML_VOIPPROFILE_CTRL_T)hInsContext;

    PDML_VOIPPROFILE pDmlVoipProfile = &(pVoipProfileCtrl->dml);

    PDML_VOIPPROFILE_RTP_SRTP pHEAD = &(pDmlVoipProfile->RTP.SRTP);

    if (strcmp(ParamName, "EncryptionKeySizes") == 0)
    {
        AnscCopyString(pValue,pHEAD->EncryptionKeySizes);
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

        BOOL TelcoVoiceMgrDml_VoipProfileList_RTP_SRTP_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString);

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

BOOL TelcoVoiceMgrDml_VoipProfileList_RTP_SRTP_SetParamStringValue(ANSC_HANDLE hInsContext, char* ParamName, char* pString)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uProfileIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_VOIPPROFILE_CTRL_T pVoipProfileCtrl = (PDML_VOIPPROFILE_CTRL_T)hInsContext;

    PDML_VOIPPROFILE pDmlVoipProfile = &(pVoipProfileCtrl->dml);

    PDML_VOIPPROFILE_RTP_SRTP pHEAD = &(pDmlVoipProfile->RTP.SRTP);


    if(pHEAD != NULL)
    {
        pDmlVoiceService = (PTELCOVOICEMGR_DML_VOICESERVICE)pDmlVoipProfile->pParentVoiceService;
    }

    if (pHEAD == NULL  || pDmlVoiceService == NULL)
    {
        TELCOVOICEMGR_UNLOCK()

        CcspTraceError(("%s:%d:: pHEAD or pDmlVoiceService NULL\n", __FUNCTION__, __LINE__));

        return ret;
    }

    uVsIndex = pDmlVoiceService->InstanceNumber;

    uProfileIndex = pDmlVoipProfile->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if (strcmp(ParamName, "EncryptionKeySizes") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.VoIPProfile.%d.RTP.SRTP.EncryptionKeySizes",uVsIndex,uProfileIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,pString) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            AnscCopyString(pHEAD->EncryptionKeySizes,pString);

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

        BOOL TelcoVoiceMgrDml_VoipProfileList_RTP_SRTP_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

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

BOOL TelcoVoiceMgrDml_VoipProfileList_RTP_SRTP_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || pBool == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_VOIPPROFILE_CTRL_T pVoipProfileCtrl = (PDML_VOIPPROFILE_CTRL_T)hInsContext;

    PDML_VOIPPROFILE pDmlVoipProfile = &(pVoipProfileCtrl->dml);

    PDML_VOIPPROFILE_RTP_SRTP pHEAD = &(pDmlVoipProfile->RTP.SRTP);

    if (strcmp(ParamName, "Enable") == 0)
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

        BOOL TelcoVoiceMgrDml_VoipProfileList_RTP_SRTP_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

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

BOOL TelcoVoiceMgrDml_VoipProfileList_RTP_SRTP_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uProfileIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_VOIPPROFILE_CTRL_T pVoipProfileCtrl = (PDML_VOIPPROFILE_CTRL_T)hInsContext;

    PDML_VOIPPROFILE pDmlVoipProfile = &(pVoipProfileCtrl->dml);

    PDML_VOIPPROFILE_RTP_SRTP pHEAD = &(pDmlVoipProfile->RTP.SRTP);


    if(pHEAD != NULL)
    {
        pDmlVoiceService = (PTELCOVOICEMGR_DML_VOICESERVICE)pDmlVoipProfile->pParentVoiceService;
    }

    if (pHEAD == NULL  || pDmlVoiceService == NULL)
    {
        TELCOVOICEMGR_UNLOCK()

        CcspTraceError(("%s:%d:: pHEAD or pDmlVoiceService NULL\n", __FUNCTION__, __LINE__));

        return ret;
    }

    uVsIndex = pDmlVoiceService->InstanceNumber;

    uProfileIndex = pDmlVoipProfile->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if (strcmp(ParamName, "Enable") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.VoIPProfile.%d.RTP.SRTP.Enable",uVsIndex,uProfileIndex);

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

        BOOL TelcoVoiceMgrDml_VoipProfileList_RTP_Redundancy_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

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

BOOL TelcoVoiceMgrDml_VoipProfileList_RTP_Redundancy_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || puLong == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_VOIPPROFILE_CTRL_T pVoipProfileCtrl = (PDML_VOIPPROFILE_CTRL_T)hInsContext;

    PDML_VOIPPROFILE pDmlVoipProfile = &(pVoipProfileCtrl->dml);

    PDML_VOIPPROFILE_RTP_REDUNDANCY pHEAD = &(pDmlVoipProfile->RTP.Redundancy);

    if (strcmp(ParamName, "PayloadType") == 0)
    {
        *puLong = pHEAD->PayloadType;
        ret = TRUE;
    }
    else if (strcmp(ParamName, "MaxSessionsUsingRedundancy") == 0)
    {
        *puLong = pHEAD->MaxSessionsUsingRedundancy;
        ret = TRUE;
    }
    else if (strcmp(ParamName, "BlockPayloadType") == 0)
    {
        *puLong = pHEAD->BlockPayloadType;
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

        BOOL TelcoVoiceMgrDml_VoipProfileList_RTP_Redundancy_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

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

BOOL TelcoVoiceMgrDml_VoipProfileList_RTP_Redundancy_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uProfileIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_VOIPPROFILE_CTRL_T pVoipProfileCtrl = (PDML_VOIPPROFILE_CTRL_T)hInsContext;

    PDML_VOIPPROFILE pDmlVoipProfile = &(pVoipProfileCtrl->dml);

    PDML_VOIPPROFILE_RTP_REDUNDANCY pHEAD = &(pDmlVoipProfile->RTP.Redundancy);


    if(pHEAD != NULL)
    {
        pDmlVoiceService = (PTELCOVOICEMGR_DML_VOICESERVICE)pDmlVoipProfile->pParentVoiceService;
    }

    if (pHEAD == NULL  || pDmlVoiceService == NULL)
    {
        TELCOVOICEMGR_UNLOCK()

        CcspTraceError(("%s:%d:: pHEAD or pDmlVoiceService NULL\n", __FUNCTION__, __LINE__));

        return ret;
    }

    uVsIndex = pDmlVoiceService->InstanceNumber;

    uProfileIndex = pDmlVoipProfile->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if (strcmp(ParamName, "MaxSessionsUsingRedundancy") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.VoIPProfile.%d.RTP.Redundancy.MaxSessionsUsingRedundancy",uVsIndex,uProfileIndex);

        if (TelcoVoiceMgrHal_SetParamULong(HalName,uValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->MaxSessionsUsingRedundancy = uValue;

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

        BOOL TelcoVoiceMgrDml_VoipProfileList_RTP_Redundancy_GetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int* pInt);

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

BOOL TelcoVoiceMgrDml_VoipProfileList_RTP_Redundancy_GetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int* pInt)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || pInt == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_VOIPPROFILE_CTRL_T pVoipProfileCtrl = (PDML_VOIPPROFILE_CTRL_T)hInsContext;

    PDML_VOIPPROFILE pDmlVoipProfile = &(pVoipProfileCtrl->dml);

    PDML_VOIPPROFILE_RTP_REDUNDANCY pHEAD = &(pDmlVoipProfile->RTP.Redundancy);

    if (strcmp(ParamName, "VoiceRedundancy") == 0)
    {
        *pInt = pHEAD->VoiceRedundancy;
        ret = TRUE;
    }
    else if (strcmp(ParamName, "ModemRedundancy") == 0)
    {
        *pInt = pHEAD->ModemRedundancy;
        ret = TRUE;
    }
    else if (strcmp(ParamName, "FaxAndModemRedundancy") == 0)
    {
        *pInt = pHEAD->FaxAndModemRedundancy;
        ret = TRUE;
    }
    else if (strcmp(ParamName, "DTMFRedundancy") == 0)
    {
        *pInt = pHEAD->DTMFRedundancy;
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

        BOOL TelcoVoiceMgrDml_VoipProfileList_RTP_Redundancy_SetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int iValue);

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

BOOL TelcoVoiceMgrDml_VoipProfileList_RTP_Redundancy_SetParamIntValue(ANSC_HANDLE hInsContext, char* ParamName, int iValue)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uProfileIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_VOIPPROFILE_CTRL_T pVoipProfileCtrl = (PDML_VOIPPROFILE_CTRL_T)hInsContext;

    PDML_VOIPPROFILE pDmlVoipProfile = &(pVoipProfileCtrl->dml);

    PDML_VOIPPROFILE_RTP_REDUNDANCY pHEAD = &(pDmlVoipProfile->RTP.Redundancy);


    if(pHEAD != NULL)
    {
        pDmlVoiceService = (PTELCOVOICEMGR_DML_VOICESERVICE)pDmlVoipProfile->pParentVoiceService;
    }

    if (pHEAD == NULL  || pDmlVoiceService == NULL)
    {
        TELCOVOICEMGR_UNLOCK()

        CcspTraceError(("%s:%d:: pHEAD or pDmlVoiceService NULL\n", __FUNCTION__, __LINE__));

        return ret;
    }

    uVsIndex = pDmlVoiceService->InstanceNumber;

    uProfileIndex = pDmlVoipProfile->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if (strcmp(ParamName, "VoiceRedundancy") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.VoIPProfile.%d.RTP.Redundancy.VoiceRedundancy",uVsIndex,uProfileIndex);

        if (TelcoVoiceMgrHal_SetParamInt(HalName,iValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->VoiceRedundancy = iValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "ModemRedundancy") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.VoIPProfile.%d.RTP.Redundancy.ModemRedundancy",uVsIndex,uProfileIndex);

        if (TelcoVoiceMgrHal_SetParamInt(HalName,iValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->ModemRedundancy = iValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "FaxAndModemRedundancy") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.VoIPProfile.%d.RTP.Redundancy.FaxAndModemRedundancy",uVsIndex,uProfileIndex);

        if (TelcoVoiceMgrHal_SetParamInt(HalName,iValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->FaxAndModemRedundancy = iValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "DTMFRedundancy") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.VoIPProfile.%d.RTP.Redundancy.DTMFRedundancy",uVsIndex,uProfileIndex);

        if (TelcoVoiceMgrHal_SetParamInt(HalName,iValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->DTMFRedundancy = iValue;

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

        BOOL TelcoVoiceMgrDml_VoipProfileList_RTP_Redundancy_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool);

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

BOOL TelcoVoiceMgrDml_VoipProfileList_RTP_Redundancy_GetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL* pBool)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || pBool == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_VOIPPROFILE_CTRL_T pVoipProfileCtrl = (PDML_VOIPPROFILE_CTRL_T)hInsContext;

    PDML_VOIPPROFILE pDmlVoipProfile = &(pVoipProfileCtrl->dml);

    PDML_VOIPPROFILE_RTP_REDUNDANCY pHEAD = &(pDmlVoipProfile->RTP.Redundancy);

    if (strcmp(ParamName, "Enable") == 0)
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

        BOOL TelcoVoiceMgrDml_VoipProfileList_RTP_Redundancy_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue);

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

BOOL TelcoVoiceMgrDml_VoipProfileList_RTP_Redundancy_SetParamBoolValue(ANSC_HANDLE hInsContext, char* ParamName, BOOL bValue)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uProfileIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_VOIPPROFILE_CTRL_T pVoipProfileCtrl = (PDML_VOIPPROFILE_CTRL_T)hInsContext;

    PDML_VOIPPROFILE pDmlVoipProfile = &(pVoipProfileCtrl->dml);

    PDML_VOIPPROFILE_RTP_REDUNDANCY pHEAD = &(pDmlVoipProfile->RTP.Redundancy);


    if(pHEAD != NULL)
    {
        pDmlVoiceService = (PTELCOVOICEMGR_DML_VOICESERVICE)pDmlVoipProfile->pParentVoiceService;
    }

    if (pHEAD == NULL  || pDmlVoiceService == NULL)
    {
        TELCOVOICEMGR_UNLOCK()

        CcspTraceError(("%s:%d:: pHEAD or pDmlVoiceService NULL\n", __FUNCTION__, __LINE__));

        return ret;
    }

    uVsIndex = pDmlVoiceService->InstanceNumber;

    uProfileIndex = pDmlVoipProfile->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if (strcmp(ParamName, "Enable") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.VoIPProfile.%d.RTP.Redundancy.Enable",uVsIndex,uProfileIndex);

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

        BOOL TelcoVoiceMgrDml_VoipProfileList_FaxT38_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong);

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

BOOL TelcoVoiceMgrDml_VoipProfileList_FaxT38_GetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG* puLong)
{
    BOOL ret = FALSE;

    if(ParamName == NULL || puLong == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_VOIPPROFILE_CTRL_T pVoipProfileCtrl = (PDML_VOIPPROFILE_CTRL_T)hInsContext;

    PDML_VOIPPROFILE pDmlVoipProfile = &(pVoipProfileCtrl->dml);

    PDML_VOIPPROFILE_FAXT38 pHEAD = &(pDmlVoipProfile->FaxT38);

    if (strcmp(ParamName, "TCFMethod") == 0)
    {
        *puLong = pHEAD->TCFMethod;
        ret = TRUE;
    }
    else if (strcmp(ParamName, "MaxBitRate") == 0)
    {
        *puLong = pHEAD->MaxBitRate;
        ret = TRUE;
    }
    else if (strcmp(ParamName, "LowSpeedRedundancy") == 0)
    {
        *puLong = pHEAD->LowSpeedRedundancy;
        ret = TRUE;
    }
    else if (strcmp(ParamName, "HighSpeedRedundancy") == 0)
    {
        *puLong = pHEAD->HighSpeedRedundancy;
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

        BOOL TelcoVoiceMgrDml_VoipProfileList_FaxT38_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue);

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

BOOL TelcoVoiceMgrDml_VoipProfileList_FaxT38_SetParamUlongValue(ANSC_HANDLE hInsContext, char* ParamName, ULONG uValue)
{
    BOOL ret = FALSE;
    ULONG uVsIndex  = 0;
    ULONG uProfileIndex = 0;
    char HalName[MAX_STR_LEN] = {0};

    PTELCOVOICEMGR_DML_VOICESERVICE pDmlVoiceService = NULL;

    if(ParamName == NULL)
    {
        CcspTraceWarning(("%s: Invalid Input Parameter [NULL]\n", __func__));
        return ret;
    }

    TELCOVOICEMGR_LOCK_OR_EXIT()

    PDML_VOIPPROFILE_CTRL_T pVoipProfileCtrl = (PDML_VOIPPROFILE_CTRL_T)hInsContext;

    PDML_VOIPPROFILE pDmlVoipProfile = &(pVoipProfileCtrl->dml);

    PDML_VOIPPROFILE_FAXT38 pHEAD = &(pDmlVoipProfile->FaxT38);


    if(pHEAD != NULL)
    {
        pDmlVoiceService = (PTELCOVOICEMGR_DML_VOICESERVICE)pDmlVoipProfile->pParentVoiceService;
    }

    if (pHEAD == NULL  || pDmlVoiceService == NULL)
    {
        TELCOVOICEMGR_UNLOCK()

        CcspTraceError(("%s:%d:: pHEAD or pDmlVoiceService NULL\n", __FUNCTION__, __LINE__));

        return ret;
    }

    uVsIndex = pDmlVoiceService->InstanceNumber;

    uProfileIndex = pDmlVoipProfile->uInstanceNumber;

    TELCOVOICEMGR_UNLOCK()

    if (strcmp(ParamName, "TCFMethod") == 0)
    {
        char enumValue[][STR_LEN_32]={"Local","Network"};

        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.VoIPProfile.%d.FaxT38.TCFMethod",uVsIndex,uProfileIndex);

        if (TelcoVoiceMgrHal_SetParamString(HalName,enumValue[uValue]) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->TCFMethod = uValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "MaxBitRate") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.VoIPProfile.%d.FaxT38.MaxBitRate",uVsIndex,uProfileIndex);

        if (TelcoVoiceMgrHal_SetParamULong(HalName,uValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->MaxBitRate = uValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "LowSpeedRedundancy") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.VoIPProfile.%d.FaxT38.LowSpeedRedundancy",uVsIndex,uProfileIndex);

        if (TelcoVoiceMgrHal_SetParamULong(HalName,uValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->LowSpeedRedundancy = uValue;

            TELCOVOICEMGR_UNLOCK()

            ret = TRUE;
        }
    }
    else if (strcmp(ParamName, "HighSpeedRedundancy") == 0)
    {
        snprintf(HalName, MAX_STR_LEN, "Device.Services.VoiceService.%d.VoIPProfile.%d.FaxT38.HighSpeedRedundancy",uVsIndex,uProfileIndex);

        if (TelcoVoiceMgrHal_SetParamULong(HalName,uValue) == ANSC_STATUS_SUCCESS)
        {
            TELCOVOICEMGR_LOCK_OR_EXIT()

            pHEAD->HighSpeedRedundancy = uValue;

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
