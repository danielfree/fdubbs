<?xml version="1.0" encoding="gb2312"?>
<xsl:stylesheet version="1.0" xmlns:xsl="http://www.w3.org/1999/XSL/Transform" xmlns="http://www.w3.org/1999/xhtml">
	<xsl:output method='html' encoding='gb2312' />
	<xsl:template match="bbsccc">
		<html>
			<head>
				<title>ת������ - ���¹⻪BBS</title>
				<meta http-equiv="content-type" content="text/html; charset=gb2312" />
			</head>
			<body>
				<xsl:choose>
					<xsl:when test='not(bid)'><p>ת�سɹ�</p><a href='javascript:history.go(-1)'>���ٷ���</a></xsl:when>
					<xsl:otherwise>
						<form method='get' action='ccc'>
							<p><strong>���±���: </strong><xsl:value-of select='title' /></p>
							<p><strong>��������: </strong><xsl:value-of select='author' /></p>
							<p><strong>ԭʼ����: </strong><xsl:value-of select='board' /></p>
							<input type='hidden' name='bid'><xsl:attribute name='value'><xsl:value-of select='bid' /></xsl:attribute></input>
							<input type='hidden' name='f'><xsl:attribute name='value'><xsl:value-of select='f' /></xsl:attribute></input>
							<label for='t'>ת�ص�����: </label><input type='text' name='t' />
							<input type='submit' value='ת��' />
							<p><strong>ת��ע�⣺δ��վ��ίԱ����׼�������ת����ͬ���������³��������ģ����ܵ�ȫվ������</strong></p>
						</form>
					</xsl:otherwise>
				</xsl:choose>
			</body>
		</html>
	</xsl:template>
</xsl:stylesheet>
