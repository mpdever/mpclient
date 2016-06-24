#include "mpclient.h"
#include "mpcomm.h"

using namespace mpcomm;

int main(){
	mpcomm::TokenReq req;
	mpcomm::TokenResp resp;
	req.set_appid("");
	req.set_secret("");
	int ret = MpClient().Token(req,&resp);
	if ( ret ){
		printf("ERROR errcode %d errmsg %s", ret, mpcomm::errmsg( ret ) );
		//do something
	}else if ( resp.errcode() != 0 ){
		printf("MpClient.Token ret 0 response errcode %ld errmsg %s", resp.errcode(), resp.errmsg().c_str() );
		//do something
	}
	//...
}
