#include "jackTokenizer.h"
#include <fstream>
#include <filesystem>

int main(int arg, char* argv[]) {
	std::filesystem::path path = argv[arg-1];
	std::ifstream file(path);
	JackTokenizer tokenizer = JackTokenizer(path);
}
