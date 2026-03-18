#include "Cache.h"
#include "CacheEntry.h"
#include <iostream>
#include <memory>
#include <vector>
using namespace std;

int main()
{
    cout << "=== Simple Cache System with weak_ptr ===" << endl;

    auto cache = make_shared<Cache>("MyCache");

    cout << "\n--- Creating cache entries ---" << endl;

    // Get some entries (they'll be created and stored as weak_ptr in cache)
    auto entry1 = cache->get("user:123");
    auto entry2 = cache->get("user:456");
    auto entry3 = cache->get("user:789");

    cache->printStatus();

    cout << "\n--- Using entries ---" << endl;

    // Use the entries
    cout << "Entry1 data: " << entry1->getData() << endl;
    entry2->updateData("Updated data for user:456");

    // Get same entries again (cache hits)
    auto entry1_again = cache->get("user:123");
    cout << "Same object? " << (entry1 == entry1_again ? "Yes" : "No") << endl;

    cout << "\n--- Releasing some entries ---" << endl;

    // Release entry2 and entry3
    entry2.reset();
    entry3.reset();

    cout << "Released entry2 and entry3, but they might still be in cache..." << endl;
    cache->printStatus();

    cout << "\n--- Cleaning expired entries ---" << endl;
    cache->cleanExpiredEntries();
    cache->printStatus();

    cout << "\n--- Testing cache destruction before entries ---" << endl;

    // Keep a reference to entry1
    auto surviving_entry = entry1;

    // Destroy the cache
    cache.reset();
    cout << "Cache destroyed, but entry1 still exists" << endl;

    // Entry can detect that cache is gone
    cout << "Is cache alive? " << (surviving_entry->isCacheAlive() ? "Yes" : "No") << endl;

    // Try to notify cache (will fail gracefully)
    surviving_entry->notifyAccess();

    cout << "\n--- Program ending ---" << endl;

    return 0;
}
