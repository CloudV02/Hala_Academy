#include "PreviewPanel.h"
#include "Document.h"
#include <iostream>
using namespace std;

PreviewPanel::PreviewPanel(shared_ptr<Document> doc) : document_(doc)
{
    cout << "Preview panel opened for " << document_->getFilename() << endl;
}

PreviewPanel::~PreviewPanel()
{
    cout << "Preview panel closed" << endl;
}

void PreviewPanel::showPreview() const
{
    cout << "=== PREVIEW ===" << endl;
    document_->print();
}
