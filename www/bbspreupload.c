#include "libweb.h"

int bbspreupload_main(void)
{
	char *board = getparm("board");
	if (!loginok)
		http_fatal("�Ҵҹ�����Ȩ���ϴ��ļ�");
	struct boardheader *bp = getbcache(board);
	if (bp == NULL || !haspostperm(&currentuser, bp))
		http_fatal("���������������Ȩ�ϴ��ļ�����������");

	xml_header("bbspreupload");
	printf("<bbspreupload><board>%s</board><user>%s</user><max>%d</max>"
			"</bbspreupload>", board, currentuser.userid, maxlen(board));
	return 0;
}
