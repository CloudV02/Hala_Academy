#ifndef CACHE_H
#define CACHE_H

#include <memory>
#include <unordered_map>
#include <string>
using namespace std;

class CacheEntry; // Forward declaration

class Cache
{
	private:
		unordered_map<string, weak_ptr<CacheEntry>> entries_;
		string cache_name_;

	public:
		Cache(const string& name);
		~Cache();

		shared_ptr<CacheEntry> get(const string& key);
		void cleanExpiredEntries();
		void printStatus() const;
};

#endif
