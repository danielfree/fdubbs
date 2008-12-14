#include "libweb.h"
char day[20], user[20], title[80];

int main() {
	char user[32], title3[80], title[80], title2[80];
	int day;
	init_all();
	//modified by iamfat 2002.08.19
	if(!(HAS_PERM(PERM_OBOARDS)&&HAS_PERM(PERM_SPECIAL0)) )http_fatal("�Բ���, ���޷�ʹ����������!\n");
  	strsncpy(user, getparm("user"), 13);
  	strsncpy(title, getparm("title"), 50);
	strsncpy(title2, getparm("title2"), 50);
	strsncpy(title3, getparm("title3"), 50);
  	day=atoi(getparm("day"));
  	if(day==0) {
		printf("%s -- վ�����²�ѯ<hr color=green>\n", BBSNAME);
		printf("<form action=bbsfind>\n");
		printf("��������: <input maxlength=12 size=12 type=text name=user> (���������������)<br>\n");
		printf("���⺬��: <input maxlength=60 size=20 type=text name=title>");
		printf(" AND <input maxlength=60 size=20 type=text name=title2><br>\n");
		printf("���ⲻ��: <input maxlength=60 size=20 type=text name=title3><br>\n");
		printf("�������: <input maxlength=5 size=5 type=text name=day value=7> �����ڵ�����<br><br>\n");
		printf("<input type=submit value=�ύ��ѯ>\n");
	} else {
		search(user, title, title2, title3, day*86400);
	}
	http_quit();
}

int search(char *id, char *pat, char *pat2, char *pat3, int dt) {
  	FILE *fp;
  	char board[256], dir[256], buf2[150];
	int total, now=time(0), i, sum=0, n, t;
  	struct fileheader x;
  	printf("%s -- վ�����²�ѯ��� <br>\n", BBSNAME);
	printf("����: %s ", id);
	printf("���⺬��: '%s' ", nohtml(pat));
	if(pat2[0]) printf("�� '%s' ", nohtml(pat2));
	if(pat3[0]) printf("���� '%s'", nohtml(pat3));
	printf("ʱ��: %d ��<br><hr color=green>\n", dt/86400);
	for(i=0; i<MAXBOARD; i++) {
		total=0;
		strcpy(board, bcache[i].filename);
		if(!has_read_perm(&currentuser, board)) continue;
		sprintf(dir, "boards/%s/.DIR", board);
		fp=fopen(dir, "r");
		if(fp==0) continue;
		n=0;
		printf("<table width=610 border=0 bgcolor=#ffffff>\n");
		int cc=0;
		while(1) {
			n++;
			if(fread(&x, sizeof(x), 1, fp)<=0) break;
		//added by iamfat 2002.08.10
		//check_anonymous(x.owner);
		//added end.
			t=atoi(x.filename+2);
			if(id[0]!=0 && strcasecmp(x.owner, trim(id))) continue;
			if(pat[0] && !strcasestr(x.title, trim(pat))) continue;
			if(abs(now-t)>dt) continue;
			if(pat2[0] && !strcasestr(x.title, trim(pat2))) continue;
			if(pat3[0] && strcasestr(x.title, trim(pat3))) continue;
			printf("<tr class=%s><td width=20>%d<td width=50><a href=bbsqry?userid=%s>%s</a>",
				((cc++)%2)?"pt9dc":"pt9lc" ,n, x.owner, x.owner);
			printf("<td width=60>%6.6s", Ctime(atoi(x.filename+2))+4);
			printf("<td><a href=bbscon?b=%s&f=%s&n=%d>%s</a>\n",
				board, x.filename, n-1, nohtml(x.title));
			total++;
			sum++;
			if(sum>1999) {
				printf("</table><br><hr color=green> ....");
				http_quit();
			}
		}
		printf("</table>\n");
		if(total==0) continue;
		printf("<br>����%dƪ���� <a href=bbsdoc?board=%s>%s</a><br><br>\n",
			total, board, board);
	}
	printf("һ���ҵ�%dƪ���·��ϲ�������<br>\n", sum);
}
