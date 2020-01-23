#include <string>
#include <set>

namespace Sniffer {
    class SimBroadcast;
    int init_multicast_socket(const char* addr, int port);
    char* getSimNameFromPath(char* path);
    void parse_broadcast(char* info[10], char* databuf);
    void get_list(std::set<SimBroadcast> &vsset, const int sd);
    std::ostream& operator<<(std::ostream& os, const SimBroadcast& sb);
    bool operator==(const SimBroadcast& l, const SimBroadcast& r);
    bool operator<(const SimBroadcast& l, const SimBroadcast& r);
}


class Sniffer::SimBroadcast {    
public:
    SimBroadcast(char* host, char* port, char* sim_name, char* input_file );
    friend std::ostream& operator<<(std::ostream& os, const SimBroadcast& sb);
    friend bool operator==(const SimBroadcast& l, const SimBroadcast& r);
    friend bool operator<(const SimBroadcast& l, const SimBroadcast& r);

    std::string host;
    std::string port;
    std::string sim_name;
    std::string input_file;
    char c_str[128] = {};
};
