#include "Cache.h"
#include "CacheEntry.h"
#include <iostream>
using namespace std;

Cache::Cache(const string& name) : cache_name_(name)
{
    cout << "Cache '" << cache_name_ << "' created" << endl;
}

Cache::~Cache()
{
    cout << "Cache '" << cache_name_ << "' destroyed" << endl;
}

shared_ptr<CacheEntry> Cache::get(const string& key)
{
    auto it = entries_.find(key);

    if (it != entries_.end())
    {
        // Try to get the entry from weak_ptr
        if (auto entry = it->second.lock())
        {
            cout << "Cache hit for key '" << key << "'" << endl;
            entry->notifyAccess();
            return entry;
        }
        else
        {
            // Entry was destroyed, remove the expired weak_ptr
            cout << "Found expired entry for key '" << key << "', removing" << endl;
            entries_.erase(it);
        }
    }

    // Cache miss - create new entry
    cout << "Cache miss for key '" << key << "', creating new entry" << endl;
    string data = "Data for " + key; // Simulate loading data

    auto entry = make_shared<CacheEntry>(key, data,
                    shared_ptr<Cache>(this, [](Cache*){})); // Custom no-op deleter

    entries_[key] = entry; // Store as weak_ptr
    return entry;
}

void Cache::cleanExpiredEntries()
{
    auto initial_size = entries_.size();

    for (auto it = entries_.begin(); it != entries_.end();)
    {
        if (it->second.expired())
        {
            cout << "Cleaning expired entry: " << it->first << endl;
            it = entries_.erase(it);
        } else
        {
            ++it;
        }
    }

    auto cleaned = initial_size - entries_.size();
    if (cleaned > 0)
    {
        cout << "Cache '" << cache_name_ << "' cleaned " << cleaned << " expired entries" << endl;
    }
}

void Cache::printStatus() const
{
    cout << "Cache '" << cache_name_ << "' status:" << endl;

    size_t alive_count = 0;
    for (const auto& pair : entries_)
    {
        if (!pair.second.expired())
        {
            alive_count++;
            cout << "  - Key '" << pair.first << "' (alive)" << endl;
        }
        else
        {
            cout << "  - Key '" << pair.first << "' (expired)" << endl;
        }
    }

    cout << "  Total: " << entries_.size() << " entries, " << alive_count << " alive" << endl;
}
