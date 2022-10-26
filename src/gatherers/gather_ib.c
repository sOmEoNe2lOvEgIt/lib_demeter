#include <stdio.h>
#include <mad.h>
#include <umad.h>
#include <inttypes.h>
#include <string.h>

#include "src/common/log.h"
#include "gather_ib.h"

uint ibd_timeout = 20;
struct ibmad_port *srcport;
struct info_s info;

// MAIN AGGREGATOR
//______________________________________________________________________________

static void aggregate_perfcounters(perf_data_t *perf_count, uint8_t pc[])
{
    static uint32_t val;

    mad_decode_field(pc, IB_PC_EXT_PORT_SELECT_F, &val);
    perf_count->portselect = val;
    mad_decode_field(pc, IB_PC_COUNTER_SELECT_F, &val);
    perf_count->counterselect = val;
    mad_decode_field(pc, IB_PC_ERR_SYM_F, &val);
    perf_count->symbolerrors = val;
    mad_decode_field(pc, IB_PC_LINK_RECOVERS_F, &val);
    perf_count->linkrecovers = val;
    mad_decode_field(pc, IB_PC_LINK_DOWNED_F, &val);
    perf_count->linkdowned = val;
    mad_decode_field(pc, IB_PC_ERR_RCV_F, &val);
    perf_count->rcverrors = val;
    mad_decode_field(pc, IB_PC_ERR_PHYSRCV_F, &val);
    perf_count->rcvremotephyerrors = val;
    mad_decode_field(pc, IB_PC_ERR_SWITCH_REL_F, &val);
    perf_count->rcvswrelayerrors = val;
    mad_decode_field(pc, IB_PC_XMT_DISCARDS_F, &val);
    perf_count->xmtdiscards = val;
    mad_decode_field(pc, IB_PC_ERR_XMTCONSTR_F, &val);
    perf_count->xmtconstrainterrors = val;
    mad_decode_field(pc, IB_PC_ERR_RCVCONSTR_F, &val);
    perf_count->rcvconstrainterrors = val;
    mad_decode_field(pc, IB_PC_ERR_LOCALINTEG_F, &val);
    perf_count->linkintegrityerrors = val;
    mad_decode_field(pc, IB_PC_ERR_EXCESS_OVR_F, &val);
    perf_count->excbufoverrunerrors = val;
    mad_decode_field(pc, IB_PC_QP1_DROP_F, &val);
    perf_count->qp1dropped = val;
    mad_decode_field(pc, IB_PC_VL15_DROPPED_F, &val);
    perf_count->vl15dropped = val;
    mad_decode_field(pc, IB_PC_XMT_BYTES_F, &val);
    perf_count->xmtdata = val;
    mad_decode_field(pc, IB_PC_RCV_BYTES_F, &val);
    perf_count->rcvdata = val;
    mad_decode_field(pc, IB_PC_XMT_PKTS_F, &val);
    perf_count->xmtpkts = val;
    mad_decode_field(pc, IB_PC_RCV_PKTS_F, &val);
    perf_count->rcvpkts = val;
    mad_decode_field(pc, IB_PC_XMT_WAIT_F, &val);
    perf_count->xmtwait = val;
}

static void aggregate_ext_perfcounters(perf_data_t *perf_count, uint8_t pc[])
{
    static u_int32_t val;

    mad_decode_field(pc, IB_PC_RCV_LOCAL_PHY_ERR_F, &val);
    perf_count->portlocalphysicalerrors = val;
    mad_decode_field(pc, IB_PC_RCV_MALFORMED_PKT_ERR_F, &val);
    perf_count->portmalformedpkterrors = val;
    mad_decode_field(pc, IB_PC_RCV_BUF_OVR_ERR_F, &val);
    perf_count->portbufferoverrunerrors = val;
    mad_decode_field(pc, IB_PC_RCV_DLID_MAP_ERR_F, &val);
    perf_count->portdlidmappingerrors = val;
    mad_decode_field(pc, IB_PC_RCV_VL_MAP_ERR_F, &val);
    perf_count->portvlmappingerrors = val;
    mad_decode_field(pc, IB_PC_RCV_LOOPING_ERR_F, &val);
    perf_count->portloopingerrors = val;
}

// DUMPER
//______________________________________________________________________________

static void dump_perfcounters(ib_portid_t * portid, int port, perf_data_t *perf_count)
{
    static uint8_t pc[1024];

    memset(pc, 0, sizeof(pc));
    if (!pma_query_via(pc, portid, port, ibd_timeout, IB_GSI_PORT_COUNTERS, srcport)) {
        debug2("demeter: perfquery failed");
        return;
    }
    aggregate_perfcounters(perf_count, pc);
    memset(pc, 0, sizeof(pc));
    if (!pma_query_via(pc, portid, port, ibd_timeout, IB_GSI_PORT_RCV_ERROR_DETAILS, srcport)){
        debug2("demeter: extperfquery failed");
        return;
    }
    aggregate_ext_perfcounters(perf_count, pc);
}

