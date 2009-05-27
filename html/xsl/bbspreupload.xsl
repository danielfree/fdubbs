<?xml version="1.0" encoding="gb2312"?>
<xsl:stylesheet version="2.0"
xmlns:xsl="http://www.w3.org/1999/XSL/Transform"
xmlns="http://www.w3.org/1999/xhtml">
	<xsl:output method='html' encoding='gb2312' />
	<xsl:template match="bbspreupload">
		<html>
			<head>
				<title>�ϴ��ļ� - ���¹⻪</title>
				<meta http-equiv="content-type" content="text/html; charset=gb2312" />
				<link rel="stylesheet" type="text/css" href="/css/bbs0.css" />
				<script type='text/javascript' src='/js/bbspreupload.js'></script>
			</head>
			<body>
				<p>�ϴ��ļ���[<strong><xsl:value-of select='board' /></strong>]������</p>
				<p>�ʺţ�[<strong><xsl:value-of select='user' /></strong>]</p>
				<p>Ŀǰ���浥���ϴ��ļ���С����Ϊ[<strong><xsl:value-of select='floor(max div 1024)' /></strong>]KB</p>
				<p>��������Դ���ޣ������ϴ�������޹ص��ļ���</p>
				<p>�����ع��ҷ��ɣ��Ͻ��ϴ��Ƿ����ϺͿ��ܵ��¾��׵����ϡ�</p>
				<p>������ļ����ͣ�jpeg/jpg/gif/png/pdf</p>
				<form method='post' name='upform' enctype='multipart/form-data'>
					<xsl:attribute name='action'>bbsupload?b=<xsl:value-of select='board' /></xsl:attribute>
					<span>�ϴ��ļ�: </span><input type='file' name='up'></input>
					<input type='button' value='�ϴ�' onclick='return clickup()' />
				</form>
			</body>
		</html>
	</xsl:template>
</xsl:stylesheet>