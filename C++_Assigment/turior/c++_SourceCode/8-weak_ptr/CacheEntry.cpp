#include "CacheEntry.h"
#include "Cache.h"
#include <iostream>
using namespace std;

CacheEntry::CacheEntry(const string& key, const string& data, shared_ptr<Cache> cache)
    : key_(key), data_(data), parent_cache_(cache)
{
    cout << "CacheEntry '" << key_ << "' created with data: " << data_ << endl;
}

CacheEntry::~CacheEntry()
{
    cout << "CacheEntry '" << key_ << "' destroyed" << endl;
}

const string& CacheEntry::getKey() const
{
    return key_;
}

const string& CacheEntry::getData() const
{
    return data_;
}

void CacheEntry::updateData(const string& new_data)
{
    cout << "Updating entry '" << key_ << "': '" << data_ << "' -> '" << new_data << "'" << endl;
    data_ = new_data;
}

bool CacheEntry::isCacheAlive() const
{
    return !parent_cache_.expired();
}

void CacheEntry::notifyAccess()
{
    // Try to notify parent cache of access (for statistics, LRU updates, etc.)
    if (auto cache = parent_cache_.lock())
    {
        cout << "Entry '" << key_ << "' notifying cache of access" << endl;
        // In real implementation, cache might update LRU order, access counts, etc.
    }
    else
    {
        cout << "Entry '" << key_ << "' cannot notify - cache destroyed!" << endl;
    }
}
