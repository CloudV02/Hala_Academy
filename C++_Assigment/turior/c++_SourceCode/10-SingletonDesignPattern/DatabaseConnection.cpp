#include "DatabaseConnection.h"
#include <iostream>
#include <string>
#include <mutex>
#include <thread>
using namespace std;

DatabaseConnection* DatabaseConnection::instance = nullptr;
mutex DatabaseConnection::mtx;

DatabaseConnection::DatabaseConnection()
{
	cout << "Database connection established." << endl;
}

DatabaseConnection* DatabaseConnection::getInstance()
{
	lock_guard lock(mtx);
	if (instance == nullptr)
	{
		instance = new DatabaseConnection();
	}
	return instance;
}

void DatabaseConnection::query(const string& sql)
{
	cout << "Executing query: " << sql << endl;
}
