///
// Created by Gabriel Robinson on 3/30/2018.
//
#include "spreadsheet_state.h"
#include <queue>
#include <vector>
#include <sstream>
#include <iostream>
#include <fstream>
#include <string>
#include <iterator>
#include <cstdlib>

//debug macro
#define USEDEBUG //comment out to disable debug output
#ifdef USEDEBUG
#define Debug(x) std::cout << "DIS_DEBUG: " << x << std::endl;
#else
#define Debug(x)
#endif
//end debug macro


spreadsheet_state::spreadsheet_state() {}
/*
 * Constructor that initializes filename and various cell contents
 */
spreadsheet_state::spreadsheet_state(std::string file_path, std::string f_name) {
    // search file system
    std::string directory_and_file = file_path + f_name;

    system(std::string("touch " + directory_and_file).c_str());

    std::ifstream input_file(directory_and_file.c_str());
    std::string file_state = "";

    spreadsheet_state my_spread;

    if(!input_file.fail()) {

        std::string line;

        std::queue<std::string> tokens;

        while (std::getline(input_file, line, '\n')){
            tokens.push(line);
        }

        input_file.close();


        while (!tokens.empty() && tokens.front() != "end\u0003"){


            std::string cell_name;

            std::string item;

            std::stringstream ss_two(tokens.front());

            tokens.pop();

            std::getline(ss_two, cell_name, (char)3);
            std::cout<<"cell_init: " + cell_name<<std::endl;
            //cell_name = cell_name.substr(0, cell_name.length()-1);
            std::stack<std::string> elements;
            std::pair<std::string, std::stack<std::string> > cell(cell_name, elements);
            this->cells.insert(cell);

            while(std::getline(ss_two, item, (char)3))
            {
                //item = item.substr(0, item.length()-1);
                std::cout<<item<<std::endl;
                this->cells[cell_name].push(item);
            }

        }

        if(!tokens.empty())
            tokens.pop();

        while (!tokens.empty() && tokens.front() != "end\u0003"){

            std::string cell_name;

            std::string item;

            std::stringstream ss_two(tokens.front());

            tokens.pop();

            std::getline(ss_two, cell_name, (char)3);
            //cell_name = cell_name.substr(0, cell_name.length()-1);

            std::cout<<"revert_init: " + cell_name<<std::endl;

            std::stack<std::string> elements;
            std::pair<std::string, std::stack<std::string> > cell(cell_name, elements);
            this->reverts.insert(cell);

            while(std::getline(ss_two, item, (char)3)) {
                //item = item.substr(0, item.length()-1);
                this->reverts[cell_name].push(item);
                std::cout<<item<<std::endl;
            }

        }

        if(!tokens.empty())
            tokens.pop();

        std::stack<std::string> flip_stack;

        while (!tokens.empty() && tokens.front() != "end\u0003"){
            flip_stack.push(tokens.front());
            tokens.pop();
        }

        while(flip_stack.size() > 0) {
            this->previous_states.push(flip_stack.top());
            flip_stack.pop();
        }

        this->file_name = f_name;
        //write_spreadsheet_file("./Spreads/");

    } else {

        spreadsheet_state(f_name);
        //write_spreadsheet_file("Spreads/");

    }
}

/*
 * Constructor that only takes the filename and creates the state of an empty spreadsheet
 */
spreadsheet_state::spreadsheet_state(std::string f_name) {

    //init file name
    this->file_name = f_name;

}

/*
 * Changes the cell with the name specified to contain the specified contents
 */
std::string spreadsheet_state::change_cell_contents(std::string cell_name, std::string new_content) {

    std::map<std::string, std::stack<std::string> >::iterator it = this->cells.end();

    if(!this->cells.empty())
        it = this->cells.find(cell_name);
    // search for the cell name
    if(this->cells.empty() || it == this->cells.end()) {

        std::stack<std::string> st;
        st.push(new_content);
        std::pair<std::string, std::stack<std::string> > na_co(cell_name, st);
        this->cells.insert(na_co);

        std::stack<std::string> empty_stack;
        std::pair<std::string, std::stack<std::string> > empty_pair(cell_name, empty_stack);
        this->reverts.insert(empty_pair);

    } else if (it != this->cells.end()) {

        this->cells[cell_name].push(new_content);

    }

    this->previous_states.push(cell_name);
    std::cout<<"undo to " + cell_name + ":" + new_content<<std::endl;
    return cell_name + ":" + new_content;

}

/*
 * Reverts most recent change to the specified cell. Returns true or false
 * based upon whether the revert was successful.
 */
std::string spreadsheet_state::revert(std::string cell_name) {

    std::string return_arg = cell_name + ":";

    if(!cells.empty()) {

        // if cells contains the cell name and the associated stack is not empty
        // we take whatever is on top of our cell state stack and push it onto our
        // stack of reverts, and then we pop our cell state stack. We then return
        // true indicating that the revert was successful.
        // otherwise we return false.
        if (this->cells.count(cell_name) && !this->cells[cell_name].empty()) {

            if(!this->reverts.empty() && this->reverts.count(cell_name))
                this->reverts[cell_name].push(this->cells[cell_name].top());
            else {

                std::stack<std::string> rev_stack;
                rev_stack.push(this->cells[cell_name].top());
                std::pair<std::string, std::stack<std::string> > rev_pair(cell_name, rev_stack);
                this->reverts.insert(rev_pair);

            }

            this->previous_states.push(cell_name);
            this->previous_states.push("revert");
            this->cells[cell_name].pop();
            if(!this->cells[cell_name].empty())
                return_arg = return_arg + this->cells[cell_name].top();

        }


    }
    std::cout<<"revert to " + return_arg<<std::endl;

    return return_arg;

}

/*
 * Reverts most recent change
 */
std::string spreadsheet_state::undo() {

    std::string return_arg = "";

    // check to see that a change has been made
    if(!this->previous_states.empty()) {

        // pop the most recent change
        std::string undo_op = this->previous_states.top();
        this->previous_states.pop();

        // if the most recent change is a revert
        if (undo_op == "revert") {

            undo_op = this->previous_states.top();
            this->previous_states.pop();

            this->cells[undo_op].push(this->reverts[undo_op].top());
            this->reverts[undo_op].pop();

            return_arg = return_arg + undo_op + ":" + this->cells[undo_op].top();

        }
            // if its not a revert just remove the top from the cells stack
        else if (this->cells.count(undo_op) && !cells[undo_op].empty()){

            this->cells[undo_op].pop();

            // if there is something on the cells stack grab the data, otherwise there is no data
            if(!cells[undo_op].empty())
                return_arg = return_arg + undo_op + ":" +this->cells[undo_op].top();
            else return_arg = return_arg + undo_op + ":";

        }
        else return_arg = return_arg + undo_op + ":";

    }
    std::cout<<"undo to " + return_arg<<std::endl;
    return return_arg;

}

std::string spreadsheet_state::get_cell_contents(std::string cell_name) {

    std::map<std::string, std::stack<std::string> >::iterator it = this->cells.find(cell_name);

    if (it != this->cells.end() && !this->cells[cell_name].empty())
        return this->cells[cell_name].top();
    else return "";

}

// returns the full state of the spreadsheet but it does not at \3 to the end of the message
// this will have to be done by the handler.
std::string spreadsheet_state::get_full_state() {

    std::string ret_string = "";

    for (std::map<std::string, std::stack<std::string> >::iterator it=this->cells.begin(); it!=this->cells.end(); ++it)
        if(!it->second.empty())
            ret_string = ret_string + it->first + ":" + it->second.top() + "\n";

    ret_string = ret_string.substr(0, ret_string.length() - 1);

    return ret_string;

}

bool spreadsheet_state::operator<(const spreadsheet_state &other) const {

    return this->file_name < other.file_name;

}


const char* spreadsheet_state::get_file_name()  {

    return this->file_name.c_str();

}



void spreadsheet_state::write_spreadsheet_file(std::string path) {

    std::ofstream output_file;

    std::string file(path + (this->file_name));

    Debug(file)

    output_file.open(file.c_str());

    if(output_file.is_open()) {

        for(std::map<std::string, std::stack<std::string> >::iterator it = this->cells.begin(); it != this->cells.end(); it++) {

            output_file << it->first << (char)3;
            output_file.flush();
            std::stack<std::string> cell = it->second;
            std::vector<std::string> cells_vec;

            while (!cell.empty()) {
                cells_vec.push_back(cell.top());
                cell.pop();
            }

            for(std::vector<std::string>::reverse_iterator rit = cells_vec.rbegin(); rit != cells_vec.rend(); rit++) {
                output_file << *rit <<(char)3;
                output_file.flush();
            }

            output_file << "\n";
            output_file.flush();

        }

        output_file<<"end"<<(char)3<<"\n";
        output_file.flush();

        for(std::map<std::string, std::stack<std::string> >::iterator it = this->reverts.begin(); it != this->reverts.end(); it++) {

            output_file << it->first << (char)3;
            output_file.flush();
            std::stack<std::string> cell = it->second;
            std::vector<std::string> reverts_vec;


            while (!cell.empty()) {
                reverts_vec.push_back(cell.top());
                cell.pop();
            }


            for(std::vector<std::string>::reverse_iterator rit = reverts_vec.rbegin(); rit != reverts_vec.rend(); rit++) {
                output_file << *rit << (char)3;
                output_file.flush();
            }

            output_file<<"\n";
            output_file.flush();

        }

        output_file<<"end"<<(char)3<<"\n";
        output_file.flush();

        std::vector<std::string> prev_vec;
        std::stack<std::string> prev_stack = this->previous_states;
        while(!prev_stack.empty()) {
            output_file << prev_stack.top() << "\n";
            output_file.flush();
            prev_stack.pop();
        }

        output_file<<"end"<<(char)3<<"\n";
        output_file.flush();

        output_file.close();
    }
}