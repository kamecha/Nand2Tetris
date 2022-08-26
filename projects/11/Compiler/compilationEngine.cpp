#include "compilationEngine.h"
#include "jackTokenizer.h"
#include "symbolTable.h"
#include <string>
#include <vector>

CompilationEngine::CompilationEngine(std::ifstream& inFile, std::ofstream& outFile) : jackTokenizer(inFile), symbolTable(), outFile(outFile) {
	if(jackTokenizer.hasMoreTokens())
		jackTokenizer.advance();
}

// keyword: 'class' | 'constructor' | 'function' | 'method' | 'field' | 'static' | 'var' | 'int' | 'char' | 'boolean' | 'void' | 'true' | 'false' | 'null' | 'this' | 'let' | 'do' | 'if' | 'else' | 'while' | 'return'
void CompilationEngine::compileKeyword() {
	switch(jackTokenizer.tokenType()) {
		case KEYWORD:
			outFile << "<keyword>" << " ";
			switch(jackTokenizer.keyWord()) {
				case KEY_CLASS:
					outFile << "class";
					break;
				case KEY_CONSTRUCTOR:
					outFile << "constructor";
					break;
				case KEY_FUNCTION:
					outFile << "function";
					break;
				case KEY_METHOD:
					outFile << "method";
					break;
				case KEY_FIELD:
					outFile << "field";
					break;
				case KEY_STATIC:
					outFile << "static";
					break;
				case KEY_VAR:
					outFile << "var";
					break;
				case KEY_INT:
					outFile << "int";
					break;
				case KEY_CHAR:
					outFile << "char";
					break;
				case KEY_BOOLEAN:
					outFile << "boolean";
					break;
				case KEY_VOID:
					outFile << "void";
					break;
				case KEY_TRUE:
					outFile << "true";
					break;
				case KEY_FALSE:
					outFile << "false";
					break;
				case KEY_NULL:
					outFile << "null";
					break;
				case KEY_THIS:
					outFile << "this";
					break;
				case KEY_LET:
					outFile << "let";
					break;
				case KEY_DO:
					outFile << "do";
					break;
				case KEY_IF:
					outFile << "if";
					break;
				case KEY_ELSE:
					outFile << "else";
					break;
				case KEY_WHILE:
					outFile << "while";
					break;
				case KEY_RETURN:
					outFile << "return";
					break;
				default:
					break;
			}
			outFile << " " << "</keyword>" << std::endl;
			break;
		default:
			break;
	}
}

// symbol: '{' | '}' | '(' | ')' | '[' | ']' | '.' | ',' | ';' | '+' | '-' | '*' | '/' | '&' | '|' | '<' | '>' | '=' | '~'  
void CompilationEngine::compileSymbol() {
	switch(jackTokenizer.tokenType()) {
		case SYMBOL:
			switch(jackTokenizer.symbol()) {
				case '<':
					outFile << "<symbol>" << " " << "&lt;" << " " << "</symbol>" << std::endl;
					break;
				case '>':
					outFile << "<symbol>" << " " << "&gt;" << " " << "</symbol>" << std::endl;
					break;
				case '&':
					outFile << "<symbol>" << " " << "&amp;" << " " << "</symbol>" << std::endl;
					break;
				default:
					outFile << "<symbol>" << " " << jackTokenizer.symbol() << " " << "</symbol>" << std::endl;
					break;
			}
			break;
		default:
			break;
	}
}

void CompilationEngine::compileIntegerConstant() {
	switch(jackTokenizer.tokenType()) {
		case INT_CONST:
			outFile << "<integerConstant>" << " " << jackTokenizer.intVal() << " " << "</integerConstant>" << std::endl;
			break;
		default:
			break;
	}
}

void CompilationEngine::compileStringConstnat() {
	switch(jackTokenizer.tokenType()) {
		case STRING_CONST:
			outFile << "<stringConstant>" << " " << jackTokenizer.stringVal() << " " << "</stringConstant>" << std::endl;
			break;
		default:
			break;
	}
}

