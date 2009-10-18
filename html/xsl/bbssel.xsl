<?xml version='1.0' encoding='gb2312'?>
<xsl:stylesheet version='1.0' xmlns:xsl='http://www.w3.org/1999/XSL/Transform'>
	<xsl:import href='misc.xsl' />
	<xsl:output method='html' encoding='gb2312' doctype-public='-//W3C//DTD HTML 4.01//EN' doctype-system='http://www.w3.org/TR/html4/strict.dtd' />
	<xsl:template match='/'>
		<xsl:call-template name='layout'>
			<xsl:with-param name='title'>ѡ��������</xsl:with-param>
			<xsl:with-param name='p'><xsl:value-of select='bbssel/@p' /></xsl:with-param>
			<xsl:with-param name='u'><xsl:value-of select='bbssel/@u' /></xsl:with-param>
		</xsl:call-template>
	</xsl:template>

	<xsl:template match='bbssel'>
		<fieldset><legend>ѡ��������</legend><form action='sel' method='get'>
			<label for='brd'>���������ƣ�</label>
			<input type='text' name='brd' size='20' maxlength='20'/><br/>
			<input type='submit' value='�ύ��ѯ'/>
		</form></fieldset>
		<xsl:if test='count(brd)!=0'>
			<p>������: <xsl:value-of select='count(brd)' />������</p>
			<table class='content'>
				<tr><th class='title'>����������</th><th class='desc'>��������</th></tr>
				<xsl:for-each select='brd'><xsl:sort select="@title" /><tr>
					<xsl:attribute name='class'>
						<xsl:if test='position() mod 2 = 1'>light</xsl:if>
						<xsl:if test='position() mod 2 = 0'>dark</xsl:if>
					</xsl:attribute>
					<td class='title'><a class='title'><xsl:choose>
						<xsl:when test='@dir="1"'><xsl:attribute name='href'>boa?board=<xsl:value-of select='@title' /></xsl:attribute>[ <xsl:value-of select='@title' /> ]</xsl:when>
						<xsl:otherwise><xsl:attribute name='href'>doc?board=<xsl:value-of select='@title' /></xsl:attribute><xsl:value-of select='@title' /></xsl:otherwise>
					</xsl:choose></a></td>
					<td class='desc'><a class='desc'><xsl:choose>
						<xsl:when test='@dir="1"'><xsl:attribute name='href'>boa?board=<xsl:value-of select='@title' /></xsl:attribute><xsl:value-of select='@desc' /></xsl:when>
						<xsl:otherwise><xsl:attribute name='href'>doc?board=<xsl:value-of select='@title' /></xsl:attribute><xsl:value-of select='@desc' /></xsl:otherwise>
					</xsl:choose></a></td>
				</tr></xsl:for-each>
			</table>
		</xsl:if>
		<xsl:if test='notfound'>û���ҵ����������İ���</xsl:if>
	</xsl:template>
</xsl:stylesheet>