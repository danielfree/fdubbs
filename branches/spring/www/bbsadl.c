#include "BBSLIB.inc"

int main() {
	int i, no=0;
	char brd[100], buf[256];
	int sec1;
	init_all();
	printf("<center>\n");
	printf("<nobr><b>���������ط��� �� %s </b>\n", BBSNAME);

	printpretable_lite();
	sec1=atoi(getsenv("QUERY_STRING"));
	if (sec1>0 && sec1<=SECNUM) 
	{
		sec1--;
		printf("<table  border=0>\n");
		printf("<tr class=pt9h ><td><font color=white>���<td><font color=white>����<td><font color=white>��С(�ֽ�)<td><font color=white>����ʱ��\n");
		int cc_one=0;
		for(i=0; i<MAXBOARD; i++) 
		{
			strsncpy(brd, bcache[i].filename, 60);
			if(!has_read_perm(&currentuser, brd)) 
				continue;
			if(!strchr(seccode[sec1], bcache[i].title[0])) 
				continue;
			sprintf(buf, "/home/httpd/html%s.tgz",anno_path_of(brd));
			//printf("buf:%s,brd:%s",buf,brd);
			if(!file_exist(buf)) 
				continue;
			if (file_size(buf)>10000000) 
				continue;
			no++;
			printf("<tr class=%s><td>%d<td>\n<a href=%s.tgz>%s</a><td>%d<td>%s\n", 
			//	no, anno_path_of(brd), brd, 0, "down");
				((cc_one++)%2)?"pt9dc":"pt9lc",no, anno_path_of(brd), brd, file_size(buf), Ctime(file_time(buf))+4);
		}
		printf("</table>");
	}else{
   		printf("<table border=0>\n");
   		printf("<tr class=pt9h ><td><font color=white>����<td><font color=white>���<td><font color=white>����<td><font color=white>����\n");
   		int cc=0;
		for(i=0; i<SECNUM; i++) 
		{
      		printf("<tr class=%s><td>%d<td><font color=white><a href=bbsboa?%d>%s</a>",((cc++)%2)?"pt9dc":"pt9lc" , i, i, secname[i][0]);
			printf("<td><a href=bbsboa?%d>%s</a>\n", i, secname[i][1]);
			printf("<td><a href=bbsadl?%d>%s</a>\n", i+1, "���ؾ�����");
   		}
   		printf("</table>\n");
	}
	printf("[<a href='javascript:history.go(-1)'>������һҳ</a>] ");
	printf("</center>\n");
}
	
