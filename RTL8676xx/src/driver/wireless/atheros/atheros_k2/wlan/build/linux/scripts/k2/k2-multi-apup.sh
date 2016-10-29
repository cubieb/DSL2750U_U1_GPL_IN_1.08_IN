#!/bin/sh
athcfg wifi0 vapcreate set ath0 hostap 1
athcfg ath0 param set bg-scan 0
athcfg ath0 mode set 11NG
athcfg wifi0 fftxq-min set 1000
athcfg ath0 param set half-gi 1
athcfg ath0 param set cwm-mode 1
athcfg ath0 param set ext-chan -1
athcfg ath0 param set extchan-protspace 0
athcfg ath0 param set 11n-ampdu 1
athcfg ath0 param set txchan-mask 1
athcfg ath0 param set rxchan-mask 1
athcfg ath0 freq set 11 0
athcfg ath0 ssid set pb-mag-1

athcfg wifi0 vapcreate set ath1 hostap 1
athcfg ath1 param set bg-scan 0
athcfg ath1 mode set 11NG
athcfg ath1 param set half-gi 1
athcfg ath1 param set cwm-mode 1
athcfg ath1 param set ext-chan -1
athcfg ath1 param set extchan-protspace 0
athcfg ath1 param set 11n-ampdu 1
athcfg ath1 param set txchan-mask 1
athcfg ath1 param set rxchan-mask 1
athcfg ath1 freq set 11 0
athcfg ath1 ssid set pb-mag-2

athcfg wifi0 vapcreate set ath2 hostap 1
athcfg ath2 param set bg-scan 0
athcfg ath2 mode set 11NG
athcfg ath2 param set half-gi 1
athcfg ath2 param set cwm-mode 1
athcfg ath2 param set ext-chan -1
athcfg ath2 param set extchan-protspace 0
athcfg ath2 param set 11n-ampdu 1
athcfg ath2 param set txchan-mask 1
athcfg ath2 param set rxchan-mask 1
athcfg ath2 freq set 11 0
athcfg ath2 ssid set pb-mag-3

athcfg wifi0 vapcreate set ath3 hostap 1
athcfg ath3 param set bg-scan 0
athcfg ath3 mode set 11NG
athcfg ath3 param set half-gi 1
athcfg ath3 param set cwm-mode 1
athcfg ath3 param set ext-chan -1
athcfg ath3 param set extchan-protspace 0
athcfg ath3 param set 11n-ampdu 1
athcfg ath3 param set txchan-mask 1
athcfg ath3 param set rxchan-mask 1
athcfg ath3 freq set 11 0
athcfg ath3 ssid set pb-mag-4

athcfg ath0 param set beacon-intv 400

#Set up VAP0 as WEP mode or No-Security
#athcfg ath0 param set privacy 1
#athcfg ath0 key set 1 wep aaaaa aaaaa 00:03:7F:0B:63:78
#athcfg ath0 key set 1

ifconfig ath0 up
brctl addif br0 ath0
sleep 1

ifconfig ath1 up
brctl addif br0 ath1
sleep 1

#Set up VAP2 as WPA mode or No-Security
#hostapd -d /etc/ath/topology_ap2.conf &
ifconfig ath2 up
brctl addif br0 ath2
sleep 1

#Set up VAP3 as WPA mode or No-Security
#hostapd -d /etc/ath/topology_ap3.conf &
ifconfig ath3 up
brctl addif br0 ath3
