CC = g++
WFLAGS = -Wall -Wextra -pedantic -Werror -Wno-missing-field-initializers
OBJECTS = sniffer.o trick_tui.o
LFLAGS = -lncurses -lmenu

trick-tui : $(OBJECTS)
	$(CC) $(WFLAGS) $? $(LFLAGS) -o $@

sniffer.o: sniffer.cpp sniffer.h
	$(CC) $(WFLAGS) -c $< -o $@

trick_tui.o: trick_tui.cpp sniffer.h
	$(CC) $(WFLAGS) -c $< -o $@

clean:
	rm trick-tui $(OBJECTS)