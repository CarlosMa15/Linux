#include <iostream>
#include <sstream>
#include <string>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <sstream>
#include <dirent.h>
#include <fstream>
#include <queue>
#include <errno.h>
#include "server.h"
#include "threading.h"
#include "Message.h"

//debug macro
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wmissing-noreturn"
#define USEDEBUG //comment out to disable debug output
#ifdef USEDEBUG
#define Debug(x) std::cout << "SER_DEBUG: " << x << std::endl;
#else
#define Debug(x)
#endif
//end debug macro

std::vector<client *> server::clients;
std::map<client *, threading *> server::threads;
pthread_mutex_t  server::server_mutex;
pthread_mutex_t  server::session_mutex;
session_dispatcher server::sd;

/**
 * Initializes server using TCP listener and sockets.
 */
server::server(){
    threading::initialize_mutex(&server_mutex);
    threading::initialize_mutex(&session_mutex);

    int yes = 1;

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    memset(&server_address, 0, sizeof(sockaddr_in));
    server_address.sin_port = htons(PORT);
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;

    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int));

    if(bind(server_socket, (struct sockaddr*) &server_address, sizeof(sockaddr_in)) < 0)
        std::cerr << "Issue binding socket.. " << "ERROR CODE: " << errno << std::endl;

    listen(server_socket, 5);

    std::cout << "server initialized" << std::endl;
}

/**
 * Deconstructs server. Closes all existing client sockets and the servers socket.
 */
server::~server(){
    for(int i = 0 ; i < clients.size() ; i++) {
        send_message(clients[i], "disconnect \u0003");
        session_dispatcher::disconnect(Message("", clients[i]));
        //close(clients[i]->socket);
        //delete clients[i];
        //clients[i] = NULL;
    }
    clients.clear();
    close(this->server_socket);
}

/**
 * Accepts new clients and creates a thread for the socket to run on.
 * The thread calls back handle_client(...).
 */
void server::accept_dispatch() {
    client * clt;
    threading thread;
    threading ping_thread = threading();

    sd.start();

    //ping_thread.create((void *) &ping_loop, NULL);

    socklen_t client_size = sizeof(sockaddr_in);

    std::cout << "Awaiting client connections..." << std::endl;

    while(true) {
        clt = new client();
        thread = threading();

        clt->socket = accept(server_socket, (struct sockaddr *) &client_address, &client_size);

        if (clt->socket < 0){
            std::cerr << "Error on accept.. " << "ERROR CODE: " << errno << std::endl;
        }
        else {
            Debug("creating socket thread..");
            thread.create((void *) &handle_client, clt);
        }
    }
}

//static
/**
 * Handles a new client and adds them to the client vector and processes the client-server handshake.
 * After the handshake a loop is started to receive messages from the client.
 * @param clt client to handle
 * @return
 */
void *server::handle_client(void *args) {

    pthread_detach(pthread_self());

    std::cout << "Contact from client" << std::endl;

    client * clt = (client *)args;
    ssize_t n;

    threading::lock_mutex(&server_mutex);
    {
        clt->set_id(clt->socket);
        std::cout << "Adding client " << clt->id << " to list" << std::endl;
        clients.push_back(clt);
    }
    threading::unlock_mutex(&server_mutex);

    clt->reset_time();

    while(true){
        memset(clt->buffer, 0, sizeof(clt->buffer));
        n = recv(clt->socket, clt->buffer, sizeof(clt->buffer), 0);

        Debug("socket loop");
        if(n <= 0 || !clt) {

            session_dispatcher::disconnect(Message("", clt));

            break;
        }
        else{
            Debug("full message: " << clt->buffer);
            clt->sb.append(clt->buffer);
            process_message(clt);
        }
      }
      Debug("Exiting handle client thread")
      return NULL;
}

/**
 * This method parses the message from a client and creates a message object which is hen passed to the message
 * queue owned by the session_dispatcher. It does not assume that messages from the client are split by the ETX unicode
 * character. Message may contain multiple protocol commands.
 * @param clt client that sent the message.
 * @param message the message that the client sent.
 */
