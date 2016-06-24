#include "mpclient.h"
#include "mpcomm.h"
#include <pthread.h>

const char * appid = "appid";
const char * secret = "secret";
int threadCnt = 10;

using namespace mpcomm;

class MyMutex : public IMutex{
	public:
		MyMutex(){
			printf("num : %d\n", GetLockNum() );
		}
		virtual ~MyMutex(){
		}
	virtual unsigned long int GetThreadId(){
		printf("GetThreadId\n");
		//to implement: return a thread id
		return 0;
	}
	virtual int Lock( int id){
		printf("Lock %d\n", id);
		//to implement: lock  Lock${id}
		return 0;
	}
	virtual int UnLock( int id){
		printf("UnLock %d\n", id);
		//to implement: unlock  Lock${id}
		return 0;
	}
}g_lock;

void * ThreadsRun( void * args ){
	MpClient client;
	client.SetLogConfig( mpcomm::ERR, 1<<20, "" );
	for ( int i = 0; i < 100; ++i ){
		TokenReq req;
		TokenResp resp;
		req.set_appid( appid );
		req.set_secret( secret );
		int ret = client.Token( req, &resp );
		printf("%d %d\n", ret, (int)(resp.errcode()) );
	}
	{
		GetCallbackIPReq req;
		GetCallbackIPResp resp;
		req.set_access_token( "access_token");
		int ret = client.GetCallbackIP(req,&resp);
		if ( ret || resp.errcode() ){
			//error 
		}
	}
	return NULL;
}

int main(){
	MpClient::GlobalInit(&g_lock);
	pthread_t list[threadCnt];
	for ( int i = 0; i < threadCnt; ++i ){
		pthread_create( &(list[i]), NULL, ThreadsRun, NULL );
	}

	for ( int i = 0; i < threadCnt; ++i ){
		void * ptr;
		pthread_join( list[i], &ptr ); 
	}

	printf("done");

	MpClient::ShutDown();
}
