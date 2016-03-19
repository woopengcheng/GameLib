#include "NetLib/inc/HttpProtocol.h"
#include "gtest/gtest.h"

TEST(HttpProtocol , Test)
{
	INT32 remain = 0;
	Net::HttpProtocol p;

	const char* s1 = "GET / HTTP/1.1" "\r\n" "Accept: */*" "\r\n";
	p.Parse(s1, (INT32)strlen(s1), &remain);
	Assert(p.m_nMode == Net::HttpProtocol::PARSE_MODE_HEADER);

	const char* s11 = "Connection: Keep-Alive" "\r\n";
	p.Parse(s11, (INT32)strlen(s11), &remain);
	Assert(p.m_nMode == Net::HttpProtocol::PARSE_MODE_HEADER);

	const char* s2 = "Content-Length: 10" "\r\n\r\n";
	p.Parse(s2, (INT32)strlen(s2), &remain);
	Assert(p.m_nMode == Net::HttpProtocol::PARSE_MODE_CONTENT);

	const char* s3 = "abcdef";
	p.Parse(s3, (INT32)strlen(s3), &remain);

	const char* s33 = "ab";
	p.Parse(s33, (INT32)strlen(s33), &remain);
	Assert(p.m_nMode == Net::HttpProtocol::PARSE_MODE_CONTENT);

	const char* s4 = "1234567890";
	p.Parse(s4, (INT32)strlen(s4), &remain);
	Assert(p.m_nMode == Net::HttpProtocol::PARSE_MODE_OK);
}