#include "BBSLIB.inc"

int main() {
	char path[80];
	init_all();
	printf("<b>ɾ����Ϣ �� %s </b><br>\n",BBSNAME);
	printpretable_lite();
	if(!loginok) http_fatal("�Ҵҹ��Ͳ��ܴ���ѶϢ, ���ȵ�¼");
	sethomefile(path, currentuser.userid, "msgfile.me");
	unlink(path);
	printf("��ɾ������ѶϢ����");
	printposttable_lite();
	http_quit();
}
