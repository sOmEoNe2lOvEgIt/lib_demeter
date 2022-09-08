/*
** ATOS PROJECT, 2022
** DEMETER
** File description:
** Wow, many include, such perfquery!
*/

#ifndef DEMETER_PERF_QUERRY_H_
#define DEMETER_PERF_QUERRY_H_

#include <inttypes.h>
#include "demeter.h"

#define ALL_PORTS 0xFF
#define MAX_PORTS 255

typedef struct perf_count {
	uint32_t portselect;
	uint32_t counterselect;
	uint32_t symbolerrors;
	uint32_t linkrecovers;
	uint32_t linkdowned;
	uint32_t rcverrors;
	uint32_t rcvremotephyerrors;
	uint32_t rcvswrelayerrors;
	uint32_t xmtdiscards;
	uint32_t xmtconstrainterrors;
	uint32_t rcvconstrainterrors;
	uint32_t linkintegrityerrors;
	uint32_t excbufoverrunerrors;
	uint32_t qp1dropped;
	uint32_t vl15dropped;
	uint32_t xmtdata;
	uint32_t rcvdata;
	uint32_t xmtpkts;
	uint32_t rcvpkts;
	uint32_t xmtwait;
} perf_data_t;

struct
{
	int reset, reset_only, all_ports, loop_ports, port, extended, xmt_sl,
		rcv_sl, xmt_disc, rcv_err, extended_speeds, smpl_ctl,
		oprcvcounters, flowctlcounters, vloppackets, vlopdata,
		vlxmitflowctlerrors, vlxmitcounters, swportvlcong, rcvcc,
		slrcvfecn, slrcvbecn, xmitcc, vlxmittimecc;
	int ports[MAX_PORTS];
	int ports_count;
} info;

// AGGREGATE

void aggregate_4bit(uint32_t * dest, uint32_t val);

void aggregate_8bit(uint32_t * dest, uint32_t val);

void aggregate_16bit(uint32_t * dest, uint32_t val);

void aggregate_32bit(uint32_t * dest, uint32_t val);

void aggregate_64bit(uint64_t * dest, uint32_t val);

// ALL GATHERED

perf_data_t *get_perf_data(demeter_conf_t *demeter_conf);

#endif /* !DEMETER_PERF_QUERRY_H_ */
