##
## ATOS PROJECT 2022
## MAKEFILE
## File description:
## Wow, such make, much file!
##

SLURM_INC_DIR		=	/root/SLURM/slurm.build
IBMAD_INC_DIR		=	/usr/include/infiniband/
LIB_SLURM			=	/usr/lib64/slurm

LIB_FILE			=	libdemeter.so

GATHER_SRC =src/gatherers/gather_cgroup.c							\
			src/gatherers/gather_logs.c								\
			src/gatherers/gather_sel.c								\
			src/gatherers/gather_ib.c								\
			src/gatherers/gather_sstat.c							\
			src/gatherers/cgroup/get_from_files.c					\
			src/gatherers/logs/gather_system_logs.c					\
			src/gatherers/logs/sys_log_tools.c						\
			src/gatherers/logs/gather_slurm_logs.c					\
			src/gatherers/logs/slurm_logs_tools.c					\
			src/gatherers/logs/get_log_time.c						\
			src/gatherers/sel/handle_sel.c							\

LOGER_SRC =	src/loggers/logger.c									\
			src/loggers/cgroup/log_cgroup.c							\
			src/loggers/cgroup/unlog_cgroup.c						\
			src/loggers/send_elasticsearch/json_formatters.c		\
			src/loggers/send_elasticsearch.c						\

TOOLS_SRC = src/tools/is_log_empty.c								\
			src/tools/strptime.c									\
			src/tools/read_conf.c									\
			src/tools/linked_list.c									\
			src/tools/handle_log_level.c							\
			src/tools/remove_newline.c								\
			src/tools/append_str.c									\
			src/tools/getline_from_end.c							\
			src/tools/init/im_init.c								\
			src/tools/free/im_free.c								\
			src/tools/get/get_job_info.c							\
			src/tools/get/get_time_str.c							\
			src/tools/get/get_len_to_char.c							\
			src/tools/get/get_job_transfer_path.c					\
			src/tools/get/get_hostname.c							\
			src/tools/get/zgetline.c								\
			src/tools/get/get_rotate_time.c							\
			src/tools/nodeset/get_nodeset.c							\
			src/tools/nodeset/is_in_nodeset.c						\

CC		=	gcc
CFLAGS	?=	-Wall -g3 -gstrict-dwarf -fPIC -Iinclude -I$(SLURM_INC_DIR) -I$(IBMAD_INC_DIR)
LDFLAGS	?=	-shared -lcurl -libmad -libumad -L$(LIB_SLURM) -lslurmfull

all:			$(LIB_FILE)

default:		$(LIB_FILE)

$(LIB_FILE):	$(GATHER_SRC) $(LOGER_SRC) $(TOOLS_SRC)
		$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

send:	all
		scp $(LIB_FILE) my_vm:/home/compose_fake_taranis/shared/

clean:
		rm -f $(LIB_FILE)

re:		clean all
