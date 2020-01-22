
    #include <arpa/inet.h>
    #include <stdio.h>
    #include <iostream>
    #include <string.h>
    #include <stdlib.h>
    #include <unistd.h>
    #include <string>
    #include <set>
    #include <chrono>
    

class SimBroadcast {    
public:
    SimBroadcast(char* host, char* port, char* sim_name, char* input_file ) : host(host), port(port), sim_name(sim_name), input_file(input_file){}
    friend std::ostream& operator<<(std::ostream& os, const SimBroadcast& sb) {
        os << sb.sim_name << "\thost: " << sb.host << "\tport: " << sb.port  << "\tinput_file: " << sb.input_file;
        return os;
    }

    friend bool operator==(const SimBroadcast& l, const SimBroadcast& r) {
        return ((!l.host.compare(r.host)) && (!l.port.compare(r.port)));
    }

    friend bool operator<(const SimBroadcast& l, const SimBroadcast& r) {
        int compare = l.host.compare(r.host);
        if(!compare) {
            return l.port.compare(r.port) < 0;
        }
        return compare < 0;
    }

private:
    std::string host;
    std::string port;
    std::string sim_name;
    std::string input_file;

    // other fields not needed for the application. Feel free to add them in.
};

static inline int init_multicast_socket(const char* addr, int port) {
    struct sockaddr_in localSock = {0};
    struct ip_mreq group = {0};
    int sd;
    sd = socket(AF_INET, SOCK_DGRAM, 0);
    if(sd < 0) {
        perror("Opening datagram socket error");
        exit(1);
    }
    int reuse = 1;
    if(setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *)&reuse, sizeof(reuse)) < 0) {
        perror("Setting SO_REUSEADDR error");
        close(sd);
        exit(1);
    }    

    /* Bind to the proper port number with the IP address */
    /* specified as INADDR_ANY. */
    memset((char *) &localSock, 0, sizeof(localSock));
    localSock.sin_family = AF_INET;
    localSock.sin_port = htons(9265);
    localSock.sin_addr.s_addr = INADDR_ANY;
    if(bind(sd, (struct sockaddr*)&localSock, sizeof(localSock))) {
        perror("Binding datagram socket error");
        close(sd);
        exit(1);
    }
    /* Join the multicast group 239.3.14.15 on the local 203.106.93.94 */
    /* interface. Note that this IP_ADD_MEMBERSHIP option must be */
    /* called for each local interface over which the multicast */
    /* datagrams are to be received. */
    group.imr_multiaddr.s_addr = inet_addr("239.3.14.15");
    //group.imr_interface.s_addr = inet_addr("239.3.14.15");
    if(setsockopt(sd, IPPROTO_IP, IP_ADD_MEMBERSHIP, (char *)&group, sizeof(group)) < 0) {
        perror("Adding multicast group error");
        close(sd);
        exit(1);
    }
    return sd;
}

static inline char* getSimNameFromPath(char* path) {
        char* sim_name = NULL;
        char* tok = strtok(path, "/");
        while(tok != NULL) {
            sim_name = tok;
            tok = strtok(NULL, "/");
        }
        // this is okay because sim_name still points to a valid address inside
        // the 'path' string. strtok always points to a piece of the old string. 
        return sim_name;
}

static inline void parse_broadcast(char* info[10], char* databuf) {
    char* tok = strtok(databuf, "\n  \t");
    for(int i = 0; tok != NULL && i < 10; i++) {
        info[i] = tok;
        tok = strtok(NULL, "\n \t");
    }
}

static inline void get_list(std::set<SimBroadcast> &vsset, const int sd) {
    char databuf[1024];
    /* Read from the socket. */
    int datalen = sizeof(databuf);
    int pass = 2;
    auto start = std::chrono::system_clock::now();
    // while for two seconds
    while((std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - start)).count() < 2.0) {
        if(read(sd, databuf, datalen) < 0) {
            perror("Reading datagram message error");
            close(sd);
            exit(1);
        }
        char* info[10];
        parse_broadcast(info, databuf);
        char* sim_name = getSimNameFromPath(info[4]);
        SimBroadcast sb(info[0], info[1], sim_name, info[6]);
        vsset.insert(sb);
    }   
}

int main(int argc, char *argv[]) {
    int sd; // socket descriptor
    std::set<SimBroadcast> vsset; // list of available variable servers

    std::cout << "gathering data..." << std::endl; 
    sd = init_multicast_socket("239.3.14.15", 9265);
    get_list(vsset, sd);
    std::cout << "----- Sim Info -----" << std::endl; 
    for(auto it = vsset.begin(); it != vsset.end(); ++it) {
        std::cout << *it << std::endl;
    }

    close(sd);
    return 0;
}
