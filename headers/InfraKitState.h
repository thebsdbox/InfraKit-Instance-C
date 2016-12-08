/* InfraKitState.h
 *
 * InfraKit
 *
 * Copyright (C) 2016 <Daniel Finneran dan@thebsdbox.co.uk>
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

//#include "oneview.h"
#include "InfraKitInstance.h"

#include "jansson.h"

json_t *openInstanceState();
int saveInstanceState(char *jsonData);
int appendInstanceToState(json_t *paramsJSON, char *name);
int compareInstanceValueToKey(char *key, const char *value);
int removeInstanceFromState(char *instanceID);
char *returnValueFromInstanceKey(char *InstanceID, char *key);
int setStatePath(char *path);
char *getStatePath();
