CC = gcc
CFLAGS = -Iutils -g -std=gnu99 -Wall -Werror
COPTFLAGS = -O3
LDFLAGS = -lm

sort: utils/driver.o utils/sort_utils.o student/funnel.o utils/timer.c
	$(CC) $(COPTFLAGS) -o $@ $^ $(LDFLAGS)

clean:
	rm -f utils/*.o student/*.o *~ sort
