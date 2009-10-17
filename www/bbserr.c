#include "libweb.h"

/**
 * Print HTTP error message with status code set.
 * @param status HTTP status code
 * @param prompt description of the error
 * @return 0
 */
static int http_fatal2(int status, const char *prompt)
{
	printf("Content-type: text/html; charset=%s\nStatus: %d\n\n",
			CHARSET, status);
	printf("<html><head><title>��������</title></head><body><div>%s</div>"
			"<a href=javascript:history.go(-1)>���ٷ���</a></body></html>",
			prompt);
	FCGI_Finish();
	return 0;
}

/**
 * Print HTTP error message with status OK (200).
 * @param prompt description of the error
 * @return 0
 */
static int http_fatal(const char *prompt)
{
	return http_fatal2(HTTP_STATUS_OK, prompt);
}

/**
 * Print error information.
 * @param err bbs errno
 * @return 0
 */
int check_bbserr(int err)
{
	if (err >= 0)
		return 0;
	switch (err) {
		case BBS_EINVAL:
			return http_fatal2(HTTP_STATUS_BADREQUEST, "��������");
		case BBS_ELGNREQ:
			return http_fatal("���ȵ�¼");
		case BBS_EACCES:
			return http_fatal("Ȩ�޲���");
		case BBS_EPST:
			return http_fatal("���Ĳ��ɻظ�������û�з���Ȩ��");
		case BBS_ENOFILE:
			return http_fatal("�Ҳ���ָ�����ļ�");
		case BBS_ENODIR:
			return http_fatal("�Ҳ���ָ����Ŀ¼");
		case BBS_ENOBRD:
			return http_fatal("�Ҳ���ָ���İ��棬��Ȩ�޲���");
		case BBS_ENOUSR:
			return http_fatal("�Ҳ���ָ�����û�");
		case BBS_ENOURL:
			return http_fatal("�Ҳ���ָ������ַ");
		case BBS_EDUPLGN:
			return http_fatal("���ڱ����Ѿ���¼��һ���ʺţ������˳�");
		case BBS_EWPSWD:
			return http_fatal("�û��������벻ƥ��");
		case BBS_EBLKLST:
			return http_fatal("���ڶԷ��ĺ�������");
		case BBS_ELGNQE:
			return http_fatal("�����ܵ�¼�����ʺ���");
		case BBS_EBRDQE:
			return http_fatal("�ղؼа������Ѵ�����");
		case BBS_EATTQE:
			return http_fatal("���渽������������");
		case BBS_EMAILQE:
			return http_fatal("��������");
		case BBS_EFRNDQE:
			return http_fatal("���ĺ������Ѵ�����");
		case BBS_EFBIG:
			return http_fatal("�ļ���С��������");
		case BBS_ELFREQ:
			return http_fatal("��¼����Ƶ��");
		case BBS_EPFREQ:
			return http_fatal("���Ĺ���Ƶ��");
		case BBS_E2MANY:
			return http_fatal("�����û����Ѵ�����");
		case BBS_EINTNL:
			return http_fatal2(HTTP_STATUS_INTERNAL_ERROR, "�ڲ�����");
		case BBS_ERMQE:
			return http_fatal("�Է���������");
		default:
			return http_fatal("δ֪����");
	}
}
