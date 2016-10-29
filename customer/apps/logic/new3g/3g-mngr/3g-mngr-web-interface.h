/*for telerforaci web  function*/
#ifdef SUPPORT_POP_ENTER_PIN_CODE_WEB	

typedef struct {
	void *mngr;
	char arg_buf[256];
	int argc;
	char *argv[32];	
	DEF_PRI;
} web_mngr_t;

web_mngr_t *web_mngr_new(void *mn);
int web_mngr_init(web_mngr_t *web);
int web_mngr_free(web_mngr_t *web);

web_state_t web_mngr_get_state(web_mngr_t *web);
void web_mngr_event(web_mngr_t *web, const char *event);

void  web_msg_init();

#endif
