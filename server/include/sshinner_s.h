#include <stdio.h>
#include <systemd/sd-id128.h> 

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "st_others.h"
#include "st_slist.h"


typedef struct {
    pthread_t thread_id;        /* unique ID of this thread */
    struct event_base *base;    /* libevent handle this thread uses */
    int notify_receive_fd;      /* receiving end of notify pipe */
    int notify_send_fd;         /* sending end of notify pipe */
} WORKER_THREAD;


typedef struct _srv_opt
{
    unsigned short port;
    WORKER_THREAD  threads[5];
    SLIST_HEAD     acct;
}SRV_OPT, *P_SRV_OPT;


enum CLT_TYPE {
    C_DAEMON, C_USR,
};

typedef struct _conn_item {
    enum CLT_TYPE c_type;
    int  sk_in;
    int  sk_out;
} CONN_ITEM, *P_CONN_ITEM;

typedef struct _acct_item {
    SLIST_HEAD      list;
    char username   [128];  //������������֤ʹ��
    unsigned long   userid;
    CONN_ITEM conns[10];
} ACCT_ITEM, *P_ACCT_ITEM;

/**
 * ʵ�ʻỰ����Ϣ�ṹ
 */
typedef struct _active_item {
    struct event_base *base;
    sd_id128_t  mach_uuid;   //�Ự�����ݣ����������˿����ҵ�����
    P_CONN_ITEM p_daemon;
    P_CONN_ITEM p_usr;
} ACTIVE_ITEM, *P_ACTIVE_ITEM;

extern RET_T load_settings_server(P_SRV_OPT p_opt);
extern void dump_srv_opts(P_SRV_OPT p_opt);


