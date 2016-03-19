#include "NetLib/inc/NetCommon.h"

namespace Net
{
	namespace HttpConsts
	{
		extern DLL_EXPORT const char* LF;
		extern DLL_EXPORT const char* LF2X;

		extern DLL_EXPORT const char* METHOD_GET;
		extern DLL_EXPORT const char* METHOD_PUT;
		extern DLL_EXPORT const char* METHOD_POST;

		extern DLL_EXPORT const char* VERSION_10;
		extern DLL_EXPORT const char* VERSION_11;

		extern DLL_EXPORT const char* HEADER_HOST;
		extern DLL_EXPORT const char* HEADER_USER_AGENT;
		extern DLL_EXPORT const char* HEADER_CONTENT_TYPE;
		extern DLL_EXPORT const char* HEADER_CONTENT_LENGTH;
		extern DLL_EXPORT const char* HEADER_CONTENT_ENCODING;

		extern DLL_EXPORT const char* HEADER_ACCEPT;
		extern DLL_EXPORT const char* HEADER_ACCEPT_CHARSET;
		extern DLL_EXPORT const char* HEADER_ACCEPT_ENCODING;
		extern DLL_EXPORT const char* HEADER_ACCEPT_LANGUAGE;

		extern DLL_EXPORT const char* HEADER_COOKIE;
		extern DLL_EXPORT const char* HEADER_CONNECTION;
		extern DLL_EXPORT const char* HEADER_AUTHORIZATION;

		extern DLL_EXPORT const char* VALUE_CONNECTION_KEEP_ALIVE;

		extern DLL_EXPORT const char* HEADER_LOCATION;
		extern DLL_EXPORT const char* HEADER_WWW_AUTHORIZATION;
	}

	class DLL_EXPORT HttpProtocol
	{
	public:
		HttpProtocol();
		virtual ~HttpProtocol();
	public:
		char* GetBuf() const { return m_pBuf; }
		INT32 GetLen() const { return m_nLenUsed; }
		char* GetContent() const { return &m_pBuf[m_nLenUsed - m_nContentLength]; }
		INT32 GetContentLength() const { return m_nContentLength; } 
		BOOL	GetContent(std::map<std::string, std::string> & mapContent);

	public:
		virtual void Clear();
		virtual bool Parse(const char* buf = NULL,INT32 len = 0, INT32* remainLen = NULL);

	public:
		virtual void Reserve(INT32 len);
	public:
		static INT32 GetHeaderLength(char* header);
		static INT32 GetHeaderValueLength(char* header);
	public:
		std::string GetInputUrl();
		std::string GetInputParam();
		std::string GetStatueCode();
		std::string GetMethod();
		std::string GetStatusText();
		std::string GetVersion();
	public:
		void WriteBuffer(const char* buf);
		void WriteBuffer(const char* buf,INT32 len);
		void WriteHeader(const char* name,const char* value);
		void WriteHeader(const char* name,INT32 value);
		void WriteHeaderTerminator();
		void WriteRequestHeader(const char* method,const char* url = "/",const char* version = HttpConsts::VERSION_11);
		void WriteResponseHeader(INT32 statusCode,const char* statusText,const char* version = HttpConsts::VERSION_11);
		void WriteContent(const void* buf,INT32 len);
	protected:
		bool ParseHeader(char* header,INT32 len);
		bool ParseHeader(char* header);
		bool ParseProtocolHeader(char* header,INT32 len);
	protected:
		virtual bool OnHeaderSet(const char* name,char* value);
		virtual void OnBufferRealloced(char* oldbuf,char* newbuf);
	protected:
		char* m_pBuf;
		INT32 m_nLen;
		INT32 m_nLenUsed;
		INT32 m_nContentLength;
		bool  m_bNeedReleaseBuf;
	public:
		enum 
		{
			PARSE_MODE_NONE,
			PARSE_MODE_HEADER,
			PARSE_MODE_CONTENT,
			PARSE_MODE_OK,
		};
		INT32 m_nMode;
		INT32 m_nLastParsePosition;
	public:
		char* m_phStatusCode;
		char* m_phStatusText;
		char* m_phMethod;
		char* m_phUrl;
		char* m_phVersion;
		char* m_phHost;
		char* Host() const { return m_phHost; }
		void Host(char* val) { m_phHost = val; }
		char* m_phCookie;
		char* Cookie() const { return m_phCookie; }
		void Cookie(char* val) { m_phCookie = val; }
		char* m_phConnection;
		char* Connection() const { return m_phConnection; }
		void Connection(char* val) { m_phConnection = val; }
		char* m_phAccept;
		char* Accept() const { return m_phAccept; }
		void Accept(char* val) { m_phAccept = val; }
		char* m_phAcceptEncoding;
		char* AcceptEncoding() const { return m_phAcceptEncoding; }
		void AcceptEncoding(char* val) { m_phAcceptEncoding = val; }
		char* m_phAcceptLanguage;
		char* AcceptLanguage() const { return m_phAcceptLanguage; }
		void AcceptLanguage(char* val) { m_phAcceptLanguage = val; }
		char* m_phAcceptCharset;
		char* AcceptCharset() const { return m_phAcceptCharset; }
		void AcceptCharset(char* val) { m_phAcceptCharset = val; }
		char* m_phContentType;
		char* ContentType() const { return m_phContentType; }
		void ContentType(char* val) { m_phContentType = val; }
		char* m_phContentLength;
		char* ContentLength() const { return m_phContentLength; }
		void ContentLength(char* val) { m_phContentLength = val; }
		char* m_phContentEndcoding;
		char* ContentEndcoding() const { return m_phContentEndcoding; }
		void ContentEndcoding(char* val) { m_phContentEndcoding = val; }
		char* m_phAuthorization;
		char* Authorization() const { return m_phAuthorization; }
		void Authorization(char* val) { m_phAuthorization = val; }
		char* m_phSetCookie;
		char* SetCookie() const { return m_phSetCookie; }
		void SetCookie(char* val) { m_phSetCookie = val; }
		char* m_phReferer;
		char* Referer() const { return m_phReferer; }
		void Referer(char* val) { m_phReferer = val; }
		char* m_phLocation;
		char* Location() const { return m_phLocation; }
		void Location(char* val) { m_phLocation = val; }
		char* m_phWWWAuthorization;
		char* WWWAuthorization() const { return m_phWWWAuthorization; }
		void WWWAuthorization(char* val) { m_phWWWAuthorization = val; }
	};

	class HttpProtocolEx : public HttpProtocol
	{
	protected:
		typedef std::map<std::string,char*> CollectionHeadersT;
	public:
		virtual void Clear() override;
	public:
		char* GetHeader(const std::string& name) 
		{
			CollectionHeadersT::iterator iter = m_mapHeaders.find(name);
			if(iter != m_mapHeaders.end())
				return iter->second;
			return NULL;
		}
	protected:
		virtual bool OnHeaderSet(const char* name,char* value) override;
		virtual void OnBufferRealloced(char* oldbuf,char* newbuf) override;
	protected:
		CollectionHeadersT m_mapHeaders;
	};

	class HttpProtocolFast : public HttpProtocol
	{
	public:
		HttpProtocolFast(char* buf,INT32 len)
		{
			m_pBuf = buf;
			m_nLen = len;
			m_bNeedReleaseBuf = false;
		}
	public:
		virtual void Reserve(INT32 len) override;
	};

	class HttpProtocolUtils
	{
	public:
		static bool IsRedirectCode(INT32 code);
		static bool Parse(const char* iurl,char* host,INT32& port,char* path,char* params);
	};
}
