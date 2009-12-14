<?xml version='1.0' encoding='gb2312'?>
<xsl:stylesheet version='1.0' xmlns:xsl='http://www.w3.org/1999/XSL/Transform'>
	<xsl:import href='misc.xsl' />
	<xsl:import href='showpost.xsl' />
	<xsl:output method='html' encoding='gb2312' doctype-public='-//W3C//DTD HTML 4.01//EN' doctype-system='http://www.w3.org/TR/html4/strict.dtd' />
	<xsl:template match='/'>
		<xsl:call-template name='layout'>
			<xsl:with-param name='title'>�ż��Ķ�</xsl:with-param>
			<xsl:with-param name='session'><xsl:value-of select='bbsmailcon/@s' /></xsl:with-param>
		</xsl:call-template>
	</xsl:template>

	<xsl:template match='bbsmailcon'>
		<table class='post'><tr>
			<td class='pleft' rowspan='3'>
				<a href='mail'>[ <img src='../images/button/back.gif' />�ż��б� ]</a>
				<xsl:if test='@prev'><a><xsl:attribute name='href'>mailcon?f=<xsl:value-of select='@prev' />&amp;n=<xsl:value-of select='mail/@n - 1'/></xsl:attribute>[ <img src='../images/button/up.gif' />��һ�� ]</a></xsl:if>
				<xsl:if test='@next'><a><xsl:attribute name='href'>mailcon?f=<xsl:value-of select='@next' />&amp;n=<xsl:value-of select='mail/@n+1'/></xsl:attribute>[ <img src='../images/button/down.gif' />��һ�� ]</a></xsl:if>
			</td>
			<td class='pmtop'><xsl:call-template name='linkbar' /></td></tr>
			<tr><td class='pmain'><xsl:call-template name='showpost'><xsl:with-param name='content' select='mail' /></xsl:call-template></td></tr>
			<tr><td class='pmbot'><xsl:call-template name='linkbar' /></td></tr>
		</table>
	</xsl:template>
	
	<xsl:template name='linkbar'>
		<a><xsl:attribute name='href'>pstmail?n=<xsl:value-of select='mail/@n'/></xsl:attribute>[ <img src='../images/button/edit.gif' />�ظ����� ]</a>
		<a onclick='return confirm("�����Ҫɾ���������")'><xsl:attribute name='href'>delmail?f=<xsl:value-of select='mail/@f' /></xsl:attribute>[ ɾ������ ]</a>
	</xsl:template>
</xsl:stylesheet>
