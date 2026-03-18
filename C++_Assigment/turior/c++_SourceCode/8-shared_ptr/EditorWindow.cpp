#include "EditorWindow.h"
#include "Document.h"
#include <iostream>
using namespace std;

EditorWindow::EditorWindow(const string& name, shared_ptr<Document> doc)
    : window_name_(name), document_(doc)
{
    cout << "Editor window '" << window_name_ << "' opened for "
              << document_->getFilename() << endl;
}

EditorWindow::~EditorWindow()
{
    cout << "Editor window '" << window_name_ << "' closed" << endl;
}

void EditorWindow::editDocument(const string& new_line)
{
    cout << "Adding line via " << window_name_ << ": " << new_line << endl;
    document_->addLine(new_line);
}

void EditorWindow::viewDocument() const
{
    cout << "Viewing from " << window_name_ << ":" << endl;
    document_->print();
}
