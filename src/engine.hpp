#pragma once
#include <string>
#include <vector>

class SkkEngine {
    enum class BufferPhase {
        IDLE, // あ
        BUFFERED, // ▽あ
        OKURI, // ▽あ*k
        HENKAN, // ▼開か
    };

    BufferPhase phase = BufferPhase::IDLE;

    std::string out_buf, henkan_buf, roman_buf;

    std::vector<std::string> henkan_candidates;
    size_t henkan_idx;

    bool roman_resolveable();

    std::string roman_resolve();
    std::string roman_confirm();

    void henkan_confirm();

    void search_candidates();

    inline bool bs_roman()  { return bs_buf(roman_buf);}
    inline bool bs_henkan() { return bs_buf(henkan_buf);}
    inline bool bs_out()    { return bs_buf(out_buf);}

    bool bs_buf(std::string& buf);

public:
    // SkkEngine();

    void print();

    void update(char c);
};
