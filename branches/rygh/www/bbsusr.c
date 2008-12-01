#include "BBSLIB.inc"
struct user_info user[MAXACTIVE];
int my_t_lines;

int cmpuser(a, b)
struct user_info *a, *b;
{
	char id1[80], id2[80];
	sprintf(id1, "%d%s", !isfriend(a->userid), a->userid);
	sprintf(id2, "%d%s", !isfriend(b->userid), b->userid);
	return strcasecmp(id1, id2);
}

int main() {
	int i, start, total=0, fh, shmkey, shmid; 
	struct user_info *x;
	char search[80];
	init_all();
	printf("<b><font style='font-size: 18pt'>�����û��б�</font> �� %s [Ŀǰ����: %d��]</b>\n", BBSNAME, count_online());
	printf("<center>\n");
	for(i=0; i<MAXACTIVE; i++) {
		x=&(shm_utmp->uinfo[i]);
		if(x->active==0) continue;
		if(x->invisible && !HAS_PERM(PERM_SEECLOAK)) continue;
		memcpy(&user[total], x, sizeof(struct user_info));
		total++;
	}
	start=atoi(getparm("start"))-1;//added "-1" by roly 02.04.07
	my_t_lines=atoi(getparm("my_t_lines"));
	if(my_t_lines<10 || my_t_lines>40) my_t_lines=TLINES;

	printpretable();

	printf("<table border=0 width=100%% bgcolor=#ffffff>\n");
//	printf("<tr class=pt9 bgcolor=#70a6ff><td><font color=white>���<td><font color=white>��<td><font color=white>ʹ���ߴ���<td><font color=white>ʹ�����ǳ�<td><font color=white>����<td><font color=white>��̬<td><font color=white>����\n");
	printf("<tr class=pt9h ></th><th nowrap>���</th><th nowrap>��</th><th nowrap>ʹ���ߴ���</th><th nowrap>ʹ�����ǳ�</th><th nowrap>��̬</th><th nowrap>����\n");
	qsort(user, total, sizeof(struct user_info), cmpuser);
	if(start>total-5) start=total-5;
	if(start<0) start=0;
	int cc=0;
	for(i=start; i<start+my_t_lines && i<total; i++) {
		int dt=(time(0)-user[i].idle_time)/60;
		printf("<tr class=%s><td nowrap>%d",((cc++)%2)?"pt9dc":"pt9lc" , i+1);
		printf("<td nowrap>%s", isfriend(user[i].userid) ? "��" : "  ");
		printf("%s", user[i].invisible ? "<font color=green>C</font>" : " ");
		printf("<td nowrap><a href=bbsqry?userid=%s><b>%s</b></a>", user[i].userid, user[i].userid);
		printf("<td width=100%%><a href=bbsqry?userid=%s>%24.24s </a>", user[i].userid, nohtml(user[i].username));
	//	printf("<td>%20.20s ", user[i].from);
		printf("<td nowrap align=center>%s", user[i].invisible ? "������..." : ModeType(user[i].mode));
		if(dt==0) {
			printf("<td nowrap> \n");
		} else {
			printf("<td nowrap>%d\n", dt);
		}
	}
	printf("</table>");
	
	printposttable();
	printf("</center>\n");

	printf("[<a href='bbsufind?search=*'>ȫ��</a>] ");
	for(i='A'; i<='Z'; i++)
		printf("[<a href=bbsufind?search=%c>%c</a>]", i, i);
	printf("<br>\n");
	printf("[<a href=bbsfriend>���ߺ���</a>] ");
	if(start>0) printf("<a href=bbsusr?start=%d><img border=0 src=/images/button/up.gif align=absmiddle>��һҳ</a>  ", start-20);
	if(start<total-my_t_lines) printf("<a href=bbsusr?start=%d><img border=0 src=/images/button/down.gif align=absmiddle>��һҳ</a>  ", start+my_t_lines);
	printf("<br><form action=bbsusr>\n");
	printf("<input type=submit value=��ת����> ");
	printf("<input type=input size=4 name=start> ��ʹ����</form>");
	http_quit();
}
