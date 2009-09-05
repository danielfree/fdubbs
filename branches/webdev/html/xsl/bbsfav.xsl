<?xml version='1.0' encoding='gb2312'?>
<xsl:stylesheet version='1.0' xmlns:xsl='http://www.w3.org/1999/XSL/Transform'>
	<xsl:import href='misc.xsl' />
	<xsl:output method='html' encoding='gb2312' doctype-public='-//W3C//DTD HTML 4.01//EN' doctype-system='http://www.w3.org/TR/html4/strict.dtd' />
	<xsl:template match='/'>
		<xsl:call-template name='layout'>
			<xsl:with-param name='title'>�ղؼ�</xsl:with-param>
			<xsl:with-param name='p'><xsl:value-of select='bbsfav/@p' /></xsl:with-param>
			<xsl:with-param name='u'><xsl:value-of select='bbsfav/@u' /></xsl:with-param>
		</xsl:call-template>
	</xsl:template>

	<xsl:template match='bbsfav'>
		<h2>�ҵ��ղؼ�</h2>
		<p><a href='mybrd'>�Զ���</a></p>
		<table class='content'>
			<tr><th class='no'>���</th><th class='title'>����������</th><th class='desc'>��������</th></tr>
			<xsl:for-each select='brd'><tr>
				<xsl:attribute name='class'><xsl:choose><xsl:when test='position() mod 2 = 1'>light</xsl:when><xsl:otherwise>dark</xsl:otherwise></xsl:choose></xsl:attribute>
				<td class='no'><xsl:value-of select='position()' /></td>
				<td class='title'><a class='title'><xsl:attribute name='href'>doc?bid=<xsl:value-of select='@bid' /></xsl:attribute><xsl:value-of select='@brd' /></a></td>
				<td class='desc'><a class='desc'><xsl:attribute name='href'>doc?bid=<xsl:value-of select='@bid' /></xsl:attribute><xsl:value-of select='.' /></a></td>
			</tr></xsl:for-each>
		</table>
	</xsl:template>
</xsl:stylesheet>