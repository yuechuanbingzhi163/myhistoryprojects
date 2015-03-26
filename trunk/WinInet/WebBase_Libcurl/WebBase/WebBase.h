#ifndef  _CWEBBASE_DEFINE__
#define  _CWEBBASE_DEFINE__
#include <windows.h>
#include <wininet.h>
#include <string>
#include "ProxySet.h"
#include <io.h>
using namespace std;
#pragma comment(lib,"wininet.lib")


//extern log objLog;

//���巵�صĺ�
#define WM_CONNECT_FAILED -1
#define WM_OPENREQUEST_FAILED -2
#define WM_SENDREQUEST_FAILED -3
#define WM_QUERYINFO_FAILED -4
#define WM_RETCODE_ERROR -5
#define WM_READFILE_ERROR -6
#define WM_GENNEWBUFF_ERROR -7
//�쳣����
#ifdef ERROR_INTERNET_CANNOT_CONNECT
#endif

#define SECURITY_IGNORE_ERROR_MASK  (INTERNET_FLAG_IGNORE_CERT_CN_INVALID   |  \
	INTERNET_FLAG_IGNORE_CERT_DATE_INVALID |  \
	SECURITY_FLAG_IGNORE_UNKNOWN_CA        |  \
	SECURITY_FLAG_IGNORE_REVOCATION    )

class CWebBase : public CProxySet
{
public:	
	CWebBase();
	~CWebBase(void);
	static void WebInit();
	int GetPage(const char* lpszObject,const char* lpszHeader = NULL,const char* lpszServer = NULL,int nPort = 0,bool bIsFile=false,const char* szFilePath=NULL);
	int PostPage(const char* lpszObject,const char* lpszData,const char* lpszHeader = NULL,const char* lpszServer = NULL,int nPort = 0,bool bIsFile=false,const char* szFilePath=NULL);
	int GetData(const char* lpszObject,const char* lpszHeader,const char* lpszServer,int nPort,const char * lpszVerb = "GET",const char* lpszData=NULL,bool bIsFile=false,const char* szFilePath=NULL);
	/************************************************************************/
	/* ���á���ȡ������Ϣ                                                   */
	/************************************************************************/
	const char * GetLastErrorInfo() { return m_strErrorInfo.c_str(); }
	char * GetContentInfo() { return (char *)m_strContent.c_str(); }
	//���ó�ʱʱ��ֵ 
	void SetTimeOutSeconnd( int nTimeOutSecond )  {	m_nTimeOutSecond = nTimeOutSecond; }
	//������HTTPS����HTTP����
	void SetHttpType( int nType = 1 ){ m_nHttpType = nType; }
	//����Ĭ�ϵķ�������ַ
	void SetDefaultServer(const char *lpszServer,const int nPort = 80){ m_strServer = string(lpszServer); m_nPort = nPort;}	
	void SetNoCookie(bool bCookie){m_bNoCookie = bCookie;}
	void SetNotAutoRedirect(bool bNoRedirect){ m_bNoAutoRedirect = bNoRedirect; }
	string GetCookie(){return m_strCookie;}
	inline DWORD GenFlags();
	string GetHeader(){ return m_strHttpHeader; }
private:
	//���ô���
	BOOL SetProxy()
	{
		if (!IsEnableProxy()){	return TRUE; }
		GenProxyParam(m_ProxyParam);
		BOOL bRet = InternetSetOption(m_hInterSessin,INTERNET_OPTION_PROXY,(LPVOID)&m_ProxyParam,sizeof(m_ProxyParam));
		if (!bRet)
		{
			SetLastErrorInfo("INTERNET_OPTION_PROXY Error:%d",GetLastError());
		}
		return bRet;
	}
	void SetLastErrorInfo(const char* szFormat, ...);
	void SetTimeOut( );
	HINTERNET GetHttpConnectionEx(const char * lpszServer,int nPort);
	
	BOOL SendRequestEx(HINTERNET hRequest,const char* lpszHeader,const char* lpszPostData = NULL,bool bPostFlag = false);
	BOOL SetSessionCookie();
	int ConvertData(string strReponseHead,string &strError,char* szData, int nLen);
	void AddCookie(string &strHeader);
private:
	HINTERNET m_hInterSessin;
	string m_strErrorInfo;
	string m_strContent;
	string m_strHttpHeader;
	string m_strCookie;
	int m_nTimeOutSecond;
	int m_nHttpType;	//0��ʾ��HTTPS��1��ʾHTTP
	string m_strServer;	//Ĭ�ϵķ�������ַ
	int m_nPort;	//Ĭ�϶˿�
	INTERNET_PROXY_INFO m_ProxyParam;
	bool m_bNoCookie;
	bool m_bNoAutoRedirect;
	char* m_pDataBuf;
	int m_nDataLen;
	int m_nBufLen;
	int m_nNewTimes;

};

#endif