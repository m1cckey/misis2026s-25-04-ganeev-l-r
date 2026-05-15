//разоюратьься что строка а что битсет
#include<string>
#include<vector>


class Reader{
public:
    virtual ~Reader() = default;
    virtual void update(const std::string& message) = 0;
};

class updater{
private:
    std::vector<Reader*> readers;
    std::string data;


    void subscribe(Reader* reader){
        readers.push_back(reader);
    }
}


