#include "symbolTable.h"
#include <string>

SymbolTable::SymbolTable() {
	scope = CLASS;
	classMap.clear();
	subroutineMap.clear();
	staticCount = 0;
	fieldCount = 0;
}

void SymbolTable::startSubroutine() {
	scope = SUBROUTINE;
	subroutineMap.clear();
	staticCount = 0;
	fieldCount = 0;
}

void SymbolTable::define(std::string name, std::string type, Kind kind) {
	SymbolInformation symInfo;
	symInfo.type = type;
	symInfo.kind = kind;
	switch(kind) {
		case STATIC:
			symInfo.number = staticCount++;
			break;
		case FIELD:
			symInfo.number = fieldCount++;
			break;
		default:
			break;
	}
	subroutineMap[name] = symInfo;
}

int SymbolTable::varCount(Kind kind) {
	int count = 0;
	for(const auto& [name, val]: classMap) {
		if(val.kind == kind)
			count++;
	}
	for(const auto& [name, val]: subroutineMap) {
		if(val.kind == kind)
			count++;
	}
	return count;
}

Kind SymbolTable::kindOf(std::string name) {
	switch(scope) {
		case CLASS:
			if(classMap.contains(name)) {
				return classMap[name].kind;
			}
			break;
		case SUBROUTINE:
			if(classMap.contains(name)) {
				return subroutineMap[name].kind;
			}
			break;
	}
	return NONE;
}

std::string SymbolTable::typeOf(std::string name) {
	switch(scope) {
		case CLASS:
			return classMap[name].type;
			break;
		case SUBROUTINE:
			return subroutineMap[name].type;
			break;
	}
}

int SymbolTable::indexOf(std::string name) {
	switch(scope) {
		case CLASS:
			return classMap[name].number;
			break;
		case SUBROUTINE:
			return subroutineMap[name].number;
			break;
	}
}
