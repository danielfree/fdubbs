#include "libweb.h"

int main() {
	int i;
	unsigned char nick[80];
	init_all();
	printf("<b>��ʱ�ı��ǳ�(�����ķ���Ч) �� %s <b>\n",BBSNAME);
	if(!loginok)
	{
		printpretable_lite();
		printf("<br>");
		http_fatal("�Ҵҹ����޷��ı��ǳ�");
	}
	strsncpy(nick, getparm("nick"), 30);
	if(nick[0]==0) {
		printf(" [ʹ����: %s]\n", currentuser.userid);
		printpretable_lite();
		printf("<form action=bbsnick>���ǳ�<input name=nick size=24 maxlength=24 type=text value='%s'> \n", 
			u_info->username);
		printf("<input type=submit value=ȷ��>");
		printf("</form>");
		printposttable_lite();
		http_quit();
	}
	for(i=0; nick[i]; i++)
		if(nick[i]<32 || nick[i]==255) nick[i]=' ';
	strsncpy(u_info->username, nick, 32);
	printf(" [ʹ����: %s]\n", currentuser.userid);
	printpretable_lite();
	printf("��ʱ����ǳƳɹ�");
	printposttable_lite();
	http_quit();
}
