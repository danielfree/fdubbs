<?xml version='1.0' encoding='gb2312'?>
<xsl:stylesheet version='1.0' xmlns:xsl='http://www.w3.org/1999/XSL/Transform' xmlns='http://www.w3.org/1999/xhtml'>
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

		
	<xsl:template name='navgation-bar'>
		<xsl:param name='perm' />
		<div>
			<ul class='nav'>
				<li id='navh'><a href='sec'>��ҳ</a></li>
				<xsl:if test='not(contains($perm, "l"))'><li id='navl'><a href='login'>��¼</a></li></xsl:if>
				<xsl:if test='contains($perm, "l")'>
					<li id='navf'><a href='fav'>�ղؼ�</a></li>
					<li id='navc'>
						<a href='#'>ȵ��</a>
						<ul>
							<li><a href='friend'>���ߺ���</a></li>
							<li><a href='usr'>�����ķ�</a></li>
							<li><a href='sendmsg'>����ѶϢ</a></li>
							<li><a href='msg'>�鿴����ѶϢ</a></li>
						</ul>
					</li>
					<li id='navm'>
						<a href='#'>�ż�</a>
						<ul>
							<li><a href='newmail'>�������ż�</a></li>
							<li><a href='mail'>�����ż�</a></li>
							<li><a href='pstmail'>�����ż�</a></li>
							<li></li>
						</ul>
					</li>
					<li id='navco'>
						<a href='#'>����</a>
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
					<li id='nave'><a href='logout'>�˳�</a></li>
				</xsl:if>
				<li id='nava'><a href='0an'>��վ����</a></li>
				<li id='navp'><a href='all'>ȫ������</a></li>
				<li id='navs'>
					<a href='#'>����</a>
					<ul>
						<li><a href='qry'>��������</a></li>
						<li><a href='qry'>��ѯ����</a></li>
						<li><a href='sel'>����������</a></li>
					</ul>
				</li>
				<li id='navt'>
					<a href='#'>ͳ��</a>
					<ul>
						<li><a href='top10'>����ʮ��</a></li>
						<li><a href='topb10'>��������</a></li>
						<li><a href='userinfo'>����ͳ��</a></li>
					</ul>
				</li>
				<li id='navte'><a href='telnet://bbs.fudan.sh.cn:23'>�ն˵�¼</a></li>
			</ul>
		</div>
	</xsl:template>
	
	<xsl:template name='bbsname'>&#160;- ���¹⻪</xsl:template>
</xsl:stylesheet>
