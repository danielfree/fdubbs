<?xml version="1.0" encoding="gb2312"?>
<xsl:stylesheet version="2.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns="http://www.w3.org/1999/xhtml">
	<xsl:output method='html' encoding='gb2312' />
	<xsl:template match="bbsupload">
		<html>
			<head>
				<title>�ļ��ϴ��ɹ� - ���¹⻪</title>
				<meta http-equiv="content-type" content="text/html; charset=gb2312" />
				<link rel="stylesheet" type="text/css" href="/css/bbs0.css" />
				<script type='text/javascript' src='/js/bbsupload.js'></script>
			</head>
			<body>
				<p>�ļ��ϴ��ɹ�, ��ϸ��Ϣ����:</p>
				<p>�ϴ��ʺţ�<strong><xsl:value-of select='user' /></strong></p>
				<p>�ļ���ַ��<strong id='url'><xsl:value-of select='url' /></strong></p>
				<p>�ϴ��ļ����Զ������������http://ת�壬�뱣���Զ���Ӳ���ԭ������web������ת�岿�ֽ��Զ�ת��Ϊ��Ӧ������/ͼƬ</p>
				<p><a href='#' onclick='return closewin()'>����</a></p>
			</body>
		</html>
	</xsl:template>
</xsl:stylesheet>