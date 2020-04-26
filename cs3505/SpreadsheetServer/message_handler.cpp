//
// Created by Lukas Gust on 4/13/2018.
//
#include "message_handler.h"

/**
 * Handles messages and this is the beginning of the chain of responsibility. This will pass the message down the chain
 * to the message handler that can handle it. This is the base class where every message passes through.
 */
message_handler::message_handler() {
    next_handler = NULL;
}

/**
 * Set the next handler that is desired.
 * @param mh handler type
 */
void message_handler::set_next(message_handler *mh) {
    next_handler = mh;
}

/**
 * Add the message handler to the chain.
 * @param mh handler type
 */
void message_handler::add(message_handler *mh) {
    if(next_handler)
        next_handler->add(mh);
    else
        next_handler = mh;
}

/**
 * Passes the message to the next handler.
 * @param message message to handle.
 */
void message_handler::handle(Message message) { if(next_handler)
        next_handler->handle(message);
    else
        return;
}

