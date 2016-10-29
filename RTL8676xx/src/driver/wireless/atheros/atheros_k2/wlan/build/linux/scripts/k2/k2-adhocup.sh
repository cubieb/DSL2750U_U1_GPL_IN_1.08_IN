athcfg wifi0 vapcreate set ath0 ibss-adhoc 1
athcfg ath0 mode set 11NG
athcfg ath0 param set bg-scan 0
#athcfg ath0 param set scancash-thres 120
athcfg wifi0 fftxq-min set 1000
athcfg ath0 param set half-gi 1
athcfg ath0 param set cwm-mode 1
#./athcfg ath0 param set cwm-mode 0
athcfg ath0 param set ext-chan -1
athcfg ath0 param set extchan-protspace 0
#./athcfg ath0 param set 11n-ampdu 0
athcfg ath0 param set 11n-ampdu 1
athcfg ath0 param set txchan-mask 1
athcfg ath0 param set rxchan-mask 1
athcfg ath0 freq set 11 0
athcfg ath0 ssid set adhoc_magpie
#athcfg ath0 key set 1 wep open 1234567890 00:03:7F:10:44:D1
#athcfg ath0 key set 1
ifconfig ath0 192.168.11.27 up

