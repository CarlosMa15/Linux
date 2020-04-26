//
// Created by Lukas Gust on 4/4/2018.
//
#include "client.h"

client::client() :ping_timer(){}

//client::~client(){}

void client::set_id(unsigned long id) {
    this->id = id;
}

void client::load_client() {this->loaded = 1;}

void client::register_client() {this->registered = 1;}

bool client::is_registered() {return this->registered;}

bool client::is_loaded() { return this->loaded;}

void client::reset_time() { ping_timer = time(NULL); }

bool client::is_timeout() { return (difftime(time(NULL), ping_timer)) >= 60.0;}

bool client::operator<(const client &other) const {
    return this->id < other.id;
}
