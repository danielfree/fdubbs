#include "libweb.h"

static char *BEGIN_MENU="stm_bm([\"noname\",400,\"\",\"/images/blank.gif\",0,\"\",\"\",0,0,0,0,50,1,0,0]);";
static char *BEGIN_SUBMENU="stm_bp(\"submenu\",[1,4,0,0,0,2,0,7,100,\"\",-2,\"\",-2,90,0,0,\"#999999\",\"#ffffff\",\"\",3,0,0,\"#ffffff\"]);";
static char *BEGIN_POPUPMENU="stm_bp(\"popupmenu\",[1,2,-3,-3,0,3,0,7,100,\"\",-2,\"\",-2,70,2,3,\"#999999\",\"#336699\",\"\",3,1,1,\"#B1C9F1\"]);";
static char *SUBMENU_FIRST="stm_ai(\"submenu\",[1,\"%s%s\",\"\",\"\",-1,-1,0,\"%s\",\"%s\",\"%s\",\"\",\"\",\"\",-1,-1,0,\"%s\",\"%s\",-1,-1,0,0,1,\"#336699\",0,\"#6699CC\",0,\"\",\"\",3,3,1,1,\"#336699 #336699 #336699 #336699\",\"#B1C9F1 #4A7AC9 #4A7AC9 #B1C9F1\",\"#ffffff\",\"#ffffff\",\"bold 9pt 'tahoma','verdana','helvetica'\",\"bold 9pt 'tahoma','verdana','helvetica'\",0,0]);\n";
static char *SUBMENU="stm_aix(\"noname\",\"submenu\",[1,\"%s%s\",\"\",\"\",-1,-1,0,\"%s\",\"%s\",\"%s\",\"\",\"\",\"\",0,0,0,\"%s\",\"%s\",0,0,0,0,1,\"#336699\",0,\"#6699CC\",0,\"\",\"\",3,3,1,1,\"#336699\"]);";
static char *END_SUBMENU="stm_ep();";
static char *END_MENU="stm_em();";
#define END_POPUPMENU END_SUBMENU

static char *FRAME_VIEW="view";

struct cgi_applet {
	char *name;
	int (*func) (void);
};

struct cgi_applet applets[] = {
	{ "bbsleft", bbsleft_main },
	{ NULL }
};

static struct cgi_applet *getapplet(char *buf, size_t len)
{
	char *surl = getenv("SCRIPT_NAME");
	if(surl == NULL)
		return NULL;
	strlcpy(buf, surl, len);
	char *result = strrchr(buf, '/');
	if (result == NULL)
		result = buf;
	else
		result++;	
	struct cgi_applet *app = applets;
	while (app->name != NULL) {
		if (!strcmp(result, app->name))
			return app;
		app++;
	}
	return NULL;
}
	
int main(void)
{
	char buf[STRLEN];

	fcgi_init_all();
	while (FCGI_Accept() >= 0) {
		fcgi_init_loop();
		struct cgi_applet *app = getapplet(buf, sizeof(buf));
		if(app == NULL)
			http_fatal("�����ҳ�� %s ������", getenv("SCRIPT_NAME"));
		(*(app->func)) ();
	}
	return 0;
}

