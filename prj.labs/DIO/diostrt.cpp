#include "dio.hpp"
#include <ostream>
#include <istream>

std::ostream& DioStrT::write_to(std::ostream& ostrm) const {
    ostrm.put(mrk_);
    ostrm.put('"');
    for (char c : str_) {
        switch (c) {
            case '\\': ostrm << "\\\\"; break;
            case '\"': ostrm << "\\\""; break;
            case '\n': ostrm << "\\n"; break;
            case '\t': ostrm << "\\t"; break;
            default: ostrm.put(c); break;
        }
    }
    ostrm.put('"');
    return ostrm;
}

std::istream& DioStrT::read_from(std::istream& istrm) {
    char mark = istrm.get();
    if (mark != mrk_) {
        istrm.setstate(std::ios::badbit);
        return istrm;
    }
    char quote = istrm.get();
    if (quote != '\"') {
        istrm.setstate(std::ios::badbit);
        return istrm;
    }
    str_.clear();
    char ch;
    while (istrm.get(ch)) {
        if (ch == '\"') break;
        if (ch == '\\') {
            char next;
            if (!istrm.get(next)) {
                istrm.setstate(std::ios::badbit);
                return istrm;
            }
            switch (next) {
                case 'n': str_ += '\n'; break;
                case 't': str_ += '\t'; break;
                case '\\': str_ += '\\'; break;
                case '\"': str_ += '\"'; break;
                default: str_ += next; break;
            }
        } else {
            str_ += ch;
        }
    }
    if (!istrm && ch != '\"') {
        istrm.setstate(std::ios::badbit);
    }
    return istrm;
}
