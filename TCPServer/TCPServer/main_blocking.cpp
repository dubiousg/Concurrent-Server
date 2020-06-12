#include <iostream>
#include <algorithm>
#include <string>
#include <stdlib.h>
#include <string>
#include <thread>

#include "config.h"
#include "TCPServer.h"
#include  "RequestParser.h"
#include "Threadpool.h"

#include <deque>
#include <functional>

#ifndef NON_BLOCKING

#define DEFAULT_PORT 12345

int main()
{
	void ClientRequestLoop(TCPServer &server); //one loop per client

	TCPServer server(DEFAULT_PORT);
	Threadpool* pool = new Threadpool(server); 
	
	auto sleeptime = std::chrono::seconds(600);


	std::cout << "Starting server. Send \"exit\" (without quotes) to terminate." << std::endl;
	
	//ClientRequestLoop(server);
	for (int i(0); i < 100; i++) {
		pool->pushFunction(ClientRequestLoop); //push function to be called by a thread
	}

	std::this_thread::sleep_for(sleeptime);

	std::cout << "Server terminated." << std::endl;
}

void ClientRequestLoop(TCPServer &server) {

	ReceivedSocketData receivedData;

	receivedData = server.accept();

	do {
		receivedData.request = "";
		while (receivedData.request == "") {
			server.receiveData(receivedData, 0);
		}

		std::cout << "Bytes received: " << receivedData.request.size() << std::endl;
		std::cout << "Data received: " << receivedData.request << std::endl;

		receivedData.reply = receivedData.request;

		PostRequest post = PostRequest::parse(receivedData.request);
		if (post.valid)
		{
			std::string postID = std::to_string(server.addPost(post)); //post added to posts map
			receivedData.reply = "Post id: " + postID; //Change to interpolation later

			std::cout << "Post request: " << post.toString() << std::endl;
			std::cout << "Post topic: " << post.getTopicId() << std::endl;
			std::cout << "Post message: " << post.getMessage() << std::endl;
			server.sendReply(receivedData);
			continue;
		}

		ReadRequest read = ReadRequest::parse(receivedData.request);
		if (read.valid)
		{
			receivedData.reply = server.readPost(read);

			std::cout << "Read request" << read.toString() << std::endl;
			std::cout << "Read topic: " << read.getTopicId() << std::endl;
			std::cout << "Read post id: " << read.getPostId() << std::endl;
			server.sendReply(receivedData);
			continue;
		}

		CountRequest count = CountRequest::parse(receivedData.request);
		if (count.valid)
		{
			receivedData.reply = std::to_string(server.countPosts(count));
			std::cout << "Count request: " << count.toString() << std::endl;
			std::cout << "Count topic: " << count.getTopicId() << std::endl;
			server.sendReply(receivedData);
			continue;
		}

		ListRequest list = ListRequest::parse(receivedData.request);
		if (list.valid)
		{
			receivedData.reply = server.List(list);
			std::cout << "List request: " << list.toString() << std::endl;
			server.sendReply(receivedData);
			continue;
		}

		//std::cout << "Unknown request: " << receivedData.request << std::endl;
		//std::cout << std::endl;

	} while (receivedData.request != "exit");

	server.closeClientSocket(receivedData);
}
#endif //NON_BLOCKING
