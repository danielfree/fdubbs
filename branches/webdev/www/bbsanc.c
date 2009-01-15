#include "libweb.h"

int main() {
	FILE *fp;
	char *board, path[512], buf[512], ch, tmp[80], *ptr; 
	struct boardheader *x1;
	init_all();
	strlcpy(path, getparm("path"), 511);
    if(strstr(path,"bbslist"))
            http_fatal("�����������");
	board=getbfroma(path);
	if(board[0]) 
	{
		if (!has_read_perm(&currentuser, board)) http_fatal("�����������");
		x1=getbcache(board);
		if ((x1->flag & BOARD_CLUB_FLAG)
			&& (x1->flag & BOARD_READ_FLAG )
			&& !has_BM_perm(&currentuser, board)
			&& !isclubmember(currentuser.userid, board))
			http_fatal("�����Ǿ��ֲ��� %s �ĳ�Ա����Ȩ���ʸ��ļ�", board);
	}
	buf[0]=0;
	if(board[0]) sprintf(buf, "%s", board);
	printf("<center><b>%s �� %s �����������Ķ�</b></center><br>\n", board, BBSNAME);
	if(strstr(path, ".Search") || strstr(path, ".Names")|| strstr(path, "..")|| strstr(path, "SYSHome"))
	{
		printpretable_lite();
		http_fatal("������ļ���");
	}
	sprintf(buf, "0Announce%s", path);
	printpretable();
	printf("<table border=0 width=100%%>");
	printf("<tr><td><pre class=ansi>");
	fp=fopen(buf, "r");
	if(fp==0) 
		printf("������ļ���");
	else
	{
		while(1) 
		{
			if(fgets(buf, 256, fp)==0) 
				break;
			hhprintf("%s", buf);
		}
		fclose(fp);
	}
   	printf("</pre>\n</table>\n");
	printposttable();
	printf("<center>[<a href='javascript:history.go(-1)'>������һҳ</a>]  ");
   	if(board[0]) 
		printf("[<a href=bbsdoc?board=%s>��������</a>] ", board);
	printf("</center>\n");
	http_quit();
}
