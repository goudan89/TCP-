// FUWU.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//
#include <iostream>
#include <string>
#include <WinSock2.h>
#include <Ws2tcpip.h>
#include <cstdio>
#include <vector>

#pragma comment(lib,"Ws2_32.lib")
using namespace std;

const int PORT = 8000;
#define MAX_SIZE 100
vector<int> sockets;   //存储套接字

// 线程回调函数（接收客户端的信息）
DWORD WINAPI ThreadProc(LPVOID lp)	// lp是接收到的最新的套接字
{
	SOCKET* 	NewConnection = (SOCKET*)lp;	// 转型
	sockets.push_back(*NewConnection);
	char RecvBuffer[MAX_SIZE];

	int recvbyt = 0;
	int sendbyt = 0;
	while (1)	// 一直要接收这个客户端的数据
	{
		// 接收数据并判断是否成功，该函数返回接收字符个数
		recvbyt = recv(*NewConnection, RecvBuffer, sizeof(RecvBuffer),0);
//		if (recvbyt > 0)
//		{
//			cout << "接收消息：" << RecvBuffer << "					来自客户端：" << *NewConnection << endl;
//		}
//		else
//		{
//			cout << "接收消息结束！" << endl;
//			break;
//		}
//		memset(RecvBuffer, 0, sizeof(RecvBuffer));
//		cout << "请输入要发送到客户端的消息：" << endl;
//		cin.getline(SendBuffer, 100);
//		int k = 0;
//		k = send(*NewConnection, SendBuffer, sizeof(SendBuffer), 0);
//		if (k < 0) {
//			cout << "发送失败" << endl;
//		}


		cout << "接收消息：" << RecvBuffer << "					来自客户端：" << *NewConnection << endl;
		if (*NewConnection == sockets[0])
		{
			sendbyt = send(sockets[1], RecvBuffer, sizeof(RecvBuffer), 0);
			cout << "发送消息至客户端:" << sockets[1] << endl;
		}
		if (*NewConnection == sockets[1])
		{
			sendbyt = send(sockets[0], RecvBuffer, sizeof(RecvBuffer), 0);
			cout << "发送消息至客户端:" << sockets[0] << endl;
		}
		if (sendbyt < 0)
		{
			cout << "发送失败" << endl;
			break;
		}
		memset(RecvBuffer, 0, sizeof(RecvBuffer));

	}
	closesocket(*NewConnection);
	free(NewConnection);
	return 0;
}

int main(int argc,char* argv[])
{
	WSADATA		wsaData;	// 一种用于初始启动信息的数据结构，加载winsocket.dll
	int Ret;
	SOCKET		ListeningSocket;// 服务端的套接字
	SOCKET		NewConnection;	// 接受到的客户端的套接字
	SOCKADDR_IN		ServerAddr;	// 服务端地址
	


	// 初始启动信息（加载版本号，模块），并判断是否成功
	if ((Ret = WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0)
	{
		cout << "WSAStartup failed with error" << endl;
		return 0;
	}


	// 创建Socket, 使用 IPv4 进行通信（Windows只有AF_INET)、流式套接字、TCP协议
	if ((ListeningSocket = socket(AF_INET, SOCK_STREAM, 0)) == SOCKET_ERROR)
	{
			cout<<"socket failed with error"<<endl;
			WSACleanup();	// 与 WSAStartup 配套使用
			return 0;
	}

	// 初始化地址信息
		ServerAddr.sin_family = AF_INET;	// 地址格式
		ServerAddr.sin_port = htons(PORT);	// 地址端口号（字节转换成网络大端模式）
	
		ServerAddr.sin_addr.S_un.S_addr = htonl(INADDR_ANY);	// 接受本机内的所有ip地址（长整形，转化字节）

		// 绑定地址，并判断是否成功
		if ((bind(ListeningSocket, (LPSOCKADDR)& ServerAddr, sizeof(ServerAddr)) == SOCKET_ERROR))
		{
			cout<<"bind failed with error"<<endl;
			//closesocket(ListeningSocket);	// 释放套接字
			WSACleanup();
			return 0;
		}

	// 监听请求信号，最多有五个客户端（自己设置，不要太多），并判断是否成功
	if (listen(ListeningSocket, 5) == SOCKET_ERROR)
	{
		cout << "listen failed with error" << endl;
		//closesocket(ListeningSocket);
		WSACleanup();
		return 0;
	}

	// 提示信息
//	cout << "We are waiting a connection on port:" << argv[1] << endl;
	cout << "Listen(正在监听)..." << endl;

	while (1)	// 一直等待客户端连接
	{
		SOCKET* ClientSocket = new SOCKET;	// 求出地址的长度
		ClientSocket = (SOCKET*)malloc(sizeof(SOCKET));
		// 接收请求（链接客户端），第二个参数是客户端的地址
		int SockAddrlen = sizeof(sockaddr);
		*ClientSocket = accept(ListeningSocket, 0, 0);
		cout << "一个客户端已连接到服务器，socket是：" << *ClientSocket << endl;
		CreateThread(NULL, 0, &ThreadProc, ClientSocket, 0, NULL);
	}//while
	closesocket(ListeningSocket);
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
