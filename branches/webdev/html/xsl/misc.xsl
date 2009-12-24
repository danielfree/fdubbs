<?xml version='1.0' encoding='gb2312'?>
<xsl:stylesheet version='1.0' xmlns:xsl='http://www.w3.org/1999/XSL/Transform'>
	<xsl:output method='html' encoding='gb2312' doctype-public='-//W3C//DTD HTML 4.01//EN' doctype-system='http://www.w3.org/TR/html4/strict.dtd' />
	<xsl:template name='timeconvert'>
		<xsl:param name='time' />
		<xsl:value-of select='concat(substring($time, 1, 10), " ", substring($time, 12, 5))' />
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
			<xsl:text>&#160;</xsl:text>
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

	<xsl:template name='show-fav'>
		<xsl:param name='fav'/>
		<xsl:variable name='brd' select='substring-before($fav, " ")'/>
		<xsl:if test='$brd!=""'><li class='sf'><a><xsl:attribute name='href'>doc?board=<xsl:value-of select='$brd'/></xsl:attribute><xsl:value-of select='$brd'/></a></li></xsl:if>
		<xsl:variable name='rest' select='substring-after($fav, " ")'/>
		<xsl:if test='$rest!=""'><xsl:call-template name='show-fav'><xsl:with-param name='fav' select='$rest'/></xsl:call-template></xsl:if>
	</xsl:template>
	
	<xsl:template name='navigation'>
		<xsl:param name='perm'/>
		<xsl:param name='fav'/>
		<xsl:variable name='bbsname'><xsl:call-template name='bbsname'/></xsl:variable>
		<ul id='nav'>
			<li id='navh'><a href='sec'>�Ƽ�����</a></li>
			<xsl:if test='$bbsname="���¹⻪"'><li id='navb'>
				<a href='#' onclick='return switchPanel(this);'>��������</a>
				<ul><li><a href='boa?s=0'>0 BBSϵͳ</a></li>
				<li><a href='boa?s=1'>1 ������ѧ</a></li>
				<li><a href='boa?s=2'>2 Ժϵ���</a></li>
				<li><a href='boa?s=3'>3 ���Լ���</a></li>
				<li><a href='boa?s=4'>4 ��������</a></li>
				<li><a href='boa?s=5'>5 ��ѧ����</a></li>
				<li><a href='boa?s=6'>6 ��������</a></li>
				<li><a href='boa?s=7'>7 ���Կռ�</a></li>
				<li><a href='boa?s=8'>8 ������Ϣ</a></li>
				<li><a href='boa?s=9'>9 ѧ��ѧ��</a></li>
				<li><a href='boa?s=A'>A ����Ӱ��</a></li>
				<li><a href='boa?s=B'>B ����ר��</a></li>
				</ul>
			</li></xsl:if>
			<li id='nava'><a href='0an'>��վ����</a></li>
			<li id='navp'><a href='all'>ȫ������</a></li>
			<li id='navt'>
				<a href='#' onclick='return switchPanel(this);'>ͳ������</a>
				<ul>
					<li><a href='top10'>����ʮ��</a></li>
				</ul>
			</li>
			<xsl:if test='contains($perm, "l")'>
				<li id='navf'>
					<a href='#' onclick='return switchPanel(this);'>�ҵ��ղ�</a>
					<ul><li><a href='fav'>�鿴����</a></li><xsl:call-template name='show-fav'><xsl:with-param name='fav' select='$fav'/></xsl:call-template></ul>
				</li>
				<li id='navc'>
					<a href='#' onclick='return switchPanel(this);'>ȵ�����</a>
					<ul>
						<li><a href='ovr'>���ߺ���</a></li>
					</ul>
				</li>
				<li id='navm'>
					<a href='#' onclick='return switchPanel(this);'>�����ż�</a>
					<ul>
						<li><a href='newmail'>��������</a></li>
						<li><a href='mail'>�����ż�</a></li>
						<li><a href='pstmail'>�����ż�</a></li>
					</ul>
				</li>
				<li id='navco'>
					<a href='#' onclick='return switchPanel(this);'>��������</a>
					<ul>
						<li><a href='info'>��������</a></li>
						<li><a href='plan'>��˵����</a></li>
						<li><a href='sig'>��ǩ����</a></li>
						<li><a href='pwd'>�޸�����</a></li>
						<li><a href='fall'>�趨����</a></li>
					</ul>
				</li>
			</xsl:if>
			<li id='navs'>
				<a href='#' onclick='return switchPanel(this);'>����ѡ��</a>
				<ul>
					<li><a href='qry'>��ѯ����</a></li>
					<li><a href='sel'>���Ұ���</a></li>
				</ul>
			</li>
		</ul>
	</xsl:template>
	
	<xsl:template name='header'>
		<xsl:param name='perm' />
		<xsl:param name='user' />
		<div id='hd'>
			<div id='hdright'><xsl:if test='$user != ""'><a id='nave' href='logout'>ע��</a></xsl:if></div>
			<xsl:if test='$user != ""'><a id='navu'><xsl:attribute name='href'>qry?u=<xsl:value-of select='$user' /></xsl:attribute><xsl:value-of select='$user' /></a></xsl:if>
			<xsl:if test='$user = ""'><a id='navl' href='login'>��¼</a></xsl:if>
			<a id='navnm' href='newmail'>����[<span id='navmc'></span>]�����ż�</a>
			<a id='navte' href='telnet://bbs.fudan.sh.cn:23'>�ն˵�¼</a>
			<span id='iewarn'><xsl:comment><![CDATA[[if lt IE 7]><![endif]]]></xsl:comment></span>
		</div>
	</xsl:template>

	<xsl:template name='foot'>
		<div id='ft'><a href='#'>[<img src='../images/button/up.gif'/>��ҳ��]</a>&#160;<xsl:call-template name='bbsname'/> &#169;1996-2009 Powered by <a href='http://code.google.com/p/fdubbs/'><strong>fdubbs</strong></a></div>
	</xsl:template>

	<xsl:template name='bbsname'>���¹⻪</xsl:template>
	<xsl:template name='include-css'><link rel='stylesheet' type='text/css' href='../css/bbs.css' /></xsl:template>
	<xsl:template name='include-js'><script type='text/javascript' src='../js/bbs.js'></script><script type='text/javascript' src='../js/persist.js'></script></xsl:template>

	<xsl:template name='layout'>
		<xsl:param name='title'/>
		<xsl:param name='session'/>
		<xsl:variable name='p' select='substring-before($session,";")'/>
		<xsl:variable name='rest' select='substring-after($session,";")'/>
		<xsl:variable name='u' select='substring-before($rest,";")'/>
		<xsl:variable name='fav' select='substring-after($rest,";")'/>
		<html>
			<head>
				<title><xsl:value-of select='$title' /> - <xsl:call-template name='bbsname' /></title>
				<meta http-equiv="content-type" content="text/html; charset=gb2312" />
				<xsl:call-template name='include-css' />
<xsl:comment><![CDATA[[if lt IE 7]><style>
#hd{position:absolute;top:0;left:144px;margin:0.5em;}
#main{position:absolute;top:32px;left:144px;margin:0.5em 0 0.5em 0}
#ft{display:none;margin:0.5em}
#main td{font-size:12px;height:1}
table.content{width:100%}
th.ptitle,td.ptitle{width:auto}
.content tr{line-height:24px}
table.post{width:100%}
</style><![endif]]]></xsl:comment>
				<xsl:call-template name='include-js' />
			</head>
			<body>
				<a name='top' />
				<xsl:call-template name='navigation'><xsl:with-param name='perm' select='$p' /><xsl:with-param name='fav' select='$fav' /></xsl:call-template>
				<xsl:call-template name='header'><xsl:with-param name='perm' select='$p' /><xsl:with-param name='user' select='$u' /></xsl:call-template>
				<div id='main'><xsl:apply-templates /></div>
				<xsl:call-template name='foot' />
			</body>
		</html>
	</xsl:template>
</xsl:stylesheet>
