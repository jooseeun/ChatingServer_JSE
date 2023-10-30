// Server_HW3.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#pragma comment(lib,"ws2_32")
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

#define SERVERIP "127.0.0.1" // 루프백주소 사용
#define SERVERPORT 9000
#define BUFSIZE 512

//사용자 정의 데이터 함수
int recvn(SOCKET s, char* buf, int len, int flags)
{
	int received; // 내부적으로 recv() 함수의 리턴값을 저장할 변수 , 현재읽은 데이터의 바이트수
	char* ptr = buf;// 응용프로그램 버퍼의 시작주소
	int left = len;//아직 읽지않는 데이터의 크기

	while (left > 0)
	{
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR) // 에러 체크
			return SOCKET_ERROR;
		else if (received == 0)
			break;
		left -= received; // 데이터를 읽을때마다 left값 감소
		ptr += received; // 데이터를 읽을때마다 주소값 이동
	}
	return(len - left);
}
int main(int argc, char* argv[])
{
	int retval;

	//윈속 초기화
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return 1;

	//소켓 생성
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);
	// serveraddr의 구조체 값들을 채워줌

	retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	// sock는 서버와 통신하기 위해 클라이언트가 만든 소켓, serveraddr은 서버의 ip주소와 port번호를 
	//적은 구조체 이다.
	// 자동으로 클라이언트 ip주소와 포트번호를 연결


	//데이터 통신에 사용할 변수
	int len;
	char buf[BUFSIZE + 1];
	char nickname[BUFSIZE];
	printf("닉네임을 입력하세요: ");
	fgets(nickname, BUFSIZE, stdin);
	send(sock, nickname, strlen(nickname), 0);

	//서버와 데이터를 통신하는 while문
	while (1)
	{
		//데이터 입력
		printf("\n[보낼데이터] ");
		if (fgets(buf, BUFSIZE + 1, stdin) == NULL) // fgets함수를 이용해서 사용자로 부터 문자열을 입력받는다.
			break;

		len = strlen(buf);
		if (buf[len - 1] == '\n') //\n 을 제거하는 코드, 데이터의 출력시 줄바꿈 여부 및 방식을 서버가 결정하게 하기 윟마이다.
			buf[len - 1] = '\0';
		if (strlen(buf) == 0) // \n을 제거후 0이면 엔터를 눌렀다는 뜻. 루프 빠져나가 socket close한다.
			break;

		//데이터 보내기
		retval = send(sock, buf, strlen(buf), 0);
		printf("[TCP 클라이언트] %d 바이트를 보냈습니다\n", retval);

		//데이터 받기
		retval = recvn(sock, buf, retval, 0);
		if (retval == 0)
			break;

		//받은 데이터 출력
		buf[retval] = '\0';
		printf("\n[TCP 클라이언트] %d 바이트를 받았습니다\n", retval);
		printf("\n[받은데이터] %s ", buf);
	}

	//소켓을 닫는다
	closesocket(sock);

	//윈속종료
	WSACleanup();
	return 0;
}

