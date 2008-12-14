#include "libweb.h"

int main() {
	FILE *fp;
	char buf[512], dir[80], file[80], path[80], *ptr, *id;
	struct fileheader x;
	int num, tmp, total;
	init_all();
	strsncpy(file, getparm("file"), 32);
	num=atoi(getparm("num"));
	printf("<center>\n");
	id=currentuser.userid;
	printf("�Ķ��ż� �� %s [ʹ����: %s]\n", BBSNAME, id);
	if(strncmp(file, "M.", 2)) 
	{
		printpretable_lite();
		http_fatal("����Ĳ���1");
	}
	if(strstr(file, "..") || strstr(file, "/")) 
	{
		printpretable_lite();
		http_fatal("����Ĳ���2");
	}
	sprintf(dir, "mail/%c/%s/.DIR", toupper(id[0]), id);
	total=file_size(dir)/sizeof(x);
	if(total<=0) 
	{
		printpretable_lite();
		http_fatal("����Ĳ���3");
	}
	printpretable();
	printf("<table width=100%% border=0>\n");
	printf("<tr><td>\n<pre class=ansi>");
	sprintf(path, "mail/%c/%s/%s", toupper(id[0]), id, file);
	fp=fopen(path, "r");
	if(fp==0) 
	{
		printf("���Ĳ����ڻ����ѱ�ɾ��");
		printf("</pre>\n</table>\n");
		printposttable();
		printf("<br>[<a href=bbsmail>�����ż��б�</a>]  ");
		printf("</center>\n");
		http_quit();
	}
	while(1) 
	{
		if(fgets(buf, 512, fp)==0) 
			break;
		hhprintf("%s", buf);
	}
	fclose(fp);
	printf("</pre>\n</table>\n");
	printposttable();
	fp=fopen(dir, "r+");
	if(fp==0) http_fatal("dir error2");
	if(num>0) {
		fseek(fp, sizeof(x)*(num-1), SEEK_SET);
		fread(&x, sizeof(x), 1, fp);
		printf("<a href=bbsmailcon?file=%s&num=%d><img border=0 src=/images/button/up.gif align=absmiddle>��һƪ</a>  ", x.filename, num-1);
	}
	if(num<total-1) {
		fseek(fp, sizeof(x)*(num+1), SEEK_SET);
      		fread(&x, sizeof(x), 1, fp);
      		printf("<a href=bbsmailcon?file=%s&num=%d><img border=0 src=/images/button/down.gif align=absmiddle>��һƪ</a>  ", x.filename, num+1);
	}
	printf("[<a onclick='return confirm(\"�����Ҫɾ���������?\")' href=bbsdelmail?file=%s> ɾ��</a>]  ", file);
	printf("[<a href=bbsmail>�����ż��б�</a>]  ");
	if(num>=0 && num<total) 
	{
		char title2[80];
		fseek(fp, sizeof(x)*num, SEEK_SET);
		if(fread(&x, sizeof(x), 1, fp)>0) 
		{
			//added by iamfat 2002.08.10
			//check_anonymous(x.owner);
			//added end.
			x.accessed[0]|=FILE_READ;
			fseek(fp, sizeof(x)*num, SEEK_SET);
			fwrite(&x, sizeof(x), 1, fp);
			ptr=strtok(x.owner, " (");
			if(ptr==0) 
				ptr=" ";
			strcpy(title2, x.title);
			if(strncmp(x.title, "Re:",3)) 
				sprintf(title2, "Re: %s", x.title);
			title2[60]=0;
			printf("[<a href='bbspstmail?userid=%s&file=%s&title=%s'>����</a>]",ptr, x.filename, title2);
		}
	}
	fclose(fp);
   	printf("</center>\n"); 
	http_quit();
}