// identifier: アルファベット、数字、アンダースコアの文字列。ただし数字から始まる文字列は除く
void CompilationEngine::compileIdentifier() {
	switch(jackTokenizer.tokenType()) {
		case IDENTIFIER:
			outFile << "symbol情報" << std::endl;
			outFile << "カテゴリ:" << std::endl;
			outFile << "属性:";
			switch(symbolTable.kindOf(jackTokenizer.identifier())) {
				case STATIC:
					outFile << "static" << std::endl;
					break;
				case FIELD:
					outFile << "field" << std::endl;
					break;
				case ARG:
					outFile << "arg" << std::endl;
					break;
				case VAR:
					outFile << "var" << std::endl;
					break;
				default:
					outFile << "登録されていないよ" << std::endl;
					break;
			}
			outFile << "実行番号:";
			outFile << symbolTable.indexOf(jackTokenizer.identifier()) << std::endl;
			outFile << "<identifier>" << " " << jackTokenizer.identifier() << " " << "</identifier>" << std::endl;
			break;
		default:
			break;
	}
}

/**
'class' className '{' classVarDec* subroutineDec* '}'
*/
void CompilationEngine::compileClass() {
	outFile << "<class>" << std::endl;
	// 'class'
	compileKeyword();
	if(jackTokenizer.hasMoreTokens()) jackTokenizer.advance();
	// className
	compileIdentifier();
	if(jackTokenizer.hasMoreTokens()) jackTokenizer.advance();
	// '{'
	compileSymbol();
	// classVarDec* 
	// ('static' | 'field') ...
	if(jackTokenizer.hasMoreTokens()) {
		std::string classVarDecStart = jackTokenizer.next();
		while(classVarDecStart == "static" || classVarDecStart == "field") {
			jackTokenizer.advance();
			compileClassVarDec();
			classVarDecStart = jackTokenizer.next();
		}
	}
	// subroutineDec*
	// ('constructor' | 'function' | 'method') ...
	if(jackTokenizer.hasMoreTokens()) {
		std::string subroutineDecStart = jackTokenizer.next();
		while(subroutineDecStart == "constructor" || subroutineDecStart == "function" || subroutineDecStart == "method") {
			jackTokenizer.advance();
			compileSubroutine();
			subroutineDecStart =  jackTokenizer.next();
		}
	}
	if(jackTokenizer.hasMoreTokens()) jackTokenizer.advance();
	// '}'
	compileSymbol();
	outFile << "</class>" << std::endl;
}

/**
('static' | 'field') type varName (',' varName)* ';'
type: 'int' | 'char' | 'boolean' | className
*/
void CompilationEngine::compileClassVarDec() {
	// symbolTable用変数
	std::string type;
	Kind kind;
	outFile << "<classVarDec>" << std::endl;
	// ('static' | 'field')
	switch(jackTokenizer.keyWord()) {
		case KEY_STATIC:
			kind = STATIC;
			compileKeyword();
			break;
		case KEY_FIELD:
			kind = FIELD;
			compileKeyword();
			break;
		default:
			break;
	}
	// type
	type = jackTokenizer.next();
	if(jackTokenizer.hasMoreTokens()) jackTokenizer.advance();
	switch(jackTokenizer.tokenType()) {
		case KEYWORD:
			switch(jackTokenizer.keyWord()) {
				case KEY_INT:
				case KEY_CHAR:
				case KEY_BOOLEAN:
					compileKeyword();
					break;
				default:
					break;
			}
			break;
		case IDENTIFIER:
			compileIdentifier();
			break;
		default:
			break;
	}
	// varName
	symbolTable.define(jackTokenizer.next(), type, kind);
	if(jackTokenizer.hasMoreTokens()) jackTokenizer.advance();
	compileIdentifier();
	// (',' varName)* ';'
	bool isEnd = false;
	while(jackTokenizer.hasMoreTokens()) {
		jackTokenizer.advance();
		switch(jackTokenizer.tokenType()) {
			case SYMBOL:
				if(jackTokenizer.symbol() == ';') {
					outFile << "<symbol>" << " ; " << "</symbol>" << std::endl;
					isEnd = true;
					break;
				}
				if(jackTokenizer.symbol() == ',') {
					outFile << "<symbol>" << " , " << "</symbol>" << std::endl;
					symbolTable.define(jackTokenizer.next(), type, kind);
					if(jackTokenizer.hasMoreTokens()) jackTokenizer.advance();
					compileIdentifier();
				}
				break;
			default:
				break;
		}
		if(isEnd)
			break;
	}
	outFile << "</classVarDec>" << std::endl;
}

