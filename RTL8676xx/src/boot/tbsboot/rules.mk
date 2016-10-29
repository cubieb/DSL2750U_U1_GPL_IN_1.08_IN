
_depend:	$(obj).depend

$(obj).depend:	$(src)Makefile $(TOPDIR)/config.mk $(SRCS)
		@rm -f $@
		@for f in $(SRCS); do \
			g=`basename $$f | sed -e 's/\(.*\)\.\w/\1.o/'`; \
			$(CC) -M $(HOST_CFLAGS) $(CPPFLAGS) -MQ $(obj)$$g $$f >> $@ ; \
		done

