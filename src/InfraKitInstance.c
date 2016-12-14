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
#include <strings.h>
#include <stdlib.h>

 /* JSON-RPC 2.0 error codes, for more info visit:
  * http://www.jsonrpc.org/specification
  */

json_int_t JSON_RPC_PARSE_ERROR = -32700;
json_int_t JSON_RPC_INVALID_REQUEST = -32600;
json_int_t JSON_RPC_METHOD_NOTFOUND = -32601;
json_int_t JSON_RPC_INVALID_PARAMS = -32602;
json_int_t JSON_RPC_INTERNAL_ERROR = -32602;


/* Check through the state file and compare the physical state
 * then update the state file so that InfraKit is kept current with
 * the physical Infrastructure state.
 */

int synchroniseStateWithProvider()
{

        json_t *StateJSON = openInstanceState();
        json_t *previousInstances = json_object_get(StateJSON, "Instances");
        json_t *previousNonFunctional = json_object_get(StateJSON, "NonFunctional");
        
        json_t *currentInstances = json_array();
        json_t *currentNonFunctional = json_array();
       
        // Iterate over the non-functional instances
        
        size_t memberIndex;
        json_t *memberValue;
    
        json_array_foreach(previousNonFunctional, memberIndex, memberValue) {
            const char *logicalID = json_string_value(json_object_get(memberValue, "LogicalID"));
            if (logicalID) {
                if (logicalID) {
                    
                    
                    /* CODE HERE -> Check with the provider if an instance we previously deemed non-functional,
                     * has changed it's state back to functional, or alternatively been removed.
                     */
                    
                    
                    // If the Instance is operational ->
                    //json_array_append(currentInstances, memberValue);
                    
                    // If the instance is still non-functional ->
                    //json_array_append(currentNonFunctional, memberValue);
                    
                }
            }
        }
        json_array_foreach(previousInstances, memberIndex, memberValue) {
            const char *logicalID = json_string_value(json_object_get(memberValue, "LogicalID"));
            if (logicalID) {
                
                    /* CODE HERE -> Check the provider that the instance created still exists, if
                     * it does then add the existing instance to the array. If not
                     * then it will effectively be removed and InfraKit will create a new instance.
                     */
                
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


/* infraKitInstanceValidate(json_t *params, long long id)
 * params = Parameter JSON that the instance uses for configuration
 * id = method call id, to ensure function sycnronisation
 *
 * Validate the JSON that has been passed as the parameters
 *
 */


char *infraKitInstanceValidate(json_t *params, long long id)
{
    

    /* CODE HERE -> change the name structure above if needed
     * take the params json and apply it to your infrastructure instance provider
     * then report back if succesful or failure.
     *
     */
    
    
    
    char *successProvisionResponse = "{s:s,s:{s:s?},s:I}";
    char *failProvisionResponse = "{s:s,s:{s:i},s:I}";
    
    char *response;
    json_t *responseJSON;
    if (params) {
        responseJSON = json_pack(successProvisionResponse,  "jsonrpc", "2.0",                   \
                                                            "result",                           \
                                                                "ID", "",                       \
                                                            "id", id);
    } else {
        responseJSON = json_pack(failProvisionResponse,     "jsonrpc", "2.0",                   \
                                                            "error",                            \
                                                                "code", JSON_RPC_PARSE_ERROR,   \
                                                            "id", id);
    }
    response = json_dumps(responseJSON, JSON_ENSURE_ASCII);
    json_decref(responseJSON);
    return response;
}


 /* infraKitInstanceProvision(json_t *params, long long id)
  * params = Parameter JSON that the instance uses for configuration
  * id = method call id, to ensure function sycnronisation
  *
  * Unlike the other methods, this will use the credentials in the parameters to configure
  *
  */


char *infraKitInstanceProvision(json_t *params, long long id)
{

    char name[100];
    sprintf(name, "InfraKit-%llu",id);
    
    
    
    /* CODE HERE -> change the name structure above if needed
     * take the params json and apply it to your infrastructure instance provider
     * then report back if succesful or failure.
     *
     */
    
    
    
    appendInstanceToState(params, name);
    char *successProvisionResponse = "{s:s,s:{s:s?},s:I}";
    char *failProvisionResponse = "{s:s,s:{s:i},s:I}";

    char *response;
    json_t *responseJSON;
    if (params) {
        responseJSON = json_pack(successProvisionResponse,   "jsonrpc", "2.0",                   \
                                                             "result",                           \
                                                                 "ID", name,                     \
                                                            "id", id);
    } else {
        responseJSON = json_pack(failProvisionResponse,      "jsonrpc", "2.0",                   \
                                                             "error",                            \
                                                                 "code", JSON_RPC_PARSE_ERROR,   \
                                                            "id", id);
    }
    response = json_dumps(responseJSON, JSON_ENSURE_ASCII);
    json_decref(responseJSON);
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

    
    
    
    /* CODE HERE -> Write code to speak with the instance provider
     * compare it with our internal state and determine the differences
     * then report the instance description back to InfraKit
     */

    
    
    json_t *instanceState = openInstanceState();
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
    json_decref(responseJSON);
    return response;
}

/* infraKitInstanceDestroy(json_t *params, long long id)
 * params = Parameter JSON that the instance uses for configuration
 * id = method call id, to ensure function sycnronisation
 *
 * This function should use get take the instance that InfraKit wants to destroy, compare the 
 * provider details with that of internal state and take care of destroying the instance and 
 * returning either success or failure to InfraKit
 */

char *infraKitInstanceDestroy(json_t *params, long long id)
{

    const char *instanceID = json_string_value(json_object_get(params, "Instance"));

    
    /* CODE HERE -> take the instanceID from InfraKit and remove it from the instance 
     * provider, verify the instance is removed then update the internal state.
     */
    
    int instanceRemoved = removeInstanceFromState((char *)instanceID);

    char *successProvisionResponse = "{s:s,s:{s:s?},s:I}";
    char *failProvisionResponse = "{s:s,s:{s:i},s:I}";
    char *response;
    json_t *responseJSON;
    if (instanceRemoved == EXIT_SUCCESS) {
         //Describe Instances function
        responseJSON = json_pack(successProvisionResponse,  "jsonrpc", "2.0",                    \
                                                            "result",                            \
                                                                "Instance", instanceID,          \
                                                            "id", id);
    } else {
        responseJSON = json_pack(failProvisionResponse,      "jsonrpc", "2.0",                   \
                                                             "error",                            \
                                                                "code", JSON_RPC_PARSE_ERROR,    \
                                                            "id", id);
    }
    response = json_dumps(responseJSON, JSON_ENSURE_ASCII);
    json_decref(responseJSON);
    return response;

}

char *infraKitInstanceMeta(json_t *params, long long id)
{
    
    //TODO
    return NULL;
}

