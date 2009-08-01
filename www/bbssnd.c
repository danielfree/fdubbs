#include "libweb.h"

int bbssnd_main(void)
{
	parse_post_data();
	int bid = strtol(getparm("bid"), NULL, 10);
	struct boardheader *bp = getbcache2(bid);

	if (!loginok)
		http_fatal("�Ҵҹ��Ͳ��ܷ������£����ȵ�¼");
	if (bp == NULL || !haspostperm(&currentuser, bp))
		http_fatal("�����������������Ȩ�ڱ�����������");
	if (bp->flag & BOARD_DIR_FLAG)
		http_fatal("��ѡ�����һ��Ŀ¼");

	unsigned int fid;
	struct fileheader fh;
	char *f = getparm("f");
	bool reply = !(*f == '\0');
	if (reply) {
		fid = strtoul(f, NULL, 10);
		if (!bbscon_search(bp, fid, 0, &fh))
			http_fatal("���������");
		if (fh.accessed[0] & FILE_NOREPLY)
			http_fatal("�����¾��в��ɻظ�����");
	}

	char title[sizeof(fh.title)];
	char *t = getparm("title");
	if (*t == '\0')
		http_fatal("���±��ⲻ��Ϊ��");
	else
		strlcpy(title, t, sizeof(title));
	ansi_filter(title, title);

// TODO: ...
#ifdef SPARC
		if(abs(time(0) - *(int*)(u_info->from+34))<6) { //modified from 36 to 34 for sparc solaris by roly 02.02.28
			*(int*)(u_info->from+34)=time(0); //modified from 36 to 34 for sparc solaris by roly 02.02.28
			http_fatal("���η��ļ������, ����Ϣ���������");
		}
		*(int*)(u_info->from+34)=time(0);//modified from 36 to 34 for sparc solaris by roly 02.02.28
#else
		if(abs(time(0) - *(int*)(u_info->from+36))<6) { //modified from 36 to 34 for sparc solaris by roly 02.02.28
			*(int*)(u_info->from+36)=time(0); //modified from 36 to 34 for sparc solaris by roly 02.02.28
			http_fatal("���η��ļ������, ����Ϣ���������");
		}
		*(int*)(u_info->from+36)=time(0);//modified from 36 to 34 for sparc solaris by roly 02.02.28
#endif
	if (post_article(&currentuser, bp, title, 
			getparm("text"), fromhost, reply ? &fh : NULL) < 0)
		http_fatal2(HTTP_STATUS_INTERNAL_ERROR, "��������ʧ��");

	if (!junkboard(bp)) {
		currentuser.numposts++;
		save_user_data(&currentuser);
	}

	char buf[sizeof(fh.title) + sizeof(bp->filename)];
	snprintf(buf, sizeof(buf), "posted '%s' on %s", title, bp->filename);
	report(buf, currentuser.userid);

	snprintf(buf, sizeof(buf), "bbsdoc?board=%s", bp->filename);
	http_header();
	refreshto(1, buf);
	printf("</head>\n<body>����ɹ���1���Ӻ��Զ�ת��<a href='%s'>����</a>\n"
			"</body>\n</html>\n", buf);
	return 0;
}

