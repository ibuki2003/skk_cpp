#include <iostream>
#include <getch.hpp>
#include <engine.hpp>
#include <romaji_table.hpp>
#include <skk_dict.hpp>

int main() {
    std::cout << "Initializing..." << std::endl;
    init_romaji();
    henkan_init();
    SkkEngine engine;

    while (true) {
        char c = getch();
        engine.update(c);
        engine.print();
    }
}