/**
('constructor' | 'function' | 'method') ('void' | type) subroutineName '(' parameterList ')' subroutineBody
type: 'int' | 'char' | 'boolean' | className
subroutineBody: '{' varDec* statements '}'
*/
void CompilationEngine::compileSubroutine() {
	outFile << "<subroutineDec>" << std::endl;
	// ('constructor' | 'function' | 'method')
	switch(jackTokenizer.keyWord()) {
		case KEY_CONSTRUCTOR:
		case KEY_FUNCTION:
		case KEY_METHOD:
			compileKeyword();
			break;
		default:
			break;
	}
	if(jackTokenizer.hasMoreTokens())	jackTokenizer.advance();
	// ('void' | type)
	switch(jackTokenizer.tokenType()) {
		case KEYWORD:
			switch(jackTokenizer.keyWord()) {
				case KEY_VOID:
				case KEY_INT:
				case KEY_CHAR:
				case KEY_BOOLEAN:
					compileKeyword();
					break;
				default:
					break;
			}
			break;
		case IDENTIFIER:
			compileIdentifier();
			break;
		default:
			break;
	}
	if(jackTokenizer.hasMoreTokens())	jackTokenizer.advance();
	// subroutineName
	switch(jackTokenizer.tokenType()) {
		case IDENTIFIER:
			compileIdentifier();
			break;
		default:
			break;
	}
	if(jackTokenizer.hasMoreTokens())	jackTokenizer.advance();
	// '('
	switch(jackTokenizer.tokenType()) {
		case SYMBOL:
			compileSymbol();
			break;
		default:
			break;
	}
	if(jackTokenizer.hasMoreTokens())	jackTokenizer.advance();
	// parameterList
	// 0回の場合もあるため、advanceをしない
	compileParameterList();
	// ')'
	switch(jackTokenizer.tokenType()) {
		case SYMBOL:
			compileSymbol();
			break;
		default:
			break;
	}
	if(jackTokenizer.hasMoreTokens())	jackTokenizer.advance();
	compileSubroutineBody();
	outFile << "</subroutineDec>" << std::endl;
}

/**
subroutineBody: '{' varDec* statements '}'
*/
void CompilationEngine::compileSubroutineBody() {
	outFile << "<subroutineBody>" << std::endl;
	// subroutineBody: '{' varDec* statements '}'
	// '{'
	switch(jackTokenizer.tokenType()) {
		case SYMBOL:
			compileSymbol();
			break;
		default:
			break;
	}
	// varDec*
	// varDec: 'var' type varName (',' varName)* ';'
	while(jackTokenizer.hasMoreTokens() && jackTokenizer.next() == "var") {
		jackTokenizer.advance();
		compileVarDec();
	}
	// statements
	compileStatements();
	if(jackTokenizer.hasMoreTokens())	jackTokenizer.advance();
	// '}'
	compileSymbol();
	outFile << "</subroutineBody>" << std::endl;
}

/**
((type varName) (',' type varName)*)?
type: 'int' | 'char' | 'boolean' | className
*/
void CompilationEngine::compileParameterList() {
	outFile << "<parameterList>" << std::endl;
	bool flag = false;
	// type
	switch(jackTokenizer.tokenType()) {
		case KEYWORD:
			switch(jackTokenizer.keyWord()) {
				case KEY_INT:
				case KEY_CHAR:
				case KEY_BOOLEAN:
					compileKeyword();
					break;
				default:
					break;
			}
			break;
		case IDENTIFIER:
			compileIdentifier();
			break;
		default:
			flag = true;
			break;
	}
	if(flag) {
		outFile << "</parameterList>" << std::endl;
		return;
	}
	if(jackTokenizer.hasMoreTokens()) jackTokenizer.advance();
	// varName
	compileIdentifier();
	// (',' type varName)*
	while(jackTokenizer.hasMoreTokens() && jackTokenizer.next() == ",") {
		jackTokenizer.advance();
		// ','
		switch(jackTokenizer.tokenType()) {
			case SYMBOL:
				compileSymbol();
				break;
			default:
				break;
		}
		if(jackTokenizer.hasMoreTokens()) jackTokenizer.advance();
		// type
		switch(jackTokenizer.tokenType()) {
			case KEYWORD:
				switch(jackTokenizer.keyWord()) {
					case KEY_INT:
					case KEY_CHAR:
					case KEY_BOOLEAN:
						compileKeyword();
						break;
					default:
						break;
				}
				break;
			case IDENTIFIER:
				compileIdentifier();
				break;
			default:
				break;
		}
		if(jackTokenizer.hasMoreTokens()) jackTokenizer.advance();
		// varName
		compileIdentifier();
	}
	if(jackTokenizer.hasMoreTokens()) jackTokenizer.advance();
	outFile << "</parameterList>" << std::endl;
}

