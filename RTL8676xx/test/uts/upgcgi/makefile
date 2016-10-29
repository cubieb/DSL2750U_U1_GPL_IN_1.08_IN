INC=-I./ -I../ -I../../../../include/  -I/usr/local/lib/include/CUnit/
LIB=-L/usr/local/lib/lib/

test: upg_cgi.o upgcgi_ut.o  upg_utils.o
	cc -g -o upgcgi_ut $(INC)  $(LIB)  $^ -lcunit  -lgcov

upg_cgi.o:  ../upg_cgi.c
	cc -g -c ../upg_cgi.c -D_UPGCGI_UT  $(INC) -fprofile-arcs -ftest-coverage   

upgcgi_ut.o: upgcgi_ut.c
	cc -g -c upgcgi_ut.c $(INC)

#upgcgi_ut_stub.o: upgcgi_ut_stub.c
#	cc -g -c upgcgi_ut_stub.c $(INC)
	        
#upg_parse.o: ../upg_parse.c
#	cc -g -c ../upg_parse.c $(INC)
	     
upg_utils.o: ../upg_utils.c
	cc -g -c ../upg_utils.c $(INC)

clean:
	rm  *.d *.o *.gcno *.gcov *.gcda *.xml *.bak *.log upgcgi_ut -f
