#pragma once
#include <string>

std::pair<int, int> Index2Pos(std::u8string_view src, int index);

std::u8string IntToU8String(int v);