//
// Created by Lukas Gust on 4/13/2018.
//

#ifndef COLLABSPREADSHEET_HANDLERS_H
#define COLLABSPREADSHEET_HANDLERS_H

#include "message_handler.h"
#include "client.h"

/**
 * All handlers for all the different types of messages.
 */
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class edit_handler:  public message_handler{
    void (*edit_func_ptr)(Message);
public:
    edit_handler(void (*edit_func_ptr)(Message message));
    void handle(Message message);
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class focus_handler: public message_handler{
    void (*focus_func_ptr)(Message);
public:
    focus_handler(void (*focus_func_ptr)(Message message));
    void handle(Message message);
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class unfocus_handler: public message_handler{
    void (*unfocus_func_ptr)(Message);
public:
    unfocus_handler(void (*unfocus_func_ptr)(Message message));
    void handle(Message message);
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class undo_handler: public message_handler{
    void (*undo_func_ptr)(Message);
public:
    undo_handler(void (*undo_func_ptr)(Message message));
    void handle(Message message);
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class revert_handler: public message_handler{
    void (*revert_func_ptr)(Message);
public:
    revert_handler(void (*revert_func_ptr)(Message message));
    void handle(Message message);
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class register_handler: public message_handler{
    void (*register_func_ptr)(Message);
public:
    register_handler(void (*register_func_ptr)(Message message));
    void handle(Message message);
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class load_handler: public message_handler{
    void (*load_func_ptr)(Message);
public:
    load_handler(void (*load_func_ptr)(Message message));
    void handle(Message message);
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class disconnect_handler: public message_handler{
    void (*disconnect_func_ptr)(Message);
public:
    disconnect_handler(void (*disconnect_func_ptr)(Message message));
    void handle(Message message);
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ping_handler: public message_handler{
    void (*ping_func_ptr)(Message);
public:
    ping_handler(void (*ping_func_ptr)(Message message));
    void handle(Message message);
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ping_resp_handler: public message_handler{
    void (*ping_resp_func_ptr)(Message);
public:
    ping_resp_handler(void (*ping_resp_func_ptr)(Message message));
    void handle(Message message);
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

#endif //COLLABSPREADSHEET_HANDLERS_H
