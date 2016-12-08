# InfraKit-Instance-C
This is a demo InfraKit Plugin written in C

This plugin was created by removing vendor specific code from an internal proof of concept, so some of the function names need changing to ensure that this plugin is completely neutral. The current functionality of the plugin is identical to the Infrakit-instance-file, in that it will create instances logically and store them in a file. The difference being that this plugin will store all of the instances in a single json file, and new instances can be appended to the end of the JSON array. 

To build:

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
There are still a lot of functions that will need renaming, along with ensuring that there is no futher vendor specific code laying around. Also there is a memory leak in the HTTPD server (simple fix). 