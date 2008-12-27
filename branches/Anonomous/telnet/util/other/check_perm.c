#include "../../include/bbs.h"

struct userec u;
int perm_special = PERM_OBOARDS
		  +PERM_OCLUB
		  +PERM_ANNOUNCE
		  +PERM_OCBOARD
		  +PERM_USER
		  +PERM_OCHAT
		  +PERM_SYSOPS
		  +PERM_CLOAK
		  +PERM_SEECLOAK
		  +PERM_ARBI
		  +PERM_SERV
		  +PERM_TECH
		  +PERM_SPECIAL0
		  +PERM_SPECIAL9;


int perm_admin = PERM_OBOARDS
		+PERM_OCLUB
		+PERM_ANNOUNCE
		+PERM_OCBOARD
		+PERM_USER
		+PERM_OCHAT
		+PERM_SYSOPS;

				
int perm_inner = PERM_ARBI
		+PERM_SERV
		+PERM_TECH
		+PERM_SPECIAL0
		+PERM_SPECIAL9;




int perm_long = PERM_XEMPT + PERM_LONGLIFE;


int perm_bm = PERM_BOARDS;



int main(int argc,char ** argv) {
	FILE	*fp=fopen("/home/bbs/.PASSWDS","r");
	bool print=false;
	int num=0;
	char genbuf[64];
	int perm;
				   
	if (argc < 2) {
		printf("Usage:\t%s h\n", argv[0]);
		return (-1);
	}
	if (argv[1][0] == 's')
	{
		printf("��������Ȩ���б�\n");
		printf("���б��漰Ȩ�����£�����������Ȩ��֮һ���������롣\n");
		printf("�������ܹ�  ���ֲ��ܹ�  �������ܹ�  ��������Ա  �˺Ź���Ա\n");
		printf("�����ҹ���Ա  ϵͳά������Ա  ����  ������  �ٲ���  ������  ������  �ڲ���0/9\n");
		printf("ע�������������������䡢����Ȩ��δ�������鷶Χ\n");
		
		perm = perm_special;
	}
	else if (argv[1][0] == 'l')
	{
		printf("���������б�\n");
		perm = perm_long;
	}
    else if (argv[1][0] == 'b')
	{
		printf("����Ȩ���б�\n");
		perm = perm_bm;
	}							
	else if (argv[1][0] == 'a')
	{
		printf("���б��漰Ȩ�����£�����������Ȩ��֮һ���������롣\n");
		printf("�������ܹ�  ���ֲ��ܹ�  �������ܹ�  ��������Ա\n");
		printf("�˺Ź���Ա  �����ҹ���Ա  ϵͳά������Ա\n");	
		perm = perm_admin;
	}
	else if (argv[1][0] == 'i')
	{
	    printf("���б��漰Ȩ�����£�����������֮һ���������롣\n");
	    printf("������  ��������  �ٲ���  ������  ������  �ڲ���0/9\n");
	    printf("ע��������Ȩ�ޡ�������δ�������鷶Χ\n");
		
		perm = perm_inner;
	}
	else if (argv[1][0] == 'h')
	{
		printf("check_perm N (0<N<31)    ���Ȩ��2^N�\n");
		printf("check_perm a/admin       ����ܹ�Ȩ�ޣ\n");
		printf("check_perm b/bm          ������Ȩ�ޣ\n");
		printf("check_perm i/inner       ����ڲ���Ȩ�ޣ\n");
		printf("check_perm l/long        ��������������\n");
		printf("check_perm s/special     �����������Ȩ�ޣ\n");
		return(-1);
	}
	else
	{
		num = atoi(argv[1]);	
		if (num >= 0 && num < 32)
			perm = 1<< num;
		else
			return (-1);
	}
		
	fread(&u,sizeof(struct userec),1,fp);
	while(!feof(fp)){
		if ( (u.userlevel & perm) != 0)
			print = true;
			
		if(print==true){
			strcpy(genbuf, "ltmprbBOCAMURS#@XLEast0123456789\0");
		 	for (num = 0; num < strlen(genbuf) ; num++)
				if (!(u.userlevel & (1 << num)))
					genbuf[num] = '-';
			printf("%-14s: %s\n",u.userid,genbuf);
			print=false;
		}
		
		//printf("%s\n",u.userid);//�������û���ID���
		fread(&u,sizeof(struct userec),1,fp);		
	}
	return 0;
}
