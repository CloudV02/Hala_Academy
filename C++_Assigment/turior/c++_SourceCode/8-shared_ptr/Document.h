#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <memory>
#include <vector>
#include <string>
using namespace std;

class Document
{
private:
    string filename_;
    vector<std::string> content_;

public:
    Document(const string& filename);
    ~Document();

    void addLine(const string& line);
    void print() const;
    const string& getFilename() const;
};

#endif
