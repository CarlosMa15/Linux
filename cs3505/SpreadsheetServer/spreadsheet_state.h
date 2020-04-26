
//
// Created by Gabriel Robinson on 3/30/2018.
//

#ifndef COLLABORATIVESPREADSHEET_SPREADSHEET_STATE_H
#define COLLABORATIVESPREADSHEET_SPREADSHEET_STATE_H
#include <string>
#include <stack>
#include <map>

class spreadsheet_state {

public:
    spreadsheet_state();
    spreadsheet_state(std::string, std::string);
    spreadsheet_state(std::string);
    std::string change_cell_contents(std::string, std::string);
    std::string revert(std::string);
    std::string undo();
    std::string get_full_state();
    std::string get_cell_contents(std::string);
    bool operator<(const spreadsheet_state &other) const;
    const char* get_file_name();
    void write_spreadsheet_file(std::string);

private:
    std::string file_name;
    std::map<std::string, std::stack<std::string> > cells;
    std::stack<std::string> previous_states;
    std::map<std::string, std::stack<std::string> > reverts;

};


#endif //COLLABORATIVESPREADSHEET_SPREADSHEET_STATE_H
