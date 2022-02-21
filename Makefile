CFLAGS = -c -Wall $(shell pkg-config --cflags check)
TST_LIBS = $(shell pkg-config --libs check)



all:
	gcc -c -Wall -pthread -fprofile-arcs -ftest-coverage test.c -o test.o
	gcc -c -Wall -pthread -fprofile-arcs -ftest-coverage finder.c -o finder.o
	ar rc lib_for_finder.a finder.o
	ranlib lib_for_finder.a
	cp lib_for_finder.a my_lib.a
	gcc test.o my_lib.a -lcheck_pic -pthread -lrt -lm -lsubunit -lgcov -coverage -o Test
	./Test
	lcov -t "gcov_report" -o gcov_report.info -c -d .
	genhtml -o report gcov_report.info


clean:
	rm *.o
	rm *.a
	rm *.gcda
	rm *.gcno
	rm -r report
	rm Test
	rm gcov_report.info

just_code:
	gcc -c finder.c -o find_me

