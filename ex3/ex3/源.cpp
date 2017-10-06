#include<iostream>
#include<thread>
#include<string>

#include<windows.h>
using namespace std;

//class Semaphore {
//public:
//	Semaphore(int value = 1) : count( value ) {}
//
//	void wait() {
//		std::unique_lock<std::mutex> lock( mutex );
//		--count;
//		if (count<0) { 
//			condition.wait(lock);
//		}
//	}
//	void signal() {
//		std::lock_guard<std::mutex> lock{ mutex };
//		++count;
//		if ( count<= 0) { 
//		
//			condition.notify_one(); 
//		}
//	}
//
//private:
//	int count;
//
//	std::mutex mutex;
//	std::condition_variable condition;
//};
class Mutex
{
public:
	Mutex()
	{
		_mutex = CreateMutex(NULL, FALSE, NULL);
	}
	~Mutex()
	{
		CloseHandle(_mutex);
	}
	void lock()const
	{
		DWORD d = WaitForSingleObject(_mutex, INFINITE);
	}
	void unlock()const
	{
		ReleaseMutex(_mutex);
	}
private:
	HANDLE _mutex;
};

class MutexUser
{
public:

	MutexUser(const Mutex &mutex) :_mutex(mutex)
	{
		_mutex.lock();
	}
	~MutexUser()
	{
		_mutex.unlock();
	}
private:
	const Mutex &_mutex;
};

Mutex rMutex;
Mutex m;
int readerCount = 0;

string board = "666";
void reader()
{
	while (true)
	{
		rMutex.lock();
		if (readerCount == 0)
		{
			m.lock();
		}
		++readerCount;
		rMutex.unlock();

	
		cout << board << endl;;
		Sleep(100);
		rMutex.lock();
		--readerCount;
		if (readerCount == 0)
			m.unlock();
		rMutex.unlock();
		Sleep(500);
	}
	


}

void writer()
{
	while(true)
	{
		m.lock();
		cin >> board;
		m.unlock();
		Sleep(1000);
	}

		
}

int main()
{
	//thread readerTh(&reader);
	//thread writerTh(&writer);

	//readerTh.join();
	//writerTh.join();
	HANDLE handle1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)reader, NULL, 1, 0);
	HANDLE handle2 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)writer, NULL, 1, 0);
	while (true)
	{
		
	}
}