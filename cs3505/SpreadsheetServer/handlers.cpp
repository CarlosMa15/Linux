//
// Created by Lukas Gust on 4/13/2018.
//

#include "handlers.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Edit message handler. Tries to handle message.
 * @param edit_func_ptr action to take if it is an edit message.
 */
edit_handler::edit_handler(void (*edit_func_ptr)(Message msg)) {
    this->edit_func_ptr = edit_func_ptr;
}

/**
 * Handle message, if it is not an edit message it will pass it to the next handler.
 * @param message message to handle.
 */
void edit_handler::handle(Message message) {
    if(message.get_key() == "edit" && message.get_sender()->is_loaded()) {
        std::string resp;
        resp.append("change ").append(message.get_content()).append("\u0003");
        message.set_response(resp);
        edit_func_ptr(message);
    }
    else
        message_handler::handle(message);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Focus message handler. Tries to handle message.
 * @param focus_func_ptr action to take if it is a focus message.
 */
focus_handler::focus_handler(void (*focus_func_ptr)(Message)) {
    this->focus_func_ptr = focus_func_ptr;
}

/**
 * Handle message, if it is not a focus message it will pass it to the next handler.
 * @param message message to handle.
 */
void focus_handler::handle(Message message) {
    if(message.get_key() == "focus" && message.get_sender()->is_loaded()) {
        std::string resp;
        resp.append("focus ").append("\u0003");
        message.set_response(resp);
        focus_func_ptr(message);
    }
    else
        message_handler::handle(message);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Unfocus message handler. Tries to handle message.
 * @param unfocus_func_ptr action to take if it is a unfocus message.
 */
unfocus_handler::unfocus_handler(void (*unfocus_func_ptr)(Message)) {
    this->unfocus_func_ptr = unfocus_func_ptr;
}

/**
 * Handle message, if it is not a unfocus message it will pass it to the next handler.
 * @param message message to handle.
 */
void unfocus_handler::handle(Message message) {
    if(message.get_key() == "unfocus" && message.get_sender()->is_loaded()) {
        std::string resp;
        resp.append("unfocus ").append("\u0003");
        message.set_response(resp);
        unfocus_func_ptr(message);
    }
    else
        message_handler::handle(message);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Undo message handler. Tries to handle message.
 * @param undo_func_ptr action to take if it is an undo message.
 */
undo_handler::undo_handler(void (*undo_func_ptr)(Message)) {
    this->undo_func_ptr = undo_func_ptr;
}

/**
 * Handle message, if it is not an undo message it will pass it to the next handler.
 * @param message message to handle.
 */
void undo_handler::handle(Message message) {
    if(message.get_key() == "undo" && message.get_sender()->is_loaded())
        undo_func_ptr(message);
    else
        message_handler::handle(message);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Revert message handler. Tries to handle message.
 * @param revert_func_ptr
 */
revert_handler::revert_handler(void (*revert_func_ptr)(Message)) {
    this->revert_func_ptr = revert_func_ptr;
}

/**
 * Handle message, if it is not a revert message it will pass it to the next handler.
 * @param message message to handle.
 */
void revert_handler::handle(Message message) {
    if(message.get_key() == "revert" && message.get_sender()->is_loaded())
        revert_func_ptr(message);
    else
        message_handler::handle(message);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Register message handler. Tries to handle message.
 * @param register_func_ptr action to take if it is a register message.
 */
register_handler::register_handler(void (*register_func_ptr)(Message)) {
    this->register_func_ptr = register_func_ptr;
}

/**
 * Handle message, if it is not a register message it will pass it to the next handler.
 * @param message message to handle.
 */
void register_handler::handle(Message message) {
    if(message.get_key() == "register" && !message.get_sender()->is_registered())
        register_func_ptr(message);
    else
        message_handler::handle(message);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Load message handler. Tries to handle message.
 * @param load_func_ptr action to take if it is a register message.
 */
load_handler::load_handler(void (*load_func_ptr)(Message message)) {
    this->load_func_ptr = load_func_ptr;
}

/**
 * Handle message, if it is not a register message it will pass it to the next handler.
 * @param message message to handle.
 */
void load_handler::handle(Message message) {
    if(message.get_key() == "load" && message.get_sender()->is_registered() && !message.get_sender()->is_loaded())
        load_func_ptr(message);
    else
        message_handler::handle(message);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Disconnect message handler. Tries to handle message.
 * @param disconnect_func_ptr action to take if it is a disconnect message.
 */
disconnect_handler::disconnect_handler(void (*disconnect_func_ptr)(Message)) {
    this->disconnect_func_ptr = disconnect_func_ptr;
}

/**
 * Handle message, if it is not a disconnect message it will pass it to the next handler.
 * @param message message to handle.
 */
void disconnect_handler::handle(Message message) {
    if(message.get_key() == "disconnect")
        disconnect_func_ptr(message);
    else
        message_handler::handle(message);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Ping message handler. Tries to handle message.
 * @param ping_func_ptr action to take if it is a ping message.
 */
ping_handler::ping_handler(void (*ping_func_ptr)(Message)) {
    this->ping_func_ptr = ping_func_ptr;
}

/**
 * Handle message, if it is not a ping message then it will pass it to the next handler.
 * @param message message to handle.
 */
void ping_handler::handle(Message message) {
    if(message.get_key() == "ping") {
        std::string resp;
        resp.append("ping_response \u0003");
        message.set_response(resp);
        ping_func_ptr(message);
    }
    else
        message_handler::handle(message);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/**
 * Ping response message handler. Tries to handle message.
 * @param ping_resp_func_ptr action to take if it is a ping response message.
 */
ping_resp_handler::ping_resp_handler(void (*ping_resp_func_ptr)(Message)) {
    this->ping_resp_func_ptr = ping_resp_func_ptr;
}

/**
 * Handles message, if it is not a ping response message then it pass it to the next handler.
 * @param message
 */
void ping_resp_handler::handle(Message message) {
    if(message.get_key() == "ping_response")
        ping_resp_func_ptr(message);
    else
        return;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
