#include "libweb.h"

static int show_file(const char *file)
{
	void *ptr;
	size_t size;
	int fd = mmap_open(file, MMAP_RDONLY, &ptr, &size);
	if (fd < 0)
		return -1;
	fputs("<post>", stdout);
	xml_fputs((char *)ptr, stdout);
	fputs("</post>\n", stdout);
	mmap_close(ptr, size, fd);
	return 0;
}

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

	xml_header("bbscon");
	printf("<bbscon>");
	char file[HOMELEN];
	setbfile(file, bp->filename, fh.filename);
	show_file(file);
	if (fh.reid != fh.id)
		printf("<reid>%u</reid>\n<gid>%u</gid>\n", fh.reid, fh.gid);
	printf("<bid>%d</bid><f>%u</f><link>con</link></bbscon>", bid, fid);
	return 0;
}

int bbsgcon_main(void)
{
	int bid = strtol(getparm("bid"), NULL, 10);
	struct boardheader *bp = getbcache2(bid);
	if (bp == NULL || !hasreadperm(&currentuser, bp))
		http_fatal2(HTTP_STATUS_NOTFOUND, "�����������");
	if (bp->flag & BOARD_DIR_FLAG)
		http_fatal("��ѡ�����һ��Ŀ¼");
	char *f = getparm("f");
	if (strstr(f, "..") || strstr(f, "/") || strncmp(f, "G.", 2))
		http_fatal("������ļ���");
	xml_header("bbscon");
	printf("<bbscon>");
	char file[HOMELEN];
	setbfile(file, bp->filename, f);
	show_file(file);
	printf("<bid>%d</bid><link>gcon</link></bbscon>", bid);
	return 0;
}
