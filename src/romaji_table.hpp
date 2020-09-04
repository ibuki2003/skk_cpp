#pragma once
#include <map>
#include <string>

extern std::map<std::string, std::pair<const char*, const char*>> romaji_table;

void init_romaji();
