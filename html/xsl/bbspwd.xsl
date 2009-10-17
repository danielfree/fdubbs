<?xml version='1.0' encoding='gb2312'?>
<xsl:stylesheet version='1.0' xmlns:xsl='http://www.w3.org/1999/XSL/Transform'>
	<xsl:import href='misc.xsl' />
	<xsl:output method='html' encoding='gb2312' doctype-public='-//W3C//DTD HTML 4.01//EN' doctype-system='http://www.w3.org/TR/html4/strict.dtd' />
	<xsl:template match='/'>
		<xsl:call-template name='layout'>
			<xsl:with-param name='title'>�޸�����</xsl:with-param>
			<xsl:with-param name='p'><xsl:value-of select='bbspwd/@p' /></xsl:with-param>
			<xsl:with-param name='u'><xsl:value-of select='bbspwd/@u' /></xsl:with-param>
		</xsl:call-template>
	</xsl:template>

	<xsl:template match='bbspwd'>
		<xsl:choose>
			<xsl:when test='@i'><form action='pwd' method='post'>
				<p><label for='pw1'>���ľ�����: </label><input maxlength='12' size='12' type='password' name='pw1' /></p>
				<p><label for='pw2'>����������: </label><input maxlength='12' size='12' type='password' name='pw2' /></p>
				<p><label for='pw3'>ȷ��������: </label><input maxlength='12' size='12' type='password' name='pw3' /></p>
				<input type='submit' value='ȷ���޸�' />
			</form></xsl:when>
			<xsl:otherwise>
				<xsl:choose><xsl:when test='string-length(.)=0'>�޸�����ɹ�<br/><a href='javascript:history.go(-2)'>����</a></xsl:when><xsl:otherwise><xsl:value-of select='.' /><br/><a href='javascript:history.go(-1)'>����</a></xsl:otherwise></xsl:choose>
			</xsl:otherwise>
		</xsl:choose>
	</xsl:template>
</xsl:stylesheet>
