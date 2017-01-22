
/* InfraKitPlugin.c
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
#include "InfraKitInstance.h"
#include "InfraKitPlugin.h"
#include "InfraKitState.h"
#include "InfraKitHTTPD.h"

#include <stdio.h>
#include <stdlib.h>
#include <strings.h>
#include <signal.h>
#include <sys/stat.h>
#include <time.h>
#include <limits.h>
#include <unistd.h>

#include "jansson.h"

char *argumentSocketPath;
char *argumentLogLevel;

char *socketName = NULL;
char *socketDefault = "instance-C";
char socketPath[PATH_MAX];

time_t pluginStartTime;

/* These functions will set an initial seconds since UNIX epoch
 * it will then use this inital time to provide a seconds that the plugin
 * has been executing for
 */

int setStartTime()
{
    pluginStartTime = time(NULL);
    if (pluginStartTime == -1) {
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}

signed long getPluginTime()
{
    time_t currentPluginTime = time(NULL);
    if (currentPluginTime !=1) {
        return currentPluginTime - pluginStartTime;
    }
    return 0;
}

int setSocketName(char *name)
{
    if (name) {
        socketName = name;
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}


int stringMatch(char *string1, char *string2)
{
    if ((string1) && (string2)) {
        while (*string1 == *string2) {
            // if both points in strings are null break out of loop
            if (*string1 == '\0' || *string2 == '\0') {
                break;
            }
            // move onto next character comparison
            string1++;
            string2++;
        }
        // Have we reached the end of the string whilst both have been the same value
        if (*string1 == '\0' && *string2 == '\0')
            return 1; // Return true (compiler)
        else
            return 0; // Return false
    }
    return 0;
}



/* This callback function will take the request data from the HTTPD server
 * process it and build a response and reponse code, that the web
 * server will then send to the client
 */

char *handlePostData(httpRequest *request)
{
    json_t *requestJSON = NULL;
    json_error_t error;
    
    requestJSON = json_loads(request->messageBody, 0, &error);
    if (requestJSON) {
        const char *methodName = json_string_value(json_object_get(requestJSON, "method"));
        long long id = json_integer_value(json_object_get(requestJSON, "id"));
        json_t *params = json_object_get(requestJSON, "params");
        const char *debugMessage = json_dumps(requestJSON, JSON_INDENT(3));
        ikPrintDebug(getPluginTime(), "Incoming Request =>");
        ikPrintDebug(getPluginTime(), (char*)debugMessage);
        if (stringMatch((char *)methodName, "Plugin.Implements")) {
            //{"jsonrpc":"2.0","result":{"APIs":[{"Name":"Instance","Version":"0.1.0"}]},"id":3618748489630577360}
            
            json_t *reponseJSON = json_pack("{s:s,s:{s:[{s:s,s:s}]},s:I}", "jsonrpc", "2.0", "result", "APIs", "Name", "Instance", "Version", "0.1.0", "id", id);
            char *test = json_dumps(reponseJSON, JSON_ENSURE_ASCII);
            setHTTPResponse(test, 200);
            json_decref(requestJSON);
            return NULL;
        }
        if (stringMatch((char *)methodName, "Instance.DescribeInstances")) {

            char *response = infraKitInstanceDescribe(params, id);
            ikPrintDebug(getPluginTime(), "Outgoing Response =>");
            ikPrintDebug(getPluginTime(), response);
            setHTTPResponse(response, 200);
            json_decref(requestJSON);
            return NULL;
        }
        if (stringMatch((char *)methodName, "Instance.Validate")) {
            
            char *response = infraKitInstanceValidate(params, id);
            ikPrintDebug(getPluginTime(), "Outgoing Response =>");
            ikPrintDebug(getPluginTime(), response);
            setHTTPResponse(response, 200);
            json_decref(requestJSON);
            return NULL;
        }
        if (stringMatch((char *)methodName, "Instance.Provision")) {
            
            json_t *spec = json_object_get(params, "Spec");
            char *response = infraKitInstanceProvision(spec, id);
            ikPrintDebug(getPluginTime(), "Outgoing Response =>");
            ikPrintDebug(getPluginTime(), response);
            setHTTPResponse(response, 200);
            json_decref(requestJSON);
            return NULL;
        }
        if (stringMatch((char *)methodName, "Instance.Destroy")) {
            char *response = infraKitInstanceDestroy(params, id);
            ikPrintDebug(getPluginTime(), "Outgoing Response =>");
            ikPrintDebug(getPluginTime(), response);
            setHTTPResponse(response, 200);
            json_decref(requestJSON);
            return NULL;
        }
        if (stringMatch((char *)methodName, "Instance.Meta")) {
            char *response = infraKitInstanceMeta(params, id);
            ikPrintDebug(getPluginTime(), "Outgoing Response =>");
            ikPrintDebug(getPluginTime(), response);
            setHTTPResponse(response, 200);
            json_decref(requestJSON);
            return NULL;
        }
        if (methodName) {
            ikPrintError(getPluginTime(), "Unknown JSON-RPC Method");
        }
        json_decref(requestJSON);
    }
    return NULL;
}

void handleInterrupt(int s){
    printf("Caught signal %d\n",(int)s);
    ikPrintError(getPluginTime(), "InfraKit-instance-C is exciting, removing UNIX Socket =>");
    ikPrintError(getPluginTime(), socketPath);
    unlink(socketPath);
}

void setInterruptHandler()
{
    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = handleInterrupt;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGINT, &sigIntHandler, NULL);
    
}

int infraKitCreateInstance()
{
    setInterruptHandler();
    
    if (setStartTime() != EXIT_SUCCESS) {
        return EXIT_FAILURE;
    }
    setConsolOutputLevel(LOGINFO);
    ikPrintInfo(getPluginTime(), "Starting InfraKit Instance Plugin");
    
    /* These two paths will build out to be the path for the socket and the state
     * we will build them out and ensure that the paths are fully created, including
     * the directory paths.
     */
    
    sprintf(socketPath, "%s/", getenv("HOME"));
    strcat(socketPath, ".infrakit/");
    int response = 0;
    response = mkdir(socketPath, 0755);
    if (response == -1) {
        ikPrintWarning(getPluginTime(), "Directory .InfraKit may already exist");
    }
    
    
    char *statePath = getStatePath();
    if (!statePath) {
        char buildStatePath[PATH_MAX];
        sprintf(buildStatePath, "%sstate/",socketPath);
        response = mkdir(buildStatePath, 0755);
        if (response == -1) {
            ikPrintWarning(getPluginTime(), "Directory .InfraKit/state/ may already exist");
        }
        strcat(buildStatePath, "instance.json");
        statePath = buildStatePath;
    }

    
    strcat(socketPath, "plugins/");
    response = mkdir(socketPath, 0755);
    if (response == -1) {
        ikPrintWarning(getPluginTime(), "Directory .InfraKit/plugins/ may already exist");
    }
    
    if (socketName) {
        strcat(socketPath, socketName);
    } else {
        strcat(socketPath, socketDefault);
    }
    
    
    ikPrintInfo(getPluginTime(), "Path for UNIX Socket =>");
    ikPrintInfo(getPluginTime(), socketPath);
    ikPrintInfo(getPluginTime(), "Path for State File =>");
    ikPrintInfo(getPluginTime(), statePath);
    
    setSocketPath(socketPath);
    setStatePath(statePath);
    
    SetPostFunction(handlePostData);

    start();
    return EXIT_SUCCESS;
}