/**
'var' type varName (',' varName)* ';'
type: 'int' | 'char' | 'boolean' | className
*/
void CompilationEngine::compileVarDec() {
	// symbolTable用変数
	std::string type;
	Kind kind = VAR;
	outFile << "<varDec>" << std::endl;
	// 'var'
	switch(jackTokenizer.tokenType()) {
		case KEYWORD:
			compileKeyword();
			break;
		default:
			break;
	}
	// type
	type = jackTokenizer.next();
	if(jackTokenizer.hasMoreTokens()) jackTokenizer.advance();
	switch(jackTokenizer.tokenType()) {
		case KEYWORD:
			switch(jackTokenizer.keyWord()) {
				case KEY_INT:
				case KEY_CHAR:
				case KEY_BOOLEAN:
					compileKeyword();
					break;
				default:
					break;
			}
			break;
		case IDENTIFIER:
			compileIdentifier();
			break;
		default:
			break;
	}
	// varName
	symbolTable.define(jackTokenizer.next(), type, kind);
	if(jackTokenizer.hasMoreTokens()) jackTokenizer.advance();
	compileIdentifier();
	// (',' varName)*
	while(jackTokenizer.hasMoreTokens() && jackTokenizer.next() == ",") {
		jackTokenizer.advance();
		compileSymbol();
		symbolTable.define(jackTokenizer.next(), type, kind);
		if(jackTokenizer.hasMoreTokens()) jackTokenizer.advance();
		compileIdentifier();
	}
	if(jackTokenizer.hasMoreTokens()) jackTokenizer.advance();
	// ';'
	compileSymbol();
	outFile << "</varDec>" << std::endl;
}



/**
statements: statement*
statement: letStatement | ifStatement | whileStatement | doStatement | returnStatement
*/
void CompilationEngine::compileStatements() {
	outFile << "<statements>" << std::endl;
	while(jackTokenizer.hasMoreTokens() 
		&& (	jackTokenizer.next() == "let"
		||	jackTokenizer.next() == "if"
		||	jackTokenizer.next() ==  "while"
		||	jackTokenizer.next() == "do"
		||	jackTokenizer.next() == "return")) {
		jackTokenizer.advance();
		switch(jackTokenizer.keyWord()) {
			case KEY_LET:
				compileLet();
				break;
			case KEY_IF:
				compileIf();
				break;
			case KEY_WHILE:
				compileWhile();
				break;
			case KEY_DO:
				compileDo();
				break;
			case KEY_RETURN:
				compileReturn();
				break;
			default:
				break;
		}
	}
	outFile << "</statements>" << std::endl;
}


/**
letStatement: 'let' varName ('[' expression ']')? '=' expression ';'
*/
void CompilationEngine::compileLet() {
	outFile << "<letStatement>" << std::endl;
	// 'let'
	compileKeyword();
	if(jackTokenizer.hasMoreTokens()) jackTokenizer.advance();
	// varName
	compileIdentifier();
	if(jackTokenizer.hasMoreTokens()) jackTokenizer.advance();
	// ('[' expression ']')?
	if(jackTokenizer.symbol() == '[') {
		// '['
		compileSymbol();
		if(jackTokenizer.hasMoreTokens()) jackTokenizer.advance();
		compileExpression();
		if(jackTokenizer.hasMoreTokens()) jackTokenizer.advance();
		// ']'
		compileSymbol();
		if(jackTokenizer.hasMoreTokens()) jackTokenizer.advance();
	}
	// '='
	compileSymbol();
	if(jackTokenizer.hasMoreTokens()) jackTokenizer.advance();
	// expression
	compileExpression();
	if(jackTokenizer.hasMoreTokens()) jackTokenizer.advance();
	// ';'
	compileSymbol();
	outFile << "</letStatement>" << std::endl;
}

