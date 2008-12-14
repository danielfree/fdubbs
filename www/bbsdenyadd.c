#include "libweb.h"

struct deny {
	char id[80];
//	char exp[80];
//	int free_time;
//	char freetime[80];
	char description[256]; //added by roly 02.03.22
} denyuser[256];
int denynum=0;
/* added by roly  02.03.22 */
int getdenyrecord(char* buf,FILE *stream)
{
	int ch,i;

	ch = fgetc( stream );
   	for( i=0; (i < 512 ) && ( feof( stream ) == 0 && ch!=0x0A); i++ )
   	{
      		buf[i] = (char)ch;
      		ch = fgetc( stream );
   	}
	buf[i]=0x0A;
	if (feof( stream ) != 0) return -1;
	return 0;
}

/* add end */

int loaddenyuser(char *board) {
	FILE *fp;
	char path[80], buf[256];
	sprintf(path, "boards/%s/deny_users", board);
	fp=fopen(path, "r");
	if(fp==0) return;
	while(denynum<100) {
		int i,j;
		//if(fgets(buf, 80, fp)==0) break;
		if (getdenyrecord(buf,fp)==-1) break; //modified by roly 02.03.22
		//sscanf(buf, "%s %s %d", denyuser[denynum].id, denyuser[denynum].exp, &denyuser[denynum].free_time);
		sscanf(buf, "%s", denyuser[denynum].id);		
		i = strlen(denyuser[denynum].id);
		j = 0;
		while (buf[i]!=0x0A) {
			denyuser[denynum].description[j]=buf[i];
			i++;j++;
		}
		denyuser[denynum].description[j]=0x0A;
		//strcpy(denyuser[denynum].description,buf+strlen(denyuser[denynum].id)); //added by roly 02.03.22
		denynum++;
	}
	fclose(fp);
}

int savedenyuser(char *board) {
/* modified by roly  02.03.22*/

	FILE *fp;
	int m;
	char path[80], buf[256], *exp;
	sprintf(path, "boards/%s/deny_users", board);
	fp=fopen(path, "a+");
	if(fp==0) return;
	
	
	if(denyuser[denynum].id[0]==0) return;
	fprintf(fp, "%-12s %s\n", denyuser[denynum].id, denyuser[denynum].description);

	fclose(fp);
/* modify end */
}

int main() {
	int i; 
	struct tm* tmtime;
	time_t daytime;

	char exp[80], board[80], *userid;
	int dt;
	init_all();
   	if(!loginok) http_fatal("����δ��¼, ���ȵ�¼");
	strsncpy(board, getparm("board"), 30);
	strsncpy(exp, getparm("exp"), 30);
	dt=atoi(getparm("dt"));
	if(!has_read_perm(&currentuser, board)) http_fatal("�����������");
	if(!has_BM_perm(&currentuser, board)) http_fatal("����Ȩ���б�����");
	loaddenyuser(board);
	userid=getparm("userid");
	if(userid[0]==0) return show_form(board);
	if(getuser(userid)==0) http_fatal("�����ʹ�����ʺ�");
	strcpy(userid, getuser(userid)->userid);
	if(dt<1 || dt>150) http_fatal("�����뱻������(1-150)");
	if(exp[0]==0) http_fatal("���������ԭ��");
   	for(i=0; i<denynum; i++)
		if(!strcasecmp(denyuser[i].id, userid)) http_fatal("���û��Ѿ�����");
	if(denynum>40) http_fatal("̫���˱�����");
	strsncpy(denyuser[denynum].id, userid, 13);
	//strsncpy(denyuser[denynum].exp, exp, 30);
	//denyuser[denynum].free_time=time(0)+dt*86400;

	/* added by roly 02.03.22 */
	daytime = time(0)+dt*86400;
	tmtime=gmtime(&daytime);
        sprintf(denyuser[denynum].description, "%-40s %02d��%02d��%02d�ս�",
		 exp , tmtime->tm_year%100, tmtime->tm_mon+1,tmtime->tm_mday);
	/* added end */
	savedenyuser(board);
	printf("��� %s �ɹ�<br>\n", userid);
	inform(board, userid, exp, dt);
	printf("[<a href=bbsdenyall?board=%s>���ر����ʺ�����</a>]", board);
	http_quit();
}

int show_form(char *board) {
	printf("<center>%s -- ������� [������: %s]<hr color=green>\n", BBSNAME, board);
	printf("<form action=bbsdenyadd><input type=hidden name=board value='%s'>", board);
	printf("���ʹ����<input name=userid size=12> ����POSTȨ <input name=dt size=2> ��, ԭ��<input name=exp size=20>\n");
	printf("<input type=submit value=ȷ��></form>");
}

int inform(char *board, char *user, char *exp, int dt) {
	FILE *fp;
	char path[80], title[80];
	struct tm* tmtime;
	time_t daytime;

	daytime = time(0)+dt*86400;
	tmtime=gmtime(&daytime);

	sprintf(title, "ȡ�� %s �� %s ��� POST Ȩ��", user, board);
	sprintf(path, "tmp/%d.tmp", getpid());
	fp=fopen(path, "w");
	fprintf(fp, "�����������Զ�����ϵͳ�Զ�������\n\n");
	//fprintf(fp, "%s��������Ա[%s]����˱���POSTȨ[%d]��.\n", user, currentuser.userid, dt);
	//fprintf(fp, "ԭ����: %s\n", exp);
	fprintf(fp, "ִ����  : %s\n����ԭ��: %s\n��������: %d\n�������: [%02d��%02d��%02d��]\n��ⷽʽ����������\n", 
         	currentuser.userid,exp,dt,tmtime->tm_year%100,tmtime->tm_mon+1,tmtime->tm_mday);


	
	fclose(fp);
	post_article(board, title, path, "deliver", "�Զ�����ϵͳ", "�Զ�����ϵͳ", -1, -1, -1);
	post_article("Notice",title,path,"deliver", "�Զ�����ϵͳ", "�Զ�����ϵͳ", -1, -1, -1);
	post_mail(user, title, path, currentuser.userid, currentuser.username, fromhost, -1);
	unlink(path);
	printf("ϵͳ�Ѿ�����֪ͨ��%s.<br>\n", user);
}
