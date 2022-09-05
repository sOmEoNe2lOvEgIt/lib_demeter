##
## ATOS PROJECT 2022
## MAKEFILE
## File description:
## Wow, such make, much file!
##

SLURM_ROOT_DIR = /usr
SLURM_INC_DIR = /root/SLURM/slurm.build
SLURM_LIB_DIR = /usr/lib64/slurm
SLURM_BUILD = 21.08.8-2
SLURM_BUILD_DIR = /root/rpmbuild/BUILD/slurm-$(SLURM_BUILD)

PLUGIN_TYPE = prep
PLUGIN_NAME = demeter
PLUGIN_FILE = $(PLUGIN_TYPE)_$(PLUGIN_NAME).so
LIB_FILE = lib$(PLUGIN_NAME).so

SRC_FILES = src/gatherers/gather_cgroup.c					\
				src/gatherers/cgroup/get_from_files.c		\
			src/loggers/logger.c							\
				src/loggers/cgroup/log_cgroup.c				\
				src/loggers/parsed_logs/log_parsed_logs.c	\
				src/loggers/parsed_sel/log_sel.c			\
			src/tools/is_log_empty.c						\
			src/tools/read_conf.c							\
			src/tools/linked_list.c							\
			src/tools/handle_log_level.c					\
				src/tools/free/im_free.c					\
				src/tools/get/get_job_info.c				\
				src/tools/get/get_time_str.c				\

CC      = gcc
CFLAGS  ?= -Wall -fPIC -g -I$(SLURM_INC_DIR) -I$(SLURM_BUILD_DIR) -Iinclude
LDFLAGS ?= -shared -L.

all: $(LIB_FILE)

default: $(LIB_FILE)

$(LIB_FILE): $(SRC_FILES)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

send: all
	scp $(LIB_FILE) my_vm:/home/compose_fake_taranis/shared/

clean:
	rm -f $(LIB_FILE)

re: clean all