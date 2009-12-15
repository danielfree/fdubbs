<?xml version='1.0' encoding='gb2312'?>
<xsl:stylesheet version='1.0' xmlns:xsl='http://www.w3.org/1999/XSL/Transform'>
	<xsl:import href='misc.xsl' />
	<xsl:output method='html' encoding='gb2312' doctype-public='-//W3C//DTD HTML 4.01//EN' doctype-system='http://www.w3.org/TR/html4/strict.dtd' />
	<xsl:template match='/'>
		<xsl:call-template name='layout'>
			<xsl:with-param name='title'>���߹�ע����</xsl:with-param>
			<xsl:with-param name='session'><xsl:value-of select='bbsovr2/@s' /></xsl:with-param>
		</xsl:call-template>
	</xsl:template>

	<xsl:template match='bbsovr2'>
		<table class='content'>
			<tr><th class='no'>���</th><th class='owner'>ʹ���ߴ���</th><th class='idesc'>�ǳ�</th><th>��վλ��</th><th>��̬</th><th>����</th></tr>
			<xsl:for-each select='ov'><xsl:sort select='@id'/><tr>
				<xsl:attribute name='class'><xsl:choose><xsl:when test='position() mod 2 = 1'>light</xsl:when><xsl:otherwise>dark</xsl:otherwise></xsl:choose></xsl:attribute>
				<td class='no'><xsl:value-of select='position()'/></td>
				<td class='owner'><a class='owner'><xsl:attribute name='href'>qry?u=<xsl:value-of select='@id'/></xsl:attribute><xsl:value-of select='@id'/></a></td>
				<td class='idesc'><xsl:value-of select='.'/></td>
				<td><xsl:value-of select='@ip'/></td>
				<td><xsl:value-of select='@action'/></td>
				<td><xsl:if test='@idle &gt; 0'><xsl:value-of select='@idle'/></xsl:if></td>
			</tr></xsl:for-each>
		</table>
	</xsl:template>
</xsl:stylesheet>
