//
// Created by Lukas Gust on 4/11/2018.
//

#ifndef COLLABSPREADSHEET_MESSAGE_H
#define COLLABSPREADSHEET_MESSAGE_H

#include <string>
#include "client.h"

class Message{

private:
    std::string key;
    std::string content;
    std::string response;
    client * sender;

public:
    Message();
    Message(std::string total_msg, client * sender);
    std::string get_key();
    std::string get_content();
    std::string get_response();
    void set_response(std::string resp);
    client * get_sender();

};

#endif //COLLABSPREADSHEET_MESSAGE_H
