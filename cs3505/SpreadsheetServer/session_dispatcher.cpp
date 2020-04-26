
//
// Created by Gabriel Robinson on 4/13/2018.
//

#include <queue>
#include <fstream>
#include <dirent.h>
#include <cstring>
#include "session_dispatcher.h"
#include "threading.h"
#include "server.h"

//debug macro
#define USEDEBUG //comment out to disable debug output
#ifdef USEDEBUG
#define Debug(x) std::cout << "DIS_DEBUG: " << x << std::endl;
#else
#define Debug(x)
#endif
//end debug macro
// split string according to the provided delimiter
        std::vector<std::string> split(const std::string &s, char delim)
{
    std::stringstream ss(s);
    std::string front;
    std::string back;
    std::vector<std::string> tokens;
    unsigned long index = s.find(delim);
    if (index >= 0) {
        front = s.substr(0, index);
        back = s.substr(index+1, s.length()-1);
        tokens.push_back(front);
        tokens.push_back(back);
    }
    return tokens;
}

bool validate_filename(std::string sprd_name) {
    for(int i = 0; i < sprd_name.length(); i++) {
        if (sprd_name[i] == '/' || sprd_name[i] == '\\' || sprd_name[i] == '*' || sprd_name[i] == '?' || sprd_name[i] == ':'|| sprd_name[i] == '<'|| sprd_name[i] == '>'
            || sprd_name[i] == '|' || sprd_name[i] == '?' || sprd_name[i] == '\"' || sprd_name[i] == '&' || sprd_name[i] == '\'' || sprd_name[i] == '#') {
            return 0;
        }
    }
    return 1;
}

std::queue<Message> session_dispatcher::msgs;
std::map<int, client> session_dispatcher::clients;
std::map<client*, spreadsheet_state *> session_dispatcher::client_to_state;
std::map<spreadsheet_state *, std::vector<client *> > session_dispatcher::state_to_client;
std::vector<spreadsheet_state> session_dispatcher::states;

session_dispatcher::session_dispatcher()
        :root_handler(&edit),handler1(&focus), handler2(&unfocus), handler3(&undo), handler4(&revert), handler5(&register_client),
         handler6(&load), handler7(&disconnect), handler8(&ping), handler9(&ping_response)
{
    root_handler.add(&handler1);
    root_handler.add(&handler2);
    root_handler.add(&handler3);
    root_handler.add(&handler4);
    root_handler.add(&handler5);
    root_handler.add(&handler6);
    root_handler.add(&handler7);
    root_handler.add(&handler8);
    root_handler.add(&handler9);
    handler9.set_next(NULL);

}

session_dispatcher::~session_dispatcher()
{

    //for(std::vector<spreadsheet_state>::iterator it = states.begin(); it != states.end(); it++) {
    //  it->write_spreadsheet_file("./Spreads/");
    //}
}


void * session_dispatcher::message_loop() {
    Debug("Message Loop")
    while(true){
        handle_messages(&root_handler);
    }
}

void session_dispatcher::start() {
    threading * thread = new threading();
    thread->create((void*)&session_dispatcher::message_loop, &root_handler);

}

void session_dispatcher::register_client(Message message)
{
    client * clt = message.get_sender();
    clt->register_client();
    std::string resp;
    resp.append("connect_accepted ");
    resp.append(read_filenames("./Spreads/"));
    message.set_response(resp + "\u0003");
    server::send_message(clt, message);
}

void session_dispatcher::load(Message message) {

    std::string sprd_name = message.get_content();

    if(!validate_filename(sprd_name)){

        message.set_response("file_load_error \u0003");
        server::send_message(message.get_sender(), message);
        return;

    } else {

        client * cl = message.get_sender();
        bool found = false;
        std::string full_state = "full_state ";
        spreadsheet_state * ss;
        std::pair<spreadsheet_state *, std::vector<client *> > ss_cl;
        std::pair<client *, spreadsheet_state *> cl_ss;
        std::vector<client *> loc_clients;

        for(std::vector<spreadsheet_state>::iterator it = states.begin(); it != states.end(); ++it)
        {
            if(it->get_file_name() == sprd_name)
            {

                found = true;
                ss = &(*it);
                full_state = full_state + ss->get_full_state() + "\u0003";
                cl_ss = std::pair<client *, spreadsheet_state *>(cl, ss);

                if(state_to_client.count(ss)) {
                    state_to_client[ss].push_back(cl);
                } else {
                    std::vector<client *> clients;
                    clients.push_back(cl);
                    std::pair<spreadsheet_state*, std::vector<client *> > ss_cl(ss, clients);
                    state_to_client.insert(ss_cl);
                }

                client_to_state.insert(cl_ss);
                cl->load_client();
                message.set_response(full_state);
                server::send_message(cl, message);
                return;

            }
        }

        if(!found)
        {
            loc_clients.push_back(message.get_sender());
            spreadsheet_state ss = spreadsheet_state("Spreads/", sprd_name);

            states.push_back(ss);
            ss_cl = std::pair<spreadsheet_state *, std::vector<client *> >(&states.back(), loc_clients);
            cl_ss = std::pair<client *, spreadsheet_state *>(cl, &states.back());
            full_state = full_state + states.back().get_full_state() + "\u0003";
            state_to_client.insert(ss_cl);
            client_to_state.insert(cl_ss);
            ss.write_spreadsheet_file("./Spreads/");
            cl->load_client();
            message.set_response(full_state);
            server::send_message(cl, message);
        }
    }
}

