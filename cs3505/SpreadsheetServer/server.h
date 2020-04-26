//
// Created by Lukas Gust on 4/3/2018.
//

#ifndef COLLABSPREADSHEET_SERVER_H
#define COLLABSPREADSHEET_SERVER_H

#include "client.h"
#include "spreadsheet_state.h"
#include "Message.h"
#include "session_dispatcher.h"
#include "threading.h"
#include <vector>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>

#define PORT 2112 //default port for connecting.

/**
 * Server library for accepting and handling clients.
 */
class server{
    private:
        static std::vector<client *> clients;
        static std::map<client *, threading *> threads;
        static pthread_mutex_t server_mutex;
        static pthread_mutex_t session_mutex;

        int server_socket;
        struct sockaddr_in server_address, client_address;

        static session_dispatcher sd;

    public:
        server();
        ~server();
        void accept_dispatch();
        static void * handle_client(void *clt);
        static void * ping_loop();

    private:
        static void process_message(client * clt);
        static void send_message(std::vector<client *> clts, Message msg);
        static void send_message(client * clt, Message msg);
        static void send_message(client * clt, std::string msg);
        static int find_client(client * clt);
        static void disconnect(client * clt);
        static std::string read_file_names(std::string);
        void write_file(std::string, std::string);
        spreadsheet_state start_session_from_file(std::string);//move to session_dispatcher
        friend class session_dispatcher;

};

#endif //COLLABSPREADSHEET_SERVER_H
