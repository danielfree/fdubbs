#include "BBSLIB.inc"
FILE *fp;

int main() {
	FILE *fp;
	char *ptr, path[256], buf[10000];
   	init_all();

	/* added by roly  2002.05.10 ȥ��cache */
	printf("<meta http-equiv=\"pragma\" content=\"no-cache\">");
	/* add end */

	if(!loginok) http_fatal("�Ҵҹ��Ͳ�������ǩ���������ȵ�¼");
	printf("<font style='font-size: 18pt'>%s</font> �� %s ����ǩ���� \n", currentuser.userid, BBSNAME);
	sprintf(path, "home/%c/%s/signatures", 
		toupper(currentuser.userid[0]), currentuser.userid);
	if(!strcasecmp(getparm("type"), "1")) save_sig(path);
	printpretable_lite();
	printf("<form method=post action=bbssig?type=1>\n");

	fp=fopen(path, "r");
	bzero(buf, 10000);
	if(fp) {
		fread(buf, 9999, 1, fp);
		ptr=strcasestr(buf, "<textarea>");
		if(ptr) ptr[0]=0;
		fclose(fp);
	}
   	printf("ǩ����ÿ6��Ϊһ����λ, �����ö��ǩ����.<br>");
   	printf("<textarea class=ansi name=text rows=20 cols=80 nowrap>\n");
	printf("%s",buf);
   	printf("</textarea></br>\n");
   	printf("<input type=submit value=����> ");
   	printf("<input type=reset value=��ԭ>\n");
   	printf("</form>\n");
	printposttable_lite();

	http_quit();
}

int save_sig(char *path) {
	char *buf;
	fp=fopen(path, "w");
	buf=getparm("text");
	fprintf(fp, "%s", buf);
	fclose(fp);
	printf("ǩ�����޸ĳɹ���");
	http_quit();
}
