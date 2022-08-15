#include "jackTokenizer.h"
#include <fstream>
#include <filesystem>
#include <iostream>

int main(int arg, char* argv[]) {
	std::filesystem::path path = argv[arg-1];
	std::ifstream file(path);
	JackTokenizer tokenizer = JackTokenizer(path);
	std::ofstream tokedFile(path.stem().string() + "T" + ".xml");
	tokedFile << "<tokens>" << std::endl;
	while(tokenizer.hasMoreTokens()) {
		tokenizer.advance();
		switch(tokenizer.tokenType()) {
			case KEYWORD:
				tokedFile << "<keyword>";
				switch(tokenizer.keyWord()) {
						case KEY_CLASS:
						tokedFile << " " << "class" << " ";
						break;
						case KEY_METHOD:
						tokedFile << " " << "method" << " ";
						break;
						case KEY_FUNCTION:
						tokedFile << " " << "function" << " ";
						break;
						case KEY_CONSTRUCTOR:
						tokedFile << " " << "constructor" << " ";
						break;
						case KEY_INT:
						tokedFile << " " << "int" << " ";
						break;
						case KEY_BOOLEAN:
						tokedFile << " " << "boolean" << " ";
						break;
						case KEY_CHAR:
						tokedFile << " " << "char" << " ";
						break;
						case KEY_VOID:
						tokedFile << " " << "void" << " ";
						break;
						case KEY_VAR:
						tokedFile << " " << "var" << " ";
						break;
						case KEY_STATIC:
						tokedFile << " " << "static" << " ";
						break;
						case KEY_FIELD:
						tokedFile << " " << "field" << " ";
						break;
						case KEY_LET:
						tokedFile << " " << "let" << " ";
						break;
						case KEY_DO:
						tokedFile << " " << "do" << " ";
						break;
						case KEY_IF:
						tokedFile << " " << "if" << " ";
						break;
						case KEY_ELSE:
						tokedFile << " " << "else" << " ";
						break;
						case KEY_WHILE:
						tokedFile << " " << "while" << " ";
						break;
						case KEY_RETURN:
						tokedFile << " " << "return" << " ";
						break;
						case KEY_TRUE:
						tokedFile << " " << "true" << " ";
						break;
						case KEY_FALSE:
						tokedFile << " " << "false" << " ";
						break;
						case KEY_NULL:
						tokedFile << " " << "null" << " ";
						break;
						case KEY_THIS:
						tokedFile << " " << "this" << " ";
						break;
				}
				tokedFile << "</keyword>" << std::endl;
				break;
			case SYMBOL:
				// &, <, > をそれぞれxml用に変更する
				// &amp;	&lt;	&gt;
				tokedFile << "<symbol>";
				tokedFile << " ";
				switch(tokenizer.symbol()) {
					case '&':
						tokedFile << "&amp;";
						break;
					case '<':
						tokedFile << "&lt;";
						break;
					case '>':
						tokedFile << "&gt;";
						break;
					default:
						tokedFile << tokenizer.symbol();
						break;
				}
				tokedFile << " ";
				tokedFile << "</symbol>" << std::endl;
				break;
			case IDENTIFIER:
				tokedFile << "<identifier>";
				tokedFile << " " << tokenizer.identifier() << " ";
				tokedFile << "</identifier>" << std::endl;
				break;
			case INT_CONST:
				tokedFile << "<integerConstant>";
				tokedFile << " " << tokenizer.intVal() << " ";
				tokedFile << "</integerConstant>" << std::endl;
				break;
			case STRING_CONST:
				tokedFile << "<stringConstant>";
				tokedFile << " " << tokenizer.stringVal() << " ";
				tokedFile << "</stringConstant>" << std::endl;
				break;
			default:
				break;
		}
	}
	tokedFile << "</tokens>" << std::endl;
}
