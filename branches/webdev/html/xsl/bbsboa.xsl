<?xml version="1.0" encoding="gb2312"?>
<xsl:stylesheet version="2.0"
xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
xmlns="http://www.w3.org/1999/xhtml">
	<xsl:import href='splitbm.xsl'/>
	<xsl:template match="bbsboa">
		<html>
			<head>
				<title>
				<xsl:choose>
					<xsl:when test='dir'>����Ŀ¼</xsl:when>
					<xsl:otherwise>����������</xsl:otherwise>
				</xsl:choose>
				</title>
				<meta http-equiv="content-type" content="text/html; charset=gb2312" />
				<link rel="stylesheet" type="text/css" href="/css/bbs0.css" />
			</head>
			<body>
				<xsl:choose>
					<xsl:when test='icon'>
						<img align="absmiddle" border="0">
							<xsl:attribute name='src'><xsl:value-of select='icon' /></xsl:attribute>
						</img>
					</xsl:when>
					<xsl:otherwise>
						<h1><xsl:value-of select='title' /></h1>
					</xsl:otherwise>
				</xsl:choose>
				<table width="100%" bgcolor="#ffffff">
					<tr class="pt9h">
						<th>���</th><th>δ��</th><th>������</th><th>����������</th><th>���</th><th>��������</th><th>����</th>
					</tr>
					<xsl:for-each select='board'>
						<xsl:sort select="title" />
						<tr>
							<xsl:attribute name='class'>
								<xsl:if test='position() mod 2 = 1'>pt9lc</xsl:if>
								<xsl:if test='position() mod 2 = 0'>pt9dc</xsl:if>
							</xsl:attribute>
							<!-- No. -->
							<td align="right"><xsl:value-of select='position()' /></td>
							<!-- Unread -->
							<td align="center">
								<xsl:if test="read='0'">��</xsl:if>
								<xsl:if test="read!='0'">��</xsl:if>
							</td>
							<!-- Post count -->
							<td align="right"><xsl:value-of select='count' /></td>
							<!-- English title -->
							<td><strong><a>
								<xsl:if test='@dir="1"'>
									<xsl:attribute name='href'>bbsboa?board=<xsl:value-of select='title' /></xsl:attribute>[ <xsl:value-of select='title' /> ]
								</xsl:if>
								<xsl:if test='@dir="0"'>
									<xsl:attribute name='href'>bbsdoc?board=<xsl:value-of select='title' /></xsl:attribute><xsl:value-of select='title' />
								</xsl:if>
							</a></strong></td>
							<!-- Category -->
							<td align="center">
								<xsl:if test='@dir="1"'><strong>[Ŀ¼]</strong></xsl:if>
								<xsl:if test='@dir="0"'><xsl:value-of select='cate' /></xsl:if>
							</td>
							<!-- Chinese description -->
							<td width="100%"><strong><a>
								<xsl:if test='@dir="1"'>
									<xsl:attribute name='href'>bbsboa?board=<xsl:value-of select='title' /></xsl:attribute><xsl:value-of select='desc' />
								</xsl:if>
								<xsl:if test='@dir="0"'>
									<xsl:attribute name='href'>bbsdoc?board=<xsl:value-of select='title' /></xsl:attribute><xsl:value-of select='desc' />
								</xsl:if>
							</a></strong></td>
							<!-- Board Masters -->
							<td><strong>
								<xsl:call-template name='splitbm'>
									<xsl:with-param name='names' select='bm' />
									<xsl:with-param name='isdir' select='@dir' />
									<xsl:with-param name='isfirst' select='1' />
								</xsl:call-template>
							</strong></td>
						</tr>
					</xsl:for-each>
				</table>
			</body>
		</html>
	</xsl:template>
</xsl:stylesheet>