#include "mpclient.h"
#include "mpcomm.h"

int main(){
	//global init once
	int ret = MpClient::GlobalInit();
	if ( ret ){
		return -1;
	}
	
	MpClient client;
	//log level : ERROR
	//log limit : 1GB
	//log directory : /home/log/
	ret = client.SetLogConfig( mpcomm::ERR, 1<<30, "/home/log/" ); 
	if ( ret ){
		return -1;
	}
	//set time out : in seconds
	//no timeout
	ret = client.SetTimeOut( 0, 0 );
	//set host
	//default is "api.weixin.qq.com"
	ret = client.SetHost( "sz.api.weixin.qq.com" );

	// do some thing
	// ...
	
	MpClient::ShutDown();
}
