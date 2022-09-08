// ATOS PROJECT, 2022
// DEMETER
// File description:
// Wow, such perf, many querry!
// (this looks complicated...)
//___________________________________________________________________________________________________________________________________________

// LIBC INCLUDES
#include <stdio.h>

// IB INCLUDES
#include <infiniband/mad.h>

// EXTRA INCLUDES
#include "demeter.h"
#include "demeter_perf_querry.h"

static void aggregate_perfcounters(perf_data_t *perf_count)
{
	static uint32_t val;
	static uint8_t pc[1024];

	mad_decode_field(pc, IB_PC_PORT_SELECT_F, &val);
	perf_count->portselect = val;
	mad_decode_field(pc, IB_PC_COUNTER_SELECT_F, &val);
	perf_count->counterselect = val;
	mad_decode_field(pc, IB_PC_ERR_SYM_F, &val);
	aggregate_16bit(&perf_count->symbolerrors, val);
	mad_decode_field(pc, IB_PC_LINK_RECOVERS_F, &val);
	aggregate_8bit(&perf_count->linkrecovers, val);
	mad_decode_field(pc, IB_PC_LINK_DOWNED_F, &val);
	aggregate_8bit(&perf_count->linkdowned, val);
	mad_decode_field(pc, IB_PC_ERR_RCV_F, &val);
	aggregate_16bit(&perf_count->rcverrors, val);
	mad_decode_field(pc, IB_PC_ERR_PHYSRCV_F, &val);
	aggregate_16bit(&perf_count->rcvremotephyerrors, val);
	mad_decode_field(pc, IB_PC_ERR_SWITCH_REL_F, &val);
	aggregate_16bit(&perf_count->rcvswrelayerrors, val);
	mad_decode_field(pc, IB_PC_XMT_DISCARDS_F, &val);
	aggregate_16bit(&perf_count->xmtdiscards, val);
	mad_decode_field(pc, IB_PC_ERR_XMTCONSTR_F, &val);
	aggregate_8bit(&perf_count->xmtconstrainterrors, val);
	mad_decode_field(pc, IB_PC_ERR_RCVCONSTR_F, &val);
	aggregate_8bit(&perf_count->rcvconstrainterrors, val);
	mad_decode_field(pc, IB_PC_ERR_LOCALINTEG_F, &val);
	aggregate_4bit(&perf_count->linkintegrityerrors, val);
	mad_decode_field(pc, IB_PC_ERR_EXCESS_OVR_F, &val);
	aggregate_4bit(&perf_count->excbufoverrunerrors, val);
	mad_decode_field(pc, IB_PC_QP1_DROP_F, &val);
	aggregate_16bit(&perf_count->qp1dropped, val);
	mad_decode_field(pc, IB_PC_VL15_DROPPED_F, &val);
	aggregate_16bit(&perf_count->vl15dropped, val);
	mad_decode_field(pc, IB_PC_XMT_BYTES_F, &val);
	aggregate_32bit(&perf_count->xmtdata, val);
	mad_decode_field(pc, IB_PC_RCV_BYTES_F, &val);
	aggregate_32bit(&perf_count->rcvdata, val);
	mad_decode_field(pc, IB_PC_XMT_PKTS_F, &val);
	aggregate_32bit(&perf_count->xmtpkts, val);
	mad_decode_field(pc, IB_PC_RCV_PKTS_F, &val);
	aggregate_32bit(&perf_count->rcvpkts, val);
	mad_decode_field(pc, IB_PC_XMT_WAIT_F, &val);
	aggregate_32bit(&perf_count->xmtwait, val);
}

// static void aggregate_perfcounters_ext(__be16 cap_mask, uint32_t cap_mask2)
// {
// 	uint32_t val;
// 	uint64_t val64;

// 	mad_decode_field(pc, IB_PC_EXT_PORT_SELECT_F, &val);
// 	perf_count_ext.portselect = val;
// 	mad_decode_field(pc, IB_PC_EXT_COUNTER_SELECT_F, &val);
// 	perf_count_ext.counterselect = val;
// 	mad_decode_field(pc, IB_PC_EXT_XMT_BYTES_F, &val64);
// 	aggregate_64bit(&perf_count_ext.portxmitdata, val64);
// 	mad_decode_field(pc, IB_PC_EXT_RCV_BYTES_F, &val64);
// 	aggregate_64bit(&perf_count_ext.portrcvdata, val64);
// 	mad_decode_field(pc, IB_PC_EXT_XMT_PKTS_F, &val64);
// 	aggregate_64bit(&perf_count_ext.portxmitpkts, val64);
// 	mad_decode_field(pc, IB_PC_EXT_RCV_PKTS_F, &val64);
// 	aggregate_64bit(&perf_count_ext.portrcvpkts, val64);

