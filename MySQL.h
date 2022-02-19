// -> mysql에 있는 것들을 사용하기 위해서 헤더 작성
#include "/usr/include/mysql/mysql.h"

#include <iostream>

using namespace std;

// -> 연결된 MYSQL을 저장을 해두죠!
MYSQL* connectedDatabase;

// -> Result! 데이터베이스에 "쿼리"를 한다고 해요!
// -> "쿼리"? 는 무엇이냐! "쿼리"는 특정 데이터를 긁어오는 행위!
// -> 긁어온 데이터에 대한 내용 결과물! 이 여기에 들어온다고 보시면 돼요!
MYSQL_RES* queryResult;

// -> 데이터를 가져왔는데..? 엄청나게 큰 데이터가 왔을 거에요!
// -> 이거를 "줄 형식" 으로 가져와야 합니다!
// -> 받아온 "줄 형식" 데이터를 여기에 넣어놓겠습니다!
MYSQL_ROW queryRow;

/// <summary>
/// => 연결
/// </summary>
/// <returns></returns>
bool MySQLInitialize()
{
	// -> mysql을 초기화한 데이터를 넣어놓는다
	if (!(connectedDatabase = mysql_init((MYSQL*)nullptr)))
	{
		// -> 초기화가 안되었다면
		cout << "MySQL 초기화에 실패하였습니다!" << endl;
		return false;
	}

	cout << "성공적으로 MySQL을 초기화했습니다!" << endl;

	// -> 초기화를 한 뒤에 MySQL에 연결을 시도한다
	// -> 파라미터로는 "MySQL을 저장할 위치", "MySQL의 주소", "ID", "비밀번호", "nullptr", "포트번호"
	if (!mysql_real_connect(connectedDatabase, "localhost", "root", "dursocjd6634", nullptr, 3306, nullptr, 0))
	{
		// -> 연결을 실패했다면
		cout << "MySQL 연결에 실패하였습니다!" << endl;
		return false;
	}

	cout << "성공적으로 MySQL에 연결했습니다!" << endl;
	return true;
}

/// <summary>
/// => 닫기
/// </summary>
void MySQLClose()
{
	mysql_close(connectedDatabase)
}