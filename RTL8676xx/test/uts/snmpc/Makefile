INC=-I/usr/local/include/CUnit/ 
LIB=-L/usr/local/lib/
CFLAGS=	-I../ \
	-I../../../om/include \
	-I../../../../ssap/cfg \
	-I../../../../ssap/pc \
	-I../../../../ssap/msg \
	-I../../../../ssap/mm \
	-I../../../../ssap/lib \
	-I../../../../include \
	-I../../../../ssap/warn \
	-I../../../../../blp/boot/base/cross_cc/i386/i386-linux-uclibc/sys-include/netinet/
all: Test
Test: snmpc.o ut_snmpc.o common.o tbsutil.o tbserror.o
	gcc -o ut_snmpc $(INC) $(LIB) $^ -lcunit -lgcov 

gcov: 
	gcov snmpc.gcno

ut_snmpc.o: ut_snmpc.c ut_stub.c
	cc -c $(CFLAGS) ut_snmpc.c $(INC)

snmpc.o: ../snmpc.c ../snmpc.h
	cc -c $(CFLAGS) -DSNMPC_UT ../snmpc.c -fprofile-arcs -ftest-coverage

common.o:
	cc -c $(CFLAGS) ../../../../ssap/lib/common.c $(INC)

tbsutil.o:
	cc -c $(CFLAGS) ../../../../ssap/lib/tbsutil.c $(INC)

tbserror.o:
	cc -c $(CFLAGS) ../../../../ssap/lib/tbserror.c $(INC)

clean:
	-rm -rf *.o *.da *.bb *.bbg *.c.gcov *.gcno *.gcda ut_snmpc
