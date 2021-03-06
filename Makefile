CFLAGS = -c -Wall $(shell pkg-config --cflags check)
TST_LIBS = $(shell pkg-config --libs check)
OS = $(shell uname)
ifeq ($(OS), Linux)
	LIBS=-lcheck_pic -pthread -lrt -lm -lsubunit
	OPEN=xdg-open
else
	LIBS=-lcheck
	OPEN=open
endif

tests:
	gcc -c -Wall -pthread -fprofile-arcs -ftest-coverage test.c -o test.o
	gcc -c -Wall -pthread -fprofile-arcs -ftest-coverage finder.c -o finder.o
	gcc test.o finder.o $(LIBS) -lgcov -coverage -o Test
	./Test
	lcov -t "gcov_report" -o gcov_report.info -c -d .
	genhtml -o report gcov_report.info
	sh integration_test.sh
	sh comparing_test.sh

main:
	gcc -DMAIN finder.c -o myprog

clean_main:
	rm myprog

rebuild_main:
	rm myprog
	gcc -DMAIN finder.c -o myprog


clean_tests:
	rm *.o
	rm *.gcda
	rm *.gcno
	rm -r report
	rm Test
	rm gcov_report.info


rebuild_tests:
	rm *.o
	rm *.gcda
	rm *.gcno
	rm -r report
	rm Test
	rm gcov_report.info
	gcc -c -Wall -Wextra -Werror -pthread -fprofile-arcs -ftest-coverage test.c -o test.o
	gcc -c -Wall -Wextra -Werror -pthread -fprofile-arcs -ftest-coverage finder.c -o finder.o
	gcc test.o finder.o -lcheck_pic -pthread -lrt -lm -lsubunit -lgcov -coverage -o Test
	./Test
	lcov -t "gcov_report" -o gcov_report.info -c -d .
	genhtml -o report gcov_report.info


