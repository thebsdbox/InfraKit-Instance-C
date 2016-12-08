/* InfraKitConsole.c

 *
 * InfraKit
 *
 * Copyright (C) 2016 <Daniel Finneran dan@thebsdbox.co.uk>
 * All rights reserved.
 *
 * This software may be modified and distributed under the terms
 * of the BSD license.  See the LICENSE file for details.
 */
#include "InfraKitConsole.h"
#include <stdio.h>

#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

int outputLevel = 4;

int setConsolOutputLevel(unsigned int level)
{
    if (level < 6) {
        outputLevel = level;
    }
    return EXIT_SUCCESS;
}

int ovPrintCritical(signed long pluginTime, char *message)
{
    if (outputLevel >= LOGCRITICAL) {
        printf(ANSI_COLOR_MAGENTA"CRIT[%05zu]"ANSI_COLOR_RESET" %s\n", pluginTime, message);
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

int ovPrintError(signed long pluginTime, char *message)
{
    if (outputLevel >= LOGERROR) {
        printf(ANSI_COLOR_RED"ERRO[%05zu]"ANSI_COLOR_RESET" %s\n", pluginTime, message);
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

int ovPrintWarning(signed long pluginTime, char *message)
{
    if (outputLevel >= LOGWARNING) {
        printf(ANSI_COLOR_YELLOW"WARN[%05zu]"ANSI_COLOR_RESET" %s\n", pluginTime, message);
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

int ovPrintNotice(signed long pluginTime, char *message)
{
    if (outputLevel >= LOGNOTICE) {
        printf(ANSI_COLOR_GREEN"NOTE[%05zu]"ANSI_COLOR_RESET" %s\n", pluginTime, message);
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

int ovPrintInfo(signed long pluginTime, char *message)
{
    if (outputLevel >= LOGINFO) {
        printf(ANSI_COLOR_BLUE"INFO[%05zu]"ANSI_COLOR_RESET" %s\n", pluginTime, message);
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}

int ovPrintDebug(signed long pluginTime, char *message)
{
    if (outputLevel >= LOGDEBUG) {
        printf(ANSI_COLOR_BLUE"DEBG[%05zu]"ANSI_COLOR_RESET" %s\n", pluginTime, message);
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}
