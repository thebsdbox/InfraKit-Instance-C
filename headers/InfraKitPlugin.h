/* InfraKitPlugin.h
 *
 * InfraKit
 *
 * Copyright (C) 2016 <Daniel Finneran dan@thebsdbox.co.uk>
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

int infraKitCreateInstance();

int processInstanceJSON(char *json_text);
int setSocketName(char *name);
int stringMatch(char *string1, char *string2);
signed long getPluginTime();


