#include "libweb.h"

int bbsanc_main()
{
	char *path = getparm("path");
	if (strstr(path, "bbslist") || strstr(path, ".Search")
			|| strstr(path, ".Names") || strstr(path, "..")
			|| strstr(path, "SYSHome"))
		http_fatal("�ļ�������");
	char *board = getbfroma(path);
	struct boardheader *bp = NULL;
	if (*board != '\0') {
		bp = getbcache(board);
		if (!hasreadperm(&currentuser, bp))
			http_fatal("�ļ������ڻ���Ȩ����");
	}

	char fname[512];
	sprintf(fname, "0Announce%s", path);
	void *ptr;
	size_t size;
	int fd;
	if (!safe_mmapfile(fname, O_RDONLY, PROT_READ, MAP_SHARED, &ptr, &size, &fd))
		http_fatal2(HTTP_STATUS_INTERNAL_ERROR, "���´�ʧ��");
	xml_header("bbsanc");
	fputs("<bbsanc><content>", stdout);
	xml_fputs((char *)ptr, stdout);
	end_mmapfile(ptr, size, fd);
	puts("</content>");
	if (bp != NULL)
		printf("<board>%s</board>", bp->filename);
	puts("</bbsanc>");
	return 0;
}
