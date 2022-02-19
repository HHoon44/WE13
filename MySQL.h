// -> mysql�� �ִ� �͵��� ����ϱ� ���ؼ� ��� �ۼ�
#include "/usr/include/mysql/mysql.h"

#include <iostream>

using namespace std;

// -> ����� MYSQL�� ������ �ص���!
MYSQL* connectedDatabase;

// -> Result! �����ͺ��̽��� "����"�� �Ѵٰ� �ؿ�!
// -> "����"? �� �����̳�! "����"�� Ư�� �����͸� �ܾ���� ����!
// -> �ܾ�� �����Ϳ� ���� ���� �����! �� ���⿡ ���´ٰ� ���ø� �ſ�!
MYSQL_RES* queryResult;

// -> �����͸� �����Դµ�..? ��û���� ū �����Ͱ� ���� �ſ���!
// -> �̰Ÿ� "�� ����" ���� �����;� �մϴ�!
// -> �޾ƿ� "�� ����" �����͸� ���⿡ �־���ڽ��ϴ�!
MYSQL_ROW queryRow;

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
	if (mysql_query(connectedDatabase, "USE ServerData") != 0)
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
/// => ���� ������ ����
/// </summary>
void SaveUser()
{

}