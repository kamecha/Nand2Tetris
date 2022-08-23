#include "compilationEngine.h"
#include "jackTokenizer.h"
#include <fstream>
#include <filesystem>
#include <iostream>

int main(int arg, char* argv[]) {
	std::filesystem::path path = argv[arg-1];
	std::ifstream inFile(path);
	std::ofstream parseredFile(path.stem().string() + ".xml");
	CompilationEngine compilationEngine(inFile, parseredFile);
	compilationEngine.compileClass();
}
