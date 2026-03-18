#include "Document.h"
#include <iostream>
using namespace std;

Document::Document(const string& filename) : filename_(filename)
{
    cout << "Loading document: " << filename_ << endl;
    // Simulate loading large document content
    content_ = {"Line 1 of document", "Line 2 of document", "Line 3 of document"};
}

Document::~Document()
{
    cout << "Document " << filename_ << " is being destroyed" << endl;
}

void Document::addLine(const string& line)
{
    content_.push_back(line);
}

void Document::print() const
{
    cout << "=== " << filename_ << " ===" << endl;
    for (const auto& line : content_)
    {
        cout << line << endl;
    }
    cout << endl;
}

const string& Document::getFilename() const
{
    return filename_;
}
