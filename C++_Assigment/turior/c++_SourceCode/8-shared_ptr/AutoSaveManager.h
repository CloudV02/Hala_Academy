#ifndef AUTOSAVEMANAGER_H
#define AUTOSAVEMANAGER_H

#include <memory>
using namespace std;

class Document; // Forward declaration

class AutoSaveManager {
private:
    shared_ptr<Document> document_;

public:
    AutoSaveManager(shared_ptr<Document> doc);
    ~AutoSaveManager();

    void performAutoSave();
};

#endif
