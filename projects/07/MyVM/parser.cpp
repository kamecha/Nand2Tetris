#include <fstream>
#include <sstream>

#include "parser.h"
using namespace std;

Parser::Parser(string fileName) {
	file.open(fileName);
	string line;
	while(getline(file, line)) {
		if(!line.empty() && line.back() == '\r') {
			line.pop_back();
		}
		lines.push_back(line);
	}
	file.close();
	ptr = -1;
}

void Parser::reset() {
	ptr = -1;
}

string omitComment(string line) {
	string ret = line;
	string::size_type ptr =  ret.find("//");
	if(ptr != string::npos) {
		ret = ret.substr(0, ptr);
	}
	return ret;
}

// string omitSpace(string line) {
// 	string ret = line; 
// 	ret.erase(remove(ret.begin(), ret.end(), ' '), ret.end());
// 	return ret;
// }

bool Parser::hasMoreCommands() {
	ptr++;
	while(ptr < lines.size()) {
		string ret = lines[ptr];
		ret = omitComment(ret);
		lines[ptr] = ret;
		if(ret.empty()) {
			ptr++;
		} else {
			return true;
		}
	}
	return false;
}

void Parser::advance() {
	command = lines[ptr];
	stringstream ss(command);
	string tmp;
	while(getline(ss, tmp, ' ')) {
		commandSplit.push_back(tmp);
	}
}

CommandType Parser::commandType() {
	string firstCommand;
	int ptr = command.find(' ');
	if(ptr == string::npos) {
		firstCommand = command;
	} else {
		firstCommand = command.substr(0, ptr);
	}

	if(firstCommand == "add")	return C_ARITHEMETIC;
	if(firstCommand == "sub")	return C_ARITHEMETIC;
	if(firstCommand == "neg")	return C_ARITHEMETIC;
	if(firstCommand == "eq")	return C_ARITHEMETIC;
	if(firstCommand == "gt")	return C_ARITHEMETIC;
	if(firstCommand == "lt")	return C_ARITHEMETIC;
	if(firstCommand == "and")	return C_ARITHEMETIC;
	if(firstCommand == "or")	return C_ARITHEMETIC;
	if(firstCommand == "not")	return C_ARITHEMETIC;

	if(firstCommand == "push")	return C_PUSH;
	if(firstCommand == "pop")	return C_POP;

	return C_NULL;
}

string Parser::arg1() {
	string ret;
	switch(Parser::commandType()) {
		case C_ARITHEMETIC:
			ret = command;
			break;
		case C_PUSH:
		case C_POP:
			ret = commandSplit[1];
			break;
		default:
			break;
	}
	return ret;
}

int Parser::arg2() {
	return stoi(commandSplit[2]);
}
