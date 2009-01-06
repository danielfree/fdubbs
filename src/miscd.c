#include "bbs.h"

extern int flush_ucache();
extern int resolve_ucache();

extern int load_ucache();
extern void resolve_boards();
//�˳�ʱִ�еĺ���
void do_exit() {
	flush_ucache();
	flush_bcache();
}

int main(int argc, char *argv[]) {
	chdir(BBSHOME); //����BBS�û���Ŀ¼
	setuid(BBSUID); //�����̵� �û�ID
	setgid(BBSGID); //��ID���ó�BBS
	setreuid(BBSUID, BBSUID); //������Ч�û�ID	
	setregid(BBSGID, BBSGID); //��Ч��IDΪBBS

	if (argc <= 1) {
		printf("usage: daemon | flushed | reload\n");
		exit(0);
	}
	if ( !strcasecmp(argv[1], "daemon") ) { // miscd daemon
		switch (fork()) { //��̨����:��Ҫ����һ���ӽ���,���ӽ���ɱ��������
			case -1: //
				printf("cannot fork\n");
				exit(0);
				break;
			case 0: // �ӽ���
				break;
			default:
				exit(0); //������
				break;
		}

		if (load_ucache(0) != 0) { //���û�������ӳ�䵽�ڴ�
			printf("load ucache error\n");
			exit(-1);
		}

		resolve_boards();
		atexit(do_exit); //ע���˳�ǰ���еĺ���.�����˳�ǰ��ִ�д˺���

		while (1) { //ѭ��
			refresh_utmp(); //ˢ���û���ʱ����
			b_closepolls(); //�ر�ͶƱ
			flush_ucache(); //���û����ڴ��е�����д��.PASSWDS
			flush_bcache();
			sleep(60 * 15); //˯��ʮ����,��ÿʮ�����ͬ��һ��.        
		}
	} else if ( !strcasecmp(argv[1], "flushed") ) { //miscd flushed
		resolve_ucache();
		flush_ucache();
		flush_bcache();
	} else if ( !strcasecmp(argv[1], "reload") ) { //miscd reload
		if (load_ucache(1) != 0) {
			printf("load ucache error\n");
			exit(-1);
		}
	} else {
		printf("usage: daemon | flushed | reload\n");
		exit(0);
	}

	return 0;
}
