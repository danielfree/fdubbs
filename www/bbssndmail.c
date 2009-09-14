#include "libweb.h"

int bbssndmail_main(void)
{
	if (!loginok)
		return BBS_ELGNREQ;
	if (!HAS_PERM2(PERM_MAIL, &currentuser))
		return BBS_EACCES;
	if (parse_post_data() < 0)
		return BBS_EINVAL;
	// TODO: mail quota
	const char *recv = getparm("recv");
	if (*recv == '\0')
		return BBS_EINVAL;
	const char *title = getparm("title");
	if (*title == '\0')
		title = "û����";
	const char *text = getparm("text");
	int len = strlen(text);
	char header[320];
	snprintf(header, sizeof(header), "������: %s (%s)\n��  ��: %s\n����վ: "
			BBSNAME" (%s)\n��  Դ: %s\n\n", currentuser.userid,
			currentuser.username, title, getdatestring(time(NULL), DATE_ZH),
			mask_host(fromhost));
	// TODO: signature, error code, backup?
	if (do_mail_file(recv, title, header, text, len, NULL) < 0)
		return BBS_EINVAL;
	const char *ref = getparm("ref");
	http_header();
	refreshto(1, ref);
	printf("</head>\n<body>����ɹ���1���Ӻ��Զ�ת��<a href='%s'>ԭҳ��</a>\n"
			"</body>\n</html>\n", ref);
	return 0;
}
