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

char *infraKitInstanceDescribe(json_t *params, long long id);
char *infraKitInstanceProvision(json_t *params, long long id);
char *infraKitInstanceDestroy(json_t *params, long long id);
char *infraKitInstanceValidate(json_t *params, long long id);
char *infraKitInstanceMeta(json_t *params, long long id);
