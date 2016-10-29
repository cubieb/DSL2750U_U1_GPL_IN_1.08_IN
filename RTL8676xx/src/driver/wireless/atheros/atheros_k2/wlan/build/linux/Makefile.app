all: athcfg

clean:
	@rm -f *.o
	@rm ../../tools/athcfg/athcfg        
	@rm ./athcfg

athcfg: 
	@cd ../../tools/athcfg/ && \
	$(CC) -Wall -g -D OS_TYPE_LINUX  -lm athcfg.c -o athcfg
	@cp ../../tools/athcfg/athcfg ./


