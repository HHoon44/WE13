// 여기서 제가 제일 먼저 볼거!
// 서버에서는 IP가 았가 머룐압니다
// 저희 내부 네트워크에도 IP를 알려줘야한다
// 내부 IP를 적으시면 됩니다
#define SERVER_IP "10.178.0.3"
// 동적포트를 사용해보록 합시다
#define SERVER_PORT 61255
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

int main()
{
	cout << "Hello Linux" << endl;
	return 0;
}
