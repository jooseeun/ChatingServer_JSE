// Server_HW3.cpp : �� ���Ͽ��� 'main' �Լ��� ���Ե˴ϴ�. �ű⼭ ���α׷� ������ ���۵ǰ� ����˴ϴ�.
//
#pragma comment(lib,"ws2_32")
#include <stdio.h>
#include <stdlib.h>
#include <winsock2.h>

#define SERVERIP "127.0.0.1" // �������ּ� ���
#define SERVERPORT 9000
#define BUFSIZE 512

//����� ���� ������ �Լ�
int recvn(SOCKET s, char* buf, int len, int flags)
{
	int received; // ���������� recv() �Լ��� ���ϰ��� ������ ���� , �������� �������� ����Ʈ��
	char* ptr = buf;// �������α׷� ������ �����ּ�
	int left = len;//���� �����ʴ� �������� ũ��

	while (left > 0)
	{
		received = recv(s, ptr, left, flags);
		if (received == SOCKET_ERROR) // ���� üũ
			return SOCKET_ERROR;
		else if (received == 0)
			break;
		left -= received; // �����͸� ���������� left�� ����
		ptr += received; // �����͸� ���������� �ּҰ� �̵�
	}
	return(len - left);
}
int main(int argc, char* argv[])
{
	int retval;

	//���� �ʱ�ȭ
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
		return 1;

	//���� ����
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);

	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr(SERVERIP);
	serveraddr.sin_port = htons(SERVERPORT);
	// serveraddr�� ����ü ������ ä����

	retval = connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	// sock�� ������ ����ϱ� ���� Ŭ���̾�Ʈ�� ���� ����, serveraddr�� ������ ip�ּҿ� port��ȣ�� 
	//���� ����ü �̴�.
	// �ڵ����� Ŭ���̾�Ʈ ip�ּҿ� ��Ʈ��ȣ�� ����


	//������ ��ſ� ����� ����
	int len;
	char buf[BUFSIZE + 1];
	char nickname[BUFSIZE];
	printf("�г����� �Է��ϼ���: ");
	fgets(nickname, BUFSIZE, stdin);
	send(sock, nickname, strlen(nickname), 0);

	//������ �����͸� ����ϴ� while��
	while (1)
	{
		//������ �Է�
		printf("\n[����������] ");
		if (fgets(buf, BUFSIZE + 1, stdin) == NULL) // fgets�Լ��� �̿��ؼ� ����ڷ� ���� ���ڿ��� �Է¹޴´�.
			break;

		len = strlen(buf);
		if (buf[len - 1] == '\n') //\n �� �����ϴ� �ڵ�, �������� ��½� �ٹٲ� ���� �� ����� ������ �����ϰ� �ϱ� �����̴�.
			buf[len - 1] = '\0';
		if (strlen(buf) == 0) // \n�� ������ 0�̸� ���͸� �����ٴ� ��. ���� �������� socket close�Ѵ�.
			break;

		//������ ������
		retval = send(sock, buf, strlen(buf), 0);
		printf("[TCP Ŭ���̾�Ʈ] %d ����Ʈ�� ���½��ϴ�\n", retval);

		//������ �ޱ�
		retval = recvn(sock, buf, retval, 0);
		if (retval == 0)
			break;

		//���� ������ ���
		buf[retval] = '\0';
		printf("\n[TCP Ŭ���̾�Ʈ] %d ����Ʈ�� �޾ҽ��ϴ�\n", retval);
		printf("\n[����������] %s ", buf);
	}

	//������ �ݴ´�
	closesocket(sock);

	//��������
	WSACleanup();
	return 0;
}

