#include "libweb.h"

int main() {
	int i; 
	init_all();

	/* added by roly  2002.05.10 ȥ��cache */
	printf("<meta http-equiv=\"pragma\" content=\"no-cache\">");
	/* add end */

   	if(!loginok) http_fatal("����δ��¼, ���ȵ�¼");
	loadfriend(currentuser.userid);
    	printf("<center>\n");
  	printf("<b>�������� �� %s [ʹ����: %s]</b><br>\n", BBSNAME, currentuser.userid);
   	printf("�����趨�� %d λ����<br>", friendnum);
	
	printf("<table align=center border=0 cellpadding=0 cellspacing=0 width=400>\n");
	printf("	<tr height=6>\n");
	printf("		<td width=6><img border=0 src='/images/lt.gif'></td>\n");
	printf("		<td background='/images/t.gif' width=100%%></td>\n");
	printf("		<td width=6><img border=0 src='/images/rt.gif'></td>\n");
	printf("	</tr>\n");
	printf("	<tr  height=100%%>\n");
	printf("		<td width=6 background='/images/l.gif'>\n");
	printf("		<td width=100%%>\n");

   	printf("<table width=100%% border=0  bgcolor=#ffffff><tr class=pt9h ><td><font color=white>���<td><font color=white>���Ѵ���<td><font color=white>����˵��<td><font color=white>ɾ������");
   	int cc=0;
	for(i=0; i<friendnum; i++) {
		printf("<tr class=%s><td>%d",((cc++)%2)?"pt9dc":"pt9lc" , i+1);
		printf("<td><a href=bbsqry?userid=%s>%s</a>", fff[i].id, fff[i].id);
		printf("<td>%s\n", nohtml(fff[i].exp));
		printf("<td>[<a onclick='return confirm(\"ȷʵɾ����?\")' href=bbsfdel?userid=%s>ɾ��</a>]", fff[i].id);
	}
   	printf("</table>\n");
	printposttable();
	printf("[<a href=bbsfadd>����µĺ���</a>]</center>\n");
	http_quit();
}
