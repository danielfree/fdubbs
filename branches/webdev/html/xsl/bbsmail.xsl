<?xml version='1.0' encoding='gb2312'?>
<xsl:stylesheet version='1.0' xmlns:xsl='http://www.w3.org/1999/XSL/Transform'>
	<xsl:import href='misc.xsl' />
	<xsl:import href='showpost.xsl' />
	<xsl:output method='html' encoding='gb2312' doctype-public='-//W3C//DTD HTML 4.01//EN' doctype-system='http://www.w3.org/TR/html4/strict.dtd' />
	<xsl:template match='bbsmail'>
	<html>
		<head>
			<title>�ż��б� - <xsl:call-template name='bbsname' /></title>
			<meta http-equiv="content-type" content="text/html; charset=gb2312" />
			<xsl:call-template name='include-css' />
			<xsl:call-template name='include-js' />
		</head>
		<body><div id='wrap'>
			<xsl:call-template name='header'><xsl:with-param name='perm' select='@p' /><xsl:with-param name='user' select='@u' /></xsl:call-template>
			<xsl:call-template name='navigation'><xsl:with-param name='perm' select='@p' /></xsl:call-template>
			<div id='main'>
				<p>�ż����� [<xsl:value-of select='@total' />]��</p>
				<form name='list' method='post' action='mailman'>
					<table class='content'>
						<tr><th class='no'>���</th><th>����</th><th class='mark'>״̬</th><th class='owner'>������</th><th>����</th><th class='ptitle'>�ż�����</th></tr>
						<xsl:for-each select='mail'><tr>
							<xsl:attribute name='class'><xsl:choose><xsl:when test='position() mod 2 = 1'>light</xsl:when><xsl:otherwise>dark</xsl:otherwise></xsl:choose></xsl:attribute>
							<td class='no'><xsl:value-of select='position() - 1 + ../@start' /></td>
							<td><input type="checkbox"><xsl:attribute name='name'>box<xsl:value-of select='@name' /></xsl:attribute></input></td>
							<td class='mark'><xsl:value-of select='@m' /></td>
							<td><a class='owner'><xsl:attribute name='href'>qry?u=<xsl:value-of select='@from' /></xsl:attribute><xsl:value-of select='@from' /></a></td>
							<td><xsl:call-template name='timeconvert'><xsl:with-param name='time' select='@date' /></xsl:call-template></td>
							<td class='ptitle'><a class='ptitle'>
								<xsl:attribute name='href'>mailcon?f=<xsl:value-of select='@name' /></xsl:attribute>
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
				<div>[<a href="#"  onclick="checkAll();">��ҳȫѡ</a>] [<a href="#"  onclick="checkReverse();">��ҳ��ѡ</a>] [<a href="#" onclick="delSelected()">ɾ����ѡ�ż�</a>]</div>
				<div>
					<xsl:if test='@start &gt; 1'>
						<xsl:variable name='prev'>
							<xsl:choose><xsl:when test='@start - @page &lt; 1'>1</xsl:when><xsl:otherwise><xsl:value-of select='@start - @page' /></xsl:otherwise></xsl:choose>
						</xsl:variable>
						<a><xsl:attribute name='href'>mail?start=<xsl:value-of select='$prev' /></xsl:attribute>[ <img src='../images/button/up.gif' />��һҳ ]</a>
					</xsl:if>
					<xsl:if test='@total &gt; @start + @page - 1'>
						<xsl:variable name='next'><xsl:value-of select='@start + @page' /></xsl:variable>
						<a><xsl:attribute name='href'>mail?start=<xsl:value-of select='$next' /></xsl:attribute>[ <img src='../images/button/down.gif' />��һҳ ]</a>
					</xsl:if>
					<form><input value='��ת��' type='submit' />��<input name='start' size='4' type='text' />��</form>
				</div>
			</div>
			<xsl:call-template name='foot' />
		</div></body>
	</html>
	</xsl:template>
	<xsl:template name='linkbar'>
	</xsl:template>
</xsl:stylesheet>