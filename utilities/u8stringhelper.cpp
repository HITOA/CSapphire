#include "u8stringhelper.hpp"
#include <assert.h>
#include <iostream>

 std::pair<int, int> Index2Pos(std::u8string_view src, int index)
 {
	 if (index == 0)
		 return std::make_pair(0, 0);

	 assert(("Index2Pos : index out of bound", index < src.length()));

	 int line = 0;
	 int column = 0;

	 for (int c = 0; c != index; c++) {
		 if (src[c] == u8'\n')
		 {
			 line++;
			 column = 0;
		 }
		 else {
			 column++;
		 }
	 }

	 return std::make_pair(line, column);
 }

 std::u8string IntToU8String(int v)
 {
	 std::u8string tb = u8"0123456789";
	 std::u8string result;

	 do {
		 int r = v % 10;
		 v /= 10;
		 result = tb[r] + result;
	 } while (v > 0);
	 return result;
 }
