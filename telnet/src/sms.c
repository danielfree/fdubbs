#include "bbs.h"

#ifdef SMS
/**
 �������ַ���mobile�Ƿ�Ϊ13λ���ֻ�����,  ���򷵻�1,���򷵻�0
 **/
int check_mobile(char *mobile)
{
	int i;

	if (strlen(mobile) != 13)
	return(0);

	for(i = 0; i < 13; i++) {
		if (*(mobile+i) < 0x30)
		return(0);
		if (*(mobile+i)> 0x39)
		return(0);
	}
	return(1);
}

//	��ʾ�ֻ�����Ϣѡ��˵�
void sms_menu()
{
	char ans[3];
	char user[18], mobile[16], message[142], regcode[12];
	int flag = 0;
	SMS_HANDLE *hsms;

	move(1, 0);
	if (currentuser.bet & 0x80000000) { /*	��������Ϊ��ֵ,��û�и�ծ	*/
		// �˴�,���ܰѴ�λ���ó��Ƿ�ע�����ֻ�
		prints("[1] ���Ͷ���Ϣ\n");
		prints("[2] ע���ֻ�\n");
		prints("[Q] �˳�\n");
		getdata(t_lines - 1, 0, "��ѡ��", ans, 2, DOECHO, YEA);

		clear();
		refresh();

		switch(ans[0]) {
			case '1':
			move(1, 0);
			pressreturn();
			break;
			case '2':
			pressreturn();
			break;
			default:
			break;
		}
		return;
	} else {
		prints("����δע��ʹ�ö��ŷ���\n");
		prints("[1] ע���ֻ�\n");
		prints("[Q] �˳�\n");
		getdata(t_lines - 1, 0, "��ѡ��", ans, 2, DOECHO, YEA);

		clear();
		refresh();

		if (ans[0] != '1')
		return;

		memset(mobile,0,14);

		while (1) {
			getdata( t_lines - 1, 0, "�����������ֻ�����[��q�˳�]��",
					mobile, 14, DOECHO, YEA);
			if ((mobile[0] == 'q')||(mobile[0] == 'Q'))
			return;
			if (check_mobile(mobile))
			break;
		}
		prints("your mobile: %s\n", mobile);
		/*sprintf(user, "%s@bbs", currentuser.userid);
		 hsms = smsmsg_open("sms.funo.net", 5919, user, pass, mobile);
		 if (hsms == NULL) {
		 prints("���������������\n");
		 pressreturn();
		 return;
		 }
		 if (smsmsg_reg_request(hsms, mobile, NULL) != 0) {
		 prints("ע��������ʧ��\n");
		 pressreturn();
		 return;
		 }*/
		//ϵͳ��¼
		//������

		memset(regcode,0,12);
		getdata(t_lines - 1, 0, "���������յ���ע����[��q�˳�]��",
				regcode, 12, DOECHO, YEA);
		if ((regcode[0] == 'q') || (regcode[0] == 'Q'))
		return;

		/*if (smsmsg_reg_confirm(hsms, mobile, regcode) != 0) {
		 prints("ע���벻��ȷ���߷�����������\n");
		 pressreturn();
		 return;
		 }*/
		pressreturn();
		prints("���ѳɹ�ע�ᣬ�����½��뱾�˵�ʹ�ö��ŷ���\n");
		//�ļ�����
		//�û�Ȩ��λ���
		//ϵͳ��¼
		return;
	}
}
#endif
