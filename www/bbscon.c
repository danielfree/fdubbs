#include "libweb.h"

int bbscon_main(void)
{
	int bid = strtol(getparm("bid"), NULL, 10);
	struct boardheader *bp = getbcache2(bid);
	if (bp == NULL || !hasreadperm(&currentuser, bp))
		http_fatal2(HTTP_STATUS_NOTFOUND, "�����������");
	if (bp->flag & BOARD_DIR_FLAG)
		http_fatal("��ѡ�����һ��Ŀ¼");
	unsigned int fid = strtoul(getparm("f"), NULL, 10);
	char *action = getparm("a");
	struct fileheader fh;
	if (!bbscon_search(bp, fid, *action, &fh))
		http_fatal2(HTTP_STATUS_NOTFOUND, "û���ҵ�ָ��������");
	fid = fh.id;

	char file[HOMELEN];
	setbfile(file, bp->filename, fh.filename);
	void *ptr;
	size_t size;
	int fd;
	if (!safe_mmapfile(file, O_RDONLY, PROT_READ, MAP_SHARED, &ptr, &size, &fd))
		http_fatal2(HTTP_STATUS_INTERNAL_ERROR, "���´�ʧ��");
	xml_header("bbscon");
	fputs("<bbscon>\n<post>", stdout);
	xml_fputs((char *)ptr, stdout);
	fputs("</post>\n", stdout);
	end_mmapfile(ptr, size, fd);
	if (fh.reid != fh.id)
		printf("<reid>%u</reid>\n<gid>%u</gid>\n", fh.reid, fh.gid);
	printf("<bid>%d</bid>\n<f>%u</f>\n</bbscon>", bid, fid);
	return 0;
}
