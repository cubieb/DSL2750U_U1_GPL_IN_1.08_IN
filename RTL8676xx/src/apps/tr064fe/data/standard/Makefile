TR64STDLIB = libtr64std.o
all : $(TR64STDLIB)


#-DINCLUDE_WANDSLCONNECTIONMGMT -DINCLUDE_WANETHERNETLINKCONFIG  -DINCLUDE_WANETHERNETCONFIG -DINCLUDE_IPPINGDIAG  
CFLAGS+=-DINCLUDE_LAYER3 -DINCLUDE_DEVICEINFO -DINCLUDE_LANCONFIGSECURITY -DINCLUDE_MANAGEMENTSERVER -DINCLUDE_TIME -DINCLUDE_DEVICECONFIG
CFLAGS+=-DINCLUDE_USERINTERFACE -DINCLUDE_LAYER2BRIDGE -DINCLUDE_QUEUEMANAGEMENT -DINCLUDE_LANHOSTCONFIGMGMT -DINCLUDE_LANETHINTERFACECONFIG
CFLAGS+=-DINCLUDE_WLANCONFIG -DWIRELESS -DINCLUDE_LANUSBINTERFACECONFIG -DUSB -DINCLUDE_LANHOSTS -DINCLUDE_WANCOMMONINTERFACE -DINCLUDE_WANDSLINTERFACE
CFLAGS+=-DINCLUDE_WANDSLDIAGNOSTICS -DINCLUDE_WANPOTSLINKCONFIG -DINCLUDE_DSLLINKCONFIG 
CFLAGS+=-DINCLUDE_WANCABLELINKCONFIG -DINCLUDE_IPCONNECTION -DINCLUDE_PPPCONNECTION 


OBJS = igdhandlers.o igdparams.o
OBJS +=layer3handlers.o layer3params.o
OBJS +=deviceinfohandlers.o deviceinfoparams.o
OBJS +=deviceconfigparams.o deviceconfighandlers.o
OBJS +=lancfgsecparams.o lancfgsechandlers.o
OBJS +=mgtserverhandlers.o mgtserverparams.o
OBJS +=timeparams.o timehandlers.o
OBJS +=layer2bridgeparams.o layer2bridgehandlers.o
OBJS +=queuemgtparams.o queuemgthandlers.o
OBJS +=lanhostcfgmgthandlers.o lanhostcfgmgtparams.o
OBJS +=lanethifcfghandlers.o lanethifcfgparams.o
OBJS +=lanwlancfghandlers.o lanwlancfgparams.o
OBJS +=lanusbifcfghandlers.o lanusbifcfgparams.o
OBJS +=lanhostsparams.o lanhostshandlers.o
OBJS +=wancommonhandlers.o wancommonparams.o
OBJS +=wandslifcfgparams.o wandslifcfghandlers.o
#OBJS +=wanethlinkconfigparams.o wanethlinkconfighandlers.o
#OBJS +=wandslconnmgtparams.o wandslconnmgthandlers.o
OBJS +=wandsllinkconfighandlers.o wandsllinkconfigparams.o
#OBJS +=wanethifcfghandlers.o wanethifcfgparams.o
OBJS +=wanipconnectionhandlers.o wanipconnectionparams.o
OBJS +=wanpppconnectionhandlers.o wanpppconnectionparams.o
#OBJS +=ippingparams.o ippinghandlers.o

$(TR64STDLIB):$(OBJS) 
	$(LD) $(EXTRA_LDFLAGS) -r -o $(TR64STDLIB) $(OBJS) 

%.o: %.c
	$(CC) $(CFLAGS) $(EXTRA_CFLAGS) $(CFLAGS_$@) -c -o $@ $<
	
clean :
	rm -f *~ $(OBJS) $(TR64STDLIB) *.o