void server::process_message(client * clt) {
    std::string message_string = clt->sb;
    std::string item;
    long index;

    std::stringstream message_ss(message_string);

    std::vector<std::string> tokens;

    while (true) {
        index = message_string.find('\u0003');
        if(index >= 0){
            item = message_string.substr(0,index+1);
            tokens.push_back(item);
            message_string.erase(0, index+1);
        }
        else
            break;
    }

    //for each token create a message object.
    for(std::vector<std::string>::iterator it = tokens.begin() ; it != tokens.end(); it++){
        if((*it).length() == 0)
            continue;

        if((*it)[it->length()-1] != '\u0003')
            break;

        Debug("parsed message: " << *it);

        sd.add_message(Message((*it), clt));

        clt->sb.erase(0, it->length());
    }
}


/**
 * Propogates all changes to registered clients for the appropriate spreadsheet structure.
 * @param message the message to send to all registered clients.
 */
void server::send_message(std::vector<client *> clts, Message msg) {
    Debug("outgoing message to all: " << msg.get_response())
    threading::lock_mutex(&server_mutex);
    {
        for (int i = 0; i < clts.size(); i++) {
            send(clts[i]->socket, msg.get_response().c_str(), strlen(msg.get_response().c_str()), 0);
        }
    }
    threading::unlock_mutex(&server_mutex);
}

void server::send_message(client * clt, Message msg) {
    Debug("outgoing message to client " << clt->id << ": " << msg.get_response())
    send(clt->socket, msg.get_response().c_str(), strlen(msg.get_response().c_str()), 0);
}

void server::send_message(client *clt, std::string msg) {
    Debug("outgoing message to client " << clt->id << ": " << msg)
    send(clt->socket, msg.c_str(), strlen(msg.c_str()), 0);
}

/**
 * Cleans up clients connection and removes them from the list of clients.
 * @param clt client to disconnect
 */
void server::disconnect(client *clt) {
    int index;
    std::cout << "Client disconnected" << std::endl;
    close(clt->socket);

    threading::lock_mutex(&server_mutex);
    {
        if(clt) {
            index = find_client(clt);
            if (index == -1) {
                threading::unlock_mutex(&server_mutex);
                return;
            }

            Debug("removing disconnected client");
            clients.erase(clients.begin() + index);
        }
    }
    threading::unlock_mutex(&server_mutex);

    delete clt;

    clt = NULL;
}

/**
 * Find the client in the client vector.
 * @param clt client to find
 * @return
 */
int server::find_client(client * clt) {
    for(int i = 0 ; i < clients.size() ; i++)
        if(clients[i]->id == clt->id) return i;

    std::cerr << "Could not find client" << std::endl;
    return -1;
}

/**
 * Method for the main method pass to a different loop to allow for
 * graceful shutdown of the server via std::cin
 * @param s server object
 * @return
 */
static void server_loop(server * s){
    s->accept_dispatch();
}

/**
 * Loops over clients every 10 seconds and pings them for a response.
 * @return
 */
void * server::ping_loop() {

    pthread_detach(pthread_self());
    
    while(true) {
        std::vector<client *> timeout_clients;
        threading::lock_mutex(&server_mutex);
        for (std::vector<client *>::iterator it = clients.begin(); it != clients.end(); it++) {
            Debug(clock() - (*it)->ping_timer)
            if((*it)->is_timeout()) {
                send_message(*it, "disconnect \u0003");
                timeout_clients.push_back(*it);
                //session_dispatcher::disconnect(Message("", *it));
            }
            else
                send_message(*it, "ping \u0003");
        }
        threading::unlock_mutex(&server_mutex);

        for(std::vector<client *>::iterator it = timeout_clients.begin(); it != timeout_clients.end(); it++)
            session_dispatcher::disconnect(Message("", *it));
        sleep(10);
    }
}

/**
 * Main method. Creates a thread for the server accept loop and waits for a shutdown response.
 * @return
 */
int main(){
    server *s = new server(); //create server
    threading server_thread;
    bool exit = false;

    pthread_create(&server_thread.thrdid, NULL, (void *(*)(void *))server_loop, s);

    while(!exit){
        std::string word = "";
        std::cin >> word;
        if(word == "quit")
            exit = true;
        std::cout << word;
    }
    //delete server_thread;
    delete s;

    s = NULL;

    server_thread.cancel();

    return 0;
}



#pragma clang diagnostic pop
