#include <string>
#include <set>

class Sniffer {
public:

    class SimBroadcast {    
    public:
        SimBroadcast(char* host, char* port, char* sim_name, char* input_file );
        friend std::ostream& operator<<(std::ostream& os, const SimBroadcast& sb);
        friend bool operator==(const SimBroadcast& l, const SimBroadcast& r);
        friend bool operator<(const SimBroadcast& l, const SimBroadcast& r);

    private:
        std::string host;
        std::string port;
        std::string sim_name;
        std::string input_file;
    };

    static int init_multicast_socket(const char* addr, int port);
    static char* getSimNameFromPath(char* path);
    static void parse_broadcast(char* info[10], char* databuf);
    static void get_list(std::set<SimBroadcast> &vsset, const int sd);
};
