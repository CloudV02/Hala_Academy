#include "DatabaseConnection.h"

int main()
{
	DatabaseConnection* db = DatabaseConnection::getInstance();
	db->query("SELECT * FROM users");

	return 0;
}
