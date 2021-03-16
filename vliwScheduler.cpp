#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#include <algorithm>
#include <vector>

/* rpodaralla */
#include <iterator> 
#include <map> 
using namespace std; 
/* rpodaralla */

std::vector<std::string> prologue;
std::vector<std::string> epilogue;

/*
Read instructions from an input file and return all instructions 
*/
std::vector<std::string> readInput(const char* fname)
{
    std::string str;
    std::vector<std::string> instructions;
    
    std::ifstream filestream(fname);
    std::string state = "prologue";
    int start_of_data = 2;

    while (std::getline(filestream, str))
    {
        std::size_t pos = str.find("#### BEGIN BASIC BLOCK ####");
        if (pos == 0) {
            state = "instructions";
            continue;
        }
        pos = str.find("#### END BASIC BLOCK ####");
        if (pos == 0) {
            state = "epilogue";
        }
        if (state == "prologue"){
            prologue.push_back(str);
        }
        if (state == "instructions"){
            instructions.push_back(str);
        }
        if (state == "epilogue"){
            epilogue.push_back(str);
        }
    }
   
   return instructions;
}

/*
Print scheduled VLIW instructions to a file.
*/
void printOutput(const char* fname, std::vector<std::string> scheduledVLIW)
{
    std::ofstream outfile;
    outfile.open(fname);
  
    for (int i = 0; i < prologue.size(); i++)
        outfile << prologue[i] << "\n";

    for (int i = 0; i < scheduledVLIW.size(); i++)
        outfile << scheduledVLIW[i] << "\n";

    for (int i = 0; i < epilogue.size(); i++)
        outfile << epilogue[i] << "\n";

    outfile.close();
}

/*
TODO : Write any helper functions that you may need here. 
*/

int get_latency(string op) {
    // latency per op type
    map<string, int> op_latencies; 
    op_latencies.insert(pair<string, int>("intALUs", 1)); 
    op_latencies.insert(pair<string, int>("intMULs", 2)); 
    op_latencies.insert(pair<string, int>("memLDs", 3)); 
    op_latencies.insert(pair<string, int>("memSWs", 1));
    return op_latencies[op];
}

int get_res_unit(string fu) {
    // functional units per op type
    //int R[4] = {4, 2, 1, 1}; //8 resources: intALUs - 4 || intMULs - 2 || memLDs - 1 || memSWs - 1
    map<string, int> functional_units; 
    functional_units.insert(pair<string, int>("intALUs", 0)); 
    functional_units.insert(pair<string, int>("intMULs", 1)); 
    functional_units.insert(pair<string, int>("memLDs",2)); 
    functional_units.insert(pair<string, int>("memSWs", 3));
    return functional_units[fu];
}

int get_res_num_units(string fu) {
    // functional units per op type
    //int R[4] = {4, 2, 1, 1}; //8 resources: intALUs - 4 || intMULs - 2 || memLDs - 1 || memSWs - 1
    map<string, int> functional_units; 
    functional_units.insert(pair<string, int>("intALUs", 4)); 
    functional_units.insert(pair<string, int>("intMULs", 2)); 
    functional_units.insert(pair<string, int>("memLDs", 1)); 
    functional_units.insert(pair<string, int>("memSWs", 1));
    return functional_units[fu];
}

