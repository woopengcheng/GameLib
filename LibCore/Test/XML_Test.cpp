#include "XMLLib/inc/xml.h"
#include "LogLib/inc/Log.h"
#include "CUtil/inc/CUtil.h" 
#include "gtest/gtest.h"

static std::string g_strStream = R"(<?xml version="1.0"?>
<RemoteRPC>
	<RPCServer>
		<ListenType>tcp</ListenType>
		<ListenAddress>127.0.0.1</ListenAddress>
		<ListenPort>8003</ListenPort>
	</RPCServer>
</RemoteRPC>
)";

TEST(XML , XML_Test)
{
	XML::XML xml;
	xml.LoadFromString(g_strStream);

	EXPECT_EQ(xml.GetXMLValue("/RemoteRPC/RPCServer/ListenType"), "tcp");
	EXPECT_EQ(xml.GetXMLValue("/RemoteRPC/RPCServer/ListenAddress"), "127.0.0.1");
	EXPECT_EQ(xml.GetXMLValue("/RemoteRPC/RPCServer/ListenPort"), "8003");

}
