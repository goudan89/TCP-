// KEHU.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//


#include <iostream>
#include <string>
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <conio.h>
#include <Windows.h>
#include <cstdio>
#pragma comment(lib,"Ws2_32.lib")
using namespace std;

const int PORT = 8000;
#define MAX_SIZE 100
int main()
{
	WSADATA wsd;// 用于初始启动信息
	SOCKADDR_IN		ServerAddr;	// 服务端地址
	if (WSAStartup(MAKEWORD(2, 2), &wsd) != 0)
	{
		cout << "WSAStartup failed with error" << endl;
		return 0;
	}
	SOCKET SocketClient = socket(AF_INET, SOCK_STREAM, 0);
	if (SocketClient == SOCKET_ERROR)
	{
		cout << "socket failed with error" << endl;
		WSACleanup();
		return 0;
	}
	ServerAddr.sin_family = AF_INET;	// 使用ip4
	ServerAddr.sin_addr.S_un.S_addr = inet_addr("127.0.0.1");
	ServerAddr.sin_port = htons(PORT);

	//大小
	char sendBuff[MAX_SIZE]; 
	char recvBuff[MAX_SIZE];
	memset(sendBuff, 0, sizeof(sendBuff));
	memset(recvBuff, 0, sizeof(recvBuff));

	cout << "we are trying to connect" << endl;
	if (connect(SocketClient, (SOCKADDR*)& ServerAddr, sizeof(ServerAddr)) == SOCKET_ERROR)
	{
		cout << "connect failed with error" << endl;
		
		WSACleanup();	// 清空启动信息
		return 0;
	}
	
	//提示
	cout << "our connection succeeded" << endl;

	//登陆（循环
	int mark = 0;
	while (mark == 0)
	{
		cout << "请输入用户名" << endl;
		char username[20];
		cin.getline(username, 20);
		cout << "请输入密码" << endl;
		char password[20];
		cin.getline(password, 20);
		if (strcmp(username, "gh") == 0 && strcmp(password, "123") == 0)
		{
			cout << "欢迎狗嗨" << endl;
			mark = 1;
		}
		else if (strcmp(username, "gd") == 0 && strcmp(password, "123456") == 0)
		{
			cout << "欢迎狗蛋" << endl;
			mark = 1;
		}
		else
		{
			cout << "请检查用户名与密码" << endl;
			mark = 0;
		}

	}
	
	
	//解决不能同步收消息
	cout << "谁先发送消息" << endl;
	char a[10];
	cin.getline(a, 10);
	if (strcmp(a, "1") == 0)
	{
		while (1)
		{
			int ret;
			cout << "发送消息：";
			cin.getline(sendBuff, 100);
			ret = send(SocketClient, sendBuff, sizeof(sendBuff), 0);
			if (ret == SOCKET_ERROR)
			{
				cout << "send failed with error" << endl;
				break;
			}
			memset(sendBuff, 0, MAX_SIZE);

			ret = recv(SocketClient, recvBuff, MAX_SIZE, 0);
			if (recvBuff != 0)
				cout << "接收到消息为：" << recvBuff << endl;
			memset(recvBuff, 0, sizeof(recvBuff));

		}

	}
	else
	{
		while (1)
		{
			int ret;
			ret = recv(SocketClient, recvBuff, MAX_SIZE, 0);
			if (recvBuff != 0)
				cout << "接收到消息为：" << recvBuff << endl;
			memset(recvBuff, 0, sizeof(recvBuff));
			cout << "发送消息：";
			cin.getline(sendBuff, 100);
			ret = send(SocketClient, sendBuff, sizeof(sendBuff), 0);
			if (ret == SOCKET_ERROR)
			{
				cout << "send failed with error" << endl;
				break;
			}
			memset(sendBuff, 0, MAX_SIZE);

		}

	}

	closesocket(SocketClient);
	WSACleanup();
	return 0;

}

// 运行程序: Ctrl + F5 或调试 >“开始执行(不调试)”菜单
// 调试程序: F5 或调试 >“开始调试”菜单

// 入门提示: 
//   1. 使用解决方案资源管理器窗口添加/管理文件
//   2. 使用团队资源管理器窗口连接到源代码管理
//   3. 使用输出窗口查看生成输出和其他消息
//   4. 使用错误列表窗口查看错误
//   5. 转到“项目”>“添加新项”以创建新的代码文件，或转到“项目”>“添加现有项”以将现有代码文件添加到项目
//   6. 将来，若要再次打开此项目，请转到“文件”>“打开”>“项目”并选择 .sln 文件
