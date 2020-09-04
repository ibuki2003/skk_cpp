#include <engine.hpp>
#include <iostream>
#include <cctype>
#include <romaji_table.hpp>
#include <skk_dict.hpp>

void SkkEngine::update(char c) {
    bool upper = false;
    if (isupper(c)) {
        upper = true;
        c = tolower(c);
    }

    if (phase == BufferPhase::IDLE) {
        if (upper) {
            out_buf += roman_confirm();
            phase = BufferPhase::BUFFERED;
            return update(c);
        }
        if (!isalpha(c)) {
            if (c == 127) { // BS
                bs_roman() || bs_henkan() || bs_out();
            }
            out_buf += roman_confirm();
        }

        roman_buf.push_back(c);
        out_buf += roman_resolve();
    } else if (phase == BufferPhase::BUFFERED) {
        if (upper) {
            henkan_buf += roman_confirm();
            phase = BufferPhase::OKURI;
            return update(c);
        } else {
            if (c == ' ') {
                search_candidates();
                phase = BufferPhase::HENKAN;
                return;
            } else {
                if (c == 127) { // BS
                    bs_henkan() || bs_out();
                }
                roman_buf.push_back(c);
                henkan_buf += roman_resolve();
            }
        }
    } else if (phase == BufferPhase::OKURI) {
        if (c == 127) { // BS
            bs_roman();
            if (roman_buf.empty()) {
                phase = BufferPhase::BUFFERED;
                return;
            }
        }
        roman_buf.push_back(c);
        if (roman_buf.size() && roman_resolveable()) {
            search_candidates();
            phase = BufferPhase::HENKAN;
        }
    } else if (phase == BufferPhase::HENKAN) {
        if (c == ' ') {
            henkan_idx++;
            henkan_idx %= henkan_candidates.size();
        } else {
            henkan_confirm();
            phase = BufferPhase::IDLE;
            if (c != '\n') return update(c);
        }
    }
}

bool SkkEngine::bs_buf(std::string& buf) {
    if (buf.size()) {
        buf.pop_back();
        return true;
    }
    return false;
}

bool SkkEngine::roman_resolveable() {
    if (roman_buf.empty()) return false;
    const auto iter = romaji_table.lower_bound(roman_buf);
    if (iter == romaji_table.end()) {
        roman_buf.erase(roman_buf.begin());
        return roman_resolveable();
    }
    for (int i = 0; i < roman_buf.size(); ++i) {
        if (iter->first.size() <= i || roman_buf[i] != iter->first[i]) {
            roman_buf.erase(roman_buf.begin());
            return roman_resolveable();
        }
    }
    return iter->first == roman_buf;
}

std::string SkkEngine::roman_resolve() {
    if (!roman_resolveable()) return "";
    const auto iter = romaji_table.find(roman_buf);
    if (iter != romaji_table.end()) {
        roman_buf = iter->second.second;
        return iter->second.first;
    }
    return "";
}

std::string SkkEngine::roman_confirm() {
    if (roman_buf == "n") {
        roman_buf = "";
        return "ん";
    }
    return roman_resolve();
}

void SkkEngine::henkan_confirm() {
    out_buf += henkan_candidates[henkan_idx];
}

void SkkEngine::print() {
    std::cout << out_buf;
    if (phase == BufferPhase::HENKAN) {
        std::cout << ">" << henkan_candidates[henkan_idx];
    } else {
        if (phase == BufferPhase::BUFFERED || phase == BufferPhase::OKURI)
            std::cout << "@";
        std::cout << henkan_buf;
        if (phase == BufferPhase::OKURI) {
            std::cout << "*";
        }
        std::cout << roman_buf;
    }
    std::cout << std::endl;
}

void SkkEngine::search_candidates() {
    henkan_candidates.clear();
    if (roman_buf.size()) { // okuri ari
        const auto match_range = henkan_okuri_ari.equal_range({henkan_buf, roman_buf[0]});
        std::string okuri_str = roman_resolve();
        for (auto it = match_range.first; it != match_range.second; ++it) {
            henkan_candidates.emplace_back(it->second + okuri_str);
        }
    } else {
        const auto match_range = henkan_okuri_nasi.equal_range(henkan_buf);
        for (auto it = match_range.first; it != match_range.second; ++it) {
            henkan_candidates.emplace_back(it->second);
        }
    }

    henkan_idx = 0;
    roman_buf = "";
    henkan_buf = "";
}
