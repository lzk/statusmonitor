#ifndef HAVE_STRTOULL
#define HAVE_STRTOULL 1
#endif
#include <net-snmp/net-snmp-config.h>
#include <net-snmp/net-snmp-includes.h>
#include "log.h"
//enterprises.2699.1.2.1.2.1.1.3.1
//enterprises.26266.86.10.1.1.1.1.0
static const oid oidName[] = {1,3,6,1,4,1,2699,1,2,1,2,1,1,3,1};
static const size_t sizeofOidName = sizeof(oidName)/sizeof(oidName[0]);
int snmpGetResponse(char* ip ,char* buffer ,int bufsize)
{
//    LOGLOG("ip is:%s" ,ip);
    bool ret = -1;
    netsnmp_session session;
//    init_snmp ("snmp");
    snmp_sess_init (&session);
    session.version = SNMP_VERSION_2c;
    session.community = (u_char*)"public";
    session.community_len = strlen ((const char*)session.community);
    session.peername = ip;//被监控主机的IP地址
    session.timeout = 5 *1000 * 1000;
    netsnmp_session *ss;
    netsnmp_pdu *pdu;
    ss = snmp_open (&session);
    int liberr,syserr;
    char* errstr;
    if (ss == NULL){
//        snmp_sess_perror ("snmp_open", &session);
        snmp_error(&session ,&liberr ,&syserr ,&errstr);
        LOGLOG("snmp_open:%s" ,errstr);
        free(errstr);
        return (1);
    }

    int new_length=sizeofOidName;
    pdu = snmp_pdu_create (SNMP_MSG_GET);
    snmp_add_null_var (pdu, oidName, new_length);

    netsnmp_pdu *response;
    int status = snmp_synch_response (ss, pdu, &response);
    if (status != STAT_SUCCESS || !response){
//        snmp_sess_perror ("snmp_synch_response", ss);
        snmp_error(ss ,&liberr ,&syserr ,&errstr);
        LOGLOG("snmp_synch_response:%s" ,errstr);
        free(errstr);
    }else{
        netsnmp_variable_list *vars;
        if (response->errstat == SNMP_ERR_NOERROR) {
            for (vars = response->variables; vars;
                 vars = vars->next_variable)
//                print_variable(vars->name, vars->name_length, vars);
                if(ASN_OCTET_STR == vars->type && bufsize >= vars->val_len){
                    memcpy(buffer ,vars->val.string ,vars->val_len);
                    ret = 0;
                }

        }else{
            LOGLOG("snmp: Error in packet: %s\n",snmp_errstring (response->errstat));
        }
        snmp_free_pdu (response);
    }
    snmp_close (ss);
    return ret;
}
