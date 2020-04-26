//
// Created by Lukas Gust on 4/11/2018.
//
#include "Message.h"
#include <iostream>
#include <string>
#include <sstream>

/**
 * Represents a message from a client. Splits the message into its parts. Keys and contents.
 * Holds onto the client who sent the message and will be assigned a response through the message handlers.
 * @param total_msg whole message recieved from server class. Expected to be a full message.
 * @param sender the client who sent the message.
 */
Message::Message(std::string total_msg, client * sender)
{
    this->sender = sender;

    std::string item;
    std::stringstream message_ss(total_msg);

    int first_space = total_msg.find(' ');

    if(first_space >= 0){

        key = total_msg.substr(0,first_space);
        if(key[key.size()-1] == ' ')
            key.erase(key.size()-1);

        content = total_msg.substr(first_space);
        if(content[0] == ' ')
            content.erase(0, 1);

        content.erase(content.length()-1);
    }



}

Message::Message() {}

/**
 * Retreive the key from the message.
 * @return the key
 */
std::string Message::get_key() {
    return key;
}
/**
 * Retreive the content from the message.
 * @return the content
 */
std::string Message::get_content() {
    return content;
}
/**
 * Retreive the sender of the message.
 * @return the sender
 */
client * Message::get_sender() {
    return sender;
}

/**
 * Retrieves the response that was generated by the the message handlers.
 * May or may not be empty. If it is empty there is no response to be sent.
 * @return the response
 */
std::string Message::get_response() {
    return response;
}

/**
 * Sets the response message.
 * @param resp response to be assigned. Must be the full response according to the protocol.
 */
void Message::set_response(std::string resp) {
    response = resp;
}
