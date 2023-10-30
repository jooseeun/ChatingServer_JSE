// Server_HW3.cpp : �� ���Ͽ��� 'main' �Լ��� ���Ե˴ϴ�. �ű⼭ ���α׷� ������ ���۵ǰ� ����˴ϴ�.
//
#pragma comment(lib,"ws2_32")
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>
#define SERVERPORT 9000 //server ��� ��Ʈ ��ȣ
#define BUFSIZE 512

int main(int argc, char* argv[])
// main �Լ��� �Ű�����
// argc=�����Լ��� ���޵Ǵ� ������ ���� (default=1, ������(�����Լ��� ���)), * argv[] = �����Լ��� ���޵Ǵ� �������� ����, ���ڿ��� �迭

{
	int retval;
	//���� �ʱ�ȭ - ���α׷��� ȣ���ϴ� ���� ������ �˷���, ���̺귯�� �ʱ�ȭ
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) // 2.2 �� ���� ����
		return 1;

	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	//SOCKET Ÿ���� listensocket ����

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));//������ �Լ� �̿��Ͽ� �ּҰ� 0���� �ʱ�ȭ
	serveraddr.sin_family = AF_INET; // ����ü �� ä���ֱ� - IPV4 �ּ� ���
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY); // �� IP �ڵ� �Ҵ�
	serveraddr.sin_port = htons(SERVERPORT);
	retval = bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	//�������� bind���� ip�ּҿ� ��Ʈ��ȣ�� ������ ����
	// 
	//listen - Ŭ���̾�Ʈ�� ���� listen socket�� ��ٸ��ٴ� ��
	// SOMAXCONN �� �Ϻ��������ݿ��� ���������� �ִ밪���� Ŭ���̾�Ʈ�� ��� ��Ų�ٴ� ��
	retval = listen(listen_sock, SOMAXCONN);

	SOCKET client_sock; // accept �Լ��� ���ϰ��� ������ ����
	SOCKADDR_IN clientaddr; // accept �Լ��� �����ϸ� ���⿡ cli �� �ּҿ� ��Ʈ��ȣ ����
	int addrlen;// �ּ��� ����
	char buf[BUFSIZE + 1];
	char clientNickname[BUFSIZE];
	while (1) // Ŭ���̾�Ʈ ���ӿ䱸 ��� ���ѷ���
	{
		addrlen = sizeof(clientaddr);
		// �����ּ� ����ü ������ ũ�⸦ ������ ������ ����
		client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
		// accept �Լ� �̸� ȣ��, ���ӿ�û�� Ŭ���̾�Ʈ�� �ּҸ� �����ϰ�
		//������ ����� ���� ������ client_socket�� �����Ų��.
		int nicknameLength = recv(client_sock, clientNickname, BUFSIZE, 0);
		if (nicknameLength <= 0) {
			printf("Ŭ���̾�Ʈ�� ������ ������ϴ�.\n");
			break;
		}
		//������ Ŭ���̾�Ʈ�� ������ ���
		printf("\n[TCP ����] Ŭ���̾�Ʈ ���� : IP �ּ� = %s, ��Ʈ��ȣ =%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));

		clientNickname[nicknameLength] = '\0';
		printf("[TCP/%s:%d] �г���: %s\n", inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port), clientNickname);
		while (1) // Ŭ���̾�Ʈ�� ��ȭ����
		{
			//������ �ޱ�
			retval = recv(client_sock, buf, BUFSIZE, 0);
			if (retval == 0) // ���������Ͱ� ���̻� ������ ����
			{
				break;
			}
			//���������� ���
			buf[retval] = '\0';
			printf("[TCP/%s:%d] : %s \n", inet_ntoa(clientaddr.sin_addr),
				ntohs(clientaddr.sin_port), buf);

			//������ ������
			retval = send(client_sock, buf, retval, 0);
		}

		//���̻� ���� �����Ͱ� ������ ����. �̶� listen socket�� ���� x
		closesocket(client_sock);
		printf("\n[TCP ����] Ŭ���̾�Ʈ ���� : IP �ּ� = %s, ��Ʈ��ȣ =%d\n",
			inet_ntoa(clientaddr.sin_addr), ntohs(clientaddr.sin_port));
	}
	//���� ���� ����
	closesocket(listen_sock);

	//���� ����
	WSACleanup();
	return 0;
}

