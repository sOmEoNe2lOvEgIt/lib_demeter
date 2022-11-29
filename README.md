
![hoe](https://user-images.githubusercontent.com/87118859/185426997-149c94e8-e33e-4621-b193-81594645686a.png)
# Demeter
Demeter lib is a lib used by the demeter slurm plugins made for harvesting data on your cluster's nodes like info about ram usage, oom state, parsed logs, etc...

Demeter gathers info by reading sys files just after a job ran as well as reading output logs to parse them into more digestible outputs.

Plugins that use this lib are a PREP and a TASK plugin both named demeter. You can find them on my github!

## Conf:
A conf file can be provided to the plugins if placed at /etc/slurm/demeter.conf.

This file has to be readable by the slurm user and has to be present on every slurmd's (slurm nodes) as well as on the slurm controller.s.

This is strictly optional but highly recommended as you will probably need to setup some file paths or your elastic database adress and index and such.

### This file can contain multiple parameters that you can define:

- Verbose: *"Verbose=(uint)"*. Sets the verbose level for the demeter logs.

- LogLevel: *"LogLevel=DEBUG||INFO||WARNING||ERROR||FATAL"*. Sets the logging level for demeter logs.

- LogStyle: *"LogStyle=FANCY||SIMPLE||SYSTEM"*. Sets the logging style for demeter logs.

- SlurmLogLevel: *"SlurmLogLevel=DEBUG||INFO||WARNING||ERROR||FATAL"*. Sets the minimum log level at which the plugin will parse slurm logs.

- SysLogLevel: *"SlurmLogLevel=DEBUG||INFO||WARNING||ERROR||FATAL"*. Sets the minimum log level at which the plugin will parse sys logs.

- LogFilePath: *"LogFilePath=path/to/an/existing/or/non/existing/file"*. Sets a custom path for the log file that demeter outputs in.

- SlurmLogPath: *"SlurmLogPath=path/to/an/existing/folder"*. Sets a custom path for the folder containing the slurm log file that demeter will parse.

- SysLogPath: *"SysLogPath=path/to/an/existing/folder"*. Sets a custom path for the folder containing the system log file that demeter will parse.

- DemeterCompLoc: *"DemeterCompLoc= protocol://link:port/to/the/elastic/database/and/index"*. Sets the link and port that demeter needs to talk with the Elasticsearch database and index the data that it gathered.

- DemeterCompProxy: *"DemeterCompProxy=proxyhost:port"*. Sets a proxy used by demeter to talk with the database if needed to access it. Can be ommited if not needed.

!!!NEW OPTIONS TO BE ADDED!!!

Note: In slurm conf, specifying the task plugin, it is better to put the task plugin first (in name order), as in cetain vertions of slurm, in the case of an OOM, the Cgroup task plugin stops the execution of any more plugins and prevents demeter gathering memory data (wich can be pretty important in the case of an OOM...). Specifying the demeter plugin first prevents this behaviour.

/!\ Task plugin doesn't work propperly with "AllowedRAMSpace" enabled in cgroup.conf in older slurm. /!\