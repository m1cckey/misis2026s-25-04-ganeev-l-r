#pragma once
#include <string>
#include <iostream>

class DioStrB{
private:
    std::string data_;
public:
    DioStrB() = default;
    DioStrB(const DioStrB& src) = default;
    DioStrB(DioStrB&& src) = default;
    ~DioStrB() = default;
    explicit DioStrB(const std::string str): data_(str){}

    
    
    const std::string& val() const { return data_; }
    const std::string& str() const { return data_; }
    void clear() { data_.clear(); }

    DioStrB& operator=(const DioStrB& other) {
        if (this != &other) {  // Самоприсваивание
            data_ = other.data_;
        }
        return *this;
    }
     
    friend std::ostream& operator << (std::ostream& os, const DioStrB& obj){
        return os << obj.val();
    }
    friend std::istream operator >> (std::istream& is, const DioStrB& obj){
        std::string temp;
        std::getline(is, temp, '\n'); 
        obj.data_ = temp;
        return is;
    }






}