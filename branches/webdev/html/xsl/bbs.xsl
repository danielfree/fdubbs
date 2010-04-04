<?xml version='1.0' encoding='gb2312'?>
<xsl:stylesheet version='1.0' xmlns:xsl='http://www.w3.org/1999/XSL/Transform'>
<xsl:import href='showpost.xsl'/>
<xsl:output method='html' encoding='gb2312' doctype-public='-//W3C//DTD HTML 4.01//EN' doctype-system='http://www.w3.org/TR/html4/strict.dtd'/>

<xsl:template name='timeconvert'>
	<xsl:param name='time'/>
	<xsl:value-of select='concat(substring($time, 1, 10), " ", substring($time, 12, 5))'/>
</xsl:template>

<xsl:template name="splitbm">
	<xsl:param name='names'/>
	<xsl:param name='isdir'/>
	<xsl:param name='isfirst'/>
	<xsl:variable name='first' select='substring-before($names," ")'/>
	<xsl:variable name='rest' select='substring-after($names," ")'/>
	<xsl:if test='$first'>
		<a><xsl:attribute name='href'>qry?u=<xsl:value-of select='$first'/></xsl:attribute><xsl:value-of select='$first'/></a>
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
			<a><xsl:attribute name='href'>qry?u=<xsl:value-of select='$names'/></xsl:attribute><xsl:value-of select='$names'/></a>
		</xsl:if>
		<xsl:if test="$names=''">
			<xsl:if test="$isdir='0'">����������</xsl:if>
			<xsl:if test="$isdir!='0'">-</xsl:if>
		</xsl:if>
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
	<xsl:param name='perm'/>
	<xsl:param name='user'/>
	<div id='hd'>
		<div id='hdright'><xsl:if test='$user != ""'><a id='nave' href='logout'>ע��</a></xsl:if></div>
		<xsl:if test='$user != ""'><a id='navu'><xsl:attribute name='href'>qry?u=<xsl:value-of select='$user'/></xsl:attribute><xsl:value-of select='$user'/></a></xsl:if>
		<xsl:if test='$user = ""'><a id='navl' href='login'>��¼</a></xsl:if>
		<a id='navnm' href='newmail'>����[<span id='navmc'></span>]�����ż�</a>
		<a id='navte' href='telnet://bbs.fudan.sh.cn:23'>�ն˵�¼</a>
		<span id='iewarn'><xsl:comment><![CDATA[[if lt IE 7]><![endif]]]></xsl:comment></span>
	</div>
</xsl:template>

<xsl:template name='foot'>
	<div id='ft'><a href='#'>[<img src='../images/button/up.gif'/>��ҳ��]</a>&#160;<xsl:call-template name='bbsname'/> &#169;1996-2010 Powered by <a href='http://code.google.com/p/fdubbs/'><strong>fdubbs</strong></a></div>
</xsl:template>

<xsl:template name='bbsname'>���¹⻪</xsl:template>
<xsl:template name='include-css'>
	<link rel='stylesheet' type='text/css' href='../css/bbs.css'/>
	<xsl:comment><![CDATA[[if lt IE 7]><link rel='stylesheet' type='text/css' href='../css/ie6fix.css'/><![endif]]]></xsl:comment>
</xsl:template>
<xsl:template name='include-js'><script type='text/javascript' src='../js/bbs.js' defer='defer'></script><script type='text/javascript' src='../js/persist-all-min.js' defer='defer'></script></xsl:template>

<xsl:template name='page-title'>
	<xsl:variable name='cgi' select='local-name(node()[2])'/>
	<xsl:choose>
		<xsl:when test='bbssec'>�Ƽ�����</xsl:when>
		<xsl:when test='bbsboa'><xsl:choose><xsl:when test='bbsboa/@dir'>����Ŀ¼</xsl:when><xsl:otherwise>����������</xsl:otherwise></xsl:choose></xsl:when>
		<xsl:when test='bbsall'>ȫ��������</xsl:when>
		<xsl:when test='bbssel'>ѡ��������</xsl:when>
		<xsl:when test='bbsdoc'><xsl:value-of select='bbsdoc/brd/@desc'/></xsl:when>
		<xsl:when test='bbscon'>�����Ķ�</xsl:when>
		<xsl:when test='bbstcon'>ͬ���������Ķ�</xsl:when>
		<xsl:when test='bbsqry'>��ѯ����</xsl:when>
		<xsl:when test='bbspst'><xsl:choose><xsl:when test='bbspst/@edit="0"'>����</xsl:when><xsl:otherwise>�޸�</xsl:otherwise></xsl:choose>����</xsl:when>
		<xsl:when test='bbstop10'>����ʮ��</xsl:when>
		<xsl:when test='bbsbfind'>�������²�ѯ</xsl:when>
		<xsl:when test='bbsnewmail'>�������ż�</xsl:when>
		<xsl:when test='bbsmail'>�ż��б�</xsl:when>
		<xsl:when test='bbsmailcon'>�ż��Ķ�</xsl:when>
		<xsl:when test='bbspstmail'>�����Ÿ�</xsl:when>
		<xsl:when test='bbs0an'>���������</xsl:when>
		<xsl:when test='bbsanc'>�����������Ķ�</xsl:when>
		<xsl:when test='bbsfwd'>ת������</xsl:when>
		<xsl:when test='bbsccc'>ת������</xsl:when>
		<xsl:when test='bbsfall'>�趨��ע����</xsl:when>
		<xsl:when test='bbsfadd'>���ӹ�ע����</xsl:when>
		<xsl:when test='bbsovr'>���߹�ע����</xsl:when>
		<xsl:when test='bbsfav'>�ղؼ�</xsl:when>
		<xsl:when test='bbsmybrd'>�趨�ղؼ�</xsl:when>
		<xsl:when test='bbseufile'><xsl:value-of select='bbseufile/@desc'/></xsl:when>
		<xsl:when test='bbsmybrd'>��������</xsl:when>
		<xsl:when test='bbspwd'>�޸�����</xsl:when>
		<xsl:when test='bbsnot'>���滭��</xsl:when>
		<xsl:otherwise></xsl:otherwise>
	</xsl:choose>
</xsl:template>

<xsl:template match='/'>
	<xsl:variable name='session' select='node()[2]/@s'/>
	<xsl:variable name='p' select='substring-before($session,";")'/>
	<xsl:variable name='rest' select='substring-after($session,";")'/>
	<xsl:variable name='u' select='substring-before($rest,";")'/>
	<xsl:variable name='fav' select='substring-after($rest,";")'/>
	<html>
		<head>
			<title><xsl:call-template name='page-title'/> - <xsl:call-template name='bbsname'/></title>
			<meta http-equiv="content-type" content="text/html; charset=gb2312"/>
			<xsl:call-template name='include-css'/>
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
			<xsl:call-template name='include-js'/>
		</head>
		<body>
			<a name='top'/>
			<xsl:call-template name='navigation'><xsl:with-param name='perm' select='$p'/><xsl:with-param name='fav' select='$fav'/></xsl:call-template>
			<xsl:call-template name='header'><xsl:with-param name='perm' select='$p'/><xsl:with-param name='user' select='$u'/></xsl:call-template>
			<div id='main'><xsl:apply-templates/></div>
			<xsl:call-template name='foot'/>
		</body>
		<xsl:comment><![CDATA[[if lt IE 7]><script type='text/javascript' defer='defer'>addLoadEvent(ie6fix);window.onresize=ie6fix;</script><![endif]]]></xsl:comment>
	</html>
</xsl:template>

<xsl:template match='bbssec'>
	<img src='../images/secbanner.jpg'/>
	<xsl:for-each select='sec'>
		<ul class='sec'>
			<li><a><xsl:attribute name='href'>boa?s=<xsl:value-of select='@id'/></xsl:attribute><xsl:value-of select='@id'/>&#160;<xsl:value-of select='@desc'/></a></li>
			<ul class='brd'>
				<xsl:for-each select='brd'>
					<li><a><xsl:attribute name='href'>doc?board=<xsl:value-of select='@name'/></xsl:attribute><xsl:value-of select='@desc'/></a></li>
				</xsl:for-each>
			</ul>
		</ul>
	</xsl:for-each>
</xsl:template>

<xsl:template match='bbsboa'>
	<h2><xsl:if test='@icon'><img><xsl:attribute name='src'><xsl:value-of select='icon'/></xsl:attribute></img></xsl:if><xsl:value-of select='@title'/></h2>
	<table class='content'>
		<tr><th class='no'>���</th><th class='read'>δ��</th><th class='no'>������</th><th class='title'>����������</th><th class='cate'>���</th><th class='desc'>��������</th><th class='bm'>����</th></tr>
		<xsl:for-each select='brd'><xsl:sort select='@title'/><tr>
			<xsl:attribute name='class'><xsl:choose><xsl:when test='position() mod 2 = 1'>light</xsl:when><xsl:otherwise>dark</xsl:otherwise></xsl:choose></xsl:attribute>
			<td class='no'><xsl:value-of select='position()'/></td>
			<td class='read'><xsl:choose><xsl:when test='@read="0"'>��</xsl:when><xsl:otherwise>��</xsl:otherwise></xsl:choose></td>
			<td class='no'><xsl:value-of select='@count'/></td>
			<td class='title'><a class='title'><xsl:choose>
				<xsl:when test='@dir="1"'><xsl:attribute name='href'>boa?board=<xsl:value-of select='@title'/></xsl:attribute>[ <xsl:value-of select='@title'/> ]</xsl:when>
				<xsl:otherwise><xsl:attribute name='href'>doc?board=<xsl:value-of select='@title'/></xsl:attribute><xsl:value-of select='@title'/></xsl:otherwise>
			</xsl:choose></a></td>
			<td class='cate'><xsl:choose><xsl:when test='@dir="1"'>[Ŀ¼]</xsl:when><xsl:otherwise><xsl:value-of select='@cate'/></xsl:otherwise></xsl:choose></td>
			<td class='desc'><a class='desc'><xsl:attribute name='href'><xsl:choose><xsl:when test='@dir="1"'>boa</xsl:when><xsl:otherwise>doc</xsl:otherwise></xsl:choose>?board=<xsl:value-of select='@title'/></xsl:attribute><xsl:value-of select='@desc'/></a></td>
			<td class='bm'><xsl:call-template name='splitbm'><xsl:with-param name='names' select='@bm'/><xsl:with-param name='isdir' select='@dir'/><xsl:with-param name='isfirst' select='1'/></xsl:call-template></td>
		</tr></xsl:for-each>
	</table>
</xsl:template>

<xsl:template match='bbsall'>
	<h2>ȫ��������</h2>
	<p>[��������: <xsl:value-of select="count(brd)"/>]</p>
	<table class='content'>
		<tr><th class='no'>���</th><th class='title'>����������</th><th class='cate'>���</th><th class='desc'>��������</th><th class='bm'>����</th></tr>
		<xsl:for-each select='brd'>
			<xsl:sort select="@title"/>
			<tr>
				<xsl:attribute name='class'>
					<xsl:if test='position() mod 2 = 1'>light</xsl:if>
					<xsl:if test='position() mod 2 = 0'>dark</xsl:if>
				</xsl:attribute>
				<td class='no'><xsl:value-of select='position()'/></td>
				<td class='title'><a class='title'><xsl:choose>
					<xsl:when test='@dir="1"'><xsl:attribute name='href'>boa?board=<xsl:value-of select='@title'/></xsl:attribute>[ <xsl:value-of select='@title'/> ]</xsl:when>
					<xsl:otherwise><xsl:attribute name='href'>doc?board=<xsl:value-of select='@title'/></xsl:attribute><xsl:value-of select='@title'/></xsl:otherwise>
				</xsl:choose></a></td>
				<td class='cate'><xsl:choose>
					<xsl:when test='@dir="1"'>[Ŀ¼]</xsl:when>
					<xsl:otherwise><xsl:value-of select='@cate'/></xsl:otherwise>
				</xsl:choose></td>
				<td class='desc'><a class='desc'><xsl:choose>
					<xsl:when test='@dir="1"'><xsl:attribute name='href'>boa?board=<xsl:value-of select='@title'/></xsl:attribute><xsl:value-of select='@desc'/></xsl:when>
					<xsl:otherwise><xsl:attribute name='href'>doc?board=<xsl:value-of select='@title'/></xsl:attribute><xsl:value-of select='@desc'/></xsl:otherwise>
				</xsl:choose></a></td>
				<td class='bm'>
					<xsl:call-template name='splitbm'>
						<xsl:with-param name='names' select='@bm'/>
						<xsl:with-param name='isdir' select='@dir'/>
						<xsl:with-param name='isfirst' select='1'/>
					</xsl:call-template>
				</td>
			</tr>
		</xsl:for-each>
	</table>
</xsl:template>

<xsl:template match='bbssel'>
	<fieldset><legend>ѡ��������</legend><form action='sel' method='get'>
		<label for='brd'>���������ƣ�</label>
		<input type='text' name='brd' size='20' maxlength='20'/><br/>
		<input type='submit' value='�ύ��ѯ'/>
	</form></fieldset>
	<xsl:if test='count(brd)!=0'>
		<p>������: <xsl:value-of select='count(brd)'/>������</p>
		<table class='content'>
			<tr><th class='title'>����������</th><th class='desc'>��������</th></tr>
			<xsl:for-each select='brd'><xsl:sort select="@title"/><tr>
				<xsl:attribute name='class'>
					<xsl:if test='position() mod 2 = 1'>light</xsl:if>
					<xsl:if test='position() mod 2 = 0'>dark</xsl:if>
				</xsl:attribute>
				<td class='title'><a class='title'><xsl:choose>
					<xsl:when test='@dir="1"'><xsl:attribute name='href'>boa?board=<xsl:value-of select='@title'/></xsl:attribute>[ <xsl:value-of select='@title'/> ]</xsl:when>
					<xsl:otherwise><xsl:attribute name='href'>doc?board=<xsl:value-of select='@title'/></xsl:attribute><xsl:value-of select='@title'/></xsl:otherwise>
				</xsl:choose></a></td>
				<td class='desc'><a class='desc'><xsl:choose>
					<xsl:when test='@dir="1"'><xsl:attribute name='href'>boa?board=<xsl:value-of select='@title'/></xsl:attribute><xsl:value-of select='@desc'/></xsl:when>
					<xsl:otherwise><xsl:attribute name='href'>doc?board=<xsl:value-of select='@title'/></xsl:attribute><xsl:value-of select='@desc'/></xsl:otherwise>
				</xsl:choose></a></td>
			</tr></xsl:for-each>
		</table>
	</xsl:if>
	<xsl:if test='notfound'>û���ҵ����������İ���</xsl:if>
</xsl:template>

<xsl:template match='bbsdoc'>
	<xsl:choose>
		<xsl:when test='brd/@banner'><img><xsl:attribute name='src'><xsl:value-of select='brd/@banner'/></xsl:attribute></img></xsl:when>
		<xsl:otherwise><h2><xsl:if test='brd/@icon'><img><xsl:attribute name='src'><xsl:value-of select='brd/@icon'/></xsl:attribute></img></xsl:if><a><xsl:attribute name='href'><xsl:value-of select='brd/@link'/>doc?bid=<xsl:value-of select='brd/@bid'/></xsl:attribute><xsl:value-of select='brd/@desc'/> [<xsl:value-of select='brd/@title'/>]<xsl:if test='brd/@link = "g"'> - ��ժ��</xsl:if><xsl:if test='brd/@link = "t"'> - ����ģʽ</xsl:if></a></h2></xsl:otherwise>
	</xsl:choose>
	<p>
		<a><xsl:attribute name='href'>pst?bid=<xsl:value-of select='brd/@bid'/></xsl:attribute>[<img src='../images/button/edit.gif'/>��������]</a>
		<a><xsl:attribute name='href'>not?board=<xsl:value-of select='brd/@title'/></xsl:attribute>[���滭��]</a>
		<a><xsl:attribute name='href'>brdadd?bid=<xsl:value-of select='brd/@bid'/></xsl:attribute>[�ղر���]</a>
		&#160;���� [<xsl:call-template name='splitbm'><xsl:with-param name='names' select='brd/@bm'/><xsl:with-param name='isdir'>0</xsl:with-param><xsl:with-param name='isfirst' select='1'/></xsl:call-template>]  
		<xsl:choose><xsl:when test='brd/@link = "t"'>����</xsl:when><xsl:otherwise>����</xsl:otherwise></xsl:choose>�� [<xsl:choose><xsl:when test='brd/@total &gt; 0'><xsl:value-of select='brd/@total'/></xsl:when><xsl:otherwise>0</xsl:otherwise></xsl:choose>]
		<form class='jump' method='get'><xsl:attribute name='action'><xsl:value-of select='brd/@link'/>doc</xsl:attribute><input type='hidden' name='bid'><xsl:attribute name='value'><xsl:value-of select='brd/@bid'/></xsl:attribute></input><img src='../images/button/forward.gif'/>��ת��<input type='text' name='start' size='6'/>ƪ</form>
	</p>
	<xsl:apply-templates select='brd'/>
	<table class='content'>
		<tr><th class='no'>���</th><th class='mark'>���</th><th>����</th><th class='time'>����ʱ��</th><th class='ptitle'>����</th></tr>
		<xsl:for-each select='po'><tr>
			<xsl:attribute name='class'><xsl:choose><xsl:when test='position() mod 2 = 1'>light</xsl:when><xsl:otherwise>dark</xsl:otherwise></xsl:choose></xsl:attribute>
			<td class='no'><xsl:choose><xsl:when test='@sticky'>���ޡ�</xsl:when><xsl:otherwise><xsl:value-of select='position() - 1 + ../brd/@start'/></xsl:otherwise></xsl:choose></td>
			<td class='mark'><xsl:value-of select='@m'/></td>
			<td class='owner'><a class='owner'><xsl:attribute name='href'>qry?u=<xsl:value-of select='@owner'/></xsl:attribute><xsl:value-of select='@owner'/></a></td>
			<td class='time'><xsl:call-template name='timeconvert'><xsl:with-param name='time' select='@time'/></xsl:call-template></td>
			<xsl:variable name='imgsrc'>../images/types/<xsl:choose><xsl:when test='substring(., 1, 4) = "Re: "'>reply</xsl:when><xsl:otherwise>text</xsl:otherwise></xsl:choose>.gif</xsl:variable>
			<xsl:variable name='text'><xsl:choose><xsl:when test='substring(., 1, 4) = "Re: "'><xsl:value-of select='substring(., 5)'/></xsl:when><xsl:otherwise><xsl:value-of select='.'/></xsl:otherwise></xsl:choose></xsl:variable>
			<td class='ptitle'><a class='ptitle'>
				<xsl:attribute name='href'><xsl:value-of select='../brd/@link'/>con?bid=<xsl:value-of select='../brd/@bid'/>&amp;f=<xsl:value-of select='@id'/><xsl:if test='@sticky'>&amp;s=1</xsl:if></xsl:attribute>
				<img><xsl:attribute name='src'><xsl:value-of select='$imgsrc'/></xsl:attribute></img>
				<xsl:call-template name='ansi-escape'>
					<xsl:with-param name='content'><xsl:value-of select='$text'/></xsl:with-param>
					<xsl:with-param name='fgcolor'>37</xsl:with-param>
					<xsl:with-param name='bgcolor'>ignore</xsl:with-param>
					<xsl:with-param name='ishl'>0</xsl:with-param>
				</xsl:call-template>
			</a></td>
		</tr></xsl:for-each>
	</table>
	<xsl:apply-templates select='brd'/>
</xsl:template>

<xsl:template match='brd'>
	<a href='javascript:location=location'>[<img src='../images/button/reload.gif'/>ˢ��]</a>
	<xsl:if test='@start > 1'>
		<xsl:variable name='prev'><xsl:choose><xsl:when test='@start - @page &lt; 1'>1</xsl:when><xsl:otherwise><xsl:value-of select='@start - @page'/></xsl:otherwise></xsl:choose></xsl:variable>
		<a><xsl:attribute name='href'><xsl:value-of select='@link'/>doc?bid=<xsl:value-of select='@bid'/>&amp;start=<xsl:value-of select='$prev'/></xsl:attribute>[<img src='../images/button/up.gif'/>��һҳ]</a>
	</xsl:if>
	<xsl:if test='@total > @start + @page - 1'>
		<xsl:variable name='next'><xsl:value-of select='@start + @page'/></xsl:variable>
		<a><xsl:attribute name='href'><xsl:value-of select='@link'/>doc?bid=<xsl:value-of select='@bid'/>&amp;start=<xsl:value-of select='$next'/></xsl:attribute>[<img src='../images/button/down.gif'/>��һҳ]</a>
	</xsl:if>
	<a><xsl:attribute name='href'>clear?board=<xsl:value-of select='@title'/>&amp;start=<xsl:value-of select='@start'/></xsl:attribute>[���δ��]</a>
	<xsl:if test='@link != ""'><a><xsl:attribute name='href'>doc?bid=<xsl:value-of select='@bid'/></xsl:attribute>[<img src='../images/button/home.gif'/>һ��ģʽ]</a></xsl:if>
	<xsl:if test='@link != "t"'><a><xsl:attribute name='href'>tdoc?bid=<xsl:value-of select='@bid'/></xsl:attribute>[<img src='../images/button/content.gif'/>����ģʽ]</a></xsl:if>
	<xsl:if test='@link != "g"'><a><xsl:attribute name='href'>gdoc?bid=<xsl:value-of select='@bid'/></xsl:attribute>[��ժ��]</a></xsl:if>
	<a><xsl:attribute name='href'>0an?bid=<xsl:value-of select='@bid'/></xsl:attribute>[<img src='../images/announce.gif'/>������]</a>
	<a><xsl:attribute name='href'>bfind?bid=<xsl:value-of select='@bid'/></xsl:attribute>[<img src='../images/search.gif'/>��������]</a>
	<a><xsl:attribute name='href'>rss?bid=<xsl:value-of select='@bid'/></xsl:attribute>[RSS]</a>
</xsl:template>

<xsl:template match='bbscon'>
	<div class='post'>
		<div class='ptop'><xsl:call-template name='con-navbar'/></div>
		<div class='plink'><xsl:call-template name='con-linkbar'/></div>
		<div class='pmain'><xsl:call-template name='showpost'><xsl:with-param name='content' select='po'/></xsl:call-template></div>
		<div class='plink'><xsl:call-template name='con-linkbar'/></div>
		<div class='pbot'><a href='#top'>[<img src='../images/button/up.gif'/>��ҳ��]</a><xsl:call-template name='con-navbar'/></div>
	</div>
</xsl:template>

<xsl:template name='con-navbar'>
	<xsl:if test='@link != "con"'><a><xsl:attribute name='href'>gdoc?bid=<xsl:value-of select='@bid'/></xsl:attribute>[��ժ��]</a></xsl:if>
	<a><xsl:attribute name='href'>doc?bid=<xsl:value-of select='@bid'/></xsl:attribute>[<img src='../images/button/home.gif'/>��������]</a>
	<a><xsl:attribute name='href'>con?bid=<xsl:value-of select='@bid'/>&amp;f=<xsl:value-of select='po/@fid'/><xsl:if test='po/@sticky'>&amp;s=1</xsl:if></xsl:attribute>[��������]</a>
	<xsl:variable name='baseurl'>con?bid=<xsl:value-of select='@bid'/>&amp;f=<xsl:value-of select='po/@fid'/>&amp;a=</xsl:variable>
	<xsl:if test='not(po/@sticky)'><a><xsl:attribute name='href'><xsl:value-of select='$baseurl'/>p</xsl:attribute>[<img src='../images/button/up.gif'/>��һƪ]</a>
	<a><xsl:attribute name='href'><xsl:value-of select='$baseurl'/>n</xsl:attribute>[<img src='../images/button/down.gif'/>��һƪ]</a>
	<xsl:if test='po/@reid != f'><a><xsl:attribute name='href'><xsl:value-of select='$baseurl'/>b</xsl:attribute>[ͬ������ƪ]</a></xsl:if>
	<a><xsl:attribute name='href'><xsl:value-of select='$baseurl'/>a</xsl:attribute>[ͬ������ƪ]</a>
	<xsl:if test='po/@gid'><a><xsl:attribute name='href'>con?bid=<xsl:value-of select='@bid'/>&amp;f=<xsl:value-of select='po/@gid'/></xsl:attribute>[ͬ������ƪ]</a></xsl:if>
	<xsl:variable name='gid'><xsl:choose><xsl:when test='po/@gid'><xsl:value-of select='po/@gid'/></xsl:when><xsl:otherwise><xsl:value-of select='po/@fid'/></xsl:otherwise></xsl:choose></xsl:variable>
	<a><xsl:attribute name='href'>tcon?bid=<xsl:value-of select='@bid'/>&amp;f=<xsl:value-of select='$gid'/></xsl:attribute>[չ������]</a>
	<a><xsl:attribute name='href'>tcon?bid=<xsl:value-of select='@bid'/>&amp;g=<xsl:value-of select='$gid'/>&amp;f=<xsl:value-of select='po/@fid'/>&amp;a=n</xsl:attribute>[���չ��]</a></xsl:if>
	<a><xsl:attribute name='href'>../static/con?bid=<xsl:value-of select='@bid'/>&amp;f=<xsl:value-of select='po/@fid'/><xsl:if test='po/@sticky'>&amp;s=1</xsl:if></xsl:attribute>[����/��ӡ]</a>
</xsl:template>

<xsl:template name='con-linkbar'>
	<xsl:variable name='param'>bid=<xsl:value-of select='@bid'/>&amp;f=<xsl:value-of select='po/@fid'/></xsl:variable>
	<xsl:if test='@link = "con"'><a><xsl:attribute name='href'>pst?<xsl:value-of select='$param'/></xsl:attribute>[ <img src='../images/button/edit.gif'/>�ظ����� ]</a></xsl:if>
	<a><xsl:attribute name='href'>edit?<xsl:value-of select='$param'/></xsl:attribute>[ �޸� ]</a>
	<a><xsl:attribute name='href'>ccc?<xsl:value-of select='$param'/></xsl:attribute>[ ת�� ]</a>
	<a><xsl:attribute name='href'>fwd?<xsl:value-of select='$param'/></xsl:attribute>[ ת�� ]</a>
	<a><xsl:attribute name='href'>del?<xsl:value-of select='$param'/></xsl:attribute>[ ɾ�� ]</a>
</xsl:template>

<xsl:template match='bbstcon'>
	<xsl:for-each select='po'>
		<div class='post'>
			<div class='ptop'><xsl:call-template name='tcon-navbar'/></div>
			<div class='plink'><xsl:call-template name='tcon-linkbar'/></div>
			<div class='pmain'><xsl:call-template name='showpost'><xsl:with-param name='content' select='.'/></xsl:call-template></div>
			<div class='plink'><xsl:call-template name='tcon-linkbar'/></div>
			<div class='pbot'><a href='#top'>[<img src='../images/button/up.gif'/>��ҳ��]</a><xsl:call-template name='tcon-navbar'/></div>
		</div>
	</xsl:for-each>
</xsl:template>

<xsl:template name='tcon-navbar'>
	<a><xsl:attribute name='href'>gdoc?bid=<xsl:value-of select='../@bid'/></xsl:attribute>[��ժ��]</a>
	<a><xsl:attribute name='href'>tdoc?bid=<xsl:value-of select='../@bid'/></xsl:attribute>[<img src='../images/button/home.gif'/>��������]</a>
	<a><xsl:attribute name='href'>con?bid=<xsl:value-of select='../@bid'/>&amp;f=<xsl:value-of select='@fid'/></xsl:attribute>[<img src='../images/button/content.gif'/>��������]</a>
	<xsl:variable name='first'><xsl:value-of select='../po[1]/@fid'/></xsl:variable>
	<xsl:variable name='last'><xsl:value-of select='../po[last()]/@fid'/></xsl:variable>
	<xsl:if test='count(../po) = ../@page'><a><xsl:attribute name='href'>tcon?bid=<xsl:value-of select='../@bid'/>&amp;g=<xsl:value-of select='../@gid'/>&amp;f=<xsl:value-of select='$last'/>&amp;a=n</xsl:attribute>[<img src='../images/button/down.gif'/>��һҳ]</a></xsl:if>
	<xsl:if test='$first != ../@gid'><a><xsl:attribute name='href'>tcon?bid=<xsl:value-of select='../@bid'/>&amp;g=<xsl:value-of select='../@gid'/>&amp;f=<xsl:value-of select='$first'/>&amp;a=p</xsl:attribute>[<img src='../images/button/up.gif'/>��һҳ]</a></xsl:if>
</xsl:template>

<xsl:template name='tcon-linkbar'>
	<a><xsl:attribute name='href'>pst?bid=<xsl:value-of select='../@bid'/>&amp;f=<xsl:value-of select='@fid'/></xsl:attribute>[ <img src='../images/button/edit.gif'/>�ظ����� ]</a>
	<a><xsl:attribute name='href'>ccc?bid=<xsl:value-of select='../@bid'/>&amp;f=<xsl:value-of select='@fid'/></xsl:attribute>[ ת�� ]</a>
	<a><xsl:attribute name='href'>qry?u=<xsl:value-of select='@owner'/></xsl:attribute>[ ��ƪ����: <xsl:value-of select='@owner'/> ]</a>
</xsl:template>

<xsl:template match='bbsqry'>
	<form action='qry' method='get'><label for='u'>����������ѯ���ʺţ�</label><input type='text' name='u' maxlength='12' size='12'/><input type='submit' value='��ѯ'/></form>
	<xsl:choose><xsl:when test='@login'><div class='post'>
		<div class='ptop'><xsl:call-template name='qry-linkbar'/></div>
		<div class='umain' id='uinfo'>
		<p><strong><xsl:value-of select='@id'/></strong> ��<strong><xsl:value-of select='nick'/></strong>�� <xsl:call-template name='show-horo'/></p>
		<p>�ϴ���:��<span class='a132'><xsl:call-template name='timeconvert'><xsl:with-param name='time' select='@lastlogin'/></xsl:call-template></span>���ӡ�<span class='a132'><xsl:value-of select='ip'/></span>������վһ�Ρ�</p>
		<xsl:if test='logout'><p>��վ��:��<span class='a132'><xsl:call-template name='timeconvert'><xsl:with-param name='time' select='logout'/></xsl:call-template></span>��</p></xsl:if>
		<p>������:��<span class='a132'><xsl:value-of select='@post'/></span>�� ������:��<span class='a132'><xsl:value-of select='@hp'/></span>��</p> 
		<p>����ֵ:��<span class='a133'><xsl:value-of select='@perf'/></span>��</p>
		<p>����ֵ:��<xsl:call-template name="show-exp"/>�� (<xsl:value-of select='@level * 10 + @repeat'/>/60)</p>
		<p>���: <xsl:call-template name='ansi-escape'><xsl:with-param name='content'><xsl:value-of select='ident'/></xsl:with-param><xsl:with-param name='fgcolor'>37</xsl:with-param><xsl:with-param name='bgcolor'>ignore</xsl:with-param><xsl:with-param name='ishl'>0</xsl:with-param></xsl:call-template></p></div>
		<xsl:if test='st'><div class='usplit'>Ŀǰ״̬</div>
		<div class='umain'><xsl:for-each select='st'><p><strong><xsl:value-of select='@desc'/></strong><xsl:if test='@idle!=0'>[����<xsl:value-of select='@idle'/>����]</xsl:if><xsl:if test='@web=1'>��web��¼��</xsl:if><xsl:if test='@vis=0'>������</xsl:if></p></xsl:for-each></div></xsl:if>
		<div class='usplit'>����˵��������</div>
		<div class='usmd'><xsl:call-template name='showpost'><xsl:with-param name='content' select='smd'/></xsl:call-template></div>
		<div class='pbot'><xsl:call-template name='qry-linkbar'/></div>
	</div></xsl:when><xsl:otherwise><xsl:if test='@id!=""'><p>û���ҵ��û���<xsl:value-of select='@id'/>��</p></xsl:if></xsl:otherwise></xsl:choose>
</xsl:template>

<xsl:template name='show-horo'>
	<xsl:if test='@horo'>
		<xsl:variable name='color'><xsl:choose><xsl:when test='@gender = "M"'>a136</xsl:when><xsl:when test='@gender = "F"'>a135</xsl:when><xsl:otherwise>a132</xsl:otherwise></xsl:choose></xsl:variable>
		<span>��</span><span><xsl:attribute name='class'><xsl:value-of select='$color'/></xsl:attribute><xsl:value-of select='@horo'/></span><span>��</span>
	</xsl:if>
</xsl:template>

<xsl:template name='show-exp'>
	<span>
		<xsl:attribute name='class'>lev<xsl:value-of select='@level'/></xsl:attribute>
		<span><xsl:attribute name='class'>lev<xsl:value-of select='@level'/></xsl:attribute><xsl:attribute name='style'>width:<xsl:value-of select='@repeat * 10'/>%;</xsl:attribute></span>
	</span>
</xsl:template>

<xsl:template name='qry-linkbar'>
	<a><xsl:attribute name='href'>pstmail?recv=<xsl:value-of select='@id'/></xsl:attribute>[�����ż�]</a>
</xsl:template>