/**
ifStatement: 'if' '(' expression ')' '{' statements '}' ('else' '{' statements '}')?
*/
void CompilationEngine::compileIf() {
	outFile << "<ifStatement>" << std::endl;
	switch(jackTokenizer.keyWord()) {
		case KEY_IF:
			compileKeyword();
			break;
		default:
			break;
	}
	if(jackTokenizer.hasMoreTokens()) jackTokenizer.advance();
	// '('
	compileSymbol();
	if(jackTokenizer.hasMoreTokens()) jackTokenizer.advance();
	// expression
	compileExpression();
	if(jackTokenizer.hasMoreTokens()) jackTokenizer.advance();
	// ')'
	compileSymbol();
	if(jackTokenizer.hasMoreTokens()) jackTokenizer.advance();
	// '{'
	compileSymbol();
	// statements
	compileStatements();
	// '}'
	if(jackTokenizer.hasMoreTokens()) jackTokenizer.advance();
	compileSymbol();
	// ('else' '{' statements '}')?
	if(jackTokenizer.hasMoreTokens() && jackTokenizer.next() == "else") {
		jackTokenizer.advance();
		// 'else'
		compileKeyword();
		if(jackTokenizer.hasMoreTokens()) jackTokenizer.advance();
		// '{'
		compileSymbol();
		// statements
		compileStatements();
		if(jackTokenizer.hasMoreTokens()) jackTokenizer.advance();
		// '}'
		compileSymbol();
	}
	outFile << "</ifStatement>" << std::endl;
}

/**
whileStatement: 'while' '(' expression ')' '{' statements '}'
*/
void CompilationEngine::compileWhile() {
	outFile << "<whileStatement>" << std::endl;
	switch(jackTokenizer.tokenType()) {
		case KEYWORD:
			compileKeyword();
			break;
		default:
			break;
	}
	if(jackTokenizer.hasMoreTokens()) jackTokenizer.advance();
	// '('
	compileSymbol();
	if(jackTokenizer.hasMoreTokens()) jackTokenizer.advance();
	// expression
	compileExpression();
	if(jackTokenizer.hasMoreTokens()) jackTokenizer.advance();
	// ')'
	compileSymbol();
	if(jackTokenizer.hasMoreTokens()) jackTokenizer.advance();
	// '}'
	compileSymbol();
	// statements
	compileStatements();
	if(jackTokenizer.hasMoreTokens()) jackTokenizer.advance();
	// '}'
	compileSymbol();
	outFile << "</whileStatement>" << std::endl;
}

/**
doStatement: 'do' subroutineCall ';'
*/
void CompilationEngine::compileDo() {
	outFile << "<doStatement>" << std::endl;
	switch(jackTokenizer.tokenType()) {
		case KEYWORD:
			compileKeyword();
			break;
		default:
			break;
	}
	if(jackTokenizer.hasMoreTokens()) jackTokenizer.advance();
	// subroutineCall
	compileSubroutineCall();
	if(jackTokenizer.hasMoreTokens()) jackTokenizer.advance();
	// ';'
	compileSymbol();
	outFile << "</doStatement>" << std::endl;
}

/**
returnStatement: 'return' expression? ';'
*/
void CompilationEngine::compileReturn() {
	outFile << "<returnStatement>" << std::endl;
	switch(jackTokenizer.tokenType()) {
		case KEYWORD:
			compileKeyword();
			break;
		default:
			break;
	}
	// expression? ';'
	if(jackTokenizer.hasMoreTokens() && jackTokenizer.next() == ";") {
		jackTokenizer.advance();
		compileSymbol();
	} else {
		jackTokenizer.advance();
		compileExpression();
		if(jackTokenizer.hasMoreTokens()) jackTokenizer.advance();
		compileSymbol();
	}
	outFile << "</returnStatement>" << std::endl;
}

/**
expression: term (op term)*
op: '+' | '-' | '*' | '/' | '&' | '|' | '<' | '>' | '='
*/

const std::vector<std::string> ops = {
	"+" , "-" , "*" , "/" , "&" , "|" , "<" , ">" , "="
};

void CompilationEngine::compileExpression() {
	outFile << "<expression>" << std::endl;
	compileTerm();
	// opを一つの文字列に変換する
	std::string op = [&]() -> std::string {
		std::string ret;
		for(std::string op: ops)	ret += op;
		return ret;
	}();
	// (op term)*
	while(jackTokenizer.hasMoreTokens() && jackTokenizer.next().find_first_of(op) != std::string::npos) {
		jackTokenizer.advance();
		// op
		compileSymbol();
		// term
		if(jackTokenizer.hasMoreTokens()) jackTokenizer.advance();
		compileTerm();
	}
	outFile << "</expression>" << std::endl;
}

