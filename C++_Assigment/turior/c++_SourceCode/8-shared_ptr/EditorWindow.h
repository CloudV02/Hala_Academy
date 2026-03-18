#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include <memory>
#include <string>
using namespace std;

class Document; // Forward declaration

class EditorWindow {
private:
    shared_ptr<Document> document_;
    string window_name_;

public:
    EditorWindow(const string& name, shared_ptr<Document> doc);
    ~EditorWindow();

    void editDocument(const string& new_line);
    void viewDocument() const;
};

#endif
