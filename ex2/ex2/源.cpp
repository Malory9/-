//#include<thread>
//#include<iostream>
//#include<windows.h>
//#include <mutex>
//using namespace std;
//
//static int accnt1 = 0;
//static int accnt2 = 0;
//
//mutex mx;
//
//void acc1()
//{
//	while (true)
//	{
//		int val = rand()%100;
//		mx.lock();
//		accnt1 -= val;
//		accnt2 += val;
//		mx.unlock();
//		cout << accnt1 << " " << accnt2 << "   acc1" << endl;
//		Sleep(500);
//	}
//}
//void acc2()
//{
//	while (true)
//	{
//		int val = rand()%100;
//		mx.lock();
//		accnt1 -= val;
//		accnt2 += val;
//		mx.unlock();
//		cout << accnt1 << " " << accnt2 << "   acc2" << endl;
//		Sleep(1000);
//	}
//	
//
//}
//int main()
//{
//	thread thr1(&acc1);
//	thread thr2(&acc2);
//	thr1.join();
//	thr2.join();
//	
//}

#include <iostream>
#include <windows.h>

static int accnt1 = 0;
static int accnt2 = 0;
using namespace std;

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
	DWORD d =	WaitForSingleObject(_mutex, INFINITE);
	}
	void unlock()const
	{
		ReleaseMutex(_mutex);
	}
private :
	HANDLE _mutex;
};

class MutexUser
{
public:

	MutexUser(const Mutex &mutex):_mutex(mutex)
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

Mutex mx;
void acc1()
{
	while (true)
	{
		Sleep(500);
		int val = rand()%100;
		MutexUser m(mx);
		//mx.lock();
		accnt1 -= val;
		accnt2 += val;
		cout << accnt1 << " " << accnt2 << "   acc1" <<flush<< endl;
		//mx.unlock();
	
	

		
	}
}
void acc2()
{
	while (true)
	{
		Sleep(500);
		int val = rand()%100;
		MutexUser m(mx);
		accnt1 -= val;
		accnt2 += val;
		cout << accnt1 << " " << accnt2 << "   acc2" << flush << endl;
		

		
	}
	

}
int main()
{
	HANDLE handle1 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)acc1, NULL, 1, 0);
	HANDLE handle2 = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)acc2, NULL, 1, 0);
	while(true)
	{
		
	}
}