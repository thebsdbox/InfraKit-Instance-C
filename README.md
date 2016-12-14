# InfraKit-Instance-C
This is a demo InfraKit Plugin written in C

This plugin was created by removing vendor specific code from an internal proof of concept, so some of the function names need changing to ensure that this plugin is completely neutral. The current functionality of the plugin is identical to the Infrakit-instance-file, in that it will create instances logically and store them in a file. The difference being that this plugin will store all of the instances in a single json file, and new instances can be appended to the end of the JSON array. 

## Adding to the plugin

All of the key areas where code will need adding have been prefixed in the comments with `CODE HERE` and these areas are typically where code will be needed to allow the infrakit-instance plugin to speak to a provider to either:

* Report the status of instances (instance count, health)
* Provision a new instance
* Destroy an instance or group of instances

### State data

One other area that needs to be considered is the `int synchroniseStateWithProvider()` function, as this function provides the capability to speak to your instance provider and determine if the plugin state matches. It will iterate through the state data and then compare each instance to the state from the provider. If also is designed around physical devices so it's possible to move a device from the active instances to the non-functional instance array and vice-versa. When an instance is non-functional it wont be reported back to InfraKit, therefore a new instance will be provisioned. When an instance is moved from non-functional to active instances, it will be added to the front of the array meaning that the last added instance will be destroyed.

## To build:

* Grab the source, either download the zip or git pull
* run the `./build_libjansson.sh` to download the jansson JSON library and build it for your Architecture
* run `make`

You'll be left with a infrakit-instance-c that will start your plugin, for further help run `./infrakit-instance-c --help`


```
$ ./infrakit-instance-c --help
InfraKit Instance Plugin for Docker

 Usage:
 ./infrakit-instance-c [flags]

 Available Commands:
 version		 print build version information

 Flags:
	--name	Plugin name to advertise
	--log	Logging level, maximum 5 being the most verbose
	--state	Path to a state file to handle instance state information
```

#### ToDo
All Vendor code has been removed!

However there is a small memory leak in the HTTPD server (simple fix). 