#include "BBSLIB.inc"

int main() {
   	FILE *fp;
   	int i;
	char userid[80], buf[512], path[512], file[512], board[512], title[80]="";
   	init_all();
	if(!loginok) http_fatal("�Ҵҹ��Ͳ���д�ţ����ȵ�¼");
	/* Added by Amigo 2002.06.19. For mail right check. */
	if (!HAS_PERM(PERM_MAIL)) 
			http_fatal("����δ���ע�ᣬ���߷����ż���Ȩ�ޱ����");
	/* Add end. */
	/* added by roly for mail check */
	if (!mailnum_under_limit(currentuser.userid) || !mailsize_under_limit(currentuser.userid)) 
					http_fatal("�����ż��������꣬�޷�����");
	/* add end */
	strsncpy(file, getparm("file"), 32);
	strsncpy(title, nohtml(getparm("title")), 50);
	strsncpy(userid, getparm("userid"), 40);
	if(file[0]!='M' && file[0]) http_fatal("������ļ���");
	printf("<b>%s -- �����Ÿ� [ʹ����: %s]</b>\n", BBSNAME, currentuser.userid);
   	printf("<center>\n");
	printpretable_lite();
   	printf("<table border=0><tr><td>\n");
	printf("<form method=post action=bbssndmail?userid=%s>\n", userid);
   	printf("�ż�����: <input class=thinborder type=text name=title size=40 maxlength=100 value='%s'> ", title);
   	printf("������: &nbsp;%s<br>\n", currentuser.userid);
 	printf("������: &nbsp;&nbsp<input class=thinborder type=text name=userid value='%s'>", nohtml(userid));
 	printf("  ʹ��ǩ���� ");
   	printf("<input type=radio name=signature value=1 checked>1\n");
   	printf("<input type=radio name=signature value=2>2\n");
   	printf("<input type=radio name=signature value=3>3\n");
	printf("<input type=radio name=signature value=4>4\n");
	printf("<input type=radio name=signature value=5>5\n");
   	printf("<input type=radio name=signature value=0>0\n"); 
	printf("����<input type=checkbox name=backup>\n");
   	printf("<br>\n");
   	printf("<textarea class=thinborder name=text rows=20 cols=80 wrap=physicle>\n\n");
	if(file[0]) {
		int lines=0;
		printf("�� �� %s ���������ᵽ: ��\n", userid);
		sprintf(path, "mail/%c/%s/%s", toupper(currentuser.userid[0]), currentuser.userid, file);
		fp=fopen(path, "r");
		if(fp) {
			for(i=0; i<4; i++)
				if(fgets(buf, 500, fp)==0) break;
			while(1) {
				if(fgets(buf, 500, fp)==0) break;
				if(!strncmp(buf, ": ��", 4)) continue;
				if(!strncmp(buf, ": : ", 4)) continue;
				if(!strncmp(buf, "--\n", 3)) break;
				if(buf[0]=='\n') continue;;
				if(!strcasestr(buf, "</textarea>")) printf(": %s", buf);
				if(lines++>20) {
					printf(": (��������ʡ�� ... ...)");
					break;
				}
			}
			fprintf(fp, "\n");
			fclose(fp);
		}
	}
   	printf("</textarea><br><div align=center>\n");
	printf("<input type=submit value=����> ");
   	printf("<input type=reset value=���></form>\n");
	printf("</div></table>");
	printposttable_lite();
	printf("</center>");
	http_quit();
}
