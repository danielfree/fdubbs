<?xml version='1.0' encoding='gb2312'?>
<xsl:stylesheet version='1.0' xmlns:xsl='http://www.w3.org/1999/XSL/Transform'>
	<xsl:import href='misc.xsl' />
	<xsl:output method='html' encoding='gb2312' doctype-public='-//W3C//DTD HTML 4.01//EN' doctype-system='http://www.w3.org/TR/html4/strict.dtd' />
	<xsl:template match='/'>
		<xsl:call-template name='layout'>
			<xsl:with-param name='title'>��������</xsl:with-param>
			<xsl:with-param name='session'><xsl:value-of select='bbsinfo/@s' /></xsl:with-param>
		</xsl:call-template>
	</xsl:template>

	<xsl:template match='bbsinfo'>
		<xsl:choose><xsl:when test='@gender'>
			<fieldset><legend>�޸ĸ�������</legend><form action='info?type=1' method='post'>
				<p>�����ǳ�: <input type='text' name='nick' maxlength='30'><xsl:attribute name='value'><xsl:value-of select='nick' /></xsl:attribute></input></p>
				<p>��������: <input type='text' name='year' size='4' maxlength='4'><xsl:attribute name='value'><xsl:value-of select='@year + 1900' /></xsl:attribute></input> �� <input type='text' name='month' size='2' maxlength='2'><xsl:attribute name='value'><xsl:value-of select='@month' /></xsl:attribute></input> �� <input type='text' name='day' size='2' maxlength='2'><xsl:attribute name='value'><xsl:value-of select='@day' /></xsl:attribute></input> ��</p>
				<p>�û��Ա�: <input type='radio' value='M' name='gender'><xsl:if test='@gender = "M"'><xsl:attribute name='checked'>checked</xsl:attribute></xsl:if></input> �� <input type='radio' value='F' name='gender'><xsl:if test='@gender = "F"'><xsl:attribute name='checked'>checked</xsl:attribute></xsl:if></input> Ů</p>
				<input type='submit' value='ȷ��' /> <input type='reset' value='��ԭ' />
			</form></fieldset>
			<p>��¼��վ: <xsl:value-of select='@login' /> ��</p>
			<p>��վʱ��: <xsl:value-of select='floor(@stay div 60)' /> Сʱ <xsl:value-of select='@stay mod 60' /> ����</p>
			<p>�������: <xsl:value-of select='@post' /> ƪ</p>
			<p>�ʺŽ���: <xsl:call-template name='timeconvert'><xsl:with-param name='time' select='@since' /></xsl:call-template></p>
			<p>�������: <xsl:call-template name='timeconvert'><xsl:with-param name='time' select='@last' /></xsl:call-template></p>
			<p>��Դ��ַ: <xsl:value-of select='@host' /></p>
		</xsl:when>
		<xsl:otherwise><xsl:choose><xsl:when test='string-length(.) = 0'>�޸ĸ������ϳɹ�<br/><a href='javascript:history.go(-2)'>����</a></xsl:when><xsl:otherwise><xsl:value-of select='.' /></xsl:otherwise></xsl:choose></xsl:otherwise>
		</xsl:choose>
	</xsl:template>
</xsl:stylesheet>
