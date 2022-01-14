#pragma once
#include <fstream>

void WriteFileU8(std::string path, std::u8string data) {
	std::ofstream ofs{ path };
	ofs.write((const char*)data.data(), data.length());
	ofs.close();
}