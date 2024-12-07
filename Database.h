#include "Win_Server_Client.h"
#include <iostream>
#include <windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>
#include <string>
#include <sstream>
#include <locale>
#include <codecvt>


constexpr auto SQL_RESULT_LEN = 240;
constexpr auto SQL_RETURN_CODE_LEN = 1024;
SQLHANDLE sqlConnHandle{ nullptr }; // Database connection detection handle
SQLHANDLE sqlEnvHandle{ nullptr }; // Database enviroment descriptor handle
SQLHANDLE sqlStmtHandle{ nullptr };  // Handle for executing database queries
SQLWCHAR retconstring[SQL_RETURN_CODE_LEN]{}; // Announcement string for return code from ODBC API functions

void disconnect_sql() {
	SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
	SQLDisconnect(sqlConnHandle);
	SQLFreeHandle(SQL_HANDLE_DBC, sqlConnHandle);
	SQLFreeHandle(SQL_HANDLE_ENV, sqlEnvHandle);

	std::cout << "\nDisconnected from sql server, press any key to exit...";
}

void connect_sql() {
	// Allocating handle for database
	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &sqlEnvHandle))
		disconnect_sql();

	if (SQL_SUCCESS != SQLSetEnvAttr(sqlEnvHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0))
		disconnect_sql();

	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, sqlEnvHandle, &sqlConnHandle))
		disconnect_sql();

	// Estab. connection to database server
	switch (SQLDriverConnect(sqlConnHandle,
		GetDesktopWindow(),
		/// ↓Connection data must be here↓
		// guest
		//(SQLWCHAR*)L"DRIVER={MySQL ODBC 9.1 ANSI Driver};SERVER=localhost;PORT=3306;DATABASE=chat;UID=guest;PWD=secret;",
		// root
		(SQLWCHAR*)L"DRIVER={MySQL ODBC 9.1 ANSI Driver};SERVER=localhost;PORT=3306;DATABASE=chat;UID=root;PWD=GHBDTNasdasdasd091;",
		SQL_NTS,
		retconstring,
		1024,
		NULL,
		SQL_DRIVER_COMPLETE)) {


	case SQL_SUCCESS:
	case SQL_SUCCESS_WITH_INFO:
		std::cout << "Successfully connected to SQL Server\n";
		break;

	case SQL_INVALID_HANDLE:
	case SQL_ERROR:
		std::cout << "Could not connect to SQL Server\n";
		disconnect_sql();

	default:
		break;
	}

	// Connection error
	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
		disconnect_sql();
}