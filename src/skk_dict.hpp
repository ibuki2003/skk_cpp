#pragma once
#include <string>
#include <map>

extern std::multimap<std::string, std::string> henkan_okuri_nasi;
extern std::multimap<std::pair<std::string, char>, std::string> henkan_okuri_ari;

void henkan_init();