/**
term: integerConstant | stringConstant | keywordConstant | varName | varName '[' expression ']' | subroutineCall | '(' expression ')' | unaryOp term
unaryOp: '-' | '~'
keywordConstant: 'true' | 'false' | 'null' | 'this'
*/
void CompilationEngine::compileTerm() {
	outFile << "<term>" << std::endl;
	switch(jackTokenizer.tokenType()) {
		case INT_CONST:
			compileIntegerConstant();
			break;
		case STRING_CONST:
			compileStringConstnat();
			break;
			// keywordConstantの部分一致
		case KEYWORD:
			switch(jackTokenizer.keyWord()) {
				case KEY_TRUE:
				case KEY_FALSE:
				case KEY_NULL:
				case KEY_THIS:
					compileKeyword();
					break;
				default:
					break;
			}
			break;
		case IDENTIFIER:
			if(jackTokenizer.hasMoreTokens()) {
				switch(jackTokenizer.next()[0]) {
					// subroutineCall: subroutineName '(' expressionList ')' | (className | varName) '.' subroutineName '(' expressionList ')'
					case '(':
					case '.':
						compileSubroutineCall();
						break;
					// varName '[' expression ']'
					case '[':
						compileIdentifier();
						if(jackTokenizer.hasMoreTokens()) jackTokenizer.advance();
						// '['
						compileSymbol();
						if(jackTokenizer.hasMoreTokens()) jackTokenizer.advance();
						// expression
						compileExpression();
						if(jackTokenizer.hasMoreTokens()) jackTokenizer.advance();
						// ']'
						compileSymbol();
						break;
					// varName
					default:
						compileIdentifier();
						break;
				}
			}
			break;
		case SYMBOL:
			// '(' expression ')'
			// unaryOp term
			switch(jackTokenizer.symbol()) {
				case '(':
					compileSymbol();
					if(jackTokenizer.hasMoreTokens()) jackTokenizer.advance();
					compileExpression();
					if(jackTokenizer.hasMoreTokens()) jackTokenizer.advance();
					compileSymbol();
					break;
				case '-':
				case '~':
					compileSymbol();
					if(jackTokenizer.hasMoreTokens()) jackTokenizer.advance();
					compileTerm();
					break;
			}
			break;
		default:
			break;
	}
	outFile << "</term>" << std::endl;
}

/**
subroutineCall: subroutineName '(' expressionList ')' | (className | varName) '.' subroutineName '(' expressionList ')'
expressionList: (expression (',' expression)*)?
*/
void CompilationEngine::compileSubroutineCall() {
	// outFile << "<subroutineCall>" << std::endl;
	// (className | varName) '.'
	if(jackTokenizer.hasMoreTokens() && jackTokenizer.next() == ".") {
		compileIdentifier();
		jackTokenizer.advance();
		compileSymbol();
		jackTokenizer.advance();
	}
	// subroutineName '(' expressionList ')'
	compileIdentifier();
	if(jackTokenizer.hasMoreTokens()) jackTokenizer.advance();
	compileSymbol();
	if(jackTokenizer.hasMoreTokens()) jackTokenizer.advance();
	compileExpressionList();
	compileSymbol();
	// outFile << "</subroutineCall>" << std::endl;
}

/**
expressionList: (expression (',' expression)* )?
*/
void CompilationEngine::compileExpressionList() {
	outFile << "<expressionList>" << std::endl;
	bool isExist = true;
	// expression
	switch(jackTokenizer.tokenType()) {
		case INT_CONST:
		case STRING_CONST:
		case IDENTIFIER:
			compileExpression();
			break;
		case KEYWORD:
			switch(jackTokenizer.keyWord()) {
				case KEY_TRUE:
				case KEY_FALSE:
				case KEY_NULL:
				case KEY_THIS:
					compileExpression();
					break;
				default:
					isExist = false;
					break;
			}
			break;
		case SYMBOL:
			switch(jackTokenizer.symbol()) {
				case '-':
				case '~':
					compileSymbol();
					break;
				case '(':
					compileExpression();
					break;
				default:
					isExist = false;
					break;
			}
			break;
		default:
			isExist = false;
			break;
	}
	if(!isExist) {
		outFile << "</expressionList>" << std::endl;
		return;
	}
	// (',' expression)*
	while(jackTokenizer.hasMoreTokens() && jackTokenizer.next() == ",") {
		jackTokenizer.advance();
		compileSymbol();
		if(jackTokenizer.hasMoreTokens()) jackTokenizer.advance();
		compileExpression();
	}
	if(jackTokenizer.hasMoreTokens()) jackTokenizer.advance();
	outFile << "</expressionList>" << std::endl;
}
