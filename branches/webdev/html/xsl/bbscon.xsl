<?xml version='1.0' encoding='gb2312'?>
<xsl:stylesheet version='1.0' xmlns:xsl='http://www.w3.org/1999/XSL/Transform'>
	<xsl:import href='misc.xsl' />
	<xsl:import href='showpost.xsl' />
	<xsl:output method='html' encoding='gb2312' doctype-public='-//W3C//DTD HTML 4.01//EN' doctype-system='http://www.w3.org/TR/html4/strict.dtd' />
	<xsl:template match='/'>
		<xsl:call-template name='layout'>
			<xsl:with-param name='title'>�����Ķ�</xsl:with-param>
			<xsl:with-param name='session'><xsl:value-of select='bbscon/@s' /></xsl:with-param>
		</xsl:call-template>
	</xsl:template>

	<xsl:template match='bbscon'>
		<table class='post'><tr>
			<td class='pleft' rowspan='2'>
				<xsl:if test='@link != "con"'><a><xsl:attribute name='href'>gdoc?bid=<xsl:value-of select='@bid' /></xsl:attribute>[��ժ��]</a></xsl:if>
				<a><xsl:attribute name='href'>doc?bid=<xsl:value-of select='@bid' /></xsl:attribute>[<img src='../images/button/home.gif' />��������]</a>
				<a><xsl:attribute name='href'>con?bid=<xsl:value-of select='@bid' />&amp;f=<xsl:value-of select='po/@fid' /></xsl:attribute>[��������]</a>
				<xsl:variable name='baseurl'>con?bid=<xsl:value-of select='@bid' />&amp;f=<xsl:value-of select='po/@fid' />&amp;a=</xsl:variable>
				<a><xsl:attribute name='href'><xsl:value-of select='$baseurl' />p</xsl:attribute>[<img src='../images/button/up.gif' />��һƪ]</a>
				<a><xsl:attribute name='href'><xsl:value-of select='$baseurl' />n</xsl:attribute>[<img src='../images/button/down.gif' />��һƪ]</a>
				<xsl:if test='po/@reid != f'><a><xsl:attribute name='href'><xsl:value-of select='$baseurl' />b</xsl:attribute>[ͬ������ƪ]</a></xsl:if>
				<a><xsl:attribute name='href'><xsl:value-of select='$baseurl' />a</xsl:attribute>[ͬ������ƪ]</a>
				<xsl:if test='po/@gid'><a><xsl:attribute name='href'>con?bid=<xsl:value-of select='@bid' />&amp;f=<xsl:value-of select='po/@gid' /></xsl:attribute>[ͬ������ƪ]</a></xsl:if>
				<xsl:variable name='gid'><xsl:choose><xsl:when test='po/@gid'><xsl:value-of select='po/@gid' /></xsl:when><xsl:otherwise><xsl:value-of select='po/@fid' /></xsl:otherwise></xsl:choose></xsl:variable>
				<a><xsl:attribute name='href'>tcon?bid=<xsl:value-of select='@bid' />&amp;f=<xsl:value-of select='$gid' /></xsl:attribute>[չ������]</a>
				<a><xsl:attribute name='href'>tcon?bid=<xsl:value-of select='@bid' />&amp;g=<xsl:value-of select='$gid' />&amp;f=<xsl:value-of select='po/@fid' />&amp;a=n</xsl:attribute>[���չ��]</a>
			</td>
			<td class='pmtop'><xsl:call-template name='linkbar' /></td></tr>
			<tr><td class='pmain'><xsl:call-template name='showpost'><xsl:with-param name='content' select='po' /></xsl:call-template></td></tr>
			<tr><td class='plbot'><a href='#top'>[ <img src='../images/button/up.gif' />��ҳ�� ]</a></td><td class='pmbot'><xsl:call-template name='linkbar' /></td></tr>
		</table>
	</xsl:template>
	
	<xsl:template name='linkbar'>
		<xsl:variable name='param'>bid=<xsl:value-of select='@bid' />&amp;f=<xsl:value-of select='po/@fid' /></xsl:variable>
		<xsl:if test='@link = "con"'><a><xsl:attribute name='href'>pst?<xsl:value-of select='$param'/></xsl:attribute>[ <img src='../images/button/edit.gif' />�ظ����� ]</a></xsl:if>
		<a><xsl:attribute name='href'>edit?<xsl:value-of select='$param'/></xsl:attribute>[ �޸� ]</a>
		<a><xsl:attribute name='href'>ccc?<xsl:value-of select='$param'/></xsl:attribute>[ ת�� ]</a>
		<a><xsl:attribute name='href'>fwd?<xsl:value-of select='$param'/></xsl:attribute>[ ת�� ]</a>
		<a><xsl:attribute name='href'>del?<xsl:value-of select='$param'/></xsl:attribute>[ ɾ�� ]</a>
	</xsl:template>
</xsl:stylesheet>
