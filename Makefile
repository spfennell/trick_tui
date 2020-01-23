CC = g++
WFLAGS = -Wall -Wextra -pedantic -Werror -Wno-missing-field-initializers
OBJECTS = sniffer.o trick_tui.o

trick-tui : $(OBJECTS)
	$(CC) $(WFLAGS) $? -lncurses -lmenu -o $@

sniffer.o: sniffer.cpp sniffer.h
	$(CC) $(WFLAGS) -c $< -o $@

trick_tui.o: trick_tui.cpp sniffer.h
	$(CC) $(WFLAGS) -c $< -lncurses -lmenu  -o $@

clean:
	rm trick-tui $(OBJECTS)