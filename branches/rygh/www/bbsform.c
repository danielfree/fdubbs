#include "BBSLIB.inc"

int main() {
  	FILE *fp;
	int type;
	init_all();
	printf("<b>��дע�ᵥ �� %s</b><br>\n", BBSNAME);
	printpretable_lite();
	type=atoi(getparm("type"));
  	if(!loginok) http_fatal("����δ��¼, �����µ�¼��");
	check_if_ok();
	if(type==1) {
		check_submit_form();
		http_quit();
	}
  	printf("����, %s, ע�ᵥͨ���󼴿ɻ��ע���û���Ȩ��, ������������������д<br><hr>\n", currentuser.userid);
  	printf("<form method=post action=bbsform?type=1>\n");
  	printf("��ʵ����: <input name=realname type=text maxlength=8 size=8 value='%s'><br>\n", 
		nohtml(currentuser.realname));
  	printf("ѧУϵ��: <input name=dept type=text maxlength=32 size=32 value='%s'>(������λ)<br>\n", 
		nohtml(currentuser.reginfo));
  	printf("��ס��ַ: <input name=address type=text maxlength=32 size=32 value='%s'><br>\n", 
		nohtml(currentuser.address));
  	printf("����绰: <input name=phone type=text maxlength=32 size=32>(û�п�д'��')<br><hr><br>\n");
  	printf("<input type=submit> <input type=reset>");
	http_quit();
}

int check_if_ok() {
  	if(user_perm(&currentuser, PERM_REGISTER)) http_fatal("���Ѿ�ͨ����վ�������֤, �����ٴ���дע�ᵥ.");
  	if(has_fill_form()) http_fatal("Ŀǰվ����δ��������ע�ᵥ�������ĵȴ�.");
}

int check_submit_form() {
	FILE *fp;
  	fp=fopen("new_register", "a");
	if(fp==0) http_fatal("ע���ļ�������֪ͨSYSOP");
  	fprintf(fp, "usernum: %d, %s\n", getusernum(currentuser.userid)+1, Ctime(time(0)));
  	fprintf(fp, "userid: %s\n", currentuser.userid);
  	fprintf(fp, "realname: %s\n", getparm("realname"));
  	fprintf(fp, "dept: %s\n", getparm("dept"));
  	fprintf(fp, "addr: %s\n", getparm("address"));
  	fprintf(fp, "phone: %s\n", getparm("phone"));
  	fprintf(fp, "----\n" );
  	fclose(fp);
  	printf("����ע�ᵥ�ѳɹ��ύ. վ�����������������, ��������������.");
}
