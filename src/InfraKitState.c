
/* InfraKitState.c
 *
 * InfraKit
 *
 * Copyright (C) 2016 <Daniel Finneran dan@thebsdbox.co.uk>
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */


#include "InfraKitState.h"
#include "InfraKitPlugin.h"

#include <string.h>

char *statePath = NULL;

char *getStatePath()
{
    return statePath;
}

int setStatePath(char *path)
{
    if (path && (strlen(path) > 1)) {
        statePath = path;
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

#define INSTANCE_RETRY "5"


 /* These functions will open a file at a specified path, and if  found attempt to read
  * the JSON. If the file isn't found or the JSON wont load, it will create a new state.
  *
  */

json_t *openInstanceState()
{
    //char *stateFilePath = "/Users/dan/Desktop/state.json";
    if (!statePath) {
        //WARNING
        return NULL;
    }
    json_t *stateJSON;
    json_error_t error;
    stateJSON = json_load_file(statePath, 0, &error);
    if (!stateJSON) {
        stateJSON = json_pack("{s:{},s:[],s:[]}", "OneViewInstance", "Instances", "NonFunctional");
    }
    return stateJSON;
}

int saveInstanceState(char *jsonData)
{
    if (!statePath) {
        //WARNING
        return EXIT_FAILURE;
    }
    
    FILE *fp;
    fp = fopen(statePath, "w");
    if (fp) { /*file opened succesfully */
        fputs(jsonData, fp);
        fclose(fp);
    } else {
        printf("Error opening file %s \n", statePath);
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}


/* This function will take a new profile, and attach the various details to the state data
 * HardwareURI, InstanceID (profile Name), it will also place the OneView credentials in the
 * state file so that other functions can find them and use them to confirm the state.
 */

int appendInstanceToState(profile *foundServer,  json_t *paramsJSON)
{
    json_t *stateJSON = openInstanceState();
    json_t *instances = json_object_get(stateJSON, "Instances");
    if (json_object_size(json_object_get(stateJSON, "OneViewInstance")) == 0) {
        char *oneviewDetails = "{s:s?,s:s?,s:s?,s:s?}";
    }
    json_t *tags = json_object_get(paramsJSON, "Tags");
    const char *sha = json_string_value(json_object_get(tags, "infrakit.config_sha"));
    const char *group = json_string_value(json_object_get(tags, "infrakit.group"));
    
    char *instanceDescription = "{s:s,s:s?,s:{s:s,s:s,s:s,s:s}}";
    
    json_t *descriptionJSON = json_pack(instanceDescription, \
                                            "ID", foundServer->profileName, \
                                            "LogicalID", foundServer->availableHardwareURI, \
                                            "Tags", \
                                                "hw_uri", foundServer->availableHardwareURI, \
                                                "retry-count", INSTANCE_RETRY, \
                                                "infrakit.config_sha", sha, \
                                                "infrakit.group", group);
    
    json_array_append(instances, descriptionJSON);
    char *json_text = json_dumps(stateJSON, JSON_ENSURE_ASCII);
    saveInstanceState(json_text);
    free(json_text);
    json_decref(stateJSON);
    return EXIT_SUCCESS;
}


/* This function will iterate through the instance state and remove an instance, 
 * by using it's instanceID
 */

int removeInstanceFromState(char *instanceID)
{
    json_t *stateJSON = openInstanceState();
    json_t *instances = json_object_get(stateJSON, "Instances");
    
    /* No Instance to compare, so failure to match
     */
    
    if (json_array_size(instances) == 0) {
        return EXIT_FAILURE;
    }
    
    if (instances) {
        size_t instanceIndex;
        json_t *instanceValue;
        signed long instanceLocation = -1;
        json_array_foreach(instances, instanceIndex, instanceValue) {
            const char *currentValue = json_string_value(json_object_get(instanceValue, "ID"));
            if (currentValue) {
                if (stringMatch((char *)currentValue, (char *)instanceID)) {
                    instanceLocation = instanceIndex;
                }
            }
        }
        // See if instance was found in array
        if (instanceLocation != -1) {
            json_array_remove(instances, instanceLocation);
            char *json_text = json_dumps(stateJSON, JSON_ENSURE_ASCII);
            saveInstanceState(json_text);
            free(json_text);
            json_decref(stateJSON);
            return EXIT_SUCCESS;
        }
    }
    json_decref(stateJSON);
    return EXIT_FAILURE;
}

/* These functions query the state of Instances, or can return values assigned to
 * instances.
 */

char *returnValueFromInstanceKey(char *InstanceID, char *key)
{
    json_t *stateJSON = openInstanceState();
    if (!stateJSON) {
        return NULL;
    }
    json_t *instances = json_object_get(stateJSON, "Instances");
    
    /* No Instance to compare, so failure to match
     */
    
    if (json_array_size(instances) == 0) {
        return NULL;
    }
    
    if (instances) {
        size_t instanceIndex;
        json_t *instanceValue;
        
        json_array_foreach(instances, instanceIndex, instanceValue) {
            const char *ID = json_string_value(json_object_get(instanceValue, "ID"));
            if (stringMatch((char *)ID, InstanceID)) {
                // found our instance
                const char *value = json_string_value(json_object_get(instanceValue, key));
                if (value) {
                    // Key exists and has returned a value
                    char *returnValue = strdup(value);
                    json_decref(stateJSON);
                    return returnValue;
                }
            }
        }
    }
    // Not found in our state
    json_decref(stateJSON);
    return NULL;
}

int compareInstanceValueToKey(char *key, const char *value)
{
    json_t *stateJSON = openInstanceState();
    json_t *instances = json_object_get(stateJSON, "Instances");
    
    /* No Instance to compare, so failure to match
     */
    
    if (json_array_size(instances) == 0) {
        return EXIT_FAILURE;
    }
    
    if (instances) {
        size_t instanceIndex;
        json_t *instanceValue;
        
        json_array_foreach(instances, instanceIndex, instanceValue) {
            const char *currentValue = json_string_value(json_object_get(instanceValue, key));
            if (currentValue) {
                if (stringMatch((char *)currentValue, (char *)value)) {
                    json_decref(stateJSON);
                    return EXIT_SUCCESS;
                }
            }
        }
    }
    json_decref(stateJSON);
    return EXIT_FAILURE;
}
