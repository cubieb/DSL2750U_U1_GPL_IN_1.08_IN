/*for ip connect check function*/
#ifdef SUPPORT_IP_CONNECT_CHECK

#define  CONN_CHECK_TIMER_TOKEN "check_timer"
#define  CONN_CHECK_PING_TIMEOUT_TOKEN "ping_timeout"
#define  CONN_CHECK_PING_STATUS "pingstatus"

#define CONN_CHECK_FOR_SUCCESS "success"
#define CONN_CHECK_FOR_FAIL "fail"

typedef enum{
	CHECK_TYPE_INVALID = 0,
	CHECK_TYPE_SUCCESS,
	CHECK_TYPE_FAIL,
}	ping_check_t;

typedef struct {
	void *mngr;

	int period;
	int timeout;
	int tolerance;//times
	char ipaddr[32];
	char intf[32];
	char nexthop[CDMG_IPV4_ADDR_LEN];

	int enable;
	ping_check_t check_type;
	
	DEF_PRI;
} conn_mngr_t;

conn_mngr_t *conn_mngr_new(void *mn);
int conn_mngr_init(conn_mngr_t *cnn);
int conn_mngr_free(conn_mngr_t *cnn);
void conn_mngr_set(conn_mngr_t *cnn);
int conn_mngr_is_start(conn_mngr_t *cnn);
void conn_mngr_cancel(conn_mngr_t *cnn);

#endif
