#include "AutoSaveManager.h"
#include "Document.h"
#include <iostream>
using namespace std;

AutoSaveManager::AutoSaveManager(shared_ptr<Document> doc) : document_(doc)
{
    cout << "Auto-save enabled for " << document_->getFilename() << endl;
}

AutoSaveManager::~AutoSaveManager()
{
    cout << "Auto-save manager destroyed" << endl;
}

void AutoSaveManager::performAutoSave()
{
    cout << "Auto-saving " << document_->getFilename() << "..." << endl;
    // In real code, this would save to disk
}
