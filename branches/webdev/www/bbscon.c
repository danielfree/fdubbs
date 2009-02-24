#include "libweb.h"

int bbscon_main(void)
{
	int bid = strtol(getparm("bid"), NULL, 10);
	struct boardheader *bp = getbcache2(bid);
	if (bp == NULL || !hasreadperm(&currentuser, bp))
		http_fatal(HTTP_STATUS_NOTFOUND, "�����������");
	if (bp->flag & BOARD_DIR_FLAG)
		http_fatal(HTTP_STATUS_BADREQUEST, "��ѡ�����һ��Ŀ¼");

	int fid = strtol(getparm("f"), NULL, 10);
	return 0;
}
