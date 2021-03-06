#include "InfraKitPlugin.h"
#include "InfraKitState.h"
#include "InfraKitConsole.h"

#include <getopt.h>
#include <stdio.h>

static struct option long_options[] =
{
    {"name", required_argument, NULL, 'n'},
    {"state", required_argument, NULL, 's'},
    {"log", required_argument, NULL, 'l'},
    {"help", optional_argument, NULL, 'h'},
    {NULL, 0, NULL, 0}
};


int main(int argc, char* argv[])
{

    int ch;
    int logLevel = 4;
    char *statepath, *socketName;
    if (argc >= 2) {
        if (stringMatch("version", argv[1])) {
            printf("\nInfraKit Instance Plugin version 0.10.0\nContact dan@thebsdbox.co.uk\n\n");
            return 0;
        }
    }
    while ((ch = getopt_long(argc, argv, "n:s:l:h:", long_options, NULL)) != -1)
    {
        // check to see if a single character or long option came through
        switch (ch)
        {
                // short option 't'
            case 'n':
                socketName = optarg;
                if (socketName) {
                    setSocketName(socketName);
                }
                break;
                // short option 'a'
            case 's':
                statepath = optarg;
                if (statepath) {
                    setStatePath(statepath);
                }
                break;
            case 'l':
                logLevel = atoi(optarg);
                if (logLevel < 6) {
                    setConsolOutputLevel(logLevel);
                } else {
                    printf("\nError incorrect log level, maximum 5");
                    return -1;
                }
                break;
            case 'h':
                printf("InfraKit Instance Plugin for Docker\n\n Usage:\n ./infrakit-instance-c [flags]\n\n Available Commands:\n version\t\t print build version information\n\n Flags:\n\t--name\tPlugin name to advertise\n\t--log\tLogging level, maximum 5 being the most verbose\n\t--state\tPath to a state file to handle instance state information\n\n");
                return 0;
                break;
        }
    }
    
    infraKitCreateInstance();
    return 0;
}