int bbsmain_main(void)
{
	char buf[1024];

	printf("<meta http-equiv=\"pragma\" content=\"no-cache\">");
	printf("<title>��ӭ����"BBSNAME"BBS</title>");
	printf("<style type=text/css>\n"
			"	*{ font-family: Tahoma; font-size: 9pt;}\n"
			"	A:link {	COLOR: #ffffff; TEXT-DECORATION: none}\n"
			"	A:visited {	COLOR: #ffffff; TEXT-DECORATION: none}\n"
			"	A:active {	COLOR: #ffffff; TEXT-DECORATION: none}\n"
			"	A:hover {	COLOR: #ffffff; TEXT-DECORATION: underline}\n"
			"        BODY {FONT-WEIGHT:bold; FONT-FAMILY: Tahoma; FONT-SIZE: 9pt; COLOR:#ffffff;}\n"
			"	 TABLE {FONT-SIZE: 9pt; color=:#FFFFFF;}TD {color: #FFFFFF} \n"
			"</style>\n\n"
			"<script type=\"text/javascript\" language=\"JavaScript1.2\" src=\"/stm31.js\"></script>\n");

	printf("<script>\n"
			"	var collapsed=0;\n"
			"	imgCollapse=new Image();\n"
			"	imgCollapse.src='/images/collapse.gif';\n"
			"	imgExpand=new Image();\n"
			"	imgExpand.src='/images/expand.gif';\n"
			"	function switchpanel()\n"
			"	{\n"
			"		if(collapsed)\n"
			"		{\n"
			"			panel.style.display='none';\n"
			"			switch_button.src=imgExpand.src;\n"
			"			collapsed=0;\n"
			"		}\n"
			"		else\n"
			"		{\n"
			"			panel.style.display='block';\n"
			"			switch_button.src=imgCollapse.src;\n"
			"			collapsed=1;\n"
			"		}\n"
			"	}\n"
			"	</script>");
	printf("<BODY bgcolor=#ffffff topmargin=0 leftmargin=0 scroll=no>\n");
	printf("<table bgcolor=#ffffff width='100%%' height='100%%' border=0 cellspacing=0 cellpadding=0><tr><td bgcolor=#336699 height=20 width=150>");
	printf("</td><td width=20 background=/images/r2.gif>");
	printf("</td><td height=20 width='100%%'>\n");
	printf("<iframe name=fmsg src=/cgi-bin/bbs/bbsgetmsg width=100%% height=20 frameborder=0 SCROLLING=no MARGINWIDTH=0 MARGINHEIGHT=0></iframe></td></tr>");
	printf("<tr><td valign=top bgcolor=#336699 width=150>");

	//TABLE CONTAIN ALL OBJECT OF MAIN
	printf("<div id=panel width=100%% height=100%% valign=top>");

	if(!loginok) {
		printf("<form action=bbslogin method=post target=_top>\n"
				"                <table border=0 width='100' cellspacing=0 cellpadding=0 align=center>\n"
				"		 <tr>\n"
				"		   <td nowrap><img border=0 src='/images/user.gif' align=absmiddle></td>\n"
				"	           <td><input style='BORDER: 0px; background-color:#6699CC; color: #ffffff; font-size: 9pt; font-weight:bold' name=id type=text maxlength=12 size=10>\n"
				"                   </td>\n"
				"                 </tr><tr>\n"
				"		   <td nowrap><img border=0 align=absmiddle src=/images/password.gif></td>\n"
				"		   <td><input style='font-weight:bold;BORDER: 0px; font-size:9pt; background-color:#6699CC;color:#ffffff' name=pw type=password maxlength=12 size=10></td>\n"
				"		 </tr><tr>\n"
				"                 <td></td><td><input type=submit value=��¼��վ style='font-size:9pt'></td>\n"
				"		 </tr></table>\n"
				"		 </form>");
	} else {
		char buf[256]="δע���û�";
		printf("&nbsp;<img border=0 align=absmiddle src=/images/user_%c.gif> <a href=bbsqry?userid=%s target=view><b>%s</b></a><br>&nbsp;", currentuser.gender=='F'?'F':'M', currentuser.userid, currentuser.userid);
		if(currentuser.userlevel & PERM_BOARDS) strcpy(buf, "����");
		if(currentuser.userlevel & PERM_XEMPT) strcpy(buf, "�����ʺ�");
		if(currentuser.userlevel & PERM_SYSOPS) strcpy(buf, "��վվ��");
		else if(currentuser.userlevel & PERM_XEMPT) strcpy(buf, "�����ʺ�");
		else if(currentuser.userlevel & PERM_BOARDS) strcpy(buf, "����");
		else if(currentuser.userlevel & PERM_REGISTER) buf[0]=0;

		if(!buf[0])iconexp(countexp(&currentuser));
		else printf("<img border=0 align=absmiddle src=/images/blankblock.gif> %s", buf);
	}

	printf("<script type=\"text/javascript\" language=\"JavaScript1.2\">");

	printf(BEGIN_MENU);
	printf(BEGIN_SUBMENU);
	printf(SUBMENU_FIRST ,"<IMG SRC=/images/announce.gif ALIGN=ABSMIDDLE> ","��վ����"
			,"/cgi-bin/bbs/bbs0an", FRAME_VIEW ,"","","");
	printf(SUBMENU ,"<IMG SRC=/images/penguin.gif ALIGN=ABSMIDDLE> ","ȫ������"
			,"/cgi-bin/bbs/bbsall", FRAME_VIEW,"","","");
	printf(SUBMENU, "<IMG SRC=/images/top10.gif ALIGN=ABSMIDDLE> ", "ͳ������","", FRAME_VIEW,"","/images/arrow_r.gif","/images/arrow_r.gif");
	printf(BEGIN_POPUPMENU);
	printf(SUBMENU ,"","����ʮ��" ,"/cgi-bin/bbs/bbstop10", FRAME_VIEW ,"","","");
	printf(SUBMENU ,"","����������" ,"/cgi-bin/bbs/bbstopb10", FRAME_VIEW ,"","","");
	printf(SUBMENU ,"","����ͳ��" ,"/cgi-bin/bbs/bbsuserinfo", FRAME_VIEW
			,"","","");

	printf(END_POPUPMENU);

	if(loginok) {
		FILE *fp;
		char *cgi="bbsdoc";
		if(atoi(getparm("my_def_mode"))!=0)
			cgi="bbstdoc";
		printf(SUBMENU ,"<IMG SRC=/images/favorite.gif ALIGN=ABSMIDDLE> ","�ҵ��ղ�"
				,"/cgi-bin/bbs/bbsmybrd", FRAME_VIEW
				,"","/images/arrow_r.gif","/images/arrow_r.gif");
		printf(BEGIN_POPUPMENU);

		sprintf(buf, "home/%c/%s/.goodbrd", toupper(currentuser.userid[0]), currentuser.userid);
		fp = fopen(buf, "r");
		if (fp != NULL) {
			char brdname[80];
			char path[80];
			int brdcount = 0;
			while (fgets(brdname,80,fp)) {
				strtok(brdname, "\t\r\n");
				sprintf(path, "/cgi-bin/bbs/%s?board=%s", cgi, brdname);
				printf(SUBMENU ,"",brdname, path, FRAME_VIEW,"","","");
				brdcount++;
				if(brdcount>=20)
					break;
			}
			fclose(fp);
		}
		printf(SUBMENU ,"","Ԥ������"
				,"/cgi-bin/bbs/bbsmybrd", FRAME_VIEW
				,"","","");
		printf(END_POPUPMENU);
	}

	printf(SUBMENU ,"<IMG SRC=/images/egroup.gif ALIGN=ABSMIDDLE> ","��������"
			,"/cgi-bin/bbs/bbssec", FRAME_VIEW
			,"","/images/arrow_r.gif","/images/arrow_r.gif");

	printf(BEGIN_POPUPMENU);
	{
		int i;
		char path[80];
		char name[80];
		for(i = 0; i < SECNUM; i++)
		{
			sprintf(path, "/cgi-bin/bbs/bbsboa?%d", i);
			sprintf(name, "%X %s",i,secname[i][0]);
			printf(SUBMENU,"",name,path,FRAME_VIEW,"","","");
		}
	}
	printf(END_POPUPMENU);

	printf(SUBMENU ,"<IMG SRC=/images/chat.gif ALIGN=ABSMIDDLE> ","ȵ�����"
			,"", FRAME_VIEW
			,"","/images/arrow_r.gif","/images/arrow_r.gif");
	printf(BEGIN_POPUPMENU);
	if(loginok) {
		printf(SUBMENU ,"","���ߺ���"
				,"/cgi-bin/bbs/bbsfriend", FRAME_VIEW
				,"","","");
	}
	printf(SUBMENU ,"","�����ķ�"
			,"/cgi-bin/bbs/bbsusr", FRAME_VIEW
			,"","","");

	if(currentuser.userlevel & PERM_TALK) {
		printf(SUBMENU ,"","����ѶϢ"
				,"/cgi-bin/bbs/bbssendmsg", FRAME_VIEW
				,"","","");
		printf(SUBMENU ,"","�鿴����ѶϢ"
				,"/cgi-bin/bbs/bbsmsg", FRAME_VIEW
				,"","","");
	}

	printf(END_POPUPMENU);

	if (loginok) {
		printf(SUBMENU ,"<IMG SRC=/images/config.gif ALIGN=ABSMIDDLE> ","��������"
				,"", FRAME_VIEW
				,"","/images/arrow_r.gif","/images/arrow_r.gif");

		printf(BEGIN_POPUPMENU);		
		printf(SUBMENU ,"","��������"
				,"/cgi-bin/bbs/bbsinfo", FRAME_VIEW
				,"","","");
		printf(SUBMENU ,"","��˵����"
				,"/cgi-bin/bbs/bbsplan", FRAME_VIEW
				,"","","");
		printf(SUBMENU ,"","��ǩ����"
				,"/cgi-bin/bbs/bbssig", FRAME_VIEW
				,"","","");
		printf(SUBMENU ,"","�޸�����"
				,"/cgi-bin/bbs/bbspwd", FRAME_VIEW
				,"","","");
		printf(SUBMENU ,"","WWW����"
				,"/cgi-bin/bbs/bbsmywww", FRAME_VIEW
				,"","","");
		printf(SUBMENU ,"","��ʱ���ǳ�"
				,"/cgi-bin/bbs/bbsnick", FRAME_VIEW
				,"","","");
		printf(SUBMENU ,"","�趨����"
				,"/cgi-bin/bbs/bbsfall", FRAME_VIEW
				,"","","");

		if(currentuser.userlevel & PERM_CLOAK)
			printf(SUBMENU ,"","�л�����"
					,"/cgi-bin/bbs/bbscloak", FRAME_VIEW
					,"","","");
		printf(END_POPUPMENU);

		printf(SUBMENU ,"<IMG SRC=/images/mail.gif ALIGN=ABSMIDDLE> ","�����ż�"
				,"", FRAME_VIEW
				,"","/images/arrow_r.gif","/images/arrow_r.gif");
		printf(BEGIN_POPUPMENU);
		printf(SUBMENU,"<img border=0 align=absmiddle src=/images/mail_new.gif> ","�������ż�"
				,"/cgi-bin/bbs/bbsnewmail",FRAME_VIEW,"","","");
		printf(SUBMENU,"<img border=0 align=absmiddle src=/images/mail.gif> ","�����ż�"
				,"/cgi-bin/bbs/bbsmail",FRAME_VIEW,"","","");
		printf(SUBMENU,"<img border=0 align=absmiddle src=/images/mail_get.gif> ","�����ż�"
				,"/cgi-bin/bbs/bbsmaildown",FRAME_VIEW,"","","");
		printf(SUBMENU,"<img border=0 align=absmiddle src=/images/mail_write.gif> ","�����ż�"
				,"/cgi-bin/bbs//bbspstmail",FRAME_VIEW,"","","");

		printf(END_POPUPMENU);
	}

	printf(SUBMENU ,"<IMG SRC=/images/search.gif ALIGN=ABSMIDDLE> ","����ѡ��"
			,"", FRAME_VIEW
			,"","/images/arrow_r.gif","/images/arrow_r.gif");
	printf(BEGIN_POPUPMENU);
	if (HAS_PERM(PERM_OBOARDS))
		printf(SUBMENU ,"","��������"
				,"/cgi-bin/bbs/bbsfind", FRAME_VIEW
				,"","","");
	printf(SUBMENU ,"","��ѯ����"
			,"/cgi-bin/bbs/bbsqry", FRAME_VIEW
			,"","","");
	printf(SUBMENU ,"","����������"
			,"/cgi-bin/bbs/bbssel", FRAME_VIEW
			,"","","");
	printf(SUBMENU ,"","YCUL����"
			,"http://www.ycul.com?ref=bbs.fudan.edu.cn", FRAME_VIEW,"","","");
	printf(END_POPUPMENU);


	printf(SUBMENU,"<IMG SRC=/images/service.gif ALIGN=ABSMIDDLE> ","��������"
			,"", FRAME_VIEW,"","/images/arrow_r.gif","/images/arrow_r.gif");
	printf(BEGIN_POPUPMENU);
	printf(SUBMENU,"","������...","",FRAME_VIEW,"","","");
	printf(END_POPUPMENU);

	printf(SUBMENU ,"<img border=0 align=absmiddle src=/images/telnet.gif> ","Telnet��¼"
			,"telnet://bbs.fudan.sh.cn:2323", "_top"
			,"","","");
	if(loginok)printf(SUBMENU ,"<img border=0 align=absmiddle src=/images/exit.gif> ","ע����¼"
			,"/cgi-bin/bbs/bbslogout", "_top"
			,"","","");

	printf(END_SUBMENU);
	printf(END_MENU);

	printf("</script>\n");
	printf("</div>");
	//END OF ALL NECESSARY
	printf("</td><td width=20 background=/images/r2.gif>");
	printf("</td><td width=100%% bgcolor=#ffffff>\n");
	printf("<iframe name=view src=/cgi-bin/bbs/bbssec width=100%% height=100%% frameborder=0></iframe>");
	printf("</td></tr><tr><td height=20 bgcolor=#ffffff background=/images/b2.gif>");
	printf("<img id=switch_button src=/images/collapse.gif onclick='switchpanel()'>");
	printf("</td><td width=20><img src=/images/rb2.gif>");
	printf("</td><td height=20 width=100%%>\n");
	printf("<iframe name=foot src=/cgi-bin/bbs/bbsfoot width=100%% height=20 frameborder=0 SCROLLING=no MARGINWIDTH=0 MARGINHEIGHT=0></iframe>");
	printf("</td></tr></table>");

	printf("</body>\n</html>");
	return 0;
}
