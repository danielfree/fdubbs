#include "BBSLIB.inc"

int main() {
	FILE *fp;
	char buf[512], path[512];
	init_all();
	/* added by roly  2002.05.10 ȥ��cache */
	printf("<meta http-equiv=\"pragma\" content=\"no-cache\">");
	/* add end */
	printf("<b>�鿴��Ϣ �� %s </b><br>\n",BBSNAME);
	printpretable_lite();
	if(!loginok) http_fatal("�Ҵҹ����޷��鿴��Ϣ, ���ȵ�¼");
	sethomefile(path, currentuser.userid, "msgfile.me");
	fp=fopen(path, "r");
	if(fp==0) http_fatal("û���κ���Ϣ");
	printf("<pre>\n");
	while(1) {
		if(fgets(buf, 256, fp)<=0) break;
		hprintf("%s", buf);
	}
	fclose(fp);
	printf("</pre>");
	printposttable_lite();
	printf("<br><a onclick='return confirm(\"�����Ҫ���������Ϣ��?\")' href=bbsdelmsg>���������Ϣ</a> ");
	printf("    <a href=bbsmailmsg>��Ϣ�Ļ�����</a>");
	http_quit();
}