// 	if (cap_mask & IB_PM_EXT_WIDTH_SUPPORTED) {
// 		mad_decode_field(pc, IB_PC_EXT_XMT_UPKTS_F, &val64);
// 		aggregate_64bit(&perf_count_ext.portunicastxmitpkts, val64);
// 		mad_decode_field(pc, IB_PC_EXT_RCV_UPKTS_F, &val64);
// 		aggregate_64bit(&perf_count_ext.portunicastrcvpkts, val64);
// 		mad_decode_field(pc, IB_PC_EXT_XMT_MPKTS_F, &val64);
// 		aggregate_64bit(&perf_count_ext.portmulticastxmitpkits, val64);
// 		mad_decode_field(pc, IB_PC_EXT_RCV_MPKTS_F, &val64);
// 		aggregate_64bit(&perf_count_ext.portmulticastrcvpkts, val64);
// 	}

// 	if (htonl(cap_mask2) & IB_PM_IS_ADDL_PORT_CTRS_EXT_SUP) {
// 		mad_decode_field(pc, IB_PC_EXT_COUNTER_SELECT2_F, &val);
// 		perf_count_ext.counterSelect2 = val;
// 		mad_decode_field(pc, IB_PC_EXT_ERR_SYM_F, &val64);
// 		aggregate_64bit(&perf_count_ext.symbolErrorCounter, val64);
// 		mad_decode_field(pc, IB_PC_EXT_LINK_RECOVERS_F, &val64);
// 		aggregate_64bit(&perf_count_ext.linkErrorRecoveryCounter, val64);
// 		mad_decode_field(pc, IB_PC_EXT_LINK_DOWNED_F, &val64);
// 		aggregate_64bit(&perf_count_ext.linkDownedCounter, val64);
// 		mad_decode_field(pc, IB_PC_EXT_ERR_RCV_F, &val64);
// 		aggregate_64bit(&perf_count_ext.portRcvErrors, val64);
// 		mad_decode_field(pc, IB_PC_EXT_ERR_PHYSRCV_F, &val64);
// 		aggregate_64bit(&perf_count_ext.portRcvRemotePhysicalErrors, val64);
// 		mad_decode_field(pc, IB_PC_EXT_ERR_SWITCH_REL_F, &val64);
// 		aggregate_64bit(&perf_count_ext.portRcvSwitchRelayErrors, val64);
// 		mad_decode_field(pc, IB_PC_EXT_XMT_DISCARDS_F, &val64);
// 		aggregate_64bit(&perf_count_ext.portXmitDiscards, val64);
// 		mad_decode_field(pc, IB_PC_EXT_ERR_XMTCONSTR_F, &val64);
// 		aggregate_64bit(&perf_count_ext.portXmitConstraintErrors, val64);
// 		mad_decode_field(pc, IB_PC_EXT_ERR_RCVCONSTR_F, &val64);
// 		aggregate_64bit(&perf_count_ext.portRcvConstraintErrors, val64);
// 		mad_decode_field(pc, IB_PC_EXT_ERR_LOCALINTEG_F, &val64);
// 		aggregate_64bit(&perf_count_ext.localLinkIntegrityErrors, val64);
// 		mad_decode_field(pc, IB_PC_EXT_ERR_EXCESS_OVR_F, &val64);
// 		aggregate_64bit(&perf_count_ext.excessiveBufferOverrunErrors, val64);
// 		mad_decode_field(pc, IB_PC_EXT_VL15_DROPPED_F, &val64);
// 		aggregate_64bit(&perf_count_ext.VL15Dropped, val64);
// 		mad_decode_field(pc, IB_PC_EXT_XMT_WAIT_F, &val64);
// 		aggregate_64bit(&perf_count_ext.portXmitWait, val64);
// 		mad_decode_field(pc, IB_PC_EXT_QP1_DROP_F, &val64);
// // 		aggregate_64bit(&perf_count_ext.QP1Dropped, val64);
// 	}
// }

perf_data_t *get_perf_data(demeter_conf_t *demeter_conf)
{	
	perf_data_t *perf_count = NULL;
    char debug_message[30];

	perf_count = malloc(sizeof(perf_data_t));
	if (perf_count == NULL)
		return NULL;
	write_log_to_file(demeter_conf, "Gathering perf data.", DEBUG, 3);
    aggregate_perfcounters(perf_count);
    sprintf(debug_message, "PORT: %u", perf_count->portselect);
    write_log_to_file(demeter_conf, debug_message, INFO, 0);
	free (perf_count);
	write_log_to_file(demeter_conf, "Gathered perf data.", DEBUG, 3);
	return NULL;
}