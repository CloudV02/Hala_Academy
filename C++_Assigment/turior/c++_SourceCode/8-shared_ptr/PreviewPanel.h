#ifndef PREVIEWPANEL_H
#define PREVIEWPANEL_H

#include <memory>
using namespace std;

class Document; // Forward declaration

class PreviewPanel
{
private:
    shared_ptr<Document> document_;

public:
    PreviewPanel(shared_ptr<Document> doc);
    ~PreviewPanel();

    void showPreview() const;
};

#endif
