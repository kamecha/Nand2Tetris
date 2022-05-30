#include <iostream>
#include <fstream>
#include <string>
#include <vector>

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

string omitSpace(string line) {
	string ret = line; 
	ret.erase(remove(ret.begin(), ret.end(), ' '), ret.end());
	return ret;
}

bool Parser::hasMoreCommands() {
	ptr++;
	while(ptr < lines.size()) {
		string ret = lines[ptr];
		ret = omitComment(omitSpace(ret));
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
}

CommandType Parser::commandType() {
	//"(OUTPUT_FIRST)"  -> "OUTPUT_FIRST)" になっている@判定が出てしまっている
	if(command[0] == '@') {
		return A_COMMAND;
	} else if(command[0] == '(') {
		return L_COMMAND;
	} else {
		return C_COMMAND;
	}
}

string Parser::symbol() {
	string ret;
	switch(commandType()) {
		//@Xxx
		case A_COMMAND:
			ret = command.substr(1);
			break;
		//(Xxx)
		case L_COMMAND:
			ret = command.substr(1);
			reverse(ret.begin(), ret.end());
			ret = ret.substr(1);
			reverse(ret.begin(), ret.end());
			break;
		case C_COMMAND:
			break;
	}
	return ret;
}

string Parser::dest() {
	string::size_type equal = command.find("=");
	if(equal == string::npos) {
		return "null";
	} else {
		return command.substr(0, equal);
	}
}

string Parser::comp() {
	string::size_type equal = command.find("=");
	string::size_type semicollon = command.find(";");
	string ret;
	if(semicollon == string::npos) {
		//dest=comp
		ret = command.substr(equal+1);
	} else {
		if(equal == string::npos) {	//comp;jump
			ret = command.substr(0, semicollon);
		} else {					//dest=comp;jump
			ret = command.substr(equal, semicollon - equal - 1);
		}
	}
	// cout << ret.size() << endl;
	// cout << "comp:" << ret << "|" << endl;
	return ret;
}

string Parser::jump() {
	string::size_type semicollon = command.find(";");
	string ret;
	if(semicollon == string::npos) {
		ret = "null";
	} else {
		//hoge;jump
		ret = command.substr(semicollon+1);
	}
	return ret;
}