void session_dispatcher::add_message(Message message)
{
    threading::lock_mutex(&server::session_mutex);
    msgs.push(message);
    threading::unlock_mutex(&server::session_mutex);

}

void session_dispatcher::handle_messages(void * args)
{
    message_handler * root = (message_handler *) args;
    threading::lock_mutex(&server::session_mutex);
    Message msg;
    while(!msgs.empty())
    {

        msg = msgs.front();

        msgs.pop();

        root->handle(msg);

    }
    threading::unlock_mutex(&server::session_mutex);

}

// reverts the cell specified by the message
void session_dispatcher::revert(Message message) {

    if(client_to_state.count(message.get_sender())) {
        client *clt = message.get_sender();
        spreadsheet_state *ss = client_to_state[clt];
        std::string result = ss->revert(message.get_content());
        message.set_response("change " + result + "\u0003");
        if (!result.empty())
            server::send_message(state_to_client[ss], message);
        ss->write_spreadsheet_file("./Spreads/");
    }

}

// undo the most recent change to the spreadsheet mapped to the client
void session_dispatcher::undo(Message message)
{
    if(client_to_state.count(message.get_sender())) {
        client *clt = message.get_sender();
        spreadsheet_state *ss = client_to_state[clt];
        std::string result = ss->undo();
        message.set_response("change " + result + "\u0003");
        if (!result.empty())
            server::send_message(state_to_client[ss], message);
        ss->write_spreadsheet_file("./Spreads/");
    }

}

// edits the cell specified in the message to the specified contents
void session_dispatcher::edit(Message message)
{
    if(client_to_state.count(message.get_sender())) {
        client *clt = message.get_sender();
        spreadsheet_state *ss = client_to_state[clt];
        std::string edit = message.get_content();
        std::vector<std::string> contents = split(edit, ':');
        std::string new_content;
        if(contents.size() == 1)
            new_content = ss->change_cell_contents(contents.front(), "");
        else
            new_content = ss->change_cell_contents(contents.front(), contents.back());


        message.set_response("change " + new_content + "\u0003");
        ss->write_spreadsheet_file("./Spreads/");
        server::send_message(state_to_client[ss], message);
    }

}

/*
 * disconnects the client from the dispatcher.
 */
void session_dispatcher::disconnect(Message message)
{
    if(client_to_state.count(message.get_sender())) {
        spreadsheet_state * state = client_to_state[message.get_sender()];

        state->write_spreadsheet_file("./Spreads/");

        for (std::vector<client *>::iterator it = state_to_client[state].begin();
             it != state_to_client[state].end(); it++) {
            if (*it == message.get_sender()) {

                state_to_client[state].erase(it);
                break;

            }
        }
        client_to_state.erase(message.get_sender());

        //      if(state_to_client[state].empty()) {

//            state_to_client.erase(state);

        //    }
    }

    server::disconnect(message.get_sender());

}

void session_dispatcher::focus(Message message) {
    if(client_to_state.count(message.get_sender())){
        client * clt = message.get_sender();
        spreadsheet_state * ss = client_to_state[clt];
        char num_str[21];
        sprintf(num_str, "%d", clt->id);
        message.set_response("focus " + message.get_content() + ":" + num_str + "\u0003");

        server::send_message(state_to_client[ss], message);
    }
}

void session_dispatcher::unfocus(Message message) {

    if(client_to_state.count(message.get_sender())){
        client * clt = message.get_sender();
        spreadsheet_state * ss = client_to_state[clt];
        char num_str[21];
        sprintf(num_str, "%d", clt->id);

        message.set_response(std::string("unfocus ") + num_str + "\u0003");
        server::send_message(state_to_client[ss], message);
    }
}

void session_dispatcher::ping(Message msg) {
    client * clt = msg.get_sender();

    msg.set_response(std::string("ping_response \u0003"));
    server::send_message(clt, msg);
}

void session_dispatcher::ping_response(Message msg) {
    Debug("ping_response from " << msg.get_sender()->id)
    msg.get_sender()->reset_time();
}

/*
 * reads and returns the names of all files in the specified directory.
 */
std::string session_dispatcher::read_filenames(std::string path) {

    Debug("reading filenames");
    std::string files = "";
    DIR *pDIR;

    struct dirent *entry;
    if( pDIR = opendir(path.c_str()) ){
        while(entry = readdir(pDIR)){
            if( strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 )
                files = files + entry->d_name + "\n";
        }
        closedir(pDIR);
    }

    return files;
}
