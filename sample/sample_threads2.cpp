#include "mpclient.h"
#include "mpcomm.h"

#include <pthread.h>
#include <sys/types.h>

using mpcomm::IMutex;

class MyMutex : public IMutex{
	public:

		MyMutex(){
			m_mutex_list = new pthread_mutex_t[GetLockNum()];
			for ( int i = 0; i < GetLockNum(); ++i ){
				pthread_mutex_init( &(m_mutex_list[i]), NULL );
			}
		}

		virtual ~MyMutex(){
			for ( int i = 0; i < GetLockNum(); ++i ){
				pthread_mutex_destroy( &(m_mutex_list[i]) );
			}
			delete[] m_mutex_list;
		}

		virtual unsigned long int GetThreadId(){
			return ( unsigned long int )pthread_self();
		}

		virtual int Lock( int id){
			return pthread_mutex_lock( &(m_mutex_list[id]) );
		}

		virtual int UnLock( int id){
			return pthread_mutex_unlock( &(m_mutex_list[id]) );
		}

	private:
		pthread_mutex_t * m_mutex_list;
};

int main(){
	MyMutex mutexs;
	MpClient::GlobalInit(&mutexs);

	//do something with threads

	MpClient::ShutDown();
}
