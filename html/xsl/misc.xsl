<?xml version='1.0' encoding='gb2312'?>
<xsl:stylesheet version='1.0' xmlns:xsl='http://www.w3.org/1999/XSL/Transform'>
	<xsl:template name='timeconvert'>
		<xsl:param name='time' />
		<xsl:value-of select='concat(substring($time, 6, 5), " ", substring($time, 12, 5))' />
	</xsl:template>

	<xsl:template name="splitbm">
		<xsl:param name='names' />
		<xsl:param name='isdir' />
		<xsl:param name='isfirst' />
        <xsl:variable name='first' select='substring-before($names," ")' />
        <xsl:variable name='rest' select='substring-after($names," ")' />
        <xsl:if test='$first'>
			<a><xsl:attribute name='href'>qry?u=<xsl:value-of select='$first' /></xsl:attribute><xsl:value-of select='$first' /></a>
		</xsl:if>
		<xsl:if test='$rest'>
			<span>&#160;</span>
			<xsl:call-template name='splitbm'>
				<xsl:with-param name='names' select='$rest'/>
				<xsl:with-param name='isdir' select='$isdir'/>
				<xsl:with-param name='isfirst' select='0'/>
			</xsl:call-template>
        </xsl:if>
		<xsl:if test='not($rest)'>
			<xsl:if test='$names'>
				<a><xsl:attribute name='href'>qry?u=<xsl:value-of select='$names' /></xsl:attribute><xsl:value-of select='$names' /></a>
			</xsl:if>
			<xsl:if test="$names=''">
				<xsl:if test="$isdir='0'">����������</xsl:if>
				<xsl:if test="$isdir!='0'">-</xsl:if>
			</xsl:if>
		</xsl:if>
	</xsl:template>
	
	<xsl:template name="showexp">
		<xsl:param name="count" />
		<xsl:if test="$count > 0">
			<img>
				<xsl:attribute name='src'>/images/level/<xsl:value-of select="level" />.gif</xsl:attribute>
			</img>
			<xsl:call-template name="showexp">
				<xsl:with-param name="count" select="$count - 1"/>
			</xsl:call-template>
		</xsl:if>
	</xsl:template>

	<xsl:template name='navigation'>
		<xsl:param name='perm' />
		<ul id='nav'>
			<li id='navh'><a href='sec'>�Ƽ�����</a></li>
			<li id='nava'><a href='0an'>��վ����</a></li>
			<li id='navp'><a href='all'>ȫ������</a></li>
			<li id='navt'>
				<a href='#' onclick='return switchPanel(this);'>ͳ��</a>
				<ul>
					<li><a href='top10'>����ʮ��</a></li>
					<li><a href='topb10'>��������</a></li>
					<li><a href='userinfo'>����ͳ��</a></li>
				</ul>
			</li>
			<xsl:if test='contains($perm, "l")'>
				<li id='navf'><a href='fav'>�ҵ��ղ�</a></li>
				<li id='navc'>
					<a href='#' onclick='return switchPanel(this);'>ȵ�����</a>
					<ul>
						<li><a href='friend'>���ߺ���</a></li>
						<li><a href='usr'>�����ķ�</a></li>
						<li><a href='sendmsg'>����ѶϢ</a></li>
						<li><a href='msg'>�鿴ѶϢ</a></li>
					</ul>
				</li>
				<li id='navm'>
					<a href='#' onclick='return switchPanel(this);'>�����ż�</a>
					<ul>
						<li><a href='newmail'>��������</a></li>
						<li><a href='mail'>�����ż�</a></li>
						<li><a href='pstmail'>�����ż�</a></li>
						<li></li>
					</ul>
				</li>
				<li id='navco'>
					<a href='#' onclick='return switchPanel(this);'>��������</a>
					<ul>
						<li><a href='info'>��������</a></li>
						<li><a href='plan'>��˵����</a></li>
						<li><a href='sig'>��ǩ����</a></li>
						<li><a href='mywww'>Web����</a></li>
						<li><a href='pwd'>�޸�����</a></li>
						<li><a href='nick'>��ʱ���ǳ�</a></li>
						<li><a href='fall'>�趨����</a></li>
						<li><a href='cloak'>�л�����</a></li>
					</ul>
				</li>
			</xsl:if>
			<li id='navs'>
				<a href='#' onclick='return switchPanel(this);'>����ѡ��</a>
				<ul>
					<li><a href='qry'>��������</a></li>
					<li><a href='qry'>��ѯ����</a></li>
					<li><a href='sel'>����������</a></li>
				</ul>
			</li>
		</ul>
	</xsl:template>
	
	<xsl:template name='header'>
		<xsl:param name='perm' />
		<xsl:param name='user' />
		<div id='hd'>
			<xsl:if test='$user != ""'><a id='navu'><xsl:attribute name='href'>qry?u=<xsl:value-of select='$user' /></xsl:attribute><xsl:value-of select='$user' /></a></xsl:if>
			<xsl:if test='$user = ""'><a id='navl' href='login'>��¼</a></xsl:if>
			<xsl:if test='$user != ""'><a id='nave' href='logout'>ע��</a></xsl:if>
			<a id='navte' href='telnet://bbs.fudan.sh.cn:23'>�ն˵�¼</a>
		</div>
	</xsl:template>

	<xsl:template name='foot'>
		<div id='ft'><xsl:call-template name='bbsname' /> &#169;1996-2009 Powered by <a href='http://code.google.com/p/fdubbs/'><strong>fdubbs</strong></a></div>
	</xsl:template>

	<xsl:template name='bbsname'>���¹⻪</xsl:template>
	<xsl:template name='include-css'><link rel='stylesheet' type='text/css' href='../css/bbs.css' /></xsl:template>
	<xsl:template name='include-js'><script type='text/javascript' src='../js/bbs.js'></script></xsl:template>
</xsl:stylesheet>
