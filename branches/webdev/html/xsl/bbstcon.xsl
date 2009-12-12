<?xml version='1.0' encoding='gb2312'?>
<xsl:stylesheet version='1.0' xmlns:xsl='http://www.w3.org/1999/XSL/Transform'>
	<xsl:import href='misc.xsl' />
	<xsl:import href='showpost.xsl' />
	<xsl:output method='html' encoding='gb2312' doctype-public='-//W3C//DTD HTML 4.01//EN' doctype-system='http://www.w3.org/TR/html4/strict.dtd' />
	<xsl:template match='/'>
		<xsl:call-template name='layout'>
			<xsl:with-param name='title'>ͬ���������Ķ�</xsl:with-param>
			<xsl:with-param name='session'><xsl:value-of select='bbstcon/@s' /></xsl:with-param>
		</xsl:call-template>
	</xsl:template>

	<xsl:template match='bbstcon'>
		<a name='top' />
		<xsl:for-each select='po'>
			<table class='post'><tr>
				<td class='pleft' rowspan='3'>
					<a href='#top'>[ <img src='../images/button/up.gif' />��ҳ�� ]</a>
					<a><xsl:attribute name='href'>gdoc?bid=<xsl:value-of select='../@bid' /></xsl:attribute>[ ��ժ�� ]</a>
					<a><xsl:attribute name='href'>tdoc?bid=<xsl:value-of select='../@bid' /></xsl:attribute>[ <img src='../images/button/home.gif' />�������� ]</a>
					<a><xsl:attribute name='href'>con?bid=<xsl:value-of select='../@bid' />&amp;f=<xsl:value-of select='@fid' /></xsl:attribute>[ <img src='../images/button/content.gif' />�������� ]</a>
					<xsl:variable name='first'><xsl:value-of select='../po[1]/@fid' /></xsl:variable>
					<xsl:variable name='last'><xsl:value-of select='../po[last()]/@fid' /></xsl:variable>
					<xsl:if test='count(../po) = ../@page'><a><xsl:attribute name='href'>tcon?bid=<xsl:value-of select='../@bid' />&amp;g=<xsl:value-of select='../@gid' />&amp;f=<xsl:value-of select='$last' />&amp;a=n</xsl:attribute>[<img src='../images/button/down.gif' />��һҳ ]</a></xsl:if>
					<xsl:if test='$first != ../@gid'><a><xsl:attribute name='href'>tcon?bid=<xsl:value-of select='../@bid' />&amp;g=<xsl:value-of select='../@gid' />&amp;f=<xsl:value-of select='$first' />&amp;a=p</xsl:attribute>[<img src='../images/button/up.gif' />��һҳ ]</a></xsl:if>
				</td>
				<td class='pmtop'><xsl:call-template name='linkbar' /></td></tr>
				<tr><td class='pmain'><xsl:call-template name='showpost'><xsl:with-param name='content' select='.' /></xsl:call-template></td></tr>
				<tr><td class='pmbot'><xsl:call-template name='linkbar' /></td></tr>
			</table>
		</xsl:for-each>
	</xsl:template>
	
	<xsl:template name='linkbar'>
		<a><xsl:attribute name='href'>pst?bid=<xsl:value-of select='../@bid' />&amp;f=<xsl:value-of select='@fid' /></xsl:attribute>[ <img src='../images/button/edit.gif' />�ظ����� ]</a>
		<a><xsl:attribute name='href'>ccc?bid=<xsl:value-of select='../@bid' />&amp;f=<xsl:value-of select='@fid' /></xsl:attribute>[ ת�� ]</a>
		<a><xsl:attribute name='href'>qry?u=<xsl:value-of select='@owner' /></xsl:attribute>[ ��ƪ����: <xsl:value-of select='@owner' /> ]</a>
	</xsl:template>
</xsl:stylesheet>
