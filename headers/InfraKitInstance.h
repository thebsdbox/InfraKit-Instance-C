/* InfraKitInstance.h
 *
 * InfraKit
 *
 * Copyright (C) 2016 <Daniel Finneran dan@thebsdbox.co.uk>
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#include "jansson.h"

#ifndef PROFILE_H
#define PROFILE_H

typedef struct {
    char *profileName;          // pointer to name of Profile
    char *templateName;         // pointer to name of Template Profile
    char *uri;                  // pointer to uri of Profile
    char *availableHardwareURI; // pointer to uri of available Hardware
    char *hardwareTypeUri;      // pointer to hardwareURI string of profile
    char *enclosureUri;         // pointer to hardwareURI string of profile
    
} profile;

#endif

char *infraKitInstanceDescribe(json_t *params, long long id);
char *infraKitInstanceProvision(json_t *params, long long id);
char *infraKitInstanceDestroy(json_t *params, long long id);
