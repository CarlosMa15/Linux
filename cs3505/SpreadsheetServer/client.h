//
// Created by Lukas Gust on 4/3/2018.
//

#ifndef COLLABSPREADSHEET_CLIENT_H
#define COLLABSPREADSHEET_CLIENT_H

#include <iostream>
#include <sstream>
#include <ctime>

class client{
public:
    time_t ping_timer;
    unsigned long id;
    int socket;
    char buffer[1024];
    std::string sb;//growable buffer

private:
    bool loaded;
    bool registered;
public:
    void load_client();
    void register_client();
    bool is_loaded();
    bool is_registered();
    void reset_time();
    bool is_timeout();
    client();
    //~client();
    void set_id(unsigned long id);
    bool operator < (const client &other) const;

};
#endif //COLLABSPREADSHEET_CLIENT_H
