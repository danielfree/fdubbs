<?xml version="1.0" encoding="gb2312"?>
<xsl:stylesheet version="2.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns="http://www.w3.org/1999/xhtml">
	<xsl:import href='showpost.xsl' />
	<xsl:output method='html' encoding='gb2312' />
	<xsl:template match="bbsmailcon">
		<html>
			<head>
				<title>�ż��Ķ� - ���¹⻪</title>
				<meta http-equiv="content-type" content="text/html; charset=gb2312" />
				<link rel="stylesheet" type="text/css" href="/css/bbs0.css" />
			</head>
			<body>
				<strong>ʹ���� [<xsl:value-of select='user' />]</strong>
				<xsl:call-template name='show-linkbar'></xsl:call-template>
				<div class='article'>	
					<xsl:call-template name='showpost'>
						<xsl:with-param name='content' select='mail' />
					</xsl:call-template>
				</div>
				<xsl:call-template name='show-linkbar'></xsl:call-template>
				<div><a onclick='return confirm("�����Ҫɾ���������")'><xsl:attribute name='href'>delmail?f=<xsl:value-of select='file' /></xsl:attribute>[ ɾ������ ]</a></div>
			</body>
		</html>
	</xsl:template>
	
	<xsl:template name='show-linkbar'>
		<div>
			<xsl:if test='prev'><a><xsl:attribute name='href'>mailcon?f=<xsl:value-of select='prev' /></xsl:attribute>[ <img src='/images/button/up.gif' />��һ�� ]</a></xsl:if>
			<xsl:if test='next'><a><xsl:attribute name='href'>mailcon?f=<xsl:value-of select='next' /></xsl:attribute>[ <img src='/images/button/down.gif' />��һ�� ]</a></xsl:if>
			<a href='mail'>[ <img src='/images/button/back.gif' />���ż��б� ]</a>
		</div>
	</xsl:template>
</xsl:stylesheet>
