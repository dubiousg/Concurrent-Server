#ifndef __TCPSERVER_H
#define __TCPSERVER_H

#include "ReceivedSocketData.h"
#include  "RequestParser.h"
#include <map>
#include <vector>
#include <mutex>

struct Post {
	int id;
	std::string text;
};

class TCPServer
{
public:
	TCPServer(unsigned short int port);
	~TCPServer();
	ReceivedSocketData accept();
	void receiveData(ReceivedSocketData& ret, bool blocking);
	int sendReply(ReceivedSocketData reply);

	void OpenListenSocket();
	void CloseListenSocket();
	int closeClientSocket(ReceivedSocketData &reply);
	std::string TCPServer::List(const ListRequest& list);
	int countPosts(const CountRequest& cr);
	int addPost(const PostRequest& pr); //adds a post and returns it's id
	std::string readPost(const ReadRequest& rr);

private:
	SOCKET ListenSocket;
	unsigned short int port;
	std::string portString;
	std::map<std::string, std::vector<Post>> posts;
	std::mutex postMutex;

	
};


#endif __TCPSERVER_H
