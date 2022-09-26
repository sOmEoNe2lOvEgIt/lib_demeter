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
LIBIBMAD_DIR = /usr/lib64/

LIB_FILE = libdemeter.so

SRC_FILES = src/gatherers/gather_cgroup.c						\
			src/gatherers/gather_logs.c							\
			src/gatherers/gather_sel.c							\
			src/gatherers/gather_ib.c							\
				src/gatherers/cgroup/get_from_files.c			\
				src/gatherers/logs/gather_kernel_logs.c			\
				src/gatherers/logs/gather_slurm_logs.c			\
				src/gatherers/logs/read_logs.c					\
				src/gatherers/logs/get_log_time.c				\
				src/gatherers/sel/handle_sel.c					\
			src/loggers/logger.c								\
				src/loggers/cgroup/log_cgroup.c					\
				src/loggers/cgroup/unlog_cgroup.c				\
				src/loggers/parsed_logs/log_parsed_logs.c		\
				src/loggers/parsed_sel/log_sel.c				\
				src/loggers/send_elasticsearch/json_formatters.c\
			src/loggers/send_elasticsearch.c					\
			src/tools/is_log_empty.c							\
			src/tools/read_conf.c								\
			src/tools/linked_list.c								\
			src/tools/handle_log_level.c						\
			src/tools/remove_newline.c							\
			src/tools/append_str.c								\
				src/tools/free/im_free.c						\
				src/tools/get/get_job_info.c					\
				src/tools/get/get_time_str.c					\
				src/tools/get/get_len_to_char.c					\
				src/tools/get/get_job_transfer_path.c			\

CC      = gcc
CFLAGS  ?= -Wall -fPIC -g3 -gstrict-dwarf -Iinclude -I$(SLURM_INC_DIR) -I$(SLURM_BUILD_DIR) -I/usr/include/infiniband/
LDFLAGS ?= -shared -lcurl -L. -L$(LIBIBMAD_DIR) -libmad -libumad

all: $(LIB_FILE)

default: $(LIB_FILE)

$(LIB_FILE): $(SRC_FILES)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

send: all
	scp $(LIB_FILE) my_vm:/home/compose_fake_taranis/shared/

clean:
	rm -f $(LIB_FILE)

re: clean all