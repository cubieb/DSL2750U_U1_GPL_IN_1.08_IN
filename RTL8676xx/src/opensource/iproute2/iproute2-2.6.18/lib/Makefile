
UTILOBJ=utils.o rt_names.o ll_types.o ll_proto.o ll_addr.o inet_proto.o
NLOBJ=ll_map.o libnetlink.o

#options for decnet
ADDLIB+=dnet_ntop.o dnet_pton.o

#options for ipx
ADDLIB+=ipx_ntop.o ipx_pton.o

#all: libnetlink.a libutil.a
all: libiproute2.so
#libnetlink.a: $(NLOBJ)
#	$(AR) rcs $@ $(NLOBJ)

#libutil.a: $(UTILOBJ) $(ADDLIB)
#	$(AR) rcs $@ $(UTILOBJ) $(ADDLIB)

libiproute2.so: $(UTILOBJ) $(NLOBJ)
	$(CC) -fPIC -shared -o $@ $^

install:
	-mkdir -p $(FSROOT)/lib
	install -c -m 777 libiproute2.so $(FSROOT)/lib

clean:
	rm -f $(NLOBJ) $(UTILOBJ) $(ADDLIB) *.a *.so

