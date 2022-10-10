![hoe](https://user-images.githubusercontent.com/87118859/185426997-149c94e8-e33e-4621-b193-81594645686a.png)
# Demeter
prep_demeter is a slurm plugin made for harvesting data on your cluster's nodes like info about ram usage, oom state, parsed logs, etc...

Demeter gathers info by reading sys files just after a job ran as well as reading output logs to parse them into more digestible outputs.

This plugin may be used in pair whith "task_demeter" plugin so that you can gather more accurate information if not any.

## Conf:
A conf file can be provided to the plugin if placed at /etc/slurm/demeter.conf.

This file has to be readable by the slurm user and has to be present on every slurmd as well as the slurm controller.

This is strictly optional but highly recommended as you will probably need to setup some file paths or your elastic database adress and index.

### This file can contain multiple parameters that you can define:

Verbose <---------- syntax: *"Verbose=(uint)"*. Sets the verbose level for the demeter logs.

LogLevel <--------- syntax: *"LogLevel=DEBUG||INFO||WARNING||ERROR||FATAL"*. Sets the logging level for demeter logs.

LogStyle <--------- syntax: *"LogStyle=FANCY||SIMPLE||SYSTEM"*. Sets the logging style for demeter logs.

SlurmLogLevel <-- syntax: *"SlurmLogLevel=DEBUG||INFO||WARNING||ERROR||FATAL"*. Sets the minimum log level at which the plugin will parse slurm logs.

SysLogLevel <------ syntax: *"SlurmLogLevel=DEBUG||INFO||WARNING||ERROR||FATAL"*. Sets the minimum log level at which the plugin will parse sys logs.

LogFilePath <----- syntax: *"LogFilePath=path/to/an/existing/or/non/existing/file"*. Sets a custom path for the log file that demeter outputs in.

SlurmLogPath <----- syntax: *"SlurmLogPath=path/to/an/existing/folder"*. Sets a custom path for the folder containing the slurm log file that demeter will parse.

SysLogPath <------- syntax: *"SysLogPath=path/to/an/existing/folder"*. Sets a custom path for the folder containing the system log file that demeter will parse.

DemeterCompLoc <--- syntax: *"DemeterCompLoc=http://link:port/to/the/elastic/database/and/index"*. Sets the link and port that demeter needs to talk with the Elasticsearch database and index the data that it gathered.

DemeterCompProxy <- syntax: *"DemeterCompProxy=proxyhost:port"*. Sets a proxy used by demeter to talk with the database if needed to access it. Can be ommited if not needed.


!!!NEW OPTIONS TO BE ADDED!!!
