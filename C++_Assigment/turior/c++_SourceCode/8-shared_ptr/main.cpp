#include "Document.h"
#include "EditorWindow.h"
#include "PreviewPanel.h"
#include "AutoSaveManager.h"
#include <iostream>
#include <memory>
using namespace std;

int main()
{
    cout << "=== Creating shared document ===" << endl;

    // Create a document - this would be expensive to copy
    auto document = make_shared<Document>("my_important_file.txt");

    cout << "\n=== Opening multiple views of the same document ===" << endl;

    // Multiple components can share the same document
    // The document stays alive as long as ANY of these components exist
    {
        EditorWindow main_editor("Main Editor", document);
        EditorWindow split_editor("Split View", document);
        PreviewPanel preview(document);
        AutoSaveManager autosave(document);

        cout << "\n=== Working with the document ===" << endl;

        // All components work with the SAME document instance
        main_editor.editDocument("Added from main editor");
        split_editor.editDocument("Added from split view");

        preview.showPreview();
        autosave.performAutoSave();

        main_editor.viewDocument();

        cout << "\n=== Closing some windows ===" << endl;
        // Some components go out of scope here, but document remains alive
        // because other shared_ptrs still reference it
    }

    cout << "\n=== Only original shared_ptr remains ===" << endl;
    cout << "Document reference count: " << document.use_count() << endl;

    // Document is still accessible through the original shared_ptr
    document->print();

    cout << "\n=== Program ending - document will be automatically destroyed ===" << endl;

    return 0;
    // Document is automatically destroyed here when the last shared_ptr goes out of scope
}
