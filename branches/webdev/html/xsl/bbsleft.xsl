<?xml version="1.0" encoding="gb2312"?>
<xsl:stylesheet version="2.0"
xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
xmlns="http://www.w3.org/1999/xhtml">
	<xsl:template match="/">
		<html>
			<head>
				<title>Main Menu</title>
				<meta http-equiv="content-type" content="text/html; charset=gb2312" />
				<link rel="stylesheet" type="text/css" href="/css/bbsleft.css" />
				<script type="text/javascript" src="/js/bbsleft.js"></script>
			</head>
		</html>
		<body>
			<div style="height:100%">
				<a href="#" onclick="switch_bar()" id="switchbar"></a>
				<div id="mainbar">
					<!-- ��վ���� -->
					<a href="/fcgi/bbs0an"><img src="/images/announce.gif" />��վ����</a>
					<!-- ȫ������ -->
					<a href="/fcgi/bbsall"><img src="/images/penguin.gif" />ȫ������</a>
					<!-- ͳ������ -->
					<a href="#" onclick="return SwitchPanel('Stat')"><img src="/images/top10.gif" />ͳ������</a>
					<div id="Stat">
						<a href="/fcgi/bbstop10"><img src="/images/blankblock.gif" />����ʮ��</a>
						<a href="/fcgi/bbstopb10"><img src="/images/blankblock.gif" />��������</a>
						<a href="/fcgi/bbsuserinfo"><img src="/images/blankblock.gif" />����ͳ��</a>
					</div>
					<!-- �ҵ��ղ� -->
					<xsl:if test="/bbsleft/favbrd">
						<a href="#" onclick="return SwitchPanel('Favorite')"><img src="/images/favorite.gif" />�ҵ��ղ�</a>
						<div id="Favorite">
							<a href="/fcgi/bbsmybrd"><img src="/images/blankblock.gif" />Ԥ������</a>
							<xsl:apply-templates select="bbsleft/favbrd" />
						</div>
					</xsl:if>
					<!-- ȵ����� -->
					<a href="#" onclick="return SwitchPanel('QueQiao')"><img src="/images/chat.gif" />ȵ�����</a>
					<div id="QueQiao">
						<xsl:if test="/bbsleft/login='1'">
							<a href="/fcgi/bbsfriend"><img src="/images/blankblock.gif" />���ߺ���</a>
						</xsl:if>
						<a href="/fcgi/bbsfriend"><img src="/images/blankblock.gif" />�����ķ�</a>
						<xsl:if test="/bbsleft/talk='1'">
							<a href="/fcgi/bbssendmsg"><img src="/images/blankblock.gif" />����ѶϢ</a>
							<a href="/fcgi/bbsmsg"><img src="/images/blankblock.gif" />�鿴����ѶϢ</a>
						</xsl:if>
					</div>
					<xsl:if test="/bbsleft/login='1'">
						<!-- �������� -->
						<a href="#" onclick="return SwitchPanel('Config')"><img src="/images/config.gif" />��������</a>
						<div id="Config">
							<a href="/fcgi/bbsinfo"><img src="/images/blankblock.gif" />��������</a>
							<a href="/fcgi/bbsplan"><img src="/images/blankblock.gif" />��˵����</a>
							<a href="/fcgi/bbssig"><img src="/images/blankblock.gif" />��ǩ����</a>
							<a href="/fcgi/bbsmywww"><img src="/images/blankblock.gif" />WWW����</a>
							<a href="/fcgi/bbspwd"><img src="/images/blankblock.gif" />�޸�����</a>
							<a href="/fcgi/bbsnick"><img src="/images/blankblock.gif" />��ʱ���ǳ�</a>
							<a href="/fcgi/bbsfall"><img src="/images/blankblock.gif" />�趨����</a>
							<xsl:if test="/bbsleft/cloak='1'">
								<a href="/fcgi/bbscloak"><img src="/images/blankblock.gif" />�л�����</a>
							</xsl:if>
						</div>
						<!-- �����ż� -->
						<a href="#" onclick="return SwitchPanel('Mail')"><img src="/images/mail.gif" />��������</a>
						<div id="Mail">
							<a href="/fcgi/bbsnewmail"><img src="/images/blankblock.gif" />�������ż�</a>
							<a href="/fcgi/bbsmail"><img src="/images/blankblock.gif" />�����ż�</a>
							<a href="/fcgi/bbsmaildown"><img src="/images/blankblock.gif" />�����ż�</a>
							<a href="/fcgi/bbspstmail"><img src="/images/blankblock.gif" />�����ż�</a>
						</div>
					</xsl:if>
					<!-- ����ѡ�� -->
					<a href="#" onclick="return SwitchPanel('Search')"><img src="/images/search.gif" />����ѡ��</a>
					<div id="Search">
						<xsl:if test="/bbsleft/find='1'">
							<a href="/fcgi/bbsfind"><img src="/images/blankblock.gif" />��������</a>
						</xsl:if>
						<xsl:if test="/bbsleft/login='1'">
							<a href="/fcgi/bbsqry"><img src="/images/blankblock.gif" />��ѯ����</a>
						</xsl:if>
						<a href="/fcgi/bbssel"><img src="/images/blankblock.gif" />����������</a>
					</div>
					<!-- �ն˵�¼ -->
					<a href="telnet://bbs.fudan.sh.cn:23"><img src="/images/telnet.gif" />�ն˵�¼</a>
					<!-- ע����¼ -->
					<xsl:if test="/bbsleft/login='1'">
							<a href="/fcgi/bbslogout"><img src="/images/exit.gif" />ע����¼</a>
					</xsl:if>
				</div>
			</div>
		</body>
	</xsl:template>

	<xsl:template match="bbsleft/favbrd">
		<xsl:for-each select="./*">
			<xsl:if test="name()='dir'">
				<a><xsl:attribute name="href">/fcgi/bbsboa?board=<xsl:value-of select="." /></xsl:attribute><xsl:value-of select="." /></a>
			</xsl:if>
			<xsl:if test="name()='board'">
				<a><xsl:attribute name="href">/fcgi/<xsl:value-of select="/bbsleft/favurl" />?board=<xsl:value-of select="." /></xsl:attribute><xsl:value-of select="." /></a>
			</xsl:if>
		</xsl:for-each>
	</xsl:template>
</xsl:stylesheet>