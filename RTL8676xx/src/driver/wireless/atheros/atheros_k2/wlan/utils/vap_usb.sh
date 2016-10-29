athcfg wifi0 vapcreate set ath0 hostap 1
athcfg ath0 param set bg-scan 0
athcfg ath0 mode set 11NG
athcfg wifi0 fftxq-min set 1000
athcfg ath0 param set half-gi 1
#athcfg ath0 param set cwm-mode 1
athcfg ath0 param set cwm-mode 2
athcfg ath0 param set ext-chan -1
athcfg ath0 param set extchan-protspace 0
athcfg ath0 param set 11n-ampdu 1
athcfg ath0 param set txchan-mask 1
athcfg ath0 param set rxchan-mask 1
#athcfg ath0 freq set 15 0
athcfg ath0 freq set 11 0
#athcfg ath0 freq set 80 0
athcfg ath0 ssid set Amazon_SE
sleep 1
brctl addif br1 ath0
sleep 2
ifconfig ath0 up
#ifconfig ath0 192.168.1.45 up

