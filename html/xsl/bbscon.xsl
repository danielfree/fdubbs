<?xml version="1.0" encoding="gb2312"?>
<xsl:stylesheet version="2.0"
xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
xmlns="http://www.w3.org/1999/xhtml">
	<xsl:import href='showpost.xsl' />
	<xsl:output method='html' encoding='gb2312' />
	<xsl:template match="bbscon">
		<html>
			<head>
				<title>�����Ķ� - ���¹⻪</title>
				<meta http-equiv="content-type" content="text/html; charset=gb2312" />
				<link rel="stylesheet" type="text/css" href="/css/bbs0.css" />
			</head>
			<body>
				<xsl:call-template name='show-linkbar'></xsl:call-template>
				<div class='article'>	
					<xsl:call-template name='showpost'>
						<xsl:with-param name='content' select='post' />
					</xsl:call-template>
				</div>
				<xsl:call-template name='show-linkbar'></xsl:call-template>
				<a href='javascript:history.go(-1)'>[ ���� ]</a>
				<a><xsl:attribute name='href'>bbsdoc?bid=<xsl:value-of select='bid' /></xsl:attribute>[ <img src='/images/button/home.gif' />�������� ]</a>
				<a><xsl:attribute name='href'>bbspst?bid=<xsl:value-of select='bid' />&amp;f=<xsl:value-of select='f' /></xsl:attribute>[ <img src='/images/button/edit.gif' />�ظ����� ]</a>
			</body>
		</html>
	</xsl:template>
	
	<xsl:template name='show-linkbar'>
		<div>
			<xsl:variable name='baseurl'>bbscon?bid=<xsl:value-of select='bid' />&amp;f=<xsl:value-of select='f' />&amp;a=</xsl:variable>
			<a><xsl:attribute name='href'><xsl:value-of select='$baseurl' />p</xsl:attribute>[ <img src='/images/button/up.gif' />��һƪ ]</a>
			<a><xsl:attribute name='href'><xsl:value-of select='$baseurl' />n</xsl:attribute>[ <img src='/images/button/down.gif' />��һƪ ]</a>
			<xsl:if test='reid != f'><a><xsl:attribute name='href'><xsl:value-of select='$baseurl' />b</xsl:attribute>[ ͬ������ƪ ]</a></xsl:if>
			<a><xsl:attribute name='href'><xsl:value-of select='$baseurl' />a</xsl:attribute>[ ͬ������ƪ ]</a>
			<xsl:if test='gid'><a><xsl:attribute name='href'>bbscon?bid=<xsl:value-of select='bid' />&amp;f=<xsl:value-of select='gid' /></xsl:attribute>[ ͬ�����һƪ ]</a></xsl:if>
		</div>
	</xsl:template>
</xsl:stylesheet>