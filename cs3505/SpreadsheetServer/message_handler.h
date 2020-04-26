//
// Created by Lukas Gust on 4/13/2018.
//

#ifndef COLLABSPREADSHEET_MESSAGE_HANDLER_H
#define COLLABSPREADSHEET_MESSAGE_HANDLER_H

#include "Message.h"

/**
 * This class is the base class for all message handlers and provide a structure for a chain of responsibility pattern.
 */
class message_handler{
private:
    message_handler * next_handler;
public:
    message_handler();

    void set_next(message_handler * mh);
    void add(message_handler * mh);

    virtual void handle(Message message);

};
#endif //COLLABSPREADSHEET_MESSAGE_HANDLER_H
