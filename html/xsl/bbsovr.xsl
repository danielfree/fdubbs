<?xml version='1.0' encoding='gb2312'?>
<xsl:stylesheet version='1.0' xmlns:xsl='http://www.w3.org/1999/XSL/Transform'>
	<xsl:import href='misc.xsl' />
	<xsl:output method='html' encoding='gb2312' doctype-public='-//W3C//DTD HTML 4.01//EN' doctype-system='http://www.w3.org/TR/html4/strict.dtd' />
	<xsl:template match='/'>
		<xsl:call-template name='layout'>
			<xsl:with-param name='title'>�趨��ע����</xsl:with-param>
			<xsl:with-param name='session'><xsl:value-of select='bbsfall/@s' /></xsl:with-param>
		</xsl:call-template>
	</xsl:template>

	<xsl:template match='bbsfall'>
		<h2>�趨��ע����</h2>
		<table class='content'>
			<tr><th class='owner'>�ʺ�</th><th class='chkbox'>����</th><th class='idesc'>˵��</th></tr>
			<xsl:for-each select='ov'><tr>
				<xsl:attribute name='class'><xsl:choose><xsl:when test='position() mod 2 = 1'>light</xsl:when><xsl:otherwise>dark</xsl:otherwise></xsl:choose></xsl:attribute>
				<td class='owner'><a class='owner'><xsl:attribute name='href'>qry?u=<xsl:value-of select='@id' /></xsl:attribute><xsl:value-of select='@id' /></a></td>
				<td class='chkbox'><a><xsl:attribute name='href'>fdel?u=<xsl:value-of select='@id' /></xsl:attribute>ɾ��</a></td>
				<td class='idesc'><xsl:value-of select='.' /></td>
			</tr></xsl:for-each>
		</table>
		<a href='fadd'>[���ӹ�ע����]</a>
	</xsl:template>
	
	<xsl:template match='bbsfadd'>
		<h2>���ӹ�ע����</h2>
		<form name='add' method='get' action='fadd'>
			<p><label for='id'>�ʺ�: </label><input class='binput' type='text' name='id' size='15' maxlength='15'></input></p>
			<p><label for='id'>˵��: </label><input class='binput' type='text' name='desc' size='50' maxlength='50'></input></p>
			<p><input type='submit' value='�ύ' size='10' /></p>
		</form>
	</xsl:template>
</xsl:stylesheet>