string get_op_type(string op) {
    // classify instruction types
    map<string, string> op_type;
    op_type.insert(pair<string, string>("ADD", "intALUs")); 
    op_type.insert(pair<string, string>("ADDCG", "intALUs")); 
    op_type.insert(pair<string, string>("AND", "intALUs")); 
    op_type.insert(pair<string, string>("ANDC", "intALUs")); 
    op_type.insert(pair<string, string>("DIVS", "intALUs")); 
    op_type.insert(pair<string, string>("MAX", "intALUs")); 
    op_type.insert(pair<string, string>("MAXU", "intALUs")); 
    op_type.insert(pair<string, string>("MIN", "intALUs")); 
    op_type.insert(pair<string, string>("MINU", "intALUs")); 
    op_type.insert(pair<string, string>("OR", "intALUs")); 
    op_type.insert(pair<string, string>("ORC", "intALUs")); 
    op_type.insert(pair<string, string>("SH1ADD", "intALUs")); 
    op_type.insert(pair<string, string>("SH2ADD", "intALUs")); 
    op_type.insert(pair<string, string>("SH3ADD", "intALUs")); 
    op_type.insert(pair<string, string>("SH4ADD", "intALUs")); 
    op_type.insert(pair<string, string>("SHL", "intALUs")); 
    op_type.insert(pair<string, string>("SHR", "intALUs")); 
    op_type.insert(pair<string, string>("SHRU", "intALUs")); 
    op_type.insert(pair<string, string>("SUB", "intALUs")); 
    op_type.insert(pair<string, string>("SXTB", "intALUs")); 
    op_type.insert(pair<string, string>("SXTH", "intALUs"));
    op_type.insert(pair<string, string>("ZXTB", "intALUs")); 
    op_type.insert(pair<string, string>("ZXTH", "intALUs"));
    op_type.insert(pair<string, string>("XOR", "intALUs"));
    op_type.insert(pair<string, string>("MOV", "intALUs")); //interpret mov a = b as add a = b, 0
    op_type.insert(pair<string, string>("CMPEQ", "intALUs")); //logical and select ops from table 3 use ALU - assumption
    op_type.insert(pair<string, string>("CMPGE", "intALUs"));
    op_type.insert(pair<string, string>("CMPGEU", "intALUs"));
    op_type.insert(pair<string, string>("CMPGT", "intALUs"));
    op_type.insert(pair<string, string>("CMPGTU", "intALUs"));
    op_type.insert(pair<string, string>("CMPLE", "intALUs"));
    op_type.insert(pair<string, string>("CMPLEU", "intALUs"));
    op_type.insert(pair<string, string>("CMPLT", "intALUs"));
    op_type.insert(pair<string, string>("CMPLTU", "intALUs"));
    op_type.insert(pair<string, string>("CMPNE", "intALUs"));
    op_type.insert(pair<string, string>("NANDL", "intALUs"));
    op_type.insert(pair<string, string>("NORL", "intALUs"));
    op_type.insert(pair<string, string>("ORL", "intALUs"));
    op_type.insert(pair<string, string>("SLCT", "intALUs"));
    op_type.insert(pair<string, string>("SLCTF", "intALUs"));


    op_type.insert(pair<string, string>("MPYLL", "intMULs"));
    op_type.insert(pair<string, string>("MPYLLU", "intMULs"));
    op_type.insert(pair<string, string>("MPYLH", "intMULs"));
    op_type.insert(pair<string, string>("MPYLHU", "intMULs"));
    op_type.insert(pair<string, string>("MPYHH", "intMULs"));
    op_type.insert(pair<string, string>("MPYHHU", "intMULs"));
    op_type.insert(pair<string, string>("MPYL", "intMULs"));
    op_type.insert(pair<string, string>("MPYLU", "intMULs"));
    op_type.insert(pair<string, string>("MPYH", "intMULs"));
    op_type.insert(pair<string, string>("MPYHU", "intMULs"));
    op_type.insert(pair<string, string>("MPYHS", "intMULs"));

    op_type.insert(pair<string, string>("LDW", "memLDs"));
    op_type.insert(pair<string, string>("LDH", "memLDs"));
    op_type.insert(pair<string, string>("LDHU", "memLDs"));
    op_type.insert(pair<string, string>("LDB", "memLDs"));
    op_type.insert(pair<string, string>("LDBU", "memLDs"));

    op_type.insert(pair<string, string>("STW", "memSWs"));
    op_type.insert(pair<string, string>("STH", "memSWs"));
    op_type.insert(pair<string, string>("STB", "memSWs"));
    return op_type[op];
}

