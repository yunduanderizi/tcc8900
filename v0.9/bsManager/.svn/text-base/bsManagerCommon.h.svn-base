/**
 * @file   bsManagerCommon.h
 * @author chentao <chentao01@chinatsp.com>
 * @date  20110124
 * 
 * @brief  
 *      bsManager module's common header 
 * 
 */
#ifndef BSMANAGERCOMMON_H_20110124
#define BSMANAGERCOMMON_H_20110124

#include "debugMonitor.h"
#include <QMutex>



#define ABS(a, b)				((a)>(b)? ((a)-(b)):((b)-(a)))

#define	BK_TUID_LENGTH_MAX			40
#define	BK_UUID_LENGTH_MAX			20
#define	BK_PASSWORD_LENGTH_MAX		20
#define	BK_TOKEN_LENGTH_MAX			50

#define URL_MAX_LENGTH		256

extern "C" void initBsManager();
extern "C" void destroyBsManager();

typedef enum _Ucs_Ap_Id
{
	Ap_Weather = 0,
	Ap_Stock,
	Ap_TrafficNotice,
	Ap_PeccancyHint,
	Ap_News,
	Ap_telephone,
	Ap_EBook,
	Ap_RoadBook,
	Ap_Email,
	Ap_Upgrade,
	Ap_Max
}Ucs_Ap_Id;

class BsBaseC
{

public:

        int getUcsApiHost(char *pHost, const char *pUuid=NULL);
        int getApApiHost(Ucs_Ap_Id apId, char* pApHost);


protected:
	BsBaseC();
	~BsBaseC();

	void stopHttp();

	void urlEncode(unsigned char *src, unsigned char *dst);

	int getTuid(char *tuid);
	int getLoginResult(char *tuid, char *uuid, char *password, char *accessToken, bool &bLogin, bool &bSavePassword);
	int saveLoginResult(const char *tuid, const char *uuid, const char *password, const char *accessToken, bool bLogin, bool bSavePassword);
	void trimStr(char* p_str);

private:
	int _getUuid(char *uuid);
	int _getUcsApiHostInside(const char *uuid, char *host);
	int _getApApiHostInside(const char *uuid, const char *pApIdStr, const char *tuid, const char *token, const char* pHost, char* pApHost);

	volatile bool m_bExitHttp;
	
//	void* operator new(size_t size){return malloc(size);}
//	void operator delete(void* pp){free(pp);}
};

#endif


