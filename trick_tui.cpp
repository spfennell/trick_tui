#include <iostream>
#include <cstdlib>
#include <unistd.h>
#include <ncurses.h>
#include <menu.h>
#include <vector>
#include "sniffer.h"



int main(void) {
    int sd; // socket descriptor
    std::set<Sniffer::SimBroadcast> vsset; // list of available variable servers

    std::cout << "gathering data..." << std::endl; 
    sd = Sniffer::init_multicast_socket("239.3.14.15", 9265);
    Sniffer::get_list(vsset, sd);
    std::cout << "----- Sim Info -----" << std::endl; 
    for(auto it = vsset.begin(); it != vsset.end(); ++it) {
        std::cout << *it << std::endl;
    }
    

    ITEM **my_items;
    int c;              
    MENU *my_menu;
    int n_choices;
    int i = 0;
    
    
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, TRUE);
    
    n_choices = vsset.size();
    my_items = (ITEM **)calloc(n_choices + 1, sizeof(ITEM *));
    for(auto it = vsset.begin(); it != vsset.end(); ++it) {
            my_items[i] = new_item((*it).c_str, "");
            i++;
    }
    my_items[n_choices] = (ITEM *)NULL;

    my_menu = new_menu((ITEM **)my_items);
    mvprintw(LINES - 2, 0, "F1 to Exit");
    post_menu(my_menu);
    refresh();

    while((c = getch()) != KEY_F(1))
    {   switch(c)
        {   case KEY_DOWN:
                menu_driver(my_menu, REQ_DOWN_ITEM);
                break;
            case KEY_UP:
                menu_driver(my_menu, REQ_UP_ITEM);
                break;
        }
    }   

    free_item(my_items[0]);
    free_item(my_items[1]);
    free_menu(my_menu);
    endwin();


    close(sd);
    return 0;
}