string get_parsed_op(string unparsed_op) {
    // return parsed instruction or returns empty for NOP
    string str = unparsed_op;
    str.erase(str.find("\tc0    "), 7);
    str = str.substr(0, str.find("##"));
    str.erase(find_if(str.rbegin(), str.rend(), bind1st(not_equal_to<char>(), ' ')).base(), str.end());
    //cout << "!!!! " << str << " !!!!" << endl;
    return str;
}

string get_fu_used(string parsed_op) {
    string fu = parsed_op.substr(0, parsed_op.find(" "));
    for (auto & c: fu) c = toupper(c);
    return get_op_type(fu);
}

//get all registers on right (R) or left (L) side of = sign in instr
vector<string> get_regs(string parsed_op, string side) {
    vector<string> regs;
    string s = "";

    // right regs
    if(side == "R") {
        unsigned first = parsed_op.find("= ");
        s = parsed_op.substr(first);
    } else if(side == "L") {
        unsigned first = parsed_op.find(" ");
        unsigned last = parsed_op.find(" =");
        s = parsed_op.substr(first,last-first);
    }

    std::string delimiter = ",";
    size_t pos = 0;
    std::string token;

    while ((pos = s.find(delimiter)) != std::string::npos) {
        token = s.substr(0, pos);
        if(token.find("$") != std::string::npos) {
            // we only care about registers with $ - else we return empty
            if(token.find("]") != std::string::npos) {
                unsigned first = token.find("$");
                unsigned last = token.find("]");
                token = token.substr(first,last-first);
                regs.push_back(token);
                //std::cout << "||" << token << "||" << std::endl;
            } else {
                token = token.substr(token.find("$"));
                regs.push_back(token);
                //std::cout << "|||" << token << "|||" << std::endl;
            }
        }
        s.erase(0, pos + delimiter.length());
    }

    if(s.find("$") != std::string::npos) {
        if(s.find("]") != std::string::npos) {
            unsigned first = s.find("$");
            unsigned last = s.find("]");
            s = s.substr(first,last-first);
            regs.push_back(s);
            //std::cout << "**" << s << "**" << std::endl;
        } else {
            s = s.substr(s.find("$"));
            regs.push_back(s);
            //std::cout << "***" << s << "***" << std::endl;
        }
    }

    //will return empty if no read registers $
    return regs;
}

vector<string> get_read_regs(string parsed_op, string op_type) {
    vector<string> read_regs;
    if(op_type == "intALUs") {
        read_regs = get_regs(parsed_op, "R");
    } else if (op_type == "intMULs") {
        read_regs =  get_regs(parsed_op, "R");
    } else if (op_type == "memLDs") {
        read_regs = get_regs(parsed_op, "R");
        read_regs.push_back("$mem");
    } else if (op_type == "memSWs") {
        read_regs = get_regs(parsed_op, "R");
        vector<string> tmp = get_regs(parsed_op, "L");
        read_regs.insert(read_regs.end(), tmp.begin(), tmp.end());
    }
    return read_regs;
}

vector<string> get_write_regs(string parsed_op, string op_type) {
    vector<string> write_regs;
    if(op_type == "intALUs") {
        write_regs = get_regs(parsed_op, "L");
    } else if (op_type == "intMULs") {
        write_regs = get_regs(parsed_op, "L");
    } else if (op_type == "memLDs") {
        write_regs = get_regs(parsed_op, "L");
    } else if (op_type == "memSWs") {
        write_regs.push_back("$mem");
    }
    return write_regs;
}

bool is_dependency(string parsed_curr_op, string parsed_prev_op, string reg, string dependancy) {
    vector<string> prev_write_regs = get_write_regs(parsed_prev_op, get_fu_used(parsed_prev_op));
    vector<string> prev_read_regs = get_read_regs(parsed_prev_op, get_fu_used(parsed_prev_op));
    bool res = false;

    if(dependancy == "RAW") { 
        //check if any matching between: prev write registers || curr read register
        if(std::find(prev_write_regs.begin(), prev_write_regs.end(), reg) != prev_write_regs.end()) {
            res = true;    /* contains x */
        } 
    } else if(dependancy == "WAW") {
        //check if any matching between: prev write registers || curr write register
        if(std::find(prev_write_regs.begin(), prev_write_regs.end(), reg) != prev_write_regs.end()) {
            res = true;    /* contains x */
        }    
    } else if(dependancy == "WAR") {
        //check if any matching between: prev read registers || curr write register
        if(std::find(prev_read_regs.begin(), prev_read_regs.end(), reg) != prev_read_regs.end()) {
            res = true;    /* contains x */
        }  
    }
    return res;
}

float get_resource_criticality(string fu, vector<string> instr_source) {
    //criticality = (#ops that use resource)/(#resources)
    int num_ops = 0;
    int num_resources = get_res_num_units(fu);
    for(int i = 0; i < instr_source.size(); i++) {
        if(get_fu_used(get_parsed_op(instr_source[i])) == fu) num_ops++;
    }
    return (float)num_ops/(float)num_resources;
}

int get_critical_path(int node, vector<string> instr_source, map<int, vector<int>> dependencies) {
    if(dependencies.find(node) == dependencies.end()) { //key not found - doesn't have children
        return get_latency(get_fu_used(get_parsed_op(instr_source[node])));
    } else {
        int max = 0;
        for(int i = 0; i < dependencies[node].size(); i++) {
            int tmp = get_critical_path(dependencies[node][i], instr_source, dependencies);
            if(tmp > max) max = tmp;
        }
        return get_latency(get_fu_used(get_parsed_op(instr_source[node]))) + max;
    }
}

bool is_parent(int node, map<int, vector<int>> dependencies) {
    //if node is a parent, it won't exist on the left side of any map values
    bool res = true;
    map<int, vector<int>>::iterator it;
    for (it = dependencies.begin(); it != dependencies.end(); it++)
    {
        for(int i: it->second) if(i == node) return false;
    }
    return res;
}

vector<int> get_ready_nodes(vector<int> priority_order, map<int, vector<int>> dependencies) {
    vector<int> ready_nodes;
    for(int i = 0; i < priority_order.size(); i++) {
        if(is_parent(priority_order[i], dependencies)) {
            ready_nodes.push_back(priority_order[i]);
        }
    }
    return ready_nodes;
}

/*
Inputs:
    - std::vector<std::string> instructions. The input is a vector of strings. Each
      string in the vector is an instruction in the original vex code.
    - <int> mode: value indicating which heuristic ordering to use
Returns : std::vector<std::string>

The function should return a vector of strings. Each string should be a scheduled instruction or ;; that marks the end of a VLIW instruction word.
*/
std::vector<std::string>  scheduleVLIW(std::vector<std::string> instructions,
                                       int mode)
{
    std::vector<std::string> scheduledVLIW;

    /* TODO : Implement your code here */
    int instr_count = 0;

    /**************************************************************/
    /* 1. (easy) Store instructions in source order 0 -> mov a, b */
    /**************************************************************/
    for(int i=0; i < instructions.size(); i++){
        if(instructions[i].find(";;") == string::npos) { //doesn't have ;;
            instr_count++;
            /*debug info
            cout << get_fu_used(get_parsed_op(instructions[i])) << endl;
            cout << get_latency(get_fu_used(get_parsed_op(instructions[i]))) << endl;

            vector<string> lregs = get_regs(get_parsed_op(instructions[i]), "L");
            cout << "left registers" << endl;
            for(int r=0; r < lregs.size(); r++) {
                cout << lregs[r] << endl;
            }

            vector<string> rregs = get_regs(get_parsed_op(instructions[i]), "R");
            cout << "right registers" << endl;
            for(int r=0; r < rregs.size(); r++) {
                cout << rregs[r] << endl;
            }
            cout << "" << endl;
            /*debug info*/
        }
    }
    vector<string> instr_source;

    instr_count = 0;
    for(int i=0; i < instructions.size(); i++){
        if(instructions[i].find(";;") == string::npos) { //doesn't have ;;
            cout << instr_count << "->" << get_parsed_op(instructions[i]) << "\tlatency: " << get_latency(get_fu_used(get_parsed_op(instructions[i]))) << endl;
            instr_source.push_back(instructions[i]);
            instr_count++;
        }
    }

    cout << "\n\n" << endl;
    /**************************************************************/
    /* 2. (hard) Make dependancy graph 
    /**************************************************************/
    map<int, vector<int>> dependencies; //parent -> children
    cout << "Dependencies" << endl;
    for (int i = instr_count-1; i >= 0; i--) {
        //http://www.webgraphviz.com/ -> to visualize while debugging
        //cout << i << ":" << instr_source[i] << endl;
        cout << "\"" << i << "\"" << endl;
        vector<string> read_regs = get_read_regs(get_parsed_op(instr_source[i]), get_fu_used(get_parsed_op(instr_source[i])));
        vector<string> write_regs = get_write_regs(get_parsed_op(instr_source[i]), get_fu_used(get_parsed_op(instr_source[i])));
        //check for dependencies with all instructions that came before it

        // a. check for latest RAW dependencies on each of the read registers
        for(int r = 0; r < read_regs.size(); r++) {
            for (int d = i-1; d >= 0; d--) {
                if(is_dependency(get_parsed_op(instr_source[i]), get_parsed_op(instr_source[d]), read_regs[r], "RAW")) {
                    if(dependencies.find(d) == dependencies.end()) { //key not found
                        vector<int> tmp = {i};
                        dependencies.insert(pair<int, vector<int>>(d, tmp)); 
                    } else dependencies[d].push_back(i);
                    //cout << d << ":" << "\t\tRAW:\t" << read_regs[r] << instr_source[d] << endl;
                    cout << "\"" << d << "\"->\"" << i << "\"" << endl;
                    break; //break and move on to next reg, we already found most recent dependency for reg
                }
            }
        }
        // b. check for latest WAR/WAW dependencies on each of the write registers
        for(int r = 0; r < write_regs.size(); r++) {
            for (int d = i-1; d >= 0; d--) {
                if(is_dependency(get_parsed_op(instr_source[i]), get_parsed_op(instr_source[d]), write_regs[r], "WAR")) {
                    if(dependencies.find(d) == dependencies.end()) { //key not found
                        vector<int> tmp = {i};
                        dependencies.insert(pair<int, vector<int>>(d, tmp)); 
                    } else dependencies[d].push_back(i);
                    //cout << d << ":" << "\t\tWAR:\t" << write_regs[r] << instr_source[d] << endl;
                    cout << "\"" << d << "\"->\"" << i << "\"" << endl;
                    break;
                } else if(is_dependency(get_parsed_op(instr_source[i]), get_parsed_op(instr_source[d]), write_regs[r], "WAW")) {
                    if(dependencies.find(d) == dependencies.end()) { //key not found
                        vector<int> tmp = {i};
                        dependencies.insert(pair<int, vector<int>>(d, tmp)); 
                    } else dependencies[d].push_back(i);
                    //cout << d << ":" << "\t\tWAW:\t" << write_regs[r] << instr_source[d] << endl;
                    cout << "\"" << d << "\"->\"" << i << "\"" << endl;
                    break;
                }
            }
        }
    }
    cout << "\n\n" << endl;

    /**************************************************************/
    /* 3. (hard) Prioritized Topological sort based on tie breakers - implement helper functtions */
    /**************************************************************/
    //get critical path length
    vector<int> pri;
    for(int i = 0; i < instr_count; i++) {
        int len = get_critical_path(i, instr_source, dependencies);
        pri.push_back(len); 
    }
    sort(pri.begin(), pri.end(), greater<int>());
    pri.erase( unique(pri.begin(), pri.end() ), pri.end() );
    cout << "Critical Path Length: " << pri[0] << endl;



    //find prioritized order
    vector<int> N;

    //get_critical_path(instr_source[0], dependencies);
    if(mode == 0 || mode == 6) { // Source (I did not implement extra credit renaming)
        for(int i = 0; i < instr_count; i++) {
            N.push_back(i); 
        }
    } else if(mode == 1) { //Critical Path - Source   
        vector<int> priorities;

        for(int i = 0; i < instr_count; i++) {
            int len = get_critical_path(i, instr_source, dependencies);
            priorities.push_back(len); 
        }
        sort(priorities.begin(), priorities.end(), greater<int>());
        priorities.erase( unique(priorities.begin(), priorities.end() ), priorities.end() );

        for(int p: priorities) {
            for(int i = 0; i < instr_count; i++) {
                int len = get_critical_path(i, instr_source, dependencies);
                if(p == len) N.push_back(i); 
            }
        }  
    } else if(mode == 2) { //Resource - Source
        vector<float> priorities;

        for(int i = 0; i < instr_count; i++) {
            float len = get_resource_criticality(get_fu_used(get_parsed_op(instr_source[i])), instr_source);
            priorities.push_back(len); 
        }
        sort(priorities.begin(), priorities.end(), greater<float>());
        priorities.erase( unique(priorities.begin(), priorities.end() ), priorities.end() );

        for(float p: priorities) {
            for(int i = 0; i < instr_count; i++) {
                float len = get_resource_criticality(get_fu_used(get_parsed_op(instr_source[i])), instr_source);
                if(p == len) N.push_back(i); 
            }
        }
    } else if(mode == 3) { //Fanout - Source
        vector<int> priorities;

        for(int i = 0; i < instr_count; i++) {
            int len = dependencies[i].size();
            priorities.push_back(len); 
        }
        sort(priorities.begin(), priorities.end(), greater<int>());
        priorities.erase( unique(priorities.begin(), priorities.end() ), priorities.end() );

        for(int p: priorities) {
            for(int i = 0; i < instr_count; i++) {
                int len = dependencies[i].size();
                if(p == len) N.push_back(i); 
            }
        }  
    } else if(mode == 4) { //Critical Path - Resource - Source
        vector<int> priorities;
        for(int i = 0; i < instr_count; i++) {
            int len = get_critical_path(i, instr_source, dependencies);
            priorities.push_back(len); 
        }
        sort(priorities.begin(), priorities.end(), greater<int>());
        priorities.erase( unique(priorities.begin(), priorities.end() ), priorities.end() );


        vector<float> resources;
        for(int i = 0; i < instr_count; i++) {
            float len = get_resource_criticality(get_fu_used(get_parsed_op(instr_source[i])), instr_source);
            resources.push_back(len); 
        }
        sort(resources.begin(), resources.end(), greater<float>());
        resources.erase( unique(resources.begin(), resources.end() ), resources.end() );

        for(int p: priorities) {
            for(float r: resources) {
                for(int i = 0; i < instr_count; i++) {
                    int p_len = get_critical_path(i, instr_source, dependencies);
                    float r_len = get_resource_criticality(get_fu_used(get_parsed_op(instr_source[i])), instr_source);
                    if((p == p_len) && (r == r_len)) N.push_back(i); 
                }
            }
        }  
    } else if(mode == 5) { //Critical Path - Fanout - Source
        vector<int> priorities;
        for(int i = 0; i < instr_count; i++) {
            int len = get_critical_path(i, instr_source, dependencies);
            priorities.push_back(len); 
        }
        sort(priorities.begin(), priorities.end(), greater<int>());
        priorities.erase( unique(priorities.begin(), priorities.end() ), priorities.end() );


        vector<int> fans;
        for(int i = 0; i < instr_count; i++) {
            int len = dependencies[i].size();
            fans.push_back(len); 
        }
        sort(fans.begin(), fans.end(), greater<int>());
        fans.erase( unique(fans.begin(), fans.end()), fans.end() );

        for(int p: priorities) {
            for(int r: fans) {
                for(int i = 0; i < instr_count; i++) {
                    int p_len = get_critical_path(i, instr_source, dependencies);
                    int r_len = dependencies[i].size();
                    if((p == p_len) && (r == r_len)) N.push_back(i); 
                }
            }
        }  
    }

    cout << "Priority Order" << endl;
    for(int i: N) cout << i << " "; 
    cout << "\n\n" << endl;

    /**************************************************************/
    /* 4. (medium) Implement List scheduling algorithm */
    /**************************************************************/
    int cycle = 1;

    map<int, vector<int>> S;
    int RT[4] = {4, 2, 1, 1};
    vector<int> ready_nodes;
    map<int, int> in_progress_nodes; //map node -> cycles complete


    while(N.size() != 0) {
        vector<int> vliw;
        vector<int> ready_nodes;
        vector<int> completed_nodes;

        map<int, int>::iterator it;
        for (it = in_progress_nodes.begin(); it != in_progress_nodes.end(); it++)
        {
            //update in progress nodes
            it->second += 1;
            //if latencies reached - clean up
            if(get_latency(get_fu_used(get_parsed_op(instr_source[it->first]))) == it->second) {
                // a. remove them from dependencies map
                dependencies.erase(it->first);
                // b. remove from N
                N.erase(std::remove(N.begin(), N.end(), it->first), N.end());
                // c. release their resources
                RT[get_res_unit(get_fu_used(get_parsed_op(instr_source[it->first])))] += 1;
                // d. remove the from in_progress so they don't get processed again
                completed_nodes.push_back(it->first);
            }
        }
        for(int c: completed_nodes)in_progress_nodes.erase(c);

        ready_nodes = get_ready_nodes(N, dependencies);

        //schedule nodes in ready_nodes in order 
        for(int i = 0; i < ready_nodes.size(); i++) {
            // make sure this node is not already being processed
            if((vliw.size() < 4) && (in_progress_nodes.find(ready_nodes[i]) == in_progress_nodes.end())) {
                //check if resource is available
                if(RT[get_res_unit(get_fu_used(get_parsed_op(instr_source[ready_nodes[i]])))] > 0) {
                    //add it to vliw/in_progress and claim resource
                    vliw.push_back(ready_nodes[i]);
                    //cout << ready_nodes[i] << endl;
                    in_progress_nodes.insert(pair<int, int>(ready_nodes[i], 0));
                    RT[get_res_unit(get_fu_used(get_parsed_op(instr_source[ready_nodes[i]])))] -= 1;
                }
            }
        }

        S.insert(pair<int, vector<int>>(cycle++, vliw)); 
    }

    /**************************************************************/
    /* 5. (easy) Write out instructions */
    /**************************************************************/
    cout << "Schedule Order" << endl;
    map<int, vector<int>>::iterator dit;
    int sched_length = 0;
    for(dit = S.begin(); dit != S.end(); dit++){
        cout << dit->first << "->";
        for(int c: dit->second) {
            cout << " " << c;
            scheduledVLIW.push_back(instr_source[c]);
        }
        scheduledVLIW.push_back(";;");
        cout << endl;
        sched_length++;
    }
    cout << "Schedule length: " << sched_length << endl;

    return scheduledVLIW;
}

int main(int argc, char *argv[])
{

   if(argc != 4) {
       std::cout << "Invalid parameters \n";
       std::cout << "Expected use ./vliwScheduler ";
       std::cout << "<input file name> <output file name> <mode>\n";
   }
 
   const char* inputFile = argv[1];
   const char* vliwSchedulerOutput = argv[2];
   int mode = atoi(argv[3]);

   std::vector<std::string> instructions;
   std::vector<std::string> scheduledVLIW;
 
   /* Read instructions from the file */
   instructions = readInput(inputFile);

   /* Schedule instructions */
   scheduledVLIW = scheduleVLIW(instructions, mode);

   /* Print scheduled instructions to file */
   printOutput(vliwSchedulerOutput, scheduledVLIW);
}
