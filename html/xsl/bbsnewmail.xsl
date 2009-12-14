<?xml version='1.0' encoding='gb2312'?>
<xsl:stylesheet version='1.0' xmlns:xsl='http://www.w3.org/1999/XSL/Transform'>
	<xsl:import href='misc.xsl' />
	<xsl:import href='showpost.xsl' />
	<xsl:output method='html' encoding='gb2312' doctype-public='-//W3C//DTD HTML 4.01//EN' doctype-system='http://www.w3.org/TR/html4/strict.dtd' />
	<xsl:template match='/'>
		<xsl:call-template name='layout'>
			<xsl:with-param name='title'>�������ż�</xsl:with-param>
			<xsl:with-param name='session'><xsl:value-of select='bbsnewmail/@s' /></xsl:with-param>
		</xsl:call-template>
	</xsl:template>

	<xsl:template match='bbsnewmail'>
		<h2>�������ż�</h2>
		<p><a href='mail'><xsl:choose><xsl:when test='count(mail)=0'>��û��30���ڵ�δ���ż�</xsl:when><xsl:otherwise>��ҳ����ʾ30����δ���ż�</xsl:otherwise></xsl:choose>���鿴ȫ���ż����˴�</a></p>
		<xsl:if test='count(mail)!=0'><form name='list' method='post' action='mailman'>
			<table class='content'>
				<tr><th class='no'>���</th><th>����</th><th class='owner'>������</th><th>����</th><th class='ptitle'>�ż�����</th></tr>
				<xsl:for-each select='mail'><tr>
					<xsl:attribute name='class'><xsl:choose><xsl:when test='position() mod 2 = 1'>light</xsl:when><xsl:otherwise>dark</xsl:otherwise></xsl:choose></xsl:attribute>
					<td class='no'><xsl:value-of select='@n'/></td>
					<td><input type="checkbox"><xsl:attribute name='name'>box<xsl:value-of select='@name' /></xsl:attribute></input></td>
					<td><a class='owner'><xsl:attribute name='href'>qry?u=<xsl:value-of select='@from' /></xsl:attribute><xsl:value-of select='@from' /></a></td>
					<td class='time'><xsl:call-template name='timeconvert'><xsl:with-param name='time' select='@date' /></xsl:call-template></td>
					<td class='ptitle'><a class='ptitle'>
						<xsl:attribute name='href'>mailcon?f=<xsl:value-of select='@name' />&amp;n=<xsl:value-of select='@n' /></xsl:attribute>
						<xsl:call-template name='ansi-escape'>
							<xsl:with-param name='content'><xsl:value-of select='.' /></xsl:with-param>
							<xsl:with-param name='fgcolor'>37</xsl:with-param>
							<xsl:with-param name='bgcolor'>ignore</xsl:with-param>
							<xsl:with-param name='ishl'>0</xsl:with-param>
						</xsl:call-template>
					</a></td>
				</tr></xsl:for-each>
			</table>
			<input name='mode' value='' type='hidden' />
		</form>
		<div>[<a href="#"  onclick="checkAll();">ȫѡ</a>] [<a href="#"  onclick="checkReverse();">��ѡ</a>] [<a href="#" onclick="delSelected()">ɾ����ѡ�ż�</a>]</div></xsl:if>
	</xsl:template>
</xsl:stylesheet>
