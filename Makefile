# _*_ Makefile _*_

LIBS=$(wildcard libs/*)#libs/connectivity libs/data
INCLUDE_PATH=libs/
CC = gcc
CFLAGS = -g -Wall
PYTHON = python3

main_exec: libraries
	${PYTHON} -m camera_spot
	$(CC) $(CFLAGS) src/main.c libraries/* -I$(INCLUDE_PATH) -o main_exec

libraries:
	mkdir libraries/
	for dir in $(LIBS); do \
		cd $$dir; \
		$(CC) -c *.c -I../; \
		mv *.o ../../libraries; \
		cd -; \
	done 

clean:
	rm -rf libraries/ main_exec data/output/*.txt
