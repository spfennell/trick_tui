#include <iostream>
#include <unistd.h>

#include "sniffer.h"



int main(int argc, char *argv[]) {
    int sd; // socket descriptor
    std::set<Sniffer::SimBroadcast> vsset; // list of available variable servers

    std::cout << "gathering data..." << std::endl; 
    sd = Sniffer::init_multicast_socket("239.3.14.15", 9265);
    Sniffer::get_list(vsset, sd);
    std::cout << "----- Sim Info -----" << std::endl; 
    for(auto it = vsset.begin(); it != vsset.end(); ++it) {
        std::cout << *it << std::endl;
    }

    close(sd);
    return 0;
}
