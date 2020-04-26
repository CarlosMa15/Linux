//
// Created by Lukas Gust on 4/10/2018.
//

#ifndef SESSION_DISPATCHER_H
#define SESSION_DISPATCHER_H

#include <vector>
#include <map>
#include <queue>
#include "client.h"
#include "Message.h"
#include "spreadsheet_state.h"
#include "message_handler.h"
#include "handlers.h"

/**
 * Creates threads for new session_managers and adds messages to the clients message queue. If a client is not yet
 * registered the session_dispatcher will wait until the client sends the register message and then initiate the
 * registration handshake. Also if the client has not yet loaded a spreadsheet the dispatcher will wait until the client
 * sends the load message and loads a valid spreadsheet. Only the register and load messages will be processed here in
 * the dispatcher class
 */

class session_dispatcher{

private:
    //list of session_managers

    static std::queue<Message> msgs;
    static std::map<int, client> clients;
    static std::map<client *, spreadsheet_state *> client_to_state;
    static std::vector<spreadsheet_state> states;
    static std::map<spreadsheet_state *, std::vector<client *> > state_to_client;
    friend class MessageHandler;
    friend class Server;

    //handlers
    edit_handler root_handler ;
    focus_handler handler1;
    unfocus_handler handler2;
    undo_handler handler3;
    revert_handler handler4;
    register_handler handler5;
    load_handler handler6;
    disconnect_handler handler7;
    ping_handler handler8;
    ping_resp_handler handler9;

public:

    session_dispatcher();
    ~session_dispatcher();
    static std::string read_filenames(std::string path);
    void add_message(Message msg);
    void start();
    void * message_loop();
    static void handle_messages(void *);
    static void register_client(Message msg);
    static void load(Message msg);
    static void focus(Message msg);
    static void unfocus(Message msg);
    static void edit(Message msg);
    static void undo(Message msg);
    static void revert(Message msg);
    static void disconnect(Message msg);
    static void ping(Message msg);
    static void ping_response(Message msg);

};
#endif //SESSION_DISPATCHER_H
