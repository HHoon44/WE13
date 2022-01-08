// 여기서 제가 제일 먼저 볼거!
// 서버에서는 IP가 았가 머룐압니다
// 저희 내부 네트워크에도 IP를 알려줘야한다
// 내부 IP를 적으시면 됩니다
#define SERVER_IP "10.178.0.3"
// 동적포트를 사용해보록 합시다
#define SERVER_PORT 51234
// 너무 많은 양의 전송을 하면 성능상도 그렇고 
// 물리적인 네트워크 기기여도 한계가 있기 때문에
// 버퍼(임시저장소)사이즈는 제한을 걸어줄게요
#define BUFFER_SIZE 1024
// 제한을 해야하는 요소는 굉장히 많이 있는데요
// 동시 접속자 -> 서버가 원할하게 돌아갈 수 있도록
// 접속 인원의 한계를 미리 정해놓습니다
// 이 한계를 넘는 인원이 들어오는 경우 대기열 서버로 넘겨줍니다
// FM : file descriptor
// 왜 FM(File Descriptor) 인가! 저희가 소켓으로 통신을 하게 될거에요
// 윈도우스 같은 경우에는 소켓이 따로 있습니다.
// 리눅스는 모든 것을 파일 형태로 관리합니다. 소켓 조차도 파일이므로
// 그래서 FM 넘버라고 한겁니다.
#define FD_NUMBER 100
// 서버가 무한한 속도로 돌아가면 물론 좋겠죠
// 서버에 틱레이트를 조절해주실 필요학 있는데요
// 클라이언트 같은 경우는 144프레임으로 하시는 분들 굉장히 많이 있습니다
#define TICK_RATE 16

#include <iostream>
// 클라이언트가 직접 주소와 포트를 이용해서 들어오라고 소켓을 사용할 거에요
#include <sys/socket.h>
// IP쓰려고 INet을 가져오도록 할게요
#include <netinet/in.h>
#include <arpa/inet.h>
// 플레이어들을 계속 순회하면서 자연스럽게 저한테 뭔가 내용 준 애가 있는 경우에만
// 활동하도록 polling을 사용할 거에요
#include <poll.h>

// 틱레이트에 대한 이야기를 했었죠
// 얼마나 시간이 지났는지 체크를 하기는 해야 계산을 시작할 수 있을거에요
#include <sys/time.h>
// 자료형을 가지고 놀아볼 예정이거든요? 그래서 타입을 가볍게 가져왔어요
#include <sys/types.h>
// 문자열
#include <string.h>
#include <unistd.h>

using namespace std;

// ===전방선언=== //

// ===전방선언=== //

class UserData
{
public:
	// -> 본인이 타고 있는 소켓의 번호를 저장해둡시다
	// -> 나중에 애한에 연락해야 하는 일이 있을 때 유용하게 사용
	int FDNumber = 0;

	UserData()
	{
		cout << "유저 데이터가 생성되었습니다." << endl;
	};

	~UserData()
	{
		cout << "유저 연결이 종료되었습니다." << endl;
	}
};


// ===전역변수 선언란=== //
struct pollfd pollFDArray[FD_NUMBER];	// -> 
UserData* userFDArray[FD_NUMBER];		// -> 유저
// ===전역변수 선언란=== //

/// <summary>
/// 서버 시작 함수
/// </summary>
/// <param name="currentFD"> 현재 제가 리슨 소켓으로 사용할 녀석을 만들어주세요 </param>
bool StartServer(int* currentFD)
{
	// -> 위에서 못 만들어 왔을 때
	if (*currentFD == -1)
	{
		// -> 오류 났어요
		perror("socket()");

		// -> 혹시 모르니까 클로즈도 넣어놓고 갑시다
		close(*currentFD);
		return true;
	};

	// -> 소켓에는 IP와 포트가 있어요 넣어주어야 하겠죠?
	sockaddr_in address;

	// -> 0으로 초기화해서 확실하게 합시다
	memset(&address, 0, sizeof(address));

	// -> IPv4의 IP를 넘겨줄테니까 그렇게 아세여
	address.sin_family = AF_INET;

	// -> IP같은 경우는 ff.ff.ff.ff 같이 점이 굉장히 많이 있어서 숫자로 표시하기 애매하니까
	//	  보통은 이렇게 문자열로 만든 다음에 함수로 그걸 풀어서 써요
	address.sin_addr.s_addr = inet_addr(SERVER_IP);

	// -> 포트도 그대로 가져와주기
	address.sin_port = htons(SERVER_PORT);

	// -> 저희가 정보를 만들어가지고 왔으니까! 정보는 그냥 만들었다고 끝이 아니죠?
	// -> 어딘가 (listen socket)에 연결해야 의미가 있는 것이죠
	if (bind(*currentFD, (struct sockaddr*)&address, sizeof(address)) == -1)
	{
		// -> 바인드가 실패했다면!
		// -> 바인드에 에러가 났어요
		perror("bind()");
		close(*currentFD);
		return true;
	};

	// -> 바인드만 하면 끝인가요? 얘가 소켓인 것도 알겠고.. IP주소도 알겠고 포트도 알겠는데
	// -> 사실 다른 애들도 가지고 있으려면 가질 수 있거든요..?
	// -> 제가 처음에 이녀석을 만들 때 [리슨 소켓]이라고 했어요

	if (listen((*currentFD), 8) == -1)
	{
		// -> 리슨에 에러가 났어요
		perror("listen()");
		close(*currentFD);
		return true;
	};


	// -> 아무런 문제 없이 다 잘 끝났어요
	return false;

}


