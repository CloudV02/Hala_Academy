#ifndef CACHEENTRY_H
#define CACHEENTRY_H

#include <memory>
#include <string>
using namespace std;

class Cache; // Forward declaration

class CacheEntry
{
	private:
		string key_;
		string data_;
		weak_ptr<Cache> parent_cache_; // weak_ptr to avoid circular reference!

	public:
		CacheEntry(const string& key, const string& data, shared_ptr<Cache> cache);
		~CacheEntry();

		const string& getKey() const;
		const string& getData() const;
		void updateData(const string& new_data);
		bool isCacheAlive() const;
		void notifyAccess();
};

#endif
