<?xml version="1.0" encoding="gb2312"?>
<xsl:stylesheet version="2.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns="http://www.w3.org/1999/xhtml">
	<xsl:import href='misc.xsl'/>
	<xsl:output method='html' encoding='gb2312' />
	<xsl:template match="bbsqry">
		<html>
			<head>
				<title>��ѯ���� - ���¹⻪BBS</title>
				<meta http-equiv="content-type" content="text/html; charset=gb2312" />
				<link rel="stylesheet" type="text/css" href="/css/bbs0.css" />
			</head>
			<body>
				<span><xsl:value-of select='id' /> (<xsl:value-of select='nick' />) ����վ </span><span class='ansi132'><xsl:value-of select='login' /></span><span> ��  </span>
				<xsl:call-template name='show-horo'></xsl:call-template><br />
				<span>�ϴ���:[</span><span class='ansi132'><xsl:value-of select='lastlogin' /></span>] �� [<span class='ansi132'><xsl:value-of select='ip' /></span>] ����վһ�Ρ�<br />
				<span>����ֵ:[</span><span class='ansi133'>�Ҵҹ���</span>]<br />
				<span>������:[</span><span class='ansi132'><xsl:value-of select='post' /></span>] ����ֵ:[<xsl:call-template name="showexp"><xsl:with-param name="count" select="repeat" /></xsl:call-template>] ������:[<span class='ansi132'><xsl:value-of select='hp' /></span>]<br />
			</body>
		</html>
	</xsl:template>
	
	<xsl:template name='show-horo'>
		<xsl:if test='horo'>
			<xsl:variable name='color'>
				<xsl:choose>
					<xsl:when test='gender = "M"'>ansi136</xsl:when>
					<xsl:when test='gender = "F"'>ansi135</xsl:when>
					<xsl:otherwise>ansi132</xsl:otherwise>			
				</xsl:choose>
			</xsl:variable>
			<span>[</span>
			<span>
				<xsl:attribute name='class'><xsl:value-of select='$color' /></xsl:attribute><xsl:value-of select='horo' />
			</span>
			<span>]</span>
		</xsl:if>
	</xsl:template>
</xsl:stylesheet>