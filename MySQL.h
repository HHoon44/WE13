// -> mysql에 있는 것들을 사용하기 위해서 헤더 작성
#include "/usr/include/mysql/mysql.h"

#include <iostream>

using namespace std;

// -> 연결된 MYSQL을 저장을 해두죠!
MYSQL* connectedDatabase;

// -> Result! 데이터베이스에 "쿼리" 를 한다고 해요!
// -> "쿼리"? 는 무엇이냐! "쿼리" 는 특정 데이터를 긁어오는 행위!
// -> 긁어온 데이터에 대한 내용 결과물! 이 여기에 들어온다고 보시면 돼요!
MYSQL_RES* queryResult;

// -> 데이터를 가져왔는데..? 엄청나게 큰 데이터가 왔을 거에요!
// -> 이거를 "줄 형식" 으로 가져와야 합니다!
// -> 받아온 "줄 형식" 데이터를 여기에 넣어놓겠습니다!
MYSQL_ROW queryRow;

// -> 나중에 테이블명 바꾸면 여기서도 바꿔주세요!
string targetTable = "UserData";

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
		cout << "Cannot Initialize MySQL" << endl;
		return false;
	}

	cout << "MySQL Successfully Initialized" << endl;

	// -> 초기화를 한 뒤에 MySQL에 연결을 시도한다
	// -> 파라미터로는 "MySQL을 저장할 위치", "MySQL의 주소", "ID", "비밀번호", "nullptr", "포트번호"
	if (!mysql_real_connect(connectedDatabase, "localhost", "root", "dursocjd6634", nullptr, 3306, nullptr, 0))
	{
		// -> 연결을 실패했다면
		cout << "Failed to Connect MySQL" << endl;
		return false;
	}

	cout << "Successfully connected to MySQL" << endl;

	// -> mysql에 질의문을 던집니다! 
	// -> 질의문을 문자열 안족에 넣으시면 됩니다! (ServerData == 데이터베이스)
	if (mysql_query(connectedDatabase, "USE ServerData;") != 0)
	{
		// -> 데이터 베이스가 유효하지 않다
		cout << "Database is Invalid" << endl;
		return false;
	}

	cout << "Database has Open" << endl;

	return true;
}

/// <summary>
/// => 닫기
/// </summary>
void MySQLClose()
{
	mysql_close(connectedDatabase);
}

/// <summary>
/// => 중복된 유저가 있는지 확인
/// </summary>
/// <param name="id"> 유저의 아이디 </param>
/// <returns></returns>
bool LoadUser(string id)
{
	// -> 유저 데이터에서 아이디가 일치하는 녀석의 줄을 가져옵니다
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

	// -> Select문은 제가 물어보고 "가져오는" 거니까! 실제로 어딘가에 저장해야겠죠!
	// -> mySQL이 제가 질문한것의 답을 가지고 있어요! 그걸 내놓으라고 말해야해요!
	queryResult = mysql_store_result(connectedDatabase);

	// -> 줄 형태로 가져와야 하기 때문에! 이걸 한 번 더 늘입니다!
	queryRow = mysql_fetch_row(queryResult);

	return true;
}

/// <summary>
/// => 유저 정보를 저장
/// </summary>
/// <param name="id"> 유저의 아이디 </param>
/// <param name="color"> 유저의 색상 </param>
void SaveUser(string id, string color)
{
	string query;

	// -> 일단 유저를 불러옵니다!
	if (LoadUser(id))
	{
		// -> 유저가 이미 있다면 값만 업데이트

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
		// -> 유저가 없다면 새로 추가

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

	// -> 파라미터로 "데이터를 저장할 저장소", "저장할 데이터"
	if (mysql_query(connectedDatabase, query.c_str()) != 0)
	{
		cout << "Cannot Save Data" << endl;
		return;
	}

	cout << "Save Success" << endl;
}

string GetColor(string id)
{
	// -> 나중에는 하나하나 불러오기보다는! 구조체를 만드셔가지구 한 번에 떠올 수 있게
	//	  준비하시는 것도 추천드립니다!

	if (LoadUser(id))
	{
		// -> LoadUser를 하면! queryRow에 정보가 담깁니다!
		// -> queryRow[0]는 ID겠죠!
		return queryRow[1];
	}

	return "B";
}

/// <summary>
/// => 테이블에서 유저를 삭제
/// </summary>
/// <param name="id"> 삭제할 유저 ID </param>
void DeleteUser(string id)
{
	if (LoadUser(id))
	{
		// -> 해당 ID가 있는 줄을 지워버리겠다!
		// -> DELETE FROM UserData WHERE ID = "id";
		string query = "DELETE FROM ";
		query += targetTable;
		query += "WHERE ID = \"";
		query += id;
		query += "\";";
	}
}