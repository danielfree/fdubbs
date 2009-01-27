#include "libweb.h"

static char *MENUPARENT="<a href=\"#\" style=\"text-decoration: none;\" onclick=\"return SwitchPanel('%s')\">\n<img src=\"%s\">%s\n</a>\n";
static char *MENUDROP_BEGIN="<div id=\"%s\">\n";
static char *MENUITEM="<a href=\"%s\" style=\"text-decoration: none;\" target=\"%s\">\n<img src=\"%s\">%s\n</a>\n";
static char *MENUSUBITEM="<a href=\"%s\" style=\"text-decoration: none;\" target=\"%s\">\n<img src=\"%s\">%s\n</a>\n";
static char *MENUSUBITEM_NOIMAGE="<a href=\"%s\" target=\"%s\">\n%s\n</a>\n";
static char *FRAME_VIEW="view";

static void BeginMenuDrop(char *mid, char *img, char *title)
{
	printf(MENUPARENT, mid, img, title);
	printf(MENUDROP_BEGIN,mid);
}

static void EndMenuDrop(void)
{
	printf("</div>");
}

int bbsleft_main(void)
{
	printf("<title>��ӭ�������¹⻪BBS</title>\n");
	printf("<link type=\"text/css\" rel=\"stylesheet\" href=\"/css/bbsleft.css\">\n");
	printf("<script type=\"text/javascript\" src=\"/js/bbsleft.js\"></script>\n");

	printf("<body>\n");
	printf("<div style=\"height:100%%\">");
	printf("<a href=\"#\" onclick=\"switch_bar()\" id=\"switchbar\"></a>");
	printf("<div id=\"mainbar\">");

	printf(MENUITEM, CGIPATH"bbs0an", FRAME_VIEW, "/images/announce.gif", "��վ����");
	printf(MENUITEM, CGIPATH"bbsall", FRAME_VIEW, "/images/penguin.gif", "ȫ������");

	BeginMenuDrop("Stat", "/images/top10.gif", "ͳ������");
	printf(MENUSUBITEM, CGIPATH"bbstop10", FRAME_VIEW, "/images/blankblock.gif","����ʮ��");
	printf(MENUSUBITEM, CGIPATH"bbstopb10", FRAME_VIEW, "/images/blankblock.gif","��������");
	printf(MENUSUBITEM, CGIPATH"bbsuserinfo", FRAME_VIEW, "/images/blankblock.gif","����ͳ��");
	EndMenuDrop();

	if(loginok) {
		char buf[HOMELEN];
		FILE *fp;
		char *cgi = "bbsdoc";
		if (atoi(getparm("my_def_mode")) != 0)
			cgi = "bbstdoc";
		BeginMenuDrop("Favorite", "/images/favorite.gif", "�ҵ��ղ�");
		printf(MENUSUBITEM, CGIPATH"bbsmybrd", FRAME_VIEW, "/images/blankblock.gif", "Ԥ������");
		sethomefile(buf, currentuser.userid, ".goodbrd");
		fp = fopen(buf, "rb");
		if (fp != NULL) {
			char path[LINKLEN];
			struct goodbrdheader gbhd;
			int brdcount = 0;
			while (fread(&gbhd, sizeof(struct goodbrdheader), 1, fp)) {
				if (gbhd.flag & BOARD_CUSTOM_FLAG)
					continue;
				if (bcache[gbhd.pos].flag & BOARD_DIR_FLAG)
					sprintf(path, CGIPATH"%s?board=%s", "bbsboa", bcache[gbhd.pos].filename);
				else
					sprintf(path, CGIPATH"%s?board=%s", cgi, bcache[gbhd.pos].filename);
				printf(MENUSUBITEM_NOIMAGE, path,FRAME_VIEW,bcache[gbhd.pos].filename); 
				brdcount++;
				if(brdcount >= GOOD_BRC_NUM)
					break;
			}
			fclose(fp);
		}
		EndMenuDrop();
	}

	BeginMenuDrop("EGroup", "/images/egroup.gif", "��������");
	{
		int i;
		char path[LINKLEN];
		char name[STRLEN];
		for(i = 0; i < SECNUM; i++)
		{
			sprintf(path, CGIPATH"bbsboa?s=%d", i);
			sprintf(name, "%X %s", i, secname[i][0]);
			printf(MENUSUBITEM, path, FRAME_VIEW, "/images/types/folder1.gif", name);
		}
	}
	EndMenuDrop();

	BeginMenuDrop("QueQiao", "/images/chat.gif", "ȵ�����");
	if (loginok)
		printf(MENUSUBITEM,CGIPATH"bbsfriend", FRAME_VIEW, "/images/blankblock.gif", "���ߺ���");
	printf(MENUSUBITEM, CGIPATH"bbsusr", FRAME_VIEW, "/images/blankblock.gif", "�����ķ�");
	if(currentuser.userlevel & PERM_TALK) {
		printf(MENUSUBITEM, CGIPATH"bbssendmsg",FRAME_VIEW, "/images/blankblock.gif", "����ѶϢ");
		printf(MENUSUBITEM, CGIPATH"bbsmsg",FRAME_VIEW, "/images/blankblock.gif", "�鿴����ѶϢ");
	}
	EndMenuDrop();

	if(loginok) {
		BeginMenuDrop("Config", "/images/config.gif", "��������");
		printf(MENUSUBITEM, CGIPATH"bbsinfo", FRAME_VIEW, "/images/blankblock.gif", "��������");
		printf(MENUSUBITEM, CGIPATH"bbsplan", FRAME_VIEW, "/images/blankblock.gif", "��˵����");
		printf(MENUSUBITEM, CGIPATH"bbssig", FRAME_VIEW, "/images/blankblock.gif", "��ǩ����");
		printf(MENUSUBITEM, CGIPATH"bbsmywww", FRAME_VIEW, "/images/blankblock.gif", "WWW����");
		printf(MENUSUBITEM, CGIPATH"bbspwd", FRAME_VIEW, "/images/blankblock.gif", "�޸�����");
		printf(MENUSUBITEM, CGIPATH"bbsnick", FRAME_VIEW, "/images/blankblock.gif", "��ʱ���ǳ�");
		printf(MENUSUBITEM, CGIPATH"bbsfall", FRAME_VIEW, "/images/blankblock.gif", "�趨����");
		if (currentuser.userlevel & PERM_CLOAK)
			printf(MENUSUBITEM, CGIPATH"bbscloak", FRAME_VIEW, "/images/blankblock.gif","�л�����");
		EndMenuDrop();

		BeginMenuDrop("Mail", "/images/mail.gif", "�����ż�");
		printf(MENUSUBITEM, CGIPATH"bbsnewmail", FRAME_VIEW, "/images/mail_new.gif", "�������ż�");
		printf(MENUSUBITEM, CGIPATH"bbsmail", FRAME_VIEW, "/images/mail.gif", "�����ż�");
		printf(MENUSUBITEM, CGIPATH"bbsmaildown", FRAME_VIEW, "/images/mail_get.gif", "�����ż�");
		printf(MENUSUBITEM, CGIPATH"bbspstmail", FRAME_VIEW, "/images/mail_write.gif", "�����ż�");
		EndMenuDrop();
	}

	BeginMenuDrop("Search", "/images/search.gif", "����ѡ��");
	if (HAS_PERM(PERM_OBOARDS) && HAS_PERM(PERM_SPECIAL0))
		printf(MENUSUBITEM, CGIPATH"bbsfind", FRAME_VIEW, "/images/blankblock.gif", "��������");
	if (loginok)
		printf(MENUSUBITEM, CGIPATH"bbsqry", FRAME_VIEW, "/images/blankblock.gif", "��ѯ����");
	printf(MENUSUBITEM, CGIPATH"bbssel", FRAME_VIEW, "/images/blankblock.gif", "����������");
	EndMenuDrop();

	printf(MENUITEM, "telnet://"BBSHOST":2323", "_top", "/images/telnet.gif", "�ն˵�¼");
	if(loginok)
		printf(MENUITEM, CGIPATH"bbslogout", "_top", "/images/exit.gif", "ע����¼");

	printf("</body>\n</html>");

	return 0;
}
