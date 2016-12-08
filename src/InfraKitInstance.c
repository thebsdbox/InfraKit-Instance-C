/* InfraKitInstance.c
 *
 * InfraKit
 *
 * Copyright (C) 2016 <Daniel Finneran dan@thebsdbox.co.uk>
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#include "InfraKitInstance.h"
#include "InfrakitState.h"
//#include "oneview.h"
#include <strings.h>
#include <stdlib.h>

json_int_t parse_error = -32700;

/* Check through the state file and compare the physical state
 * then update the state file so that InfraKit is kept current with
 * the physical Infrastructure state.
 */

int synchroniseStateWithPhysical()
{

        json_t *StateJSON = openInstanceState();
        json_t *previousInstances = json_object_get(StateJSON, "Instances");
        json_t *previousNonFunctional = json_object_get(StateJSON, "NonFunctional");
        
        json_t *currentInstances = json_array();
        json_t *currentNonFunctional = json_array();
       
        // Iterate over the non-functional instances
        
        size_t memberIndex;
        json_t *memberValue;
        
        /*  If one member is part of both it will compound the problem...
         *
         */
        
        
        json_array_foreach(previousNonFunctional, memberIndex, memberValue) {
            const char *hardwareURI = json_string_value(json_object_get(memberValue, "LogicalID"));
            if (hardwareURI) {
                char *profileURI;
                if (profileURI) {
                    // Check power state and add to active / non-functional
                    json_array_append(currentInstances, memberValue);
                    //json_array_append(currentNonFunctional, memberValue);
                }
            }
        }
        json_array_foreach(previousInstances, memberIndex, memberValue) {
            const char *hardwareURI = json_string_value(json_object_get(memberValue, "LogicalID"));
            if (hardwareURI) {
                    json_array_append(currentInstances, memberValue);
            }
        }
        // Two updated new arrays to replace inside our state
        json_object_set(StateJSON, "Instances", currentInstances);
        json_object_set(StateJSON, "NonFunctional", currentNonFunctional);

        char *json_text = json_dumps(StateJSON, JSON_ENSURE_ASCII);
        saveInstanceState(json_text);
        json_decref(StateJSON);
        return EXIT_SUCCESS;
}


 /*******************************************************************************************/

 /* Docker InfraKit methods, these methods are called directly from the HTTPD server and
  * provide the capability for provisioning, monitoring and destroying of infrastructure.
  *
  */


 /* infraKitInstanceProvision(json_t *params, long long id)
  * params = Parameter JSON that the instance uses for configuration
  * id = method call id, to ensure function sycnronisation
  *
  * Unlike the other methods, this will use the credentials in the parameters to configure
  *
  */


char *infraKitInstanceProvision(json_t *params, long long id)
{

    char *new_instance  = "instance-1243253647";
    
    char *successProvisionResponse = "{s:s,s:{s:s?},s:I}";
    char *failProvisionResponse = "{s:s,s:{s:i},s:I}";

    char *response;
    json_t *reponseJSON;
    if (new_instance) {
        reponseJSON = json_pack(successProvisionResponse,   "jsonrpc", "2.0",                   \
                                                            "result",                           \
                                                                "ID", new_instance,             \
                                                            "id", id);
    } else {
        reponseJSON = json_pack(failProvisionResponse,      "jsonrpc", "2.0",                   \
                                                            "error",                            \
                                                                "code", parse_error,            \
                                                            "id", id);
    }
    response = json_dumps(reponseJSON, JSON_ENSURE_ASCII);
    return response;
}

/* infraKitInstanceDescribe(json_t *params, long long id)
 * params = Parameter JSON that the instance uses for configuration
 * id = method call id, to ensure function sycnronisation
 *
 * This function should use the plugin-state, defined in a file and compare it with
 * the state of the infrastructure we're hoping to configure.
 */

char *infraKitInstanceDescribe(json_t *params, long long id)
{
    if (synchroniseStateWithPhysical() == EXIT_SUCCESS) {
        printf ("\n Sync \n");
    }
    json_t *instanceState = openInstanceState("");
    json_t *instanceArray = json_object_get(instanceState, "Instances");
    char *DescriptionResponse = "{s:s,s:{s:[]},s:s?,s:I}";
    json_t *responseJSON = json_pack(DescriptionResponse,   "jsonrpc", "2.0",                   \
                                                            "result",                           \
                                                                "Descriptions",                 \
                                                            "error", NULL,                      \
                                                            "id", id);
    json_t *array = json_object_get(responseJSON, "result");
    json_object_set(array, "Descriptions", instanceArray);
    char *response = json_dumps(responseJSON, JSON_ENSURE_ASCII);
    return response;
}

char *infraKitInstanceDestroy(json_t *params, long long id)
{
    int InstanceRemoved;

    const char *instanceID = json_string_value(json_object_get(params, "Instance"));

    InstanceRemoved = removeInstanceFromState((char *)instanceID);

    char *successProvisionResponse = "{s:s,s:{s:s?},s:I}";
    char *failProvisionResponse = "{s:s,s:{s:i},s:I}";
    char *response;
    json_t *reponseJSON;
    if (InstanceRemoved == EXIT_SUCCESS) {
         //Describe Instances function
        reponseJSON = json_pack(successProvisionResponse,  "jsonrpc", "2.0",                    \
                                                            "result",                           \
                                                                "Instance", instanceID,         \
                                                            "id", id);
    } else {
        reponseJSON = json_pack(failProvisionResponse,      "jsonrpc", "2.0",                   \
                                                            "error",                            \
                                                                "code", parse_error,            \
                                                            "id", id);
    }
    response = json_dumps(reponseJSON, JSON_ENSURE_ASCII);
    return response;

}

