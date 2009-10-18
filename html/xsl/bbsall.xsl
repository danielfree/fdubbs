<?xml version='1.0' encoding='gb2312'?>
<xsl:stylesheet version='1.0' xmlns:xsl='http://www.w3.org/1999/XSL/Transform'>
	<xsl:import href='misc.xsl' />
	<xsl:output method='html' encoding='gb2312' doctype-public='-//W3C//DTD HTML 4.01//EN' doctype-system='http://www.w3.org/TR/html4/strict.dtd' />
	<xsl:template match='/'>
		<xsl:call-template name='layout'>
			<xsl:with-param name='title'>ȫ��������</xsl:with-param>
			<xsl:with-param name='session'><xsl:value-of select='bbsall/@s' /></xsl:with-param>
		</xsl:call-template>
	</xsl:template>

	<xsl:template match='bbsall'>
		<h3>[��������: <xsl:value-of select="count(brd)" />]</h3>
		<table class='content'>
			<tr><th class='no'>���</th><th class='title'>����������</th><th class='cate'>���</th><th class='desc'>��������</th><th class='bm'>����</th></tr>
			<xsl:for-each select='brd'>
				<xsl:sort select="@title" />
				<tr>
					<xsl:attribute name='class'>
						<xsl:if test='position() mod 2 = 1'>light</xsl:if>
						<xsl:if test='position() mod 2 = 0'>dark</xsl:if>
					</xsl:attribute>
					<td class='no'><xsl:value-of select='position()' /></td>
					<td class='title'><a class='title'><xsl:choose>
						<xsl:when test='@dir="1"'><xsl:attribute name='href'>boa?board=<xsl:value-of select='@title' /></xsl:attribute>[ <xsl:value-of select='@title' /> ]</xsl:when>
						<xsl:otherwise><xsl:attribute name='href'>doc?board=<xsl:value-of select='@title' /></xsl:attribute><xsl:value-of select='@title' /></xsl:otherwise>
					</xsl:choose></a></td>
					<td class='cate'><xsl:choose>
						<xsl:when test='@dir="1"'>[Ŀ¼]</xsl:when>
						<xsl:otherwise><xsl:value-of select='@cate' /></xsl:otherwise>
					</xsl:choose></td>
					<td class='desc'><a class='desc'><xsl:choose>
						<xsl:when test='@dir="1"'><xsl:attribute name='href'>boa?board=<xsl:value-of select='@title' /></xsl:attribute><xsl:value-of select='@desc' /></xsl:when>
						<xsl:otherwise><xsl:attribute name='href'>doc?board=<xsl:value-of select='@title' /></xsl:attribute><xsl:value-of select='@desc' /></xsl:otherwise>
					</xsl:choose></a></td>
					<td class='bm'>
						<xsl:call-template name='splitbm'>
							<xsl:with-param name='names' select='@bm' />
							<xsl:with-param name='isdir' select='@dir' />
							<xsl:with-param name='isfirst' select='1' />
						</xsl:call-template>
					</td>
				</tr>
			</xsl:for-each>
		</table>
	</xsl:template>
</xsl:stylesheet>
