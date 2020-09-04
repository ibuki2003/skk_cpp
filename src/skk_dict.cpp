#include "skk_dict.hpp"
#include <iostream>
#include <fstream>
#include <cctype>

std::multimap<std::string, std::string> henkan_okuri_nasi;
std::multimap<std::pair<std::string, char>, std::string> henkan_okuri_ari;

void henkan_init() {
    std::ifstream ifs("skk.dict");
    std::string l;
    std::string yomi, out;
    char yomi_alpha;
    while (std::getline(ifs, l)) {
        auto it = l.begin();
        if (*it == ';') continue;
        yomi = "";
        while(*it != ' ') {
            yomi += *it;
            ++it;
        }

        bool is_okuri = isalpha(*yomi.rbegin());
        if (is_okuri) {
            yomi_alpha = *yomi.rbegin();
            yomi = yomi.substr(0, yomi.size() - 1);
        }
        while(*it != '/') ++it;
        ++it;

        while (true) {
            out = "";
            while (*it != ';' && *it != '/') {
                if (it == l.end()) break;
                out += *it;
                ++it;
            }
            if (it == l.end()) break;
            if (is_okuri) {
                henkan_okuri_ari.insert({{yomi, yomi_alpha}, out});
            } else {
                henkan_okuri_nasi.insert({yomi, out});
            }

            while(*it != '/') ++it;
            ++it;
        }
    }
}
