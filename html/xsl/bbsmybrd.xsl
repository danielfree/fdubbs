<?xml version="1.0" encoding="gb2312"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns="http://www.w3.org/1999/xhtml">
	<xsl:import href='misc.xsl' />
	<xsl:output method='html' encoding='gb2312' />
	<xsl:template match="bbsmybrd">
		<html>
			<head>
				<title>�ղؼ��趨 - ���¹⻪</title>
				<meta http-equiv='content-type' content='text/html; charset=gb2312' />
				<link rel='stylesheet' type='text/css' href='/css/bbs.css' />
			</head>
			<body><div id='wrap'>
				<xsl:call-template name='navgation-bar'><xsl:with-param name='perm' select='@p' /></xsl:call-template>
				<div id='main'>
					<h2>�ղؼ��趨</h2>
					<xsl:choose>
						<xsl:when test='selected'>
							<div>�޸�Ԥ���������ɹ���������һ��Ԥ���� <xsl:value-of select='selected'/> ��������</div>
							<a href='javascript:history.go(-2)'>����</a>
						</xsl:when>
						<xsl:otherwise>
							<form action='mybrd?type=1' method='post'>
								<table class='content'>
									<xsl:for-each select='b'>
										<xsl:if test='position() mod 3 = 1'><tr>
											<xsl:apply-templates select='.'/><xsl:apply-templates select='following-sibling::b[1]'/><xsl:apply-templates select='following-sibling::b[2]'/>
										</tr></xsl:if>
									</xsl:for-each>
								</table>
								<input type='submit' value='ȷ��Ԥ��'/><input type='reset' value='��ԭ'/>
							</form>
						</xsl:otherwise>
					</xsl:choose>
				</div>
			</div></body>
		</html>
	</xsl:template>

	<xsl:template match='bbsbrdadd'>
		<html>
			<head>
				<title>�ղؼ��趨 - ���¹⻪</title>
				<meta http-equiv="content-type" content="text/html; charset=gb2312" />
			</head>
			<body><p>�ղؼ���Ӱ��� <a><xsl:attribute name='href'>doc?bid=<xsl:value-of select='bid' /></xsl:attribute><xsl:value-of select='brd' /></a> �ɹ�</p><a href='javascript:history.go(-1)'>���ٷ���</a></body>
		</html>
	</xsl:template>

	<xsl:template match='b'>
		<xsl:variable name='check'><xsl:call-template name='is-mybrd'><xsl:with-param name='bid' select='@bid'/></xsl:call-template></xsl:variable>
		<td class='idesc'>
			<input type='checkbox'>
				<xsl:attribute name='name'><xsl:value-of select='@bid'/></xsl:attribute>
				<xsl:if test='$check = 1'><xsl:attribute name='checked'>checked</xsl:attribute></xsl:if>
			</input>
			<a class='idesc'><xsl:attribute name='href'>doc?bid=<xsl:value-of select='@bid'/></xsl:attribute><xsl:value-of select='@desc'/></a>
		</td>
	</xsl:template>
	
	<xsl:template name='is-mybrd'>
		<xsl:param name='bid'/>
		<xsl:for-each select='/bbsmybrd/my'><xsl:if test='@bid = $bid'>1</xsl:if></xsl:for-each>
	</xsl:template>

</xsl:stylesheet>