#!/bin/sh

CONF_FILE=/etc/upnpd.conf

if [ ! -n "$3" ]; then
  echo "insufficient arguments!"
  echo "Usage: $0 <Upstream_If> <Downstream_If_Ip> <uuid>"
  exit 0
fi

UPSTREAM_IF="$1"
DOWNSTREAM_IF_IP="$2"
UUID="$3"


echo "ext_ifname=$UPSTREAM_IF" > $CONF_FILE
echo "listening_ip=$DOWNSTREAM_IF_IP/24" >> $CONF_FILE
echo "port=0">> $CONF_FILE
echo "enable_natpmp=yes">> $CONF_FILE
echo "enable_upnp=yes">> $CONF_FILE
echo "secure_mode=no">> $CONF_FILE
echo "system_uptime=yes">> $CONF_FILE
echo "notify_interval=60">> $CONF_FILE
echo "clean_ruleset_interval=600">> $CONF_FILE
echo "uuid=$UUID">> $CONF_FILE
echo "serial=001EE3010203">> $CONF_FILE
echo "model_number=1.0.0">> $CONF_FILE
