#include "libweb.h"

static bool bbscon_search(struct boardheader *bp, unsigned int fid, struct fileheader *fp)
{
	if (bp == NULL)
		return false;
	char dir[HOMELEN];
	setwbdir(dir, bp->filename);
	void *ptr;
	size_t size;
	int fd;
	if (!safe_mmapfile(dir, O_RDONLY, PROT_READ, MAP_SHARED, &ptr, &size, &fd))
		return false;
	// Binary search.
	struct fileheader *begin = ptr, *end, *mid;
	end = begin + (size / sizeof(struct fileheader)) - 1;
	while (begin <= end) {
		mid = begin + (end - begin) / 2;
		if (mid->id == fid) {
			*fp = *mid;
			end_mmapfile(ptr, size, fd);
			return true;
		}
		if (mid->id < fid) {
			begin = mid + 1;
		} else {
			end = mid - 1;
		}
	}
	end_mmapfile(ptr, size, fd);
	return false;
}

int bbscon_main(void)
{
	int bid = strtol(getparm("bid"), NULL, 10);
	struct boardheader *bp = getbcache2(bid);
	if (bp == NULL || !hasreadperm(&currentuser, bp))
		http_fatal(HTTP_STATUS_NOTFOUND, "�����������");
	if (bp->flag & BOARD_DIR_FLAG)
		http_fatal(HTTP_STATUS_BADREQUEST, "��ѡ�����һ��Ŀ¼");
	unsigned int fid = strtoul(getparm("f"), NULL, 10);
	struct fileheader fh;
	if (!bbscon_search(bp, fid, &fh))
		http_fatal(HTTP_STATUS_NOTFOUND, "���������");

	char file[HOMELEN];
	setbfile(file, bp->filename, fh.filename);
	void *ptr;
	size_t size;
	int fd;
	if (!safe_mmapfile(file, O_RDONLY, PROT_READ, MAP_SHARED, &ptr, &size, &fd))
		http_fatal(HTTP_STATUS_INTERNAL_ERROR, "���´�ʧ��");
	xml_header("bbscon");
	fputs("<bbscon>\n<post>", stdout);
	xml_fputs((char *)ptr, stdout);
	fputs("</post>\n", stdout);
	end_mmapfile(ptr, size, fd);
	printf("<bid>%d</bid>\n<f>%u</f>\n</bbscon>", bid, fid);
	return 0;
}