// RESOLVE SELF
//______________________________________________________________________________

static int resolve_self(char *ca_name, uint8_t ca_port, ib_portid_t *portid, int *portnum)
{
    umad_port_t port;
    int rc;

    if (!(portid || portnum))
        return (21);
    if ((rc = umad_get_port(ca_name, ca_port, &port)) < 0)
        return rc;
    if (portid) {
        memset(portid, 0, sizeof(*portid));
        portid->lid = port.base_lid;
        portid->sl = port.sm_sl;
    }
    if (portnum)
        *portnum = port.portnum;
    umad_release_port(&port);
    return (0);
}

// MAIN
//______________________________________________________________________________

perf_data_t *gather_ib(void)
{
    perf_data_t *perf_count = NULL;
    char *ibd_ca = NULL;
    int ibd_ca_port = 1, mgmt_classes[3] = { IB_SMI_CLASS, IB_SA_CLASS, IB_PERFORMANCE_CLASS };
    ib_portid_t portid;

    perf_count = malloc(sizeof(perf_data_t));
    if (!perf_count) {
        debug2("demeter: malloc failed");
        return (NULL);
    }
    if (resolve_self(ibd_ca, ibd_ca_port, &portid, &info.port) < 0) {
        debug2("demeter: resolve_self failed");
        free(perf_count);
        return (NULL);
    }
    srcport = mad_rpc_open_port(ibd_ca, ibd_ca_port, mgmt_classes, 3);
    if (!srcport) {
        debug2("demeter: mad_rpc_open_port failed");
        free(perf_count);
        return (NULL);
    }
    perf_count->portlocalphysicalerrors = 3600;
    dump_perfcounters(&portid, 1, perf_count);
    mad_rpc_close_port(srcport);
    return (perf_count);
}


perf_data_t *gather_ib_diff(perf_data_t *prolog_perf_count)
{
    perf_data_t *epilog_perf_count = NULL, *res = NULL;

    if (!prolog_perf_count)
        return (NULL);
    epilog_perf_count = gather_ib();
    if (!epilog_perf_count)
        return (NULL);
    perf_data_t diff_perf_count = {epilog_perf_count->portselect, epilog_perf_count->counterselect, (epilog_perf_count->symbolerrors - prolog_perf_count->symbolerrors),
    (epilog_perf_count->linkrecovers - prolog_perf_count->linkrecovers),
    (epilog_perf_count->linkdowned - prolog_perf_count->linkdowned),
    (epilog_perf_count->rcverrors - prolog_perf_count->rcverrors),
    (epilog_perf_count->rcvremotephyerrors - prolog_perf_count->rcvremotephyerrors),
    (epilog_perf_count->rcvswrelayerrors - prolog_perf_count->rcvswrelayerrors),
    (epilog_perf_count->xmtdiscards - prolog_perf_count->xmtdiscards),
    (epilog_perf_count->xmtconstrainterrors - prolog_perf_count->xmtconstrainterrors),
    (epilog_perf_count->rcvconstrainterrors - prolog_perf_count->rcvconstrainterrors),
    (epilog_perf_count->linkintegrityerrors - prolog_perf_count->linkintegrityerrors),
    (epilog_perf_count->excbufoverrunerrors - prolog_perf_count->excbufoverrunerrors),
    (epilog_perf_count->qp1dropped - prolog_perf_count->qp1dropped),
    (epilog_perf_count->vl15dropped - prolog_perf_count->vl15dropped),
    (epilog_perf_count->xmtdata - prolog_perf_count->xmtdata),
    (epilog_perf_count->rcvdata - prolog_perf_count->rcvdata),
    (epilog_perf_count->xmtpkts - prolog_perf_count->xmtpkts),
    (epilog_perf_count->rcvpkts - prolog_perf_count->rcvpkts),
    (epilog_perf_count->xmtwait - prolog_perf_count->xmtwait),
    (epilog_perf_count->portlocalphysicalerrors - prolog_perf_count->portlocalphysicalerrors),
    (epilog_perf_count->portmalformedpkterrors - prolog_perf_count->portmalformedpkterrors),
    (epilog_perf_count->portbufferoverrunerrors - prolog_perf_count->portbufferoverrunerrors),
    (epilog_perf_count->portdlidmappingerrors - prolog_perf_count->portdlidmappingerrors),
    (epilog_perf_count->portvlmappingerrors - prolog_perf_count->portvlmappingerrors),
    (epilog_perf_count->portloopingerrors - prolog_perf_count->portloopingerrors)};
    res = malloc(sizeof(perf_data_t));
    if (!res)
        return (NULL);
    memcpy(res, &diff_perf_count, sizeof(perf_data_t));
    free(epilog_perf_count);
    return (res);
}