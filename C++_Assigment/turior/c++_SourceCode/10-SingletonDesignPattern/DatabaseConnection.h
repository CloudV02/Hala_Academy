#ifndef DATABASECONNECTION_H
#define DATABASECONNECTION_H

#include <mutex>
#include <string>
using namespace std;

class DatabaseConnection
{
	private:
		static DatabaseConnection* instance;
		static mutex mtx;

		DatabaseConnection();

	public:
		static DatabaseConnection* getInstance();
		void query(const string& sql);
};

#endif
