#include "libweb.h"

int main() {
	int type;
  	char pw1[20], pw2[20], pw3[20];
	init_all();
	if(!loginok) http_fatal("����δ��¼, ���ȵ�¼");
	type=atoi(getparm("type"));
	if(type==0) {
		printf("<b>%s -- �޸����� [�û�: %s]</b>\n", BBSNAME, currentuser.userid);
		printpretable_lite();
		printf("<form action=bbspwd?type=1 method=post>\n");
		printf("���ľ�����: <input maxlength=12 size=12 type=password name=pw1><br>\n");
		printf("����������: <input maxlength=12 size=12 type=password name=pw2><br>\n");
		printf("������һ��: <input maxlength=12 size=12 type=password name=pw3><br><br>\n");
		printf("<input type=submit value=ȷ���޸�>\n");
		printposttable_lite();
		http_quit();
	}
  	strlcpy(pw1, getparm("pw1"), 13);
  	strlcpy(pw2, getparm("pw2"), 13);
  	strlcpy(pw3, getparm("pw3"), 13);
  	if(strcmp(pw2, pw3)) http_fatal("�������벻��ͬ");
  	if(strlen(pw2)<2) http_fatal("������̫��");
  	if(!checkpasswd(currentuser.passwd, pw1)) http_fatal("���벻��ȷ");
  	strcpy(currentuser.passwd, crypt(pw2, pw2));
  	save_user_data(&currentuser);
  	printf("[%s] �����޸ĳɹ�.", currentuser.userid);
}

