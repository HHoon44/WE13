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
		cout << "MySQL �ʱ�ȭ�� �����Ͽ����ϴ�!" << endl;
		return false;
	}

	cout << "���������� MySQL�� �ʱ�ȭ�߽��ϴ�!" << endl;

	// -> �ʱ�ȭ�� �� �ڿ� MySQL�� ������ �õ��Ѵ�
	// -> �Ķ���ͷδ� "MySQL�� ������ ��ġ", "MySQL�� �ּ�", "ID", "��й�ȣ", "nullptr", "��Ʈ��ȣ"
	if (!mysql_real_connect(connectedDatabase, "localhost", "root", "dursocjd6634", nullptr, 3306, nullptr, 0))
	{
		// -> ������ �����ߴٸ�
		cout << "MySQL ���ῡ �����Ͽ����ϴ�!" << endl;
		return false;
	}

	cout << "���������� MySQL�� �����߽��ϴ�!" << endl;
	return true;
}

/// <summary>
/// => �ݱ�
/// </summary>
void MySQLClose()
{
	mysql_close(connectedDatabase)
}