<?xml version='1.0' encoding='gb2312'?>
<xsl:stylesheet version='1.0' xmlns:xsl='http://www.w3.org/1999/XSL/Transform'>
	<xsl:import href='misc.xsl' />
	<xsl:output method='html' encoding='gb2312' doctype-public='-//W3C//DTD HTML 4.01//EN' doctype-system='http://www.w3.org/TR/html4/strict.dtd' />
	<xsl:template match='root'>
		<xsl:call-template name='layout'>
			<xsl:with-param name='title'>���������</xsl:with-param>
			<xsl:with-param name='p'><xsl:value-of select='bbs0an/@p' /></xsl:with-param>
			<xsl:with-param name='u'><xsl:value-of select='bbs0an/@u' /></xsl:with-param>
		</xsl:call-template>
	</xsl:template>

	<xsl:template match='bbs0an'>
		<p>��Ŀ¼web���������[<xsl:value-of select='@v' />]</p>
		<table class='content'>
			<tr><th class='no'>���</th><th class='ptitle'>����</th><th class='bm'>������</th><th>����</th></tr>
			<xsl:for-each select='ent'><tr>
					<xsl:attribute name='class'><xsl:choose><xsl:when test='position() mod 2 = 1'>light</xsl:when><xsl:otherwise>dark</xsl:otherwise></xsl:choose></xsl:attribute>
					<td class='no'><xsl:value-of select='position()' /></td>
					<td class='ptitle'><xsl:choose>
						<xsl:when test='@t = "d"'><a class='ptitle'><xsl:attribute name='href'>0an?path=<xsl:value-of select='../@path' /><xsl:value-of select='@path' /></xsl:attribute><img src='../images/types/folder.gif' /><xsl:value-of select='.' /></a></xsl:when>
						<xsl:when test='@t = "f"'><a class='ptitle'><xsl:attribute name='href'>anc?path=<xsl:value-of select='../@path' /><xsl:value-of select='@path' /></xsl:attribute><img src='../images/types/text.gif' /><xsl:value-of select='.' /></a></xsl:when>
						<xsl:otherwise><img src='../images/types/error.gif' /><xsl:value-of select='@t' /></xsl:otherwise>
					</xsl:choose></td>
					<td class='bm'><xsl:if test='@id'>
						<xsl:call-template name='splitbm'>
							<xsl:with-param name='names' select='@id' />
							<xsl:with-param name='isdir' select='0' />
							<xsl:with-param name='isfirst' select='1' />
						</xsl:call-template>
					</xsl:if></td>
					<td class='time'><xsl:if test='@t != "e"'><xsl:call-template name='timeconvert'><xsl:with-param name='time' select='@time' /></xsl:call-template></xsl:if></td>
			</tr></xsl:for-each>
			<xsl:if test='not(ent)'>
				<td /><td width='80%'>&lt;&lt;Ŀǰû������&gt;&gt;</td>
			</xsl:if>
		</table>
		<xsl:if test='@brd'><a><xsl:attribute name='href'>doc?board=<xsl:value-of select='@brd' /></xsl:attribute>[<img src='../images/button/home.gif' />��������]</a></xsl:if>
	</xsl:template>
</xsl:stylesheet>