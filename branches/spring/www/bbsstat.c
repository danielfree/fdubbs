#include "BBSLIB.inc"

int main() {
	FILE *fp;
	struct userec x;
	int logins=0, posts=0, stays=0, lifes=0, total=0;
	init_all();
	if(!loginok) http_fatal("�Ҵҹ��Ͳ���������");
	fp=fopen(".PASSWDS", "r");
	while(1) {
		if(fread(&x, sizeof(x), 1, fp)<=0) break;
		if(x.userid[0]<'A') continue;
		if(x.userlevel==0) continue;
		if(x.numposts>=currentuser.numposts) posts++;
		if(x.numlogins>=currentuser.numlogins) logins++;
		if(x.stay>=currentuser.stay) stays++;
		if(x.firstlogin<=currentuser.firstlogin) lifes++;
		total++;
	}
	fclose(fp);
	printf("<center>\n");
	printf("<font style='font-size: 18pt'>%s</font> �� %s ��������ͳ��\n", currentuser.userid, BBSNAME);
	
	printf("<table border=0 width=400>\n");
	printf("	<tr height=6><td background=/images/b.gif width=100%%></td></tr>\n");
	printf("	<tr><td>\n");

	printf("<table width=100%%>\n");
	printf("<tr class=pt9h><td>��Ŀ<td>��ֵ<td>ȫվ����<td>��Ա���\n");
	printf("<tr class=pt9lc><td>��վ����<td>%d��<td>%d<td>TOP %5.2f%%", 
		(time(0)-currentuser.firstlogin)/86400, lifes, (lifes*100.)/total);
	printf("<tr class=pt9dc><td>��վ����<td>%d��<td>%d<td>TOP %5.2f%%",
		currentuser.numlogins, logins, logins*100./total);
	printf("<tr class=pt9lc><td>��������<td>%d��<td>%d<td>TOP %5.2f%%",
		currentuser.numposts, posts, posts*100./total);
	printf("<tr class=pt9dc><td>����ʱ��<td>%d��<td>%d<td>TOP %5.2f%%",
		currentuser.stay/60, stays, stays*100./total);
	printf("</table>");
	printposttable_lite();
	printf("<br>���û���: %d", total);
	printf("</center>\n");
	http_quit();
}