<xsl:template match='bbspst'>
	<p>���棺<xsl:value-of select='@brd'/></p>
	<form id='postform' name='postform' method='post'>
		<xsl:attribute name='action'>snd?bid=<xsl:value-of select='@bid'/>&amp;f=<xsl:value-of select='po/@f'/>&amp;e=<xsl:value-of select='@edit'/></xsl:attribute>
		<input type='hidden' id='brd'><xsl:attribute name='value'><xsl:value-of select='@brd'/></xsl:attribute></input>
		<p>���⣺<xsl:choose>
		<xsl:when test='@edit=0'><input class='binput' type='text' name='title' size='60' maxlength='50'>
			<xsl:variable name='retitle'>
				<xsl:choose>
					<xsl:when test='substring(t, 1, 4) = "Re: "'><xsl:value-of select='t'/></xsl:when>
					<xsl:when test='not(t)'></xsl:when>
					<xsl:otherwise><xsl:value-of select='concat("Re: ", t)'/></xsl:otherwise>
				</xsl:choose>
			</xsl:variable>
			<xsl:attribute name='value'>
				<xsl:call-template name='remove-ansi'>
					<xsl:with-param name='str' select='$retitle'/>
				</xsl:call-template>
			</xsl:attribute>
		</input></xsl:when>
		<xsl:otherwise><xsl:call-template name='remove-ansi'><xsl:with-param name='str' select='t'/></xsl:call-template></xsl:otherwise>
		</xsl:choose></p>
		<p>ǩ����: <input type='radio' name='sig' value='1' checked='checked'/>1 <input type='radio' name='sig' value='2'/>2 <input type='radio' name='sig' value='3'/>3 <input type='radio' name='sig' value='4'/>4 <input type='radio' name='sig' value='5'/>5 <input type='radio' name='sig' value='6'/>6</p>
		<p><textarea class='binput' name='text' rows='20' cols='85' wrap='virtual'>
			<xsl:if test='@edit=0'><xsl:text> &#x0d;&#x0a;</xsl:text></xsl:if>
			<xsl:call-template name='show-quoted'>
				<xsl:with-param name='content' select='po'/>
			</xsl:call-template>
		</textarea></p>
		<input type='submit' value='����' id='btnPost' size='10'/>
		<input type='reset' value='��ԭ' size='10'/>
		<xsl:if test='@edit="0" and @att!=0'><input type='button' name='attach' value='�ϴ�����' onclick='return preUpload() '/></xsl:if>
	</form>
	<xsl:choose>
		<xsl:when test='not(t)'><script type='text/javascript' defer='defer'>addLoadEvent(function(){document.postform.title.focus();})</script></xsl:when>
		<xsl:otherwise><script type='text/javascript' defer='defer'>addLoadEvent(function() {var text = document.postform.text; text.selectionStart = 0; text.selectionEnd = 1; text.focus();})</script></xsl:otherwise>
	</xsl:choose>
</xsl:template>

<xsl:template match='bbstop10'>
	<h2>24Сʱʮ�����Ż���</h2>
	<table class='content'>
		<tr><th class='no'>����</th><th class='owner'>����</th><th class='title'>����</th><th class='no'>ƪ��</th><th class='ptitle'>����</th></tr>
		<xsl:for-each select='top'><tr>
			<xsl:attribute name='class'><xsl:choose><xsl:when test='position() mod 2 = 1'>light</xsl:when><xsl:otherwise>dark</xsl:otherwise></xsl:choose></xsl:attribute>
			<td class='no'><xsl:value-of select='position()'/></td>
			<td class='owner'><a class='owner'><xsl:attribute name='href'>qry?u=<xsl:value-of select='@owner'/></xsl:attribute><xsl:value-of select='@owner'/></a></td>
			<td class='title'><a class='title'><xsl:attribute name='href'>doc?board=<xsl:value-of select='@board'/></xsl:attribute><xsl:value-of select='@board'/></a></td>
			<td class='no'><xsl:value-of select='@count'/></td>
			<td class='ptitle'><a class='ptitle'><xsl:attribute name='href'>tcon?board=<xsl:value-of select='@board'/>&amp;f=<xsl:value-of select='@gid'/></xsl:attribute><xsl:call-template name='ansi-escape'>	<xsl:with-param name='content' select='.'/><xsl:with-param name='fgcolor'>37</xsl:with-param><xsl:with-param name='bgcolor'>ignore</xsl:with-param><xsl:with-param name='ishl'>0</xsl:with-param></xsl:call-template></a></td>
		</tr></xsl:for-each>
	</table>
</xsl:template>

<xsl:template match='bbsbfind'>
	<h2>������������</h2>
	<xsl:variable name='count' select='count(po)'/>
	<xsl:choose><xsl:when test='@result'><p>���ҵ� <xsl:value-of select='$count'/> ƪ���� <xsl:if test='$count&gt;=100'>��100ƪ���ϲ���ʡ�ԣ�</xsl:if></p><xsl:if test='$count!=0'><p>�������¿�ǰ</p><table class='content'>
		<tr><th class='no'>���</th><th class='mark'>���</th><th class='owner'>����</th><th class='time'>����ʱ��</th><th class='ptitle'>����</th></tr>
		<xsl:for-each select='po'><tr>
			<xsl:attribute name='class'><xsl:choose><xsl:when test='position() mod 2 = 1'>light</xsl:when><xsl:otherwise>dark</xsl:otherwise></xsl:choose></xsl:attribute>
			<td class='no'><xsl:value-of select='position()'/></td>
			<td class='mark'><xsl:value-of select='@m'/></td>
			<td class='owner'><a class='owner'><xsl:attribute name='href'>qry?u=<xsl:value-of select='@owner'/></xsl:attribute><xsl:value-of select='@owner'/></a></td>
			<td class='time'><xsl:call-template name='timeconvert'><xsl:with-param name='time' select='@time'/></xsl:call-template></td>
			<xsl:variable name='imgsrc'>../images/types/<xsl:choose><xsl:when test='substring(., 1, 4) = "Re: "'>reply</xsl:when><xsl:otherwise>text</xsl:otherwise></xsl:choose>.gif</xsl:variable>
			<xsl:variable name='text'><xsl:choose><xsl:when test='substring(., 1, 4) = "Re: "'><xsl:value-of select='substring(., 5)'/></xsl:when><xsl:otherwise><xsl:value-of select='.'/></xsl:otherwise></xsl:choose></xsl:variable>
			<td class='ptitle'><a class='ptitle'>
				<xsl:attribute name='href'><xsl:value-of select='../brd/@link'/>con?bid=<xsl:value-of select='../@bid'/>&amp;f=<xsl:value-of select='@id'/></xsl:attribute>
				<img><xsl:attribute name='src'><xsl:value-of select='$imgsrc'/></xsl:attribute></img>
				<xsl:call-template name='ansi-escape'>
					<xsl:with-param name='content'><xsl:value-of select='$text'/></xsl:with-param>
					<xsl:with-param name='fgcolor'>37</xsl:with-param>
					<xsl:with-param name='bgcolor'>ignore</xsl:with-param>
					<xsl:with-param name='ishl'>0</xsl:with-param>
				</xsl:call-template>
			</a></td>
		</tr></xsl:for-each>
	</table></xsl:if></xsl:when>
	<xsl:otherwise><form action='bfind' method='get'><fieldset><legend>������дһ��</legend>
		<input name='bid' type='hidden'><xsl:attribute name='value'><xsl:value-of select='@bid'/></xsl:attribute></input>
		<p>���⺬��: <input name='t1' type='text' maxlength='50' size='20'/> �� <input name='t2' type='text' maxlength='50' size='20'/></p>
		<p>���ⲻ��: <input name='t3' type='text' maxlength='50' size='20'/></p>
		<p>�����ʺ�: <input name='user' type='text' maxlength='12' size='16'/></p></fieldset>
		<fieldset><legend>ѡ��</legend><p>ʱ�䷶Χ: <input name='limit' type='text' maxlength='4' size='4' value='7'/>������ (����30��)</p>
		<p>���±��: <input name='mark' type='checkbox'/></p>
		<p>��������: <input name='nore' type='checkbox'/></p></fieldset>
		<p><input type='submit' value='�����ѣ�'/></p>
	</form></xsl:otherwise></xsl:choose>
</xsl:template>

