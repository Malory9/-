#include<iostream>
#include<thread>
#include<string>
#include<condition_variable>
#include<mutex>
#include<windows.h>
#include<fstream>
#include <conio.h>
using namespace std;



#define INTE_PER_SEC  1000
#define MAX_THREAD_NUM  64
#define MAX_FILE_NUM  32
#define MAX_STR_LEN  32

class Semaphore {
public:
	Semaphore(int value = 1) : count( value ) {}

	void wait() {
		std::unique_lock<std::mutex> lock( mutex );
		--count;
		if (count<0) { 
			condition.wait(lock);
		}
	}
	void signal() {
		std::lock_guard<std::mutex> lock{ mutex };
		++count;
		if ( count<= 0) { 
		
			condition.notify_one(); 
		}
	}

private:
	int count;

	std::mutex mutex;
	std::condition_variable condition;
};
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

Semaphore rMutex;
Semaphore m;
int readerCount = 0;
struct ThreadInfo
{
	int	serial;
	char	entity;
	double	delay;
	double	persist;
};
void reader(void* p)
{
	
DWORD wait_for_mutex;
DWORD m_delay;
DWORD m_persist;
int m_serial;

//get info froam para

m_serial = ((ThreadInfo*) (p)) -> serial;
m_delay  = (DWORD) (((ThreadInfo*)(p)) -> delay*INTE_PER_SEC);
m_persist = (DWORD) (((ThreadInfo*)(p)) -> persist*INTE_PER_SEC);

Sleep(m_delay);

printf("Reader thread %d sents the reading require .\n",m_serial);

// wait mutex
	
	
		rMutex.wait();
		if (readerCount == 0)
		{
			m.wait();
		}
		++readerCount;
		rMutex.signal();

	
		printf("Reader thread %d begins to read file.\n",m_serial);
		Sleep(m_persist);

//exit thread

		printf("Reader thread %d finished reading file.\n",m_serial);

		rMutex.wait();
		--readerCount;
		if (readerCount == 0)
			m.signal();
		rMutex.signal();
		
	
	


}

void writer(void* p)
{
DWORD m_delay;
DWORD m_persist;
int m_serial;

//get info froam para

m_serial = ((ThreadInfo*) (p)) -> serial;
m_delay  = (DWORD) (((ThreadInfo*)(p)) -> delay*INTE_PER_SEC);
m_persist = (DWORD) (((ThreadInfo*)(p)) -> persist*INTE_PER_SEC);

Sleep(m_delay);

printf("Writer thread %d sents the writing require .\n",m_serial);

// wait resource



		m.wait();
		
		
		printf("Writer thread %d begins to write to the file.\n",m_serial);
        Sleep(m_persist);

//exit thread

        printf("Writer thread %d finished Writing to the file.\n",m_serial);

		
		m.signal();
		
	

		
}

int main()
{
	//thread readerTh(&reader);
	//thread writerTh(&writer);

	//readerTh.join();
	//writerTh.join();
	
DWORD n_thread = 0;
DWORD thread_ID;
DWORD wait_for_all;

HANDLE h_Thread[MAX_THREAD_NUM];
ThreadInfo  thread_info[MAX_THREAD_NUM];
std::ifstream  inFile;
inFile.open("sm5.dat");		//open file
printf( "Reader Priority:\n\n" );
while ( inFile )
{
	// read every reader/writer info
	inFile>>thread_info[n_thread].serial;
	inFile>>thread_info[n_thread].entity;
	inFile>>thread_info[n_thread].delay;
	inFile>>thread_info[n_thread++].persist;
	inFile.get();
} //end while
for( int i = 0; i < (int)(n_thread); i++)
{
	if(thread_info[i].entity == 'R' || thread_info[1].entity == 'r')
	{
	// Create Reader thread
    	h_Thread[i] = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)(reader), &thread_info[i], 0, &thread_ID);
	}	
	else {
	// Create Writer thread
   	h_Thread[i] = CreateThread( NULL, 0, (LPTHREAD_START_ROUTINE)(writer), &thread_info[i], 0, &thread_ID);
	}
} //end for

	
	
	//HANDLE handle1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)reader, NULL, 1, 0);
  _getch();
}
