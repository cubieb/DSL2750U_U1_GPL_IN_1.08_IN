########################定义模块ID############################
set MID_PC         [format %u 0x0101]
set MID_SNMPA      [format %u 0x0201]

set MID_CCP        [format %u 0x0401]
set MID_AUTH       [format %u 0x0402]
set MID_IGMP_PROXY [format %u 0x0403]
set MID_CMM        [format %u 0x0404]
set MID_LAN        [format %u 0x0405]
set MID_IPT        [format %u 0x0406]
set MID_ETHLAN     [format %u 0x0407]
set MID_ETHWAN     [format %u 0x0408]
set MID_PPPOE      [format %u 0x0409]
set MID_WLAN       [format %u 0x040a]
set MID_TR069BE    [format %u 0x040b]
set MID_DGN        [format %u 0x040c]
set MID_DHCPR      [format %u 0x040d]
set MID_DHCPS      [format %u 0x040e]
set MID_TIMER      [format %u 0x040f]
set MID_IPCONN     [format %u 0x0410]
set MID_FIREWALL   [format %u 0x0411]
set MID_SNMPC      [format %u 0x0412]
set MID_QOS        [format %u 0x0413]
set MID_STATIC_ROUTING [format %u 0x0414]
set MID_VDSL       [format %u 0x0415]
set MID_DNS        [format %u 0x0416]
set MID_ALG        [format %u 0x0417]
set MID_WAN        [format %u 0x0418]
set MID_DYNAMIC_ROUTING [format %u 0x0419]
set MID_SNTP       [format %u 0x041a]
set MID_VLAN       [format %u 0x041b]
set MID_USB_MASS   [format %u 0x041c]
set MID_LOG        [format %u 0x041d]
set MID_FTPD       [format %u 0x041e]
set MID_NATPRIO    [format %u 0x041f]
set MID_ACL        [format %u 0x0421]
set MID_UPNP       [format %u 0x0422]
set MID_LSVLAN     [format %u 0x0423]
set MID_PORTOFF    [format %u 0x0424]
set MID_ANTIATTACK [format %u 0x0425]
set MID_DEVINFO    [format %u 0x0426]
set MID_PORTMAPPING [format %u 0x0427]
set MID_URLFILTER   [format %u 0x0428]
set MID_ATM         [format %u 0x0429]
set MID_DSL         [format %u 0x042a]
set MID_DDNS        [format %u 0x042b]
set MID_PROUTE      [format %u 0x042e]
set MID_CFG         [format %u 0x042f]
set MID_SUPP        [format %u 0x0430]
set MID_MACFILTER   [format %u 0x0431]
set MID_TRACERT     [format %u 0x0432]
set MID_IPPD        [format %u 0x0433]
set MID_WEBP        [format %u 0x0434]
set MID_BRIDGE_FILTER [format %u 0x0435]
set MID_SCHED         [format %u 0x0436]



set MID_WEB       [format %u 0x0501]
set MID_LNB       [format %u 0x0601]
set MID_CLI       [format %u 0x0701]

set MID_TR069FE     [format %u 0x0901]
set MID_TR069_HTTPD [format %u 0x0a01]
set MID_ELM         [format %u 0x0b01]
set MID_UPG         [format %u 0x0c01]
set MID_VDSLD       [format %u 0x0d01]
set MID_TM          [format %u 0x0e01]
set MID_MON         [format %u 0x0f01]
set MID_UPGCGI      [format %u 0x1001]
set MID_FTPUPG      [format %u 0x1101]
set MID_PTI         [format %u 0x1201]
set MID_MSG4UDP     [format %u 0x1301]

set MID_GRP_CCP     [format %u 0x04ff]

set MID_GRP_MACLIST_UPDATE  [format %u 0x04d1]
set MID_GRP_MON_LINK_UPDATE [format %u 0x04d2]
set MID_GRP_WAN_LINK_UPDATE [format %u 0x04d3]
set MID_GRP_WAN_CONN_UPDATE [format %u 0x04d4]
set MID_GRP_DNS_UPDATE      [format %u 0x04d5]
set MID_GRP_WAN_DEL_INST    [format %u 0x04d6]
set MID_GRP_WAN_ADD_INST    [format %u 0x04d7]
set MID_GRP_BUTTON          [format %u 0x04d8]
set MID_GRP_TIME_CHANGED    [format %u 0x04d9]
set MID_GRP_WAN_CONN_ENABLE_UPDATE [format %u 0x04da]
set MID_GRP_LAN_DEL_INST    [format %u 0x04db]
set MID_GRP_LAN_ADD_INST    [format %u 0x04dc]
set MID_GRP_WLAN_DEL_INST   [format %u 0x04dd]
set MID_GRP_WLAN_ADD_INST   [format %u 0x04de]
set MID_GRP_LAN_ETH_INST_CHG [format %u 0x04df]

###############################定义消息类型#####################################
###
set MSG_PC_START_PROC  [format %u 0x0002]
set MSG_PC_SEND_SIGNAL [format %u 0x0003]
set MSG_PC_PROC_STATE  [format %u 0x0004]
set MSG_PC_PROC_OUTPUT [format %u 0x0005]
set MSG_PC_EXIT        [format %u 0x0006]


set	MSG_PC_START_PROC_ACK   [format %u 0x8002]
set	MSG_PC_SEND_SIGNAL_ACK  [format %u 0x8003]
set MSG_PC_PROC_STATE_ACK   [format %u 0x8004]
set MSG_PC_PROC_OUTPUT_ACK  [format %u 0x8005]
set MSG_PC_EXIT_ACK         [format %u 0x8006]

###
set MSG_CMM_GET_VAL     [format %u 0x0007]        
set MSG_CMM_SET_VAL     [format %u 0x0008]        
set MSG_CMM_ADD_NODE    [format %u 0x0009]
set MSG_CMM_DEL_NODE    [format %u 0x000a]
set MSG_CMM_GET_NAME    [format %u 0x000b]
set MSG_CMM_GET_ATTR    [format %u 0x000c]
set MSG_CMM_SET_ATTR    [format %u 0x000d]
set MSG_CMM_INFORM_NOTI [format %u 0x000e]
set MSG_CMM_GET_NOTI    [format %u 0x000f]
set MSG_CMM_CLEAR_NOTI  [format %u 0x0010]
set MSG_CMM_COMMIT      [format %u 0x0011]
set MSG_CMM_CANCEL      [format %u 0x0012]
set MSG_CMM_UPDATE      [format %u 0x0013]
set MSG_CMM_SAVE_CFG     [format %u 0x0014]
set MSG_CMM_RECOVER_CFG  [format %u 0x0015]
set MSG_CMM_UPGRADE      [format %u 0x0016]
set MSG_CMM_BACKUP       [format %u 0x0017]
set MSG_CMM_REBOOT       [format %u 0x0018]


set MSG_CMM_GET_VAL_ACK  [format %u 0x8007]
set MSG_CMM_SET_VAL_ACK  [format %u 0x8008]
set MSG_CMM_ADD_NODE_ACK [format %u 0x8009]
set MSG_CMM_DEL_NODE_ACK [format %u 0x800a]
set MSG_CMM_GET_NAME_ACK [format %u 0x800b]
set MSG_CMM_GET_ATTR_ACK [format %u 0x800c]
set MSG_CMM_SET_ATTR_ACK [format %u 0x800d]
set MSG_CMM_INFORM_NOTI_ACK [format %u 0x800e]
set MSG_CMM_GET_NOTI_ACK    [format %u 0x800f]
set MSG_CMM_CLEAR_NOTI_ACK  [format %u 0x8010]
set MSG_CMM_COMMIT_ACK      [format %u 0x8011]
set MSG_CMM_CANCEL_ACK      [format %u 0x8012]
set MSG_CMM_UPDATE_ACK      [format %u 0x8013]
set MSG_CMM_SAVE_CFG_ACK    [format %u 0x8014]
set MSG_CMM_RECOVER_CFG_ACK [format %u 0x8015]
set MSG_CMM_UPGRADE_ACK     [format %u 0x8016]
set MSG_CMM_BACKUP_ACK      [format %u 0x8017]
set MSG_CMM_REBOOT_ACK      [format %u 0x8018]

set MSG_CMM_VAL             [format %u 0x7001] ;#未定义类型，做异常测试用

###
set MSG_TIMER_REGSTER        [format %u 0x0019]
set MSG_TIMER_UNREGSTER      [format %u 0x001a]
set MSG_TIMER_TIMEOUT        [format %u 0x001b]

set	MSG_TIMER_REGSTER_ACK    [format %u 0x8019]
set	MSG_TIMER_UNREGSTER_ACK  [format %u 0x801a]
set MSG_TIMER_TIMEOUT_ACK    [format %u 0x801b]

###
set MSG_WAN_INTERFACE_STATE  [format %u 0x001c]
set MSG_WAN_CONN_EST         [format %u 0x001d]
set MSG_WAN_CONN_FIN         [format %u 0x001e]
set MSG_WAN_LINK_CHG         [format %u 0x001f]


set MSG_WAN_INTERFACE_STATE_ACK  [format %u 0x801c]
set MSG_WAN_CONN_EST_ACK         [format %u 0x801d]
set MSG_WAN_CONN_FIN_ACK         [format %u 0x801e]
set MSG_WAN_LINK_CHG_ACK         [format %u 0x801f]

###
set MSG_LAN_IP_UPDATE            [format %u 0x0020]
set MSG_LAN_MACLIST_UPDATE       [format %u 0x0021]
set MSG_LAN_DOMAIN_NAME_UPDATE   [format %u 0x0022]


set MSG_LAN_IP_UPDATE_ACK           [format %u 0x8020]
set MSG_LAN_MACLIST_UPDATE_ACK      [format %u 0x8021]
set MSG_LAN_DOMAIN_NAME_UPDATE_ACK  [format %u 0x8022]

###
set MSG_GET_DHOST_LIST      [format %u 0x0023]
set MSG_GET_DHOST_LIST_ACK  [format %u 0x8023]

###
set MSG_DNS_CHANGE          [format %u 0x0024]
set MSG_DNS_CHANGE_ACK      [format %u 0x8024]

###
set	MSG_WLAN_ADD_DEVICE     [format %u 0x0025]
set MSG_WLAN_DEL_DEVICE     [format %u 0x0026]

set MSG_WLAN_ADD_DEVICE_ACK [format %u 0x8025]
set MSG_WLAN_DEL_DEVICE_ACK [format %u 0x8026]

###
set MSG_TR069_SET_PARKEY      [format %u 0x0027]
set MSG_TR069_GET_EVENTS      [format %u 0x0028]
set MSG_TR069_CLEAR_EVENTS    [format %u 0x0029]
set MSG_TR069_DOWNLOAD        [format %u 0x002a]
set MSG_TR069_UPLOADLOAD      [format %u 0x002b]
set MSG_TR069_REBOOT          [format %u 0x002c]
set MSG_TR069_FACTORYRESET    [format %u 0x002d]
set MSG_TR069_SCHEDULEINFORM  [format %u 0x002e]
set MSG_TR069_GET_TRANSFERCOMPLETEINFO [format %u 0x002f]
set MSG_LBT_WLAN       [format %u 0x0030]
set MSG_LBT_WPS        [format %u 0x0031]
set MSG_TR069_HTTPD_CONNECT         [format %u 0x0032]
set MSG_TR069_HTTPD_CREATE_UPFILE   [format %u 0x0033]


set MSG_TR069_SET_PARKEY_ACK     [format %u 0x8027]
set MSG_TR069_GET_EVENTS_ACK     [format %u 0x8028]
set MSG_TR069_CLEAR_EVENTS_ACK   [format %u 0x8029]
set MSG_TR069_DOWNLOAD_ACK       [format %u 0x802a]
set MSG_TR069_UPLOADLOAD_ACK     [format %u 0x802b]
set MSG_TR069_REBOOT_ACK         [format %u 0x802c]
set MSG_TR069_FACTORYRESET_ACK   [format %u 0x802d]
set MSG_TR069_SCHEDULEINFORM_ACK [format %u 0x802e]
set MSG_TR069_GET_TRANSFERCOMPLETEINFO_ACK  [format %u 0x802f]
set MSG_LBT_WLAN_ACK             [format %u 0x8030]
set MSG_LBT_WPS_ACK              [format %u 0x8031]
set MSG_TR069_HTTPD_CONNECT_ACK        [format %u 0x8032]
set MSG_TR069_HTTPD_CREATE_UPFILE_ACK  [format %u 0x8033]

###
set MSG_ETHNET_LINK_STATE   [format %u 0x0034]
set MSG_ETHWAN_INTF_NAME    [format %u 0x0035]

set MSG_ETHNET_LINK_STATE_ACK [format %u 0x8034]
set MSG_ETHWAN_INTF_NAME_ACK  [format %u 0x8035]

###
set MSG_MON_INTF_REGISTER          [format %u 0x0036]
set MSG_MON_INTF_UNREGISTER        [format %u 0x0037]
set MSG_MON_INTF_STATUS_QUERY      [format %u 0x0038]
set MSG_MON_INTF_STATUS_INFORM     [format %u 0x0039]

set MSG_MON_INTF_REGISTER_ACK      [format %u 0x8036]
set MSG_MON_INTF_UNREGISTER_ACK    [format %u 0x8037]
set MSG_MON_INTF_STATUS_QUERY_ACK  [format %u 0x8038]
set MSG_MON_INTF_STATUS_INFORM_ACK [format %u 0x8039]

###
set MSG_AUTH     [format %u 0x003a]
set MSG_AUTH_ACK [format %u 0x803a]

###
set MSG_WAN_DEL_INST [format %u 0x003b]
set MSG_WAN_ADD_INST [format %u 0x003c]

set MSG_WAN_DEL_INST_ACK [format %u 0x803b]
set MSG_WAN_ADD_INST_ACK [format %u 0x803c]

###
set MSG_LAN_DEL_INST [format %u 0x003d]
set MSG_LAN_ADD_INST [format %u 0x003e]

set MSG_LAN_DEL_INST_ACK [format %u 0x803d]
set MSG_LAN_ADD_INST_ACK [format %u 0x803e]

###
set MSG_WLAN_DEL_INST [format %u 0x003f]
set MSG_WLAN_ADD_INST [format %u 0x0040]

set MSG_WLAN_DEL_INST_ACK [format %u 0x803f]
set MSG_WLAN_ADD_INST_ACK [format %u 0x8040]

###
set MSG_CMD       [format %u 0x0041]
set MSG_CMD_ACK   [format %u 0x8041]

###
set MSG_RESP            [format %u 0x0042]
set MSG_RESP_FRAGMENT   [format %u 0x0043]
set MSG_RESP_ERR        [format %u 0x0044]

set MSG_RESP_ACK          [format %u 0x8042]
set MSG_RESP_FRAGMENT_ACK [format %u 0x8043]
set MSG_RESP_ERR_ACK      [format %u 0x8044]

###
set  MSG_CMM_CLEAR_CFG      [format %u 0x0045]
set  MSG_CMM_CLEAR_CFG_ACK  [format %u 0x8045]

###
set  MSG_BUTTON             [format %u 0x0046]
set  MSG_BUTTON_ACK         [format %u 0x8046]

###
set MSG_SNTP_TIME_UPDATE    [format %u 0x0047]
set MSG_SNTP_TIME_CHANGED   [format %u 0x0048]

set MSG_SNTP_UPDATE_TIME_ACK [format %u 0x8047]
set MSG_SNTP_TIME_CHANGED_ACK [format %u 0x8048]
###
set MSG_NTPS_CHANGED    [format %u 0x0049]
set MSG_NTPS_CHANGED_ACK [format %u 0x8049]

###
set MSG_TR111_DEVICE_ADD [format %u 0x004a]
set MSG_TR111_DEVICE_DEL [format %u 0x004b]

set MSG_TR111_DEVICE_ADD_ACK [format %u 0x804a]
set MSG_TR111_DEVICE_DEL_ACK [format %u 0x804b]
###
set MSG_PING_COMPLETE [format %u 0x004c]
set MSG_PING_COMPLETE_ACK [format %u 0x804c]
  
 
###
set MSG_MSG4UDP_REGISTER   [format %u 0x004d]
set MSG_MSG4UDP_UNREGISTER [format %u 0x004e]
set MSG_MSG4UDP_MESSAGE    [format %u 0x004f]

set MSG_MSG4UDP_REGISTER_ACK   [format %u 0x804d]
set MSG_MSG4UDP_UNREGISTER_ACK [format %u 0x804e]
set MSG_MSG4UDP_MESSAGE_ACK    [format %u 0x804f]

###
set MSG_WAN_CONN_ENABLE_UPDATE     [format %u 0x0050]
set MSG_WAN_CONN_ENABLE_UPDATE_ACK [format %u 0x8050]
###
set MSG_VLAN_ADD_BRIDGE  [format %u 0x0051]
set MSG_VLAN_DEL_BRIDGE  [format %u 0x0052]
set MSG_VLAN_BIND_PORT   [format %u 0x0053]
set MSG_VLAN_UNBIND_PORT [format %u 0x0054]

set MSG_VLAN_ADD_BRIDGE_ACK  [format %u 0x8051]
set MSG_VLAN_DEL_BRIDGE_ACK  [format %u 0x8052]
set MSG_VLAN_BIND_PORT_ACK   [format %u 0x8053]
set MSG_VLAN_UNBIND_PORT_ACK [format %u 0x8054]

###
set MSG_CMM_INST_ADDED       [format %u 0x005d]
set MSG_CMM_INST_DELED       [format %u 0x005e]
set MSG_CMM_INST_ADDED_ACK   [format %u 0x805d]
set MSG_CMM_INST_DELED_ACK   [format %u 0x805e]

set MSG_WAN_CONN_SET_CHNG    [format %u 0x005f] 

