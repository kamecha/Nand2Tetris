#include "compilationEngine.h"
#include "jackTokenizer.h"

CompilationEngine::CompilationEngine(std::ifstream inFile, std::ofstream outFile) {
	this.outFile = outFile;
	jackTokenizer = JackTokenizer(inFile);
	if(jackTokenizer.hasMoreTokens())
		jackTokenizer.advance();
}

// keyword: 'class' | 'constructor' | 'function' | 'method' | 'field' | 'static' | 'var' | 'int' | 'char' | 'boolean' | 'void' | 'true' | 'false' | 'null' | 'this' | 'let' | 'do' | 'if' | 'else' | 'while' | 'return'
void compileKeyword() {
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
void compileSymbol() {
	switch(jackTokenizer.tokenType()) {
		case SYMBOL:
			outFile << "<symbol>" << " " << jackTokenizer.symbol() << " " << "</symbol>" << std::endl;
			break;
		default:
			break;
	}
}

// identifier: アルファベット、数字、アンダースコアの文字列。ただし数字から始まる文字列は除く
void compileIdentifier() {
	switch(jackTokenizer.tokenType()) {
		case IDENTIFIER:
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
	compileKeyword();
	if(jackTokenizer.hasMoreTokens()) jackTokenizer.advance();
	compileIdentifier();
	if(jackTokenizer.hasMoreTokens()) jackTokenizer.advance();
	compileSymbol();
	while(jackTokenizer.hasMoreTokens()) {
		jackTokenizer.advance();
		switch(jackTokenizer.tokenType()) {
			case KEYWORD:
				switch(jackTokenizer.keyWord()) {
					case KEY_VAR:
						CompilationEngine::compileClassVarDec();
						break;
					case KEY_CONSTRUCTOR:
					case KEY_FUNCTION:
					case KEY_METHOD:
						CompilationEngine::compileSubroutine();
						break;
					default:
						break;
				}
				break;
			case SYMBOL:
				compileSymbol();
				break;
			default:
				break;
		}
	}
	outFile << "</class>" << std::endl;
}

/**
('static' | 'field') type varName (',' varName)* ';'
type: 'int' | 'char' | 'boolean' | className
*/
void CompilationEngine::compileClassVarDec() {
	// ('static' | 'field')
	switch(jackTokenizer.keyWord()) {
		case KEY_STATIC:
		case KEY_FIELD:
			compileKeyword();
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
					if(jackTokenizer.hasMoreTokens) jackTokenizer.advance();
					compileIdentifier();
				}
				break;
			default:
				break;
		}
		if(isEnd)
			break;
	}
}

/**
('constructor' | 'function' | 'method') ('void' | type) subroutineName '(' parameterList ')' subroutineBody
type: 'int' | 'char' | 'boolean' | className
subroutineBody: '{' varDec* statements '}'
*/
void CompilationEngine::compileSubroutine() {
	// ('constructor' | 'function' | 'method')
	switch(jackTokenizer.keyWord()) {
		case KEY_CONSTRUCTOR:
		case KEY_FUNCTION:
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
	// subroutineBody: '{' varDec* statements '}'
	// '{'
	switch(jackTokenizer.tokenType()) {
		case SYMBOL:
			compileSymbol();
			break;
		default:
			break;
	}
	if(jackTokenizer.hasMoreTokens())	jackTokenizer.advance();
	// varDec*
	// varDec: 'var' type varName (',' varName)* ';'
	while(jackTokenizer.tokenType() == KEYWORD && jackTokenizer.keyWord() == KEY_VAR) {
		compileKeyword();
		if(jackTokenizer.hasMoreTokens())	jackTokenizer.advance();
		// type: 'int' | 'char' | 'boolean' | className
		switch(jackTokenizer.tokenType()) {
			case KEYWORD:
				compileKeyword();
				break;
			case IDENTIFIER:
				compileIdentifier();
				break;
			default:
				break;
		}
		if(jackTokenizer.hasMoreTokens())	jackTokenizer.advance();
		// varName
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
						if(jackTokenizer.hasMoreTokens) jackTokenizer.advance();
						compileIdentifier();
					}
					break;
				default:
					break;
			}
			if(isEnd)
				break;
		}
	}
	if(jackTokenizer.hasMoreTokens())	jackTokenizer.advance();
	// statements
	compileStatements();
	if(jackTokenizer.hasMoreTokens())	jackTokenizer.advance();
	// '}'
	compileSymbol();
}

/**
((type varName) (',' type varName)*)?
type: 'int' | 'char' | 'boolean' | className
*/
void CompilationEngine::compileParameterList() {
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
			}
			break;
		case IDENTIFIER:
			compileIdentifier();
			break;
		default:
			flag = true;
			break;
	}
	if(flag)	return;
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
}

/**
statements: statement*
statement: letStatement | ifStatement | whileStatement | doStatement | returnStatement
*/
void CompilationEngine::compileStatements() {
}
