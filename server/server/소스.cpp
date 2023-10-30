// Server_HW3.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//
#pragma comment(lib,"ws2_32")
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#define SERVERPORT 9000 //server 대기 포트 번호
#define BUFSIZE 512

int main(int argc, char* argv[])
// main 함수의 매개변수
// argc=매인함수에 전달되는 정보의 개수 (default=1, 절대경로(실행함수의 경로)), * argv[] = 메인함수에 전달되는 실질적인 정보, 문자열의 배열

{
	int retval;
	//윈속 초기화 - 프로그램에 호출하는 윈속 버전을 알려줌, 라이브러리 초기화
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) // 2.2 는 윈속 버전
		return 1;

	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	//SOCKET 타입의 listensocket 생성

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));//윈도우 함수 이용하여 주소값 0으로 초기화
	serveraddr.sin_family = AF_INET; // 구조체 값 채워넣기 - IPV4 주소 사용
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY); // 내 IP 자동 할당
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	//설정값을 bind해줌 ip주소와 포트번호가 설정된 상태
	// 
	//listen - 클라이언트의 연결 listen socket이 기다린다는 뜻
	// SOMAXCONN 는 하부프로토콜에서 지원가능한 최대값으로 클라이언트를 대기 시킨다는 뜻
	retval = listen(listen_sock, SOMAXCONN);

	SOCKET client_sock; // accept 함수의 리턴값을 저장할 소켓
	SOCKADDR_IN clientaddr; // accept 함수가 리턴하면 여기에 cli 의 주소와 포트번호 저장
	int addrlen;// 주소의 길이
	char buf[BUFSIZE + 1];
	char clientNickname[BUFSIZE];
	while (1) // 클라이언트 접속요구 대기 무한루프
	{
		addrlen = sizeof(clientaddr);
		// 소켓주소 구조체 변수의 크기를 정수형 변수에 저장
		client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
		// accept 함수 미리 호출, 접속요청한 클라이언트의 주소를 저장하고
		//데이터 통신을 위한 소켓인 client_socket에 연결시킨다.
		int nicknameLength = recv(client_sock, clientNickname, BUFSIZE, 0);
		if (nicknameLength <= 0) {
			printf("클라이언트와 연결이 끊겼습니다.\n");
			break;
		}
		//접속한 클라이언트와 데이터 통신
		printf("\n[TCP 서버] 클라이언트 접속 : IP 주소 = %s, 포트번호 =%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		clientNickname[nicknameLength] = '\0';
		printf("[TCP/%s:%d] 닉네임: %s\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), clientNickname);
		while (1) // 클라이언트와 대화시작
		{
			//데이터 받기
			retval = recv(client_sock, buf, BUFSIZE, 0);
			if (retval == 0) // 받은데이터가 더이상 없으면 종료
			{
				break;
			}
			//받은데이터 출력
			buf[retval] = '\0';
			printf("[TCP/%s:%d] : %s \n", inet_ntoa(clientaddr.sin_addr),
				ntohs(clientaddr.sin_port), buf);

			//데이터 보내기
			retval = send(client_sock, buf, retval, 0);
		}

		//더이상 받을 데이터가 없으면 종료. 이때 listen socket은 종료 x
		closesocket(client_sock);
		printf("\n[TCP 서버] 클라이언트 종료 : IP 주소 = %s, 포트번호 =%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
	}
	//리슨 소켓 종료
	closesocket(listen_sock);

	//윈속 종료
	WSACleanup();
	return 0;
}

