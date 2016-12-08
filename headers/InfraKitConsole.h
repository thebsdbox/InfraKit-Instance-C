/* InfraKitConsole.h
 *
 * InfraKit
 *
 * Copyright (C) 2016 <Daniel Finneran dan@thebsdbox.co.uk>
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

#define LOGCRITICAL     0
#define LOGERROR        1
#define LOGWARNING      2
#define LOGNOTICE       3
#define LOGINFO         4 // Default
#define LOGDEBUG        5

int setConsolOutputLevel(unsigned int level);

int ovPrintCritical(signed long pluginTime, char *message);
int ovPrintError(signed long pluginTime, char *message);
int ovPrintWarning(signed long pluginTime, char *message);
int ovPrintNotice(signed long pluginTime, char *message);
int ovPrintInfo(signed long pluginTime, char *message);
int ovPrintDebug(signed long pluginTime, char *message);
