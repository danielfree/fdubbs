#include "BBSLIB.inc"

int main() {
	FILE *fp;
	char buf[512], board[80], filename[80];
	struct boardheader *x1;
	init_all();
	strsncpy(board, getparm("board"), 32);
	x1=getbcache(board);
	if(!has_read_perm(&currentuser, board)) http_fatal("����İ���");
	if ((x1->flag & BOARD_CLUB_FLAG)
	&& (x1->flag & BOARD_READ_FLAG )
	&& !has_BM_perm(&currentuser, board)
	&& !isclubmember(currentuser.userid, board))
		http_fatal("�����Ǿ��ֲ��� %s �ĳ�Ա����Ȩ���ʸð���", board);
		   
	printf("<b><font style='font-size: 18pt'>���滭��</font> �� %s [������: %s]\n", BBSNAME, board);
   	sprintf(filename, "vote/%s/notes", board);
	fp=fopen(filename, "r");
	if(fp==0) {
		printpretable_lite();
		printf("<br>�����������ޡ����滭�桹��\n");
		printposttable_lite();
		http_quit();
	}
 	printf("<center>\n");
	printpretable();
  	printf("<table><pre class=ansi>\n");
   	while(1) {
		char *s;
		bzero(buf, 512);
		if(fgets(buf, 512, fp)==0) break;
		while(1) {
			int i;
			s=strstr(buf, "$userid");
			if(s==0) break;
			for(i=0; i<7; i++) s[i]=32;
			for(i=0; i<strlen(currentuser.userid); i++)
				s[i]=currentuser.userid[i];
		}
		hhprintf("%s", buf);
	}
 	fclose(fp); 
 	printf("</pre></table>\n");
	printposttable();
	printf("</center>\n");
   	printf("[<a href=bbsdoc?board=%s>��������</a>] ", board);
	if(has_BM_perm(&currentuser, board)) 
		printf("[<a href=bbsmnote?board=%s>�༭���滭��</a>]", board);
	http_quit();
}
