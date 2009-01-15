#include "libweb.h"

int main() {
	FILE *fp;
	int num=0, total=0, type, dt, mg=0, og=0;
	char dir[80], title[80], title2[80], title3[80], board[80], userid[80];
	struct boardheader *brd;
	struct fileheader x;
	init_all();
	printf("<center>%s -- ������������<hr color=green><br>\n", BBSNAME);
	type=atoi(getparm("type"));
	strlcpy(board, getparm("board"), 30);
	if(type==0) return show_form(board);
	strlcpy(title, getparm("title"), 60);
	strlcpy(title2, getparm("title2"), 60);
	strlcpy(title3, getparm("title3"), 60);
	strlcpy(userid, getparm("userid"), 60);
	dt=atoi(getparm("dt"));
	if(!strcasecmp(getparm("mg"), "on")) mg=1;
	if(!strcasecmp(getparm("og"), "on")) og=1;
	if(dt<0) dt=0;
	if(dt>9999) dt=9999;
	brd=getbcache(board);
	if(brd==0) http_fatal("�����������");
	strcpy(board, brd->filename);
	if(!has_read_perm(&currentuser, board)) http_fatal("�����������");
	if (brd ->flag & BOARD_DIR_FLAG)
			http_fatal("��ѡ�����һ��Ŀ¼"); //add by Danielfree 06.3.5
	if ((brd->flag & BOARD_CLUB_FLAG)&& (brd->flag & BOARD_READ_FLAG )&& !has_BM_perm(&currentuser, board)&& !isclubmember(currentuser.userid, board))
		http_fatal("�����Ǿ��ֲ��� %s �ĳ�Ա����Ȩ���ʸð���", board);	
	//modified by iamfat 2002.08.19
	//if(!has_BM_perm(&currentuser, board))http_fatal("�Բ���, ���޷�ʹ����������!\n");
	sprintf(dir, "boards/%s/.DIR", board);
	fp=fopen(dir, "r");
	if(fp==0) http_fatal("�����������û��Ŀǰ����");
	printf("����������'%s'��, ���⺬: '%s' ", board, nohtml(title));
	if(title2[0]) printf("�� '%s' ", nohtml(title2));
	if(title3[0]) printf("���� '%s' ", nohtml(title3));
	printf("����Ϊ: '%s', '%d'�����ڵ�%s����.<br>\n", 
		userid[0] ? userid_str(userid) : "��������", dt, mg ? "����" : "����");
	printpretable();
	printf("<table width=100%%  bgcolor=#ffffff>\n");
	printf("<tr class=pt9h><th nowrap>���<th nowrap>���<th nowrap>����<th nowrap>����<th nowrap>����\n");
	int cc=0;
	int isreply=0;
	while(1) {
		if(fread(&x, sizeof(x), 1, fp)==0) break;
		num++;
		//added by iamfat 2002.08.10
		//check_anonymous(x.owner);
		//added end.
		if(title[0] && !strcasestr_gbk(x.title, trim(title))) continue;
		if(title2[0] && !strcasestr_gbk(x.title, trim(title2))) continue;
		if(userid[0] && strcasecmp(x.owner, trim(userid))) continue;
		if(title3[0] && strcasestr_gbk(x.title, trim(title3))) continue;
		if(abs(time(0)-atoi(x.filename+2))>dt*86400) continue;
		if(mg && !(x.accessed[0] & FILE_MARKED) && !(x.accessed[0] & FILE_DIGEST)) continue;
		if(og && !strncmp(x.title, "Re: ", 4)) continue;
		total++;
		printf("<tr class=%s nowrap><td>%d", ((cc++)%2)?"pt9dc":"pt9lc" , num);
		printf("<td nowrap align=center><b>%s</b>", flag_str(x.accessed[0]));
		printf("<td nowrap><b>%s</b>", userid_str(x.owner));
		printf("<td nowrap>%12.12s", 4+Ctime(atoi(x.filename+2)));

		isreply=!strncmp(x.title, "Re: ", 4);
		//fix bug: ��ʾbug by DeepOcean:
		printf("<td nowrap width=100%%><a href=bbscon?b=%s&f=%s&n=%d>%s%s </a>\n", board, x.filename, num, isreply? "<img src=/images/types/reply.gif align=absmiddle border=0> "
		                :"<img src=/images/types/text.gif align=absmiddle border=0> ",nohtml(isreply?(x.title+4):x.title));
		if(total>=999) break;
	}
	fclose(fp);
	printf("</table>\n");
	printposttable();
	printf("<br>���ҵ� %d ƪ���·�������", total);
	if(total>999) printf("(ƥ��������, ʡ�Ե�1000�Ժ�Ĳ�ѯ���)");
	printf("<br>\n");
	printf("[<a href=bbsdoc?board=%s>���ر�������</a>] [<a href='javascript:history.go(-1)'>������һҳ</a>]", board);
	http_quit();
}

int show_form(char *board) {
	printf("<table><form action=bbsbfind?type=1 method=post>\n");
	printf("<tr><td>��������: <input type=text maxlength=24 size=24 name=board value='%s'><br>\n", board);
	printf("<tr><td>���⺬��: <input type=text maxlength=50 size=20 name=title> AND ");
	printf("<input type=text maxlength=50 size=20 name=title2>\n");
	printf("<tr><td>���ⲻ��: <input type=text maxlength=50 size=20 name=title3>\n");
	printf("<tr><td>�����ʺ�: <input type=text maxlength=12 size=12 name=userid><br>\n");
	printf("<tr><td>ʱ�䷶Χ: <input type=text maxlength=4  size=4  name=dt value=7> ������<br>\n");
	printf("<tr><td>��������: <input type=checkbox name=mg> ");
	printf("��������: <input type=checkbox name=og><br><br>\n");
	printf("<tr><td><input type=submit value=�ݽ���ѯ���>\n");
	printf("</form></table>");
	if(HAS_PERM(PERM_OBOARDS)&&HAS_PERM(PERM_SPECIAL0)){//���Ȩ��λ����0  06.1.2
	printf("[<a href='bbsdoc?board=%s'>������һҳ</a>] [<a href=bbsfind>ȫվ���²�ѯ</a>]", board);
	}
	else {
	  printf("[<a href='bbsdoc?board=%s'>������һҳ</a>]", board);
	}
	http_quit();
}
