<?xml version="1.0" encoding="gb2312"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns="http://www.w3.org/1999/xhtml">
	<xsl:output method='html' encoding='gb2312' />
	<xsl:template match="bbsmybrd">
		<html>
			<head>
				<title>�ղؼ��趨 - ���¹⻪</title>
				<meta http-equiv="content-type" content="text/html; charset=gb2312" />
				<link rel="stylesheet" type="text/css" href="/css/bbs0.css" />
				<xsl:if test='selected'><script>top.f2.location='left'</script></xsl:if>
			</head>
			<body>
				<xsl:choose>
					<xsl:when test='selected'>
						<div>�޸�Ԥ���������ɹ���������һ��Ԥ���� <xsl:value-of select='selected'/> ��������</div>
						<a href='javascript:history.go(-2)'>����</a>
					</xsl:when>
					<xsl:otherwise>
						<form action='mybrd?type=1' method='post'>
							<table>
								<xsl:for-each select='b'>
									<xsl:if test='position() mod 3 = 1'>
										<tr><xsl:apply-templates select='.'/><xsl:apply-templates select='following-sibling::b[1]'/><xsl:apply-templates select='following-sibling::b[2]'/></tr>
									</xsl:if>
								</xsl:for-each>
							</table>
							<input type='submit' value='ȷ��Ԥ��'/><input type='reset' value='��ԭ'/>
						</form>
					</xsl:otherwise>
				</xsl:choose>
			</body>
		</html>
	</xsl:template>

	<xsl:template match='bbsbrdadd'>
		<html>
			<head>
				<title>�ղؼ��趨 - ���¹⻪</title>
				<meta http-equiv="content-type" content="text/html; charset=gb2312" />
				<script>top.f2.location='left'</script>
			</head>
			<body><p>�ղؼ���Ӱ��� <a><xsl:attribute name='href'>doc?bid=<xsl:value-of select='bid' /></xsl:attribute><xsl:value-of select='brd' /></a> �ɹ�</p><a href='javascript:history.go(-1)'>���ٷ���</a></body>
		</html>
	</xsl:template>

	<xsl:template match='b'>
		<xsl:variable name='check'><xsl:call-template name='is-mybrd'><xsl:with-param name='bid' select='@bid'/></xsl:call-template></xsl:variable>
		<td>
			<input type='checkbox'>
				<xsl:attribute name='name'><xsl:value-of select='@bid'/></xsl:attribute>
				<xsl:if test='$check = 1'><xsl:attribute name='checked'>checked</xsl:attribute></xsl:if>
			</input>
			<a><xsl:attribute name='href'>doc?bid=<xsl:value-of select='@bid'/></xsl:attribute><xsl:value-of select='@desc'/></a>
		</td>
	</xsl:template>
	
	<xsl:template name='is-mybrd'>
		<xsl:param name='bid'/>
		<xsl:for-each select='/bbsmybrd/my'><xsl:if test='@bid = $bid'>1</xsl:if></xsl:for-each>
	</xsl:template>

</xsl:stylesheet>