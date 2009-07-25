#include "libweb.h"

int bbsnot_main(void)
{
	struct boardheader *bp = getbcache(getparm("board"));
	if (bp == NULL || !hasreadperm(&currentuser, bp))
		http_fatal2(HTTP_STATUS_NOTFOUND, "�����������");
	if (bp->flag & BOARD_DIR_FLAG)
		http_fatal("��ѡ�����һ��Ŀ¼");
	char fname[HOMELEN];
	snprintf(fname, sizeof(fname), "vote/%s/notes", bp->filename);
	void *ptr;
	size_t size;
	int fd = mmap_open(fname, MMAP_RDONLY, &ptr, &size);
	if (fd < 0)
		http_fatal2(HTTP_STATUS_NOTFOUND, "�����������޽��滭��");
	xml_header("bbsnot");
	printf("<bbsnot><content>");
	xml_fputs((char *)ptr, stdout);
	mmap_close(ptr, size, fd);
	printf("</content><board>%s</board></bbsnot>", bp->filename);
	return 0;
}
