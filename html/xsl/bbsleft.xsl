<?xml version="1.0" encoding="gb2312"?>
<xsl:stylesheet version="2.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns="http://www.w3.org/1999/xhtml">
	<xsl:output method='html' encoding='gb2312' />
	<xsl:template match="bbsleft">
		<html>
			<head>
				<title>���˵�</title>
				<meta http-equiv="content-type" content="text/html; charset=gb2312" />
				<link rel="stylesheet" type="text/css" href="/css/bbsleft.css" />
				<script type="text/javascript" src="/js/bbsleft.js"></script>
			</head>
			<body>
				<div class="main">
					<a href="#" onclick="switch_bar()" id="switchbar">&#160;</a>
					<div id="mainbar">
						<!-- ��վ���� -->
						<a href="0an" target="view"><img src="/images/announce.gif" />��վ����</a>
						<!-- ȫ������ -->
						<a href="all" target="view"><img src="/images/penguin.gif" />ȫ������</a>
						<!-- ͳ������ -->
						<a href="#" onclick="return SwitchPanel('Stat')"><img src="/images/top10.gif" />ͳ������</a>
						<div id="Stat">
							<a href="top10" target="view"><img src="/images/blankblock.gif" />����ʮ��</a>
							<a href="topb10" target="view"><img src="/images/blankblock.gif" />��������</a>
							<a href="userinfo" target="view"><img src="/images/blankblock.gif" />����ͳ��</a>
						</div>
						<!-- �ҵ��ղ� -->
						<xsl:if test="favbrd">
							<a href="#" onclick="return SwitchPanel('Favorite')"><img src="/images/favorite.gif" />�ҵ��ղ�</a>
							<div id="Favorite">
								<a href="mybrd" target="view"><img src="/images/blankblock.gif" />Ԥ������</a>
								<xsl:apply-templates select="favbrd" />
							</div>
						</xsl:if>
						<!-- ȵ����� -->
						<a href="#" onclick="return SwitchPanel('QueQiao')"><img src="/images/chat.gif" />ȵ�����</a>
						<div id="QueQiao">
							<xsl:if test="login='1'">
								<a href="friend" target="view"><img src="/images/blankblock.gif" />���ߺ���</a>
							</xsl:if>
							<a href="friend" target="view"><img src="/images/blankblock.gif" />�����ķ�</a>
							<xsl:if test="talk!='0'">
								<a href="sendmsg" target="view"><img src="/images/blankblock.gif" />����ѶϢ</a>
								<a href="msg" target="view"><img src="/images/blankblock.gif" />�鿴����ѶϢ</a>
							</xsl:if>
						</div>
						<xsl:if test="login='1'">
							<!-- �������� -->
							<a href="#" onclick="return SwitchPanel('Config')"><img src="/images/config.gif" />��������</a>
							<div id="Config">
								<a href="info" target="view"><img src="/images/blankblock.gif" />��������</a>
								<a href="plan" target="view"><img src="/images/blankblock.gif" />��˵����</a>
								<a href="sig" target="view"><img src="/images/blankblock.gif" />��ǩ����</a>
								<a href="mywww" target="view"><img src="/images/blankblock.gif" />WWW����</a>
								<a href="pwd" target="view"><img src="/images/blankblock.gif" />�޸�����</a>
								<a href="nick" target="view"><img src="/images/blankblock.gif" />��ʱ���ǳ�</a>
								<a href="fall" target="view"><img src="/images/blankblock.gif" />�趨����</a>
								<xsl:if test="cloak!='0'">
									<a href="cloak" target="view"><img src="/images/blankblock.gif" />�л�����</a>
								</xsl:if>
							</div>
							<!-- �����ż� -->
							<a href="#" onclick="return SwitchPanel('Mail')"><img src="/images/mail.gif" />�����ż�</a>
							<div id="Mail">
								<a href="newmail" target="view"><img src="/images/blankblock.gif" />�������ż�</a>
								<a href="mail" target="view"><img src="/images/blankblock.gif" />�����ż�</a>
								<a href="maildown" target="view"><img src="/images/blankblock.gif" />�����ż�</a>
								<a href="pstmail" target="view"><img src="/images/blankblock.gif" />�����ż�</a>
							</div>
						</xsl:if>
						<!-- ����ѡ�� -->
						<a href="#" onclick="return SwitchPanel('Search')"><img src="/images/search.gif" />����ѡ��</a>
						<div id="Search">
							<xsl:if test="find!='0'">
								<a href="find" target="view"><img src="/images/blankblock.gif" />��������</a>
							</xsl:if>
							<xsl:if test="login='1'">
								<a href="qry" target="view"><img src="/images/blankblock.gif" />��ѯ����</a>
							</xsl:if>
							<a href="sel" target="view"><img src="/images/blankblock.gif" />����������</a>
						</div>
						<!-- �ն˵�¼ -->
						<a href="telnet://bbs.fudan.sh.cn:23"><img src="/images/telnet.gif" />�ն˵�¼</a>
						<!-- ע����¼ -->
						<xsl:if test="login='1'">
								<a href="logout" target="_top"><img src="/images/exit.gif" />ע����¼</a>
						</xsl:if>
					</div>
				</div>
			</body>
		</html>
	</xsl:template>

	<xsl:template match="favbrd">
		<xsl:for-each select="./*">
			<xsl:if test="name()='dir'">
				<a target="view">
					<xsl:attribute name="href">boa?board=<xsl:value-of select="." /></xsl:attribute><xsl:value-of select="." />
				</a>
			</xsl:if>
			<xsl:if test="name()='board'">
				<a target="view">
					<xsl:attribute name="href"><xsl:value-of select="/bbsleft/favurl" />?board=<xsl:value-of select="." /></xsl:attribute><xsl:value-of select="." />
				</a>
			</xsl:if>
		</xsl:for-each>
	</xsl:template>
</xsl:stylesheet>
