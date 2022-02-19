// -> mysql�� �ִ� �͵��� ����ϱ� ���ؼ� ��� �ۼ�
#include "/usr/include/mysql/mysql.h"

#include <iostream>

using namespace std;

// -> ����� MYSQL�� ������ �ص���!
MYSQL* connectedDatabase;

// -> Result! �����ͺ��̽��� "����" �� �Ѵٰ� �ؿ�!
// -> "����"? �� �����̳�! "����" �� Ư�� �����͸� �ܾ���� ����!
// -> �ܾ�� �����Ϳ� ���� ���� �����! �� ���⿡ ���´ٰ� ���ø� �ſ�!
MYSQL_RES* queryResult;

// -> �����͸� �����Դµ�..? ��û���� ū �����Ͱ� ���� �ſ���!
// -> �̰Ÿ� "�� ����" ���� �����;� �մϴ�!
// -> �޾ƿ� "�� ����" �����͸� ���⿡ �־���ڽ��ϴ�!
MYSQL_ROW queryRow;

// -> ���߿� ���̺�� �ٲٸ� ���⼭�� �ٲ��ּ���!
string targetTable = "UserData";

/// <summary>
/// => ����
/// </summary>
/// <returns></returns>
bool MySQLInitialize()
{
	// -> mysql�� �ʱ�ȭ�� �����͸� �־���´�
	if (!(connectedDatabase = mysql_init((MYSQL*)nullptr)))
	{
		// -> �ʱ�ȭ�� �ȵǾ��ٸ�
		cout << "Cannot Initialize MySQL" << endl;
		return false;
	}

	cout << "MySQL Successfully Initialized" << endl;

	// -> �ʱ�ȭ�� �� �ڿ� MySQL�� ������ �õ��Ѵ�
	// -> �Ķ���ͷδ� "MySQL�� ������ ��ġ", "MySQL�� �ּ�", "ID", "��й�ȣ", "nullptr", "��Ʈ��ȣ"
	if (!mysql_real_connect(connectedDatabase, "localhost", "root", "dursocjd6634", nullptr, 3306, nullptr, 0))
	{
		// -> ������ �����ߴٸ�
		cout << "Failed to Connect MySQL" << endl;
		return false;
	}

	cout << "Successfully connected to MySQL" << endl;

	// -> mysql�� ���ǹ��� �����ϴ�! 
	// -> ���ǹ��� ���ڿ� ������ �����ø� �˴ϴ�! (ServerData == �����ͺ��̽�)
	if (mysql_query(connectedDatabase, "USE ServerData;") != 0)
	{
		// -> ������ ���̽��� ��ȿ���� �ʴ�
		cout << "Database is Invalid" << endl;
		return false;
	}

	cout << "Database has Open" << endl;

	return true;
}

/// <summary>
/// => �ݱ�
/// </summary>
void MySQLClose()
{
	mysql_close(connectedDatabase);
}

/// <summary>
/// => �ߺ��� ������ �ִ��� Ȯ��
/// </summary>
/// <param name="id"> ������ ���̵� </param>
/// <returns></returns>
bool LoadUser(string id)
{
	// -> ���� �����Ϳ��� ���̵� ��ġ�ϴ� �༮�� ���� �����ɴϴ�
	// -> SELECT * FROM UserData WHERE ID = 'id';
	string query = "SELECT * FROM ";
	query += targetTable;
	query += " WHERE ID = \"";
	query += id;
	query += "\";";

	if (mysql_query(connectedDatabase, query.c_str()) != 0)
	{
		return false;
	}

	// -> Select���� ���� ����� "��������" �Ŵϱ�! ������ ��򰡿� �����ؾ߰���!
	// -> mySQL�� ���� �����Ѱ��� ���� ������ �־��! �װ� ��������� ���ؾ��ؿ�!
	queryResult = mysql_store_result(connectedDatabase);

	// -> �� ���·� �����;� �ϱ� ������! �̰� �� �� �� ���Դϴ�!
	queryRow = mysql_fetch_row(queryResult);

	return true;
}

/// <summary>
/// => ���� ������ ����
/// </summary>
/// <param name="id"> ������ ���̵� </param>
/// <param name="color"> ������ ���� </param>
void SaveUser(string id, string color)
{
	string query;

	// -> �ϴ� ������ �ҷ��ɴϴ�!
	if (LoadUser(id))
	{
		// -> ������ �̹� �ִٸ� ���� ������Ʈ

		// -> UPDATE UserData SET COLOR = "color" WHERE ID = "id";
		// -> UPDATE UserData SET COLOR = "color", INI = "a" WHERE ID = "id";
		query = "UPDATE ";
		query += targetTable;
		query += " SET COLOR=\"";
		query += color;
		query += "\" WHERE ID = \"";
		query += id;
		query += "\";";
	}
	else
	{
		// -> ������ ���ٸ� ���� �߰�

		// -> INSERT INTO UserData (ID, COLOR) VALUES ("id", "color");
		// -> INSERT INTO UserData (ID, COLOR, INI) VALUES ("id", "color", "a");
		query = "INSERT INTO ";
		query += targetTable;
		query += "(ID, COLOR) VALUES(\"";
		query += id;
		query += "\", \"";
		query += color;
		query += "\");";
	}

	// -> �Ķ���ͷ� "�����͸� ������ �����", "������ ������"
	if (mysql_query(connectedDatabase, query.c_str()) != 0)
	{
		cout << "Cannot Save Data" << endl;
		return;
	}

	cout << "Save Success" << endl;
}

string GetColor(string id)
{
	// -> ���߿��� �ϳ��ϳ� �ҷ����⺸�ٴ�! ����ü�� ����Ű����� �� ���� ���� �� �ְ�
	//	  �غ��Ͻô� �͵� ��õ�帳�ϴ�!

	if (LoadUser(id))
	{
		// -> LoadUser�� �ϸ�! queryRow�� ������ ���ϴ�!
		// -> queryRow[0]�� ID����!
		return queryRow[1];
	}

	return "B";
}

/// <summary>
/// => ���̺��� ������ ����
/// </summary>
/// <param name="id"> ������ ���� ID </param>
void DeleteUser(string id)
{
	if (LoadUser(id))
	{
		// -> �ش� ID�� �ִ� ���� ���������ڴ�!
		// -> DELETE FROM UserData WHERE ID = "id";
		string query = "DELETE FROM ";
		query += targetTable;
		query += "WHERE ID = \"";
		query += id;
		query += "\";";
	}
}