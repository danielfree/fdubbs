#include "BBSLIB.inc"
struct user_info user[MAXACTIVE];

int cmpuser(a, b)
struct user_info *a, *b;
{
	char id1[80], id2[80];
	sprintf(id1, "%d%s", !isfriend(a->userid), a->userid);
	sprintf(id2, "%d%s", !isfriend(b->userid), b->userid);
	return strcasecmp(id1, id2);
}

int main() {
	int i, start, total=0, total2=0, fh, shmkey, shmid; 
	struct user_info *x;
	char search;
	init_all();
	printf("<b><font style='font-size: 18pt'>�����û���ѯ</font> �� %s [����������: %d��]</b> --\n", BBSNAME, count_online());
	for(i=0; i<MAXACTIVE; i++) {
		x=&(shm_utmp->uinfo[i]);
		if(x->active==0) continue;
		if(x->invisible && !HAS_PERM(PERM_SEECLOAK)) continue;
		memcpy(&user[total], x, sizeof(struct user_info));
		total++;
	}
	search=toupper(getparm("search")[0]);
	if(search!='*' && (search<'A' || search>'Z')) http_fatal("����Ĳ���");
	if(search=='*') {
		printf("��������ʹ����<br>\n");
	} else {
		printf("��ĸ'%c'��ͷ������ʹ����.<br>\n", search);
	}
	printf("<center>\n");
	printpretable();
	printf("<table width=100%% border=0 cellpadding=1>\n");
	//printf("<tr bgcolor=#70a6ff><td>���<td>��<td>ʹ���ߴ���<td>ʹ�����ǳ�<td>����<td>��̬<td>����\n");
	printf("<tr class=pt9h ><td>���<td>��<td>ʹ���ߴ���<td>ʹ�����ǳ�<td>��̬<td>����\n");
	qsort(user, total, sizeof(struct user_info), cmpuser);
	int cc=0;
	for(i=0; i<total; i++) {
		int dt=(time(0)-user[i].idle_time)/60;
		if(toupper(user[i].userid[0])!=search && search!='*') continue;
		printf("<tr class=%s><td>%d",((cc++)%2)?"pt9dc":"pt9lc" , i+1);
		printf("<td>%s", isfriend(user[i].userid) ? "��" : "  ");
		printf("%s", user[i].invisible ? "<font color=green>C</font>" : " ");
		printf("<td><a href=bbsqry?userid=%s>%s</a>", user[i].userid, user[i].userid);
		printf("<td><a href=bbsqry?userid=%s>%24.24s </a>", user[i].userid, nohtml(user[i].username));
		//printf("<td>%20.20s ", user[i].from);
		printf("<td>%s", user[i].invisible ? "������..." : ModeType(user[i].mode));
		if(dt==0) {
			printf("<td> \n");
		} else {
			printf("<td>%d\n", dt);
		}
		total2++;
	}
	printf("</table>\n");
	printposttable();
	printf("</center>\n");
	printf("��������: %d��", total2);
	printf("<br>");
        if(search!='*') printf("[<a href='bbsufind?search=*'>ȫ��</a>] ");
        for(i='A'; i<='Z'; i++) {
		if(i==search) {
			printf("[%c]", i);
		} else {
                	printf("[<a href=bbsufind?search=%c>%c</a>]", i, i);
		}
	}
        printf("<br>\n");
	printf("[<a href='javascript:history.go(-1)'>����</a>] [<a href=bbsusr>һ��ģʽ</a>] ");
	http_quit();
}
