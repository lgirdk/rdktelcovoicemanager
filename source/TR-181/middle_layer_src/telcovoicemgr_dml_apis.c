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

#include "telcovoicemgr_dml_apis.h"
#include "ccsp_dm_api.h"
#include "telcovoicemgr_dml_services.h"
#include "telcovoicemgr_dml_backendmgr.h"


#ifdef _ANSC_LINUX
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <net/if.h>
#include <sys/types.h>
#endif
#include "ansc_platform.h"

/**********************************************************************

    caller:     owner of the object

    prototype:

        ANSC_HANDLE TelcoVoiceMgrServicesCreate(VOID);

    description:

        This function constructs cosa Services object and return handle.

    argument:

    return:     newly created Services object.

**********************************************************************/

ANSC_HANDLE TelcoVoiceMgrServicesCreate(VOID)
{
    ANSC_STATUS                     returnStatus = ANSC_STATUS_SUCCESS;
    PTELCOVOICEMGR_DATAMODEL_SERVICES        pMyObject    = (PTELCOVOICEMGR_DATAMODEL_SERVICES)NULL;

    /*
     * We create object by first allocating memory for holding the variables and member functions.
     */

    pMyObject = (PTELCOVOICEMGR_DATAMODEL_SERVICES)AnscAllocateMemory(sizeof(TELCOVOICEMGR_DATAMODEL_SERVICES));

    if ( !pMyObject )
    {
        return  (ANSC_HANDLE)NULL;
    }


    pMyObject->data = TelcoVoiceMgrDmlGetData();
    if ( !pMyObject->data )
    {
        AnscFreeMemory((ANSC_HANDLE)pMyObject);
        return  (ANSC_HANDLE)NULL;
    }

    /*
     * Initialize the common variables and functions for a container object.
     */
    pMyObject->Create            = TelcoVoiceMgrServicesCreate;
    pMyObject->Remove            = TelcoVoiceMgrServicesRemove;
    pMyObject->Initialize        = TelcoVoiceMgrServicesInitialize;

    pMyObject->Initialize   ((ANSC_HANDLE)pMyObject);

    return  (ANSC_HANDLE)pMyObject;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS TelcoVoiceMgrServicesInitialize(ANSC_HANDLE hThisObject);

    description:

        This function initiate  cosa device info object and return handle.

    argument:   ANSC_HANDLE                 hThisObject
            This handle is actually the pointer of this object
            itself.

    return:     operation status.

**********************************************************************/

ANSC_STATUS TelcoVoiceMgrServicesInitialize(ANSC_HANDLE hThisObject)
{
    ANSC_STATUS                  returnStatus        = ANSC_STATUS_SUCCESS;
    PTELCOVOICEMGR_DATAMODEL_SERVICES     pMyObject           = (PTELCOVOICEMGR_DATAMODEL_SERVICES)hThisObject;

    //Initialise dml data handler
    returnStatus = TelcoVoiceMgrDmlDataInit();
    if(returnStatus != ANSC_STATUS_SUCCESS)
    {
        return returnStatus;
    }

    //Wait HAL initilisation
    returnStatus = TelcoVoiceMgrHal_Init();
    if(returnStatus != ANSC_STATUS_SUCCESS)
    {
        return returnStatus;
    }

    //Get HAL data
    returnStatus = TelcoVoiceMgrHal_GetInitData();
    if(returnStatus != ANSC_STATUS_SUCCESS)
    {
        return returnStatus;
    }

    // Initialize sysevent daemon
    if (TelcoVoiceMgrSyseventInit() < 0)
    {
        CcspTraceError(("Sysevent Init failed !!!!\n"));
        return ANSC_STATUS_FAILURE;
    }
 
    //Register subscribe event callback and send data to hal
    returnStatus = TelcoVoiceMgr_Controller_Init();
    if(returnStatus != ANSC_STATUS_SUCCESS)
    {
        CcspTraceError(("TelcoVoiceMgr_Controller_Init failed !!!!\n"));
        return returnStatus;
    }
    return returnStatus;
}

/**********************************************************************

    caller:     self

    prototype:

        ANSC_STATUS TelcoVoiceMgrServicesRemove(ANSC_HANDLE hThisObject);

    description:

        This function initiate  cosa device info object and return handle.

    argument:   ANSC_HANDLE                 hThisObject
            This handle is actually the pointer of this object
            itself.

    return:     operation status.

**********************************************************************/

ANSC_STATUS TelcoVoiceMgrServicesRemove(ANSC_HANDLE hThisObject)
{
    ANSC_STATUS                  returnStatus        = ANSC_STATUS_SUCCESS;
    PTELCOVOICEMGR_DATAMODEL_SERVICES     pMyObject           = (PTELCOVOICEMGR_DATAMODEL_SERVICES)hThisObject;

    //delete DML data 
    TelcoVoiceMgrDmlDataDelete();


    //close sysevent
    TelcoVoiceMgrSyseventDeInit();

    /* Remove self */
    AnscFreeMemory((ANSC_HANDLE)pMyObject);

    return returnStatus;
}
