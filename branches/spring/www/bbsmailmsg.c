#include "BBSLIB.inc"

int main() {
	char filename[80];
	init_all();
	printf("<b>��Ϣ�Ļ����� �� %s </b><br>\n",BBSNAME);
	printpretable_lite();

	if(!loginok) http_fatal("�Ҵҹ��Ͳ��ܴ�����Ϣ�����ȵ�¼");
	/* modified by roly ? bug??*/
	//sprintf(filename, "home/%c/%s/msgfile", toupper(currentuser.userid[0]), currentuser.userid);
	sprintf(filename, "home/%c/%s/msgfile.me", toupper(currentuser.userid[0]), currentuser.userid);
	/* modify end */
	post_mail(currentuser.userid, "������Ϣ����", filename, currentuser.userid, currentuser.username, fromhost, -1);
	unlink(filename);
	printf("��Ϣ�����Ѿ��Ļ���������");
	printposttable_lite();
	printf("<a href='javascript:history.go(-2)'>����</a>");
	http_quit();
}
