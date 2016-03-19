<?xml version="1.0" encoding="UTF-8"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform">
	<xsl:template match="/RpcMsgs">
		<html>
			<head>
				<title>GameDB's RPC</title>
			</head>
			<body>
				<xsl:apply-templates select="RpcServerName"/>
			</body>
		</html>
	</xsl:template>
	<xsl:variable name="ucase" select="'ABCDEFGHIJKLMNOPQRSTUVWXYZ'"/>
	<xsl:variable name="lcase" select="'abcdefghijklmnopqrstuvwxyz'"/>
	<xsl:template match="RpcServerName">
		<xsl:variable name="serverName" select="@serverName"/>
		<xsl:variable name="outputPath" select="@outputPath"/>
		<xsl:value-of select="@serverName"/>
		<a>
			<xsl:attribute name="href"><xsl:value-of select="translate(@outputPath, $ucase, $lcase)"/></xsl:attribute>
			<B>
			服务器生成RPC处理文件路径:
			<xsl:value-of select="@outputPath"/>
			</B>
		</a>
		<br/>
		
		<!-->处理Client<-->
		<xsl:for-each select="/RpcMsgs/Rpcs/Rpc">
			<xsl:variable name="rpcName" select="@name"/>
			
			<!-->处理returns<-->
			<xsl:variable name="returns">
				<xsl:for-each select="Return">
					<xsl:for-each select="Param">
						<xsl:value-of select="@type"/><xsl:text> </xsl:text>
						<xsl:value-of select="@name"/><xsl:text> ,</xsl:text>
					</xsl:for-each>
				</xsl:for-each>
			</xsl:variable>
			<!-->处理Call<-->
			<xsl:variable name="params">
				<xsl:for-each select="Call">
					<xsl:for-each select="Param">
						<xsl:value-of select="@type"/><xsl:text> </xsl:text>
						<xsl:value-of select="@name"/><xsl:text> ,</xsl:text>
					</xsl:for-each>
				</xsl:for-each>
			</xsl:variable>
			
			<xsl:for-each select="Targets">
				<xsl:for-each select="Client">
					<xsl:choose>
						<xsl:when test="@name = $serverName">
							<a>
								<xsl:attribute name="href"><xsl:choose><xsl:when test="@class"><xsl:value-of select="$outputPath"/><xsl:value-of select="@class"/>_<xsl:value-of select="$rpcName"/>_Client.cpp
									</xsl:when><xsl:otherwise><xsl:value-of select="$outputPath"/>GlobalRpc_<xsl:value-of select="$rpcName"/>_Client.cpp
									</xsl:otherwise></xsl:choose></xsl:attribute>
								<B>
									<xsl:value-of select="$rpcName"/>
								</B>
							</a>
							<br/>
						</xsl:when>
					</xsl:choose>
				</xsl:for-each>
			</xsl:for-each>
		</xsl:for-each>
		
		<!-->处理Proxy<-->
		<xsl:for-each select="/RpcMsgs/Rpcs/Rpc">
			<xsl:variable name="rpcName" select="@name"/>
			<!-->处理returns<-->
			<xsl:variable name="returns">
				<xsl:for-each select="Return">
					<xsl:for-each select="Param">
						<xsl:value-of select="@type"/><xsl:text> </xsl:text>
						<xsl:value-of select="@name"/><xsl:text> ,</xsl:text>
					</xsl:for-each>
				</xsl:for-each>
			</xsl:variable>
			<!-->处理Call<-->
			<xsl:variable name="params">
				<xsl:for-each select="Call">
					<xsl:for-each select="Param">
						<xsl:value-of select="@type"/><xsl:text> </xsl:text>
						<xsl:value-of select="@name"/><xsl:text> ,</xsl:text>
					</xsl:for-each>
				</xsl:for-each>
			</xsl:variable>
			
			<xsl:for-each select="Targets">
				<xsl:for-each select="Proxy">
					<xsl:choose>
						<xsl:when test="@name = $serverName">
							<a>
								<xsl:attribute name="href"><xsl:choose><xsl:when test="@class"><xsl:value-of select="$outputPath"/><xsl:value-of select="@class"/>_<xsl:value-of select="$rpcName"/>_Proxy.cpp
									</xsl:when><xsl:otherwise><xsl:value-of select="$outputPath"/>GlobalRpc_<xsl:value-of select="$rpcName"/>_Proxy.cpp
									</xsl:otherwise></xsl:choose></xsl:attribute>
								<B>
									<xsl:value-of select="$rpcName"/>
								</B>
							</a>
							<br/>
						</xsl:when>
					</xsl:choose>
				</xsl:for-each>
			</xsl:for-each>
		</xsl:for-each>
		
		<!-->处理Server<-->
		<xsl:for-each select="/RpcMsgs/Rpcs/Rpc">
			<xsl:variable name="rpcName" select="@name"/>
			<!-->处理returns<-->
			<xsl:variable name="returns">
				<xsl:for-each select="Return">
					<xsl:for-each select="Param">
						<xsl:value-of select="@type"/><xsl:text> </xsl:text>
						<xsl:value-of select="@name"/><xsl:text> ,</xsl:text>
					</xsl:for-each>
				</xsl:for-each>
			</xsl:variable>
			<!-->处理Call<-->
			<xsl:variable name="params">
				<xsl:for-each select="Call">
					<xsl:for-each select="Param">
						<xsl:value-of select="@type"/><xsl:text> </xsl:text>
						<xsl:value-of select="@name"/><xsl:text> ,</xsl:text>
					</xsl:for-each>
				</xsl:for-each>
			</xsl:variable>
			<xsl:for-each select="Targets">
				<xsl:for-each select="Server">
					<xsl:choose>
						<xsl:when test="@name = $serverName">
							<a>
								<xsl:attribute name="href"><xsl:choose><xsl:when test="@class"><xsl:value-of select="$outputPath"/><xsl:value-of select="@class"/>_<xsl:value-of select="$rpcName"/>_Server.cpp
									</xsl:when><xsl:otherwise><xsl:value-of select="$outputPath"/>GlobalRpc_<xsl:value-of select="$rpcName"/>_Server.cpp
									</xsl:otherwise></xsl:choose></xsl:attribute>
								<B>
									<xsl:value-of select="$rpcName"/>
								</B>
							</a>
							<br/>
						</xsl:when>
					</xsl:choose>
				</xsl:for-each>
			</xsl:for-each>
		</xsl:for-each>		
		<br/>		
	</xsl:template>
	
</xsl:stylesheet>
