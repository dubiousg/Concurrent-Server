#include "Threadpool.h"


Threadpool::Threadpool(TCPServer &s) : server(s) {

	for (unsigned int i(0); i < maxThreads; i++)
		threads.push_back(std::thread(&Threadpool::workerThread, this));
}


Threadpool::~Threadpool(){
    for (unsigned int i(0); i < maxThreads; i++)
        threads[i].join();
}

void Threadpool::workerThread()
{

    while (true)
    {
        std::function<void(TCPServer &s)> task;
        
        if (!functionQueue.empty()) { //this may need to be a cv
            //mutex added to avoid race conditions on the functionQueue
            std::unique_lock<std::mutex> queueLock(queueMutex);
            //while (!queueLock.try_lock());
			if (!functionQueue.empty()) {
				if (functionQueue.front()) {
					task = functionQueue.front();
					functionQueue.pop_front();
					
				}
			}
			queueLock.unlock();
			if(task)
				task(server);


        }
        else
        {
            std::this_thread::yield();
        }
    }
}

void Threadpool::pushFunction(std::function<void(TCPServer &s)> f) {
    functionQueue.push_back(f);
}

int Threadpool::threadsActive() {
    
    return threads.size();
}

