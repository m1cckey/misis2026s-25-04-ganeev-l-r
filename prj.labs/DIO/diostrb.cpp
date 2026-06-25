#include "dio.hpp"
#include <iostream>


std::ostream& operator<<(std::ostream& ostrm, const DioStrB& str) {
    return str.write_to(ostrm);
}


std::istream& operator>>(std::istream& istrm, DioStrB& str) {
    return str.read_from(istrm);
}


std::ostream& DioStrB::write_to(std::ostream& ostrm) const {
    ostrm.put(mrk_);
    int32_t size = static_cast<int32_t>(str_.size());
    ostrm.write(reinterpret_cast<const char*>(&size), sizeof(size));
    ostrm.write(str_.data(), size);
    return ostrm;
}


std::istream& DioStrB::read_from(std::istream& istrm) {
    char mark = istrm.get();
    if (mrk_ == mark) {
        int32_t size = 0;
        istrm.read(reinterpret_cast<char*>(&size), sizeof(size));
        if (size < 0) {
            istrm.setstate(std::ios::badbit);
            return istrm;
        }
        str_.resize(size);
        if (size > 0) {
            istrm.read(&str_[0], size);
        }
    }
    else {
        istrm.setstate(std::ios::badbit);
    }
    return istrm;
}

std::ostream& operator<<(std::ostream& ostrm, const DioStrT& str) {
    return str.write_to(ostrm);
}


std::istream& operator>>(std::istream& istrm, DioStrT& str) {
    return str.read_from(istrm);
}