<xsl:template match='bbsnewmail'>
	<h2>�������ż�</h2>
	<p><a href='mail'><xsl:choose><xsl:when test='count(mail)=0'>��û��30���ڵ�δ���ż�</xsl:when><xsl:otherwise>��ҳ����ʾ30����δ���ż�</xsl:otherwise></xsl:choose>���鿴ȫ���ż����˴�</a></p>
	<xsl:if test='count(mail)!=0'><form name='list' method='post' action='mailman'>
		<table class='content'>
			<tr><th class='no'>���</th><th class='chkbox'>����</th><th class='owner'>������</th><th class='time'>����</th><th class='ptitle'>�ż�����</th></tr>
			<xsl:for-each select='mail'><tr>
				<xsl:attribute name='class'><xsl:choose><xsl:when test='position() mod 2 = 1'>light</xsl:when><xsl:otherwise>dark</xsl:otherwise></xsl:choose></xsl:attribute>
				<td class='no'><xsl:value-of select='@n'/></td>
				<td class='chkbox'><input type="checkbox"><xsl:attribute name='name'>box<xsl:value-of select='@name'/></xsl:attribute></input></td>
				<td class='owner'><a class='owner'><xsl:attribute name='href'>qry?u=<xsl:value-of select='@from'/></xsl:attribute><xsl:value-of select='@from'/></a></td>
				<td class='time'><xsl:call-template name='timeconvert'><xsl:with-param name='time' select='@date'/></xsl:call-template></td>
				<td class='ptitle'><a class='ptitle'>
					<xsl:attribute name='href'>mailcon?f=<xsl:value-of select='@name'/>&amp;n=<xsl:value-of select='@n'/></xsl:attribute>
					<xsl:call-template name='ansi-escape'>
						<xsl:with-param name='content'><xsl:value-of select='.'/></xsl:with-param>
						<xsl:with-param name='fgcolor'>37</xsl:with-param>
						<xsl:with-param name='bgcolor'>ignore</xsl:with-param>
						<xsl:with-param name='ishl'>0</xsl:with-param>
					</xsl:call-template>
				</a></td>
			</tr></xsl:for-each>
		</table>
		<input name='mode' value='' type='hidden'/>
	</form>
	<div>[<a href="#"  onclick="checkAll();">ȫѡ</a>] [<a href="#"  onclick="checkReverse();">��ѡ</a>] [<a href="#" onclick="delSelected()">ɾ����ѡ�ż�</a>]</div></xsl:if>
</xsl:template>

<xsl:template match='bbsmail'>
	<h2>�ż��б�</h2>
	<p>�ż����� [<xsl:value-of select='@total'/>]��</p>
	<form name='list' method='post' action='mailman'>
		<table class='content'>
			<tr><th class='no'>���</th><th class='chkbox'>����</th><th class='mark'>״̬</th><th class='owner'>������</th><th class='time'>����</th><th class='ptitle'>�ż�����</th></tr>
			<xsl:for-each select='mail'><tr>
				<xsl:attribute name='class'><xsl:choose><xsl:when test='position() mod 2 = 1'>light</xsl:when><xsl:otherwise>dark</xsl:otherwise></xsl:choose></xsl:attribute>
				<td class='no'><xsl:value-of select='position() - 1 + ../@start'/></td>
				<td class='chkbox'><input type="checkbox"><xsl:attribute name='name'>box<xsl:value-of select='@name'/></xsl:attribute></input></td>
				<td class='mark'><xsl:value-of select='@m'/></td>
				<td><a class='owner'><xsl:attribute name='href'>qry?u=<xsl:value-of select='@from'/></xsl:attribute><xsl:value-of select='@from'/></a></td>
				<td class='time'><xsl:call-template name='timeconvert'><xsl:with-param name='time' select='@date'/></xsl:call-template></td>
				<td class='ptitle'><a class='ptitle'>
					<xsl:attribute name='href'>mailcon?f=<xsl:value-of select='@name'/>&amp;n=<xsl:value-of select='position() - 1 + ../@start'/></xsl:attribute>
					<xsl:call-template name='ansi-escape'>
						<xsl:with-param name='content'><xsl:value-of select='.'/></xsl:with-param>
						<xsl:with-param name='fgcolor'>37</xsl:with-param>
						<xsl:with-param name='bgcolor'>ignore</xsl:with-param>
						<xsl:with-param name='ishl'>0</xsl:with-param>
					</xsl:call-template>
				</a></td>
			</tr></xsl:for-each>
		</table>
		<input name='mode' value='' type='hidden'/>
	</form>
	<div>[<a href="#"  onclick="checkAll();">��ҳȫѡ</a>] [<a href="#"  onclick="checkReverse();">��ҳ��ѡ</a>] [<a href="#" onclick="delSelected()">ɾ����ѡ�ż�</a>]</div>
	<div>
		<xsl:if test='@start &gt; 1'>
			<xsl:variable name='prev'>
				<xsl:choose><xsl:when test='@start - @page &lt; 1'>1</xsl:when><xsl:otherwise><xsl:value-of select='@start - @page'/></xsl:otherwise></xsl:choose>
			</xsl:variable>
			<a><xsl:attribute name='href'>mail?start=<xsl:value-of select='$prev'/></xsl:attribute>[ <img src='../images/button/up.gif'/>��һҳ ]</a>
		</xsl:if>
		<xsl:if test='@total &gt; @start + @page - 1'>
			<xsl:variable name='next'><xsl:value-of select='@start + @page'/></xsl:variable>
			<a><xsl:attribute name='href'>mail?start=<xsl:value-of select='$next'/></xsl:attribute>[ <img src='../images/button/down.gif'/>��һҳ ]</a>
		</xsl:if>
		<form><input value='��ת��' type='submit'/>��<input name='start' size='4' type='text'/>��</form>
	</div>
</xsl:template>

<xsl:template match='bbsmailcon'>
	<div class='post'>
		<div class='ptop'><xsl:call-template name='mailcon-navbar'/></div>
		<div class='plink'><xsl:call-template name='mailcon-linkbar'/></div>
		<div class='pmain'><xsl:call-template name='showpost'><xsl:with-param name='content' select='mail'/></xsl:call-template></div>
		<div class='plink'><xsl:call-template name='mailcon-linkbar'/></div>
		<div class='pbot'><xsl:call-template name='mailcon-navbar'/></div>
	</div>
	<xsl:if test='@new'><script type='text/javascript' defer='defer'>bbs.init();bbs.store.set('last', 0);</script></xsl:if>
</xsl:template>

<xsl:template name='mailcon-navbar'>
	<a href='mail'>[ <img src='../images/button/back.gif'/>�ż��б� ]</a>
	<xsl:if test='@prev'><a><xsl:attribute name='href'>mailcon?f=<xsl:value-of select='@prev'/>&amp;n=<xsl:value-of select='mail/@n - 1'/></xsl:attribute>[ <img src='../images/button/up.gif'/>��һ�� ]</a></xsl:if>
	<xsl:if test='@next'><a><xsl:attribute name='href'>mailcon?f=<xsl:value-of select='@next'/>&amp;n=<xsl:value-of select='mail/@n+1'/></xsl:attribute>[ <img src='../images/button/down.gif'/>��һ�� ]</a></xsl:if>
</xsl:template>

<xsl:template name='mailcon-linkbar'>
	<a><xsl:attribute name='href'>pstmail?n=<xsl:value-of select='mail/@n'/></xsl:attribute>[ <img src='../images/button/edit.gif'/>�ظ����� ]</a>
	<a onclick='return confirm("�����Ҫɾ���������")'><xsl:attribute name='href'>delmail?f=<xsl:value-of select='mail/@f'/></xsl:attribute>[ ɾ������ ]</a>
</xsl:template>

<xsl:template match='bbspstmail'>
	<form id='postform' name='postform' method='post' action='sndmail'>
		<input type='hidden' name='ref'><xsl:attribute name='value'><xsl:value-of select='@ref'/></xsl:attribute></input>
		<p><label for='recv'>������:&#160;&#160;&#160;</label><input class='binput' type='text' name='recv' size='15' maxlength='15'><xsl:attribute name='value'><xsl:value-of select='@recv'/></xsl:attribute></input></p>
		<p><label for='title'>�ż����� </label>
		<input class='binput' type='text' name='title' size='60' maxlength='50'>
			<xsl:variable name='retitle'>
				<xsl:choose>
					<xsl:when test='substring(t, 1, 4) = "Re: "'><xsl:value-of select='t'/></xsl:when>
					<xsl:when test='not(t)'></xsl:when>
					<xsl:otherwise><xsl:value-of select='concat("Re: ", t)'/></xsl:otherwise>
				</xsl:choose>
			</xsl:variable>
			<xsl:attribute name='value'>
				<xsl:call-template name='remove-ansi'>
					<xsl:with-param name='str' select='$retitle'/>
				</xsl:call-template>
			</xsl:attribute>
		</input></p>
		<p><textarea class='binput' name='text' rows='20' cols='85' wrap='virtual'>
			<xsl:text>&#x0d;&#x0a;</xsl:text>
			<xsl:call-template name='show-quoted'>
				<xsl:with-param name='content' select='m'/>
			</xsl:call-template>
		</textarea></p>
		<input type='submit' value='�ĳ�' id='btnPost' size='10'/>
		<input type='reset' value='����'/>
	</form>
</xsl:template>

<xsl:template match='bbs0an'>
	<p>��Ŀ¼web���������[<xsl:value-of select='@v'/>]</p>
	<table class='content'>
		<tr><th class='no'>���</th><th class='ptitle'>����</th><th class='bm'>������</th><th class='time'>����</th></tr>
		<xsl:for-each select='ent'><tr>
				<xsl:attribute name='class'><xsl:choose><xsl:when test='position() mod 2 = 1'>light</xsl:when><xsl:otherwise>dark</xsl:otherwise></xsl:choose></xsl:attribute>
				<td class='no'><xsl:value-of select='position()'/></td>
				<td class='ptitle'><xsl:choose>
					<xsl:when test='@t = "d"'><a class='ptitle'><xsl:attribute name='href'>0an?path=<xsl:value-of select='../@path'/><xsl:value-of select='@path'/></xsl:attribute><img src='../images/types/folder.gif'/><xsl:value-of select='.'/></a></xsl:when>
					<xsl:when test='@t = "f"'><a class='ptitle'><xsl:attribute name='href'>anc?path=<xsl:value-of select='../@path'/><xsl:value-of select='@path'/></xsl:attribute><img src='../images/types/text.gif'/><xsl:value-of select='.'/></a></xsl:when>
					<xsl:otherwise><img src='../images/types/error.gif'/><xsl:value-of select='@t'/></xsl:otherwise>
				</xsl:choose></td>
				<td class='bm'><xsl:if test='@id'>
					<xsl:call-template name='splitbm'>
						<xsl:with-param name='names' select='@id'/>
						<xsl:with-param name='isdir' select='0'/>
						<xsl:with-param name='isfirst' select='1'/>
					</xsl:call-template>
				</xsl:if></td>
				<td class='time'><xsl:if test='@t != "e"'><xsl:call-template name='timeconvert'><xsl:with-param name='time' select='@time'/></xsl:call-template></xsl:if></td>
		</tr></xsl:for-each>
		<xsl:if test='not(ent)'>
			<td/><td width='80%'>&lt;&lt;Ŀǰû������&gt;&gt;</td>
		</xsl:if>
	</table>
	<xsl:if test='@brd'><a><xsl:attribute name='href'>doc?board=<xsl:value-of select='@brd'/></xsl:attribute>[<img src='../images/button/home.gif'/>��������]</a></xsl:if>
</xsl:template>

<xsl:template match='bbsanc'>
	<h3><xsl:if test='@brd'><xsl:value-of select='@brd'/>�� - </xsl:if>�����������Ķ�</h3>
	<div class='post'>
		<div class='ptop'><xsl:call-template name='anc-navbar'/></div>
		<div class='pmain'><xsl:call-template name='showpost'><xsl:with-param name='content' select='.'/></xsl:call-template></div>
		<div class='pbot'><xsl:call-template name='anc-navbar'/></div>
	</div>
</xsl:template>

<xsl:template name='anc-navbar'>
	<xsl:if test='@brd'>
		<a><xsl:attribute name='href'>gdoc?board=<xsl:value-of select='@brd'/></xsl:attribute>[ ��ժ�� ]</a>
		<a><xsl:attribute name='href'>doc?board=<xsl:value-of select='@brd'/></xsl:attribute>[<img src='../images/button/home.gif'/>��������]</a>
	</xsl:if>
</xsl:template>

<xsl:template match='bbsfwd'>
	<form action='fwd' method='post'>
		<input type='hidden' name='bid'><xsl:attribute name='value'><xsl:value-of select='@bid'/></xsl:attribute></input>
		<input type='hidden' name='f'><xsl:attribute name='value'><xsl:value-of select='@f'/></xsl:attribute></input>
		<label for='u'>������:&#160;</label><input type='text' name='u' size='16'></input><br/>
		<input value='ת��' type='submit'/>
	</form>
</xsl:template>

<xsl:template match='bbsccc'>
	<xsl:choose>
		<xsl:when test='not(@bid)'>
			<p>ת�سɹ�</p>
			<p><a><xsl:attribute name='href'>doc?bid=<xsl:value-of select='@b'/></xsl:attribute>[ <img src='../images/button/back.gif'/>����ԭ�Ȱ��� ]</a></p>
			<p><a><xsl:attribute name='href'>doc?bid=<xsl:value-of select='@t'/></xsl:attribute>[ <img src='../images/button/forward.gif'/>����Ŀ����� ]</a></p>
		</xsl:when>
		<xsl:otherwise>
			<form method='get' action='ccc'>
				<p>���±���: <xsl:value-of select='.'/></p>
				<p>��������: <xsl:value-of select='@owner'/></p>
				<p>ԭʼ����: <xsl:value-of select='@brd'/></p>
				<input type='hidden' name='bid'><xsl:attribute name='value'><xsl:value-of select='@bid'/></xsl:attribute></input>
				<input type='hidden' name='f'><xsl:attribute name='value'><xsl:value-of select='@fid'/></xsl:attribute></input>
				<label for='t'>ת�ص�����: </label><input type='text' name='t'/>
				<input type='submit' value='ת��'/>
				<p><strong>ת��ע�⣺δ��վ��ίԱ����׼�������ת����ͬ���������³��������ģ����ܵ�ȫվ������</strong></p>
			</form>
		</xsl:otherwise>
	</xsl:choose>
</xsl:template>

<xsl:template match='bbsfall'>
	<h2>�趨��ע����</h2>
	<table class='content'>
		<tr><th class='owner'>�ʺ�</th><th class='chkbox'>����</th><th class='idesc'>˵��</th></tr>
		<xsl:for-each select='ov'><tr>
			<xsl:attribute name='class'><xsl:choose><xsl:when test='position() mod 2 = 1'>light</xsl:when><xsl:otherwise>dark</xsl:otherwise></xsl:choose></xsl:attribute>
			<td class='owner'><a class='owner'><xsl:attribute name='href'>qry?u=<xsl:value-of select='@id'/></xsl:attribute><xsl:value-of select='@id'/></a></td>
			<td class='chkbox'><a><xsl:attribute name='href'>fdel?u=<xsl:value-of select='@id'/></xsl:attribute>ɾ��</a></td>
			<td class='idesc'><xsl:value-of select='.'/></td>
		</tr></xsl:for-each>
	</table>
	<a href='fadd'>[���ӹ�ע����]</a>
</xsl:template>

<xsl:template match='bbsfadd'>
	<h2>���ӹ�ע����</h2>
	<form name='add' method='get' action='fadd'>
		<p><label for='id'>�ʺ�: </label><input class='binput' type='text' name='id' size='15' maxlength='15'></input></p>
		<p><label for='id'>˵��: </label><input class='binput' type='text' name='desc' size='50' maxlength='50'></input></p>
		<p><input type='submit' value='�ύ' size='10'/></p>
	</form>
</xsl:template>

<xsl:template match='bbsovr'>
	<table class='content'>
		<tr><th class='no'>���</th><th class='owner'>ʹ���ߴ���</th><th class='idesc'>�ǳ�</th><th>��վλ��</th><th>��̬</th><th>����</th></tr>
		<xsl:for-each select='ov'><xsl:sort select='@id'/><tr>
			<xsl:attribute name='class'><xsl:choose><xsl:when test='position() mod 2 = 1'>light</xsl:when><xsl:otherwise>dark</xsl:otherwise></xsl:choose></xsl:attribute>
			<td class='no'><xsl:value-of select='position()'/></td>
			<td class='owner'><a class='owner'><xsl:attribute name='href'>qry?u=<xsl:value-of select='@id'/></xsl:attribute><xsl:value-of select='@id'/></a></td>
			<td class='idesc'><xsl:value-of select='.'/></td>
			<td><xsl:value-of select='@ip'/></td>
			<td><xsl:value-of select='@action'/></td>
			<td><xsl:if test='@idle &gt; 0'><xsl:value-of select='@idle'/></xsl:if></td>
		</tr></xsl:for-each>
	</table>
</xsl:template>

<xsl:template match='bbsbrdadd'>
	<h2>����ղذ���</h2>
	<p>�ɹ���� <a><xsl:attribute name='href'>doc?bid=<xsl:value-of select='bid'/></xsl:attribute><xsl:value-of select='brd'/></a> �浽�ղؼ�</p>
</xsl:template>

<xsl:template match='bbsfav'>
	<h2>�ҵ��ղؼ�</h2>
	<p><a href='mybrd'>�Զ���</a></p>
	<table class='content'>
		<tr><th class='no'>���</th><th class='title'>����������</th><th class='desc'>��������</th></tr>
		<xsl:for-each select='brd'><tr>
			<xsl:attribute name='class'><xsl:choose><xsl:when test='position() mod 2 = 1'>light</xsl:when><xsl:otherwise>dark</xsl:otherwise></xsl:choose></xsl:attribute>
			<td class='no'><xsl:value-of select='position()'/></td>
			<td class='title'><a class='title'><xsl:attribute name='href'>doc?bid=<xsl:value-of select='@bid'/></xsl:attribute><xsl:value-of select='@brd'/></a></td>
			<td class='desc'><a class='desc'><xsl:attribute name='href'>doc?bid=<xsl:value-of select='@bid'/></xsl:attribute><xsl:value-of select='.'/></a></td>
		</tr></xsl:for-each>
	</table>
</xsl:template>

<xsl:template match='bbsmybrd'>
	<h2>�ղؼ��趨</h2>
	<xsl:choose>
		<xsl:when test='@selected'>
			<div>�޸�Ԥ���������ɹ���������һ��Ԥ���� <xsl:value-of select='@selected'/> ��������</div>
		</xsl:when>
		<xsl:otherwise>
			<form action='mybrd?type=1' method='post'>
				<table class='content'>
					<xsl:for-each select='mbrd'>
						<xsl:if test='position() mod 3 = 1'><tr>
							<xsl:apply-templates select='.'/><xsl:apply-templates select='following-sibling::mbrd[1]'/><xsl:apply-templates select='following-sibling::mbrd[2]'/>
						</tr></xsl:if>
					</xsl:for-each>
				</table>
				<input type='submit' value='ȷ��Ԥ��'/><input type='reset' value='��ԭ'/>
			</form>
		</xsl:otherwise>
	</xsl:choose>
</xsl:template>

<xsl:template match='mbrd'>
	<xsl:variable name='check'><xsl:call-template name='is-mybrd'><xsl:with-param name='bid' select='@bid'/></xsl:call-template></xsl:variable>
	<td class='idesc'>
		<input type='checkbox'>
			<xsl:attribute name='name'><xsl:value-of select='@bid'/></xsl:attribute>
			<xsl:if test='$check = 1'><xsl:attribute name='checked'>checked</xsl:attribute></xsl:if>
		</input>
		<a class='idesc'><xsl:attribute name='href'>doc?bid=<xsl:value-of select='@bid'/></xsl:attribute><xsl:value-of select='@desc'/></a>
	</td>
</xsl:template>

<xsl:template name='is-mybrd'>
	<xsl:param name='bid'/>
	<xsl:for-each select='../my'><xsl:if test='@bid = $bid'>1</xsl:if></xsl:for-each>
</xsl:template>

<xsl:template match='bbseufile'>
	<h2><xsl:value-of select='@desc'/></h2>
	<xsl:choose>
		<xsl:when test='@submit'>
			<form name='postform' method='post'>
				<xsl:attribute name='action'><xsl:value-of select='@submit'/></xsl:attribute>
				<p><textarea class='binput' name='text' rows='20' cols='85' wrap='virtual'><xsl:call-template name='show-quoted'><xsl:with-param name='content' select='.'/></xsl:call-template></textarea></p>
				<p><input type='submit' value='����' id='btnPost' size='10'/></p>
			</form>
		</xsl:when>
		<xsl:otherwise><p>����ɹ�</p><a href='javascript:history.go(-2)'>����</a></xsl:otherwise>
	</xsl:choose>
</xsl:template>

<xsl:template match='bbsinfo'>
	<xsl:choose><xsl:when test='@gender'>
		<fieldset><legend>�޸ĸ�������</legend><form action='info?type=1' method='post'>
			<p>�����ǳ�: <input type='text' name='nick' maxlength='30'><xsl:attribute name='value'><xsl:value-of select='nick'/></xsl:attribute></input></p>
			<p>��������: <input type='text' name='year' size='4' maxlength='4'><xsl:attribute name='value'><xsl:value-of select='@year + 1900'/></xsl:attribute></input> �� <input type='text' name='month' size='2' maxlength='2'><xsl:attribute name='value'><xsl:value-of select='@month'/></xsl:attribute></input> �� <input type='text' name='day' size='2' maxlength='2'><xsl:attribute name='value'><xsl:value-of select='@day'/></xsl:attribute></input> ��</p>
			<p>�û��Ա�: <input type='radio' value='M' name='gender'><xsl:if test='@gender = "M"'><xsl:attribute name='checked'>checked</xsl:attribute></xsl:if></input> �� <input type='radio' value='F' name='gender'><xsl:if test='@gender = "F"'><xsl:attribute name='checked'>checked</xsl:attribute></xsl:if></input> Ů</p>
			<input type='submit' value='ȷ��'/> <input type='reset' value='��ԭ'/>
		</form></fieldset>
		<p>��¼��վ: <xsl:value-of select='@login'/> ��</p>
		<p>��վʱ��: <xsl:value-of select='floor(@stay div 60)'/> Сʱ <xsl:value-of select='@stay mod 60'/> ����</p>
		<p>�������: <xsl:value-of select='@post'/> ƪ</p>
		<p>�ʺŽ���: <xsl:call-template name='timeconvert'><xsl:with-param name='time' select='@since'/></xsl:call-template></p>
		<p>�������: <xsl:call-template name='timeconvert'><xsl:with-param name='time' select='@last'/></xsl:call-template></p>
		<p>��Դ��ַ: <xsl:value-of select='@host'/></p>
	</xsl:when>
	<xsl:otherwise><xsl:choose><xsl:when test='string-length(.) = 0'>�޸ĸ������ϳɹ�<br/><a href='javascript:history.go(-2)'>����</a></xsl:when><xsl:otherwise><xsl:value-of select='.'/></xsl:otherwise></xsl:choose></xsl:otherwise>
	</xsl:choose>
</xsl:template>

<xsl:template match='bbspwd'>
	<xsl:choose>
		<xsl:when test='@i'><form action='pwd' method='post'>
			<p><label for='pw1'>���ľ�����: </label><input maxlength='12' size='12' type='password' name='pw1'/></p>
			<p><label for='pw2'>����������: </label><input maxlength='12' size='12' type='password' name='pw2'/></p>
			<p><label for='pw3'>ȷ��������: </label><input maxlength='12' size='12' type='password' name='pw3'/></p>
			<input type='submit' value='ȷ���޸�'/>
		</form></xsl:when>
		<xsl:otherwise>
			<xsl:choose><xsl:when test='string-length(.)=0'>�޸�����ɹ�<br/><a href='javascript:history.go(-2)'>����</a></xsl:when><xsl:otherwise><xsl:value-of select='.'/><br/><a href='javascript:history.go(-1)'>����</a></xsl:otherwise></xsl:choose>
		</xsl:otherwise>
	</xsl:choose>
</xsl:template>

<xsl:template match='bbsnot'>
	<h2>���滭�� - [<xsl:value-of select='@brd'/>]</h2>
	<div class='post'>
		<div class='ptop'><xsl:call-template name='not-navbar'/></div>
		<div class='pmain'><xsl:call-template name='showpost'><xsl:with-param name='content' select='.'/></xsl:call-template></div>
		<div class='pbot'><xsl:call-template name='not-navbar'/></div>
	</div>
</xsl:template>

<xsl:template name='not-navbar'>
	<xsl:if test='@brd'>
		<a><xsl:attribute name='href'>gdoc?board=<xsl:value-of select='@brd'/></xsl:attribute>[ ��ժ�� ]</a>
		<a><xsl:attribute name='href'>doc?board=<xsl:value-of select='@brd'/></xsl:attribute>[<img src='../images/button/home.gif'/>��������]</a>
	</xsl:if>
</xsl:template>
</xsl:stylesheet>