int main()
{
	// -> 소켓들은 전부 int로 관리 될 거에요
	// -> 함수를 통해서 접근 할 거니까 너무 걱정하실 필요 없어요
	// -> 사실 컴퓨터의 연결이라고 하는 건 생각보다 까다롭습니다
	// -> 컴퓨터가 내용을 받아주려고 한다면 상대방의 메세지를 받을 준비가 되어있어야 합니다
	// -> 안그러면 그냥 지나가던 이상한 정보를 받을 수도 있고, 해커의 공격이 담긴 메세지를 받을 수도 있어요
	// -> 보통은 소켓이 닫혀있습니다. 무엇을 받는 무시하는 거에요 ( 열리게 되는 조건이 있어요 )
	// -> 제가 이미 그 주소로 메세지를 보냈다면! 소켓이 받아줘요!
	// -> 소켓을 열어주는 소켓이 필요한 거에요 소켓 하나를 [ 리슨 소켓 ]으로 만듭니다
	// -> "접속 요청"만 받아주는 소켓을 여는 거에요! 누가 접속 요청을 한다면! 비어있는 소켓 하나를 찾아요 그래서 개랑 연결시켜주는
	//	  하나의 창구가 되는거에요

	// -> IPv4로 연결을 받는다, 연결을 지속시킨다(TCP 서버)
	int listenFD = socket(AF_INET, SOCK_STREAM, 0);

	// -> 연결할 FD
	int connetFD;

	// -> 연결 결과 저장
	int result = 0;

	struct sockaddr_in listenSocket, connectSocket;
	socklen_t addressSize;

	// -> 받는 버퍼
	char buffRecv[BUFFER_SIZE];

	// -> 주는 버퍼
	char buffSend[BUFFER_SIZE];

	// -> 일단 0으로 초기화
	memset(buffRecv, 0, sizeof(buffRecv));
	memset(buffSend, 0, sizeof(buffSend));

	// -> 서버 시작
	// -> 실패하면 그대로 프로그램을 종료합시다
	if (StartServer(&listenFD))
	{
		return 0;
	};

	cout << "서버가 정상적으로 실행 되었습니다." << endl;

	// -> pollFDArray가 제가 연락을 기다리고 있는 애들이에요
	// -> 그러다 보니까! 일단 처음에는 연락해줄 애가 없다는 것은 확인해야겠죠!

	for (int i = 0; i < FD_NUMBER; i++)
	{
		// -> -1이 없다는 뜻!
		pollFDArray[i].fd = -1;
	};

	// -> 리슨 소켓도 따로 함수 만들어서 돌릴 건 아니니까!
	pollFDArray[0].fd = listenFD;

	// -> 읽기 대기중! 지금 가져왔어요~
	pollFDArray[0].events = POLLIN;
	pollFDArray[0].revents = 0;

	// -> 무한 반복!
	for (;;)
	{
		// -> 기다려요! 만약에 누군가가 저한테 메세지를 건내준다면! 그 때에서야 제가 움직이는 거에요
		// -> 메세지가 있는지 없는지를 확인하는 방법
		result = poll(pollFDArray, FD_NUMBER, -1);

		// -> 메세지가 있어야만 뭔가 할 거에요!
		if (result > 0)
		{
			// -> 0번이 리슨 소켓이었습니다!
			// -> 0번에 들어려고 하는 애들을 체크해주긴 해야 해요!
			if (pollFDArray[0].revents == POLLIN)
			{
				// -> 누가 들어왔다면!
				// -> 들어 오세요
				connetFD = accept(listenFD, (struct sockaddr*)&connectSocket, &addressSize);

				//- > 어디보자.. 자리가 있나..
				// -> 0번은 리슨 소켓이니까! 1번부터 찾아봅시다~
				for (int i = 1; i < FD_NUMBER; i++)
				{
					if (pollFDArray[i].fd == -1)
					{
						// -> 자리가 있다!
						pollFDArray[i].fd = connetFD;
						pollFDArray[i].events = POLLIN;
						pollFDArray[i].revents = 0;

						// -> 새로운 유저 정보를 생성 합니다
						userFDArray[i] = new UserData();

						// -> 너가 이 자리에 있는 거야!
						userFDArray[i]->FDNumber = i;
						break;
					};
				};
			};
		};
	};

	return 0;
}
