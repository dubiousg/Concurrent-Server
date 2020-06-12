#pragma once
#include <vector>
#include <thread>
#include <functional>
#include <deque>
#include <mutex>

#include "TCPServer.h"

class Threadpool
{



public:
	Threadpool(TCPServer &s);
	~Threadpool();

	void workerThread();
	void pushFunction(std::function<void(TCPServer &s)> f); //pushes function to function queue

	int threadsActive(); //returns # of instantiated threads.


private:
	const unsigned int maxThreads = 100;
	std::vector<std::thread> threads; //thread collection
	std::deque<std::function<void(TCPServer &s)>> functionQueue; //queue that holds void functions
	//std::unique_lock<std::mutex> queueLock;
	std::mutex queueMutex;
	TCPServer &server;
};


