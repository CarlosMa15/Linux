
RP_C = redpin.c
OPT = -O2
CFLAGS = $(OPT) -g -Wall -I.

redpin: $(RP_C) dictionary.c dictionary.h csapp.c csapp.h more_string.c more_string.h
	$(CC) $(CFLAGS) -o redpin $(RP_C) dictionary.c more_string.c csapp.c -pthread

clean:
	rm redpin
