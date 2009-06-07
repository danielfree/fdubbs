#ifndef _FUNCTIONS_H_
#define _FUNCTIONS_H_

#ifdef FDQUAN
#define ENABLE_SHOWONLINE
#endif
#define ENABLE_NOTICE	//�õ׹���
#define IP_2_NAME //����IP��ʾ������ͬʱ������ʹ���������Ĺ���
#define NEWONLINECOUNT
// ������һЩ���ܵĿ��ƣ�������θÿ��ƣ����ʾ��ʹ���������
// �������ǰ���� "//" ���ʾ�����Σ�Ҫ���θù��ܣ������ "//"
#ifdef FDQUAN
#define ENABLE_PREFIX
#endif
#define SHOW_THANKYOU		/* ��ʾԴ�����ṩ����Ϣ */

/*  ע����ز��ִ���� define */
/*  һ��������ע���ⲿ������һЩ����� �������ܿ�������Ƚ϶����ѵ���Ҫ */
#define NEWCOMERREPORT		/* ������·�� newcomers ���Զ����� */
//#define MAILCHECK		/* �ṩ�ʼ�ע�Ṧ�� */
//#define CODE_VALID 		/* ������֤ */
#define SAVELIVE		/* �����ʺ� ��ֹ�û���ʱ�䲻���߶�����*/
//#define AUTOGETPERM		/* ����ע�ἴ��ȡ����Ȩ�� */
//#define PASSAFTERTHREEDAYS	/* ������·�������� */
//#define MAILCHANGED		/* �޸� e-mail ��Ҫ������ע��ȷ�� */

/* ��Ϸ��ش���� define */
#ifdef FDQUAN
#define ALLOWGAME		/* ֧����Ϸ, �ṩ��Ǯ��ʾ */
//#define FIVEGAME 		/* ������ */
#endif
/* ϵͳ��ȫ��ش���� define */
//#define MUDCHECK_BEFORELOGIN 	// ��½ǰ����ȷ�� 
//#define AddWaterONLY		// �����ǿ����ˮվ�Լ�ʹ�õĴ��룬������

/* ĳЩ�����Դ������� define */
//#define KEEP_DELETED_HEADER 	/* ����ɾ�����¼�¼ */
//#define BELL_DELAY_FILTER	/* ���������е��������ʱ���� */
#define CHECK_FREQUENTLOGIN /* Ƶ����¼��� */
//#define MARK_X_FLAG		/* ����ˮ���¼��� 'X' ��� */

/* һ�㲻��Ҫ����� define */
//#define SHOWMETOFRIEND		/* �����ķ����Ƿ�Ϊ�Է����ѵ���ʾ */
#define BBSD    // Always run bbs in daemon mode. Never comment out this line.
#define ALLOWAUTOWRAP		/* �����Զ��Ű湦�� */
#define ALLOWSWITCHCODE		/* ���� GB �� <==> Big5 �� �л� */
#define USE_NOTEPAD		/* ʹ�����԰� */
#define INTERNET_EMAIL		/* ���� InterNet Mail */
#define COLOR_POST_DATE 	/* ����������ɫ */
#define TALK_LOG 		/* �����¼���� */
#define RNDSIGN			/* ����ǩ���� */
//#define DOMAIN_NAME		/* ��½ʱ������������ */

/* ��վ����ϲ������ ����ξ���ΰɣ� ��Ȼ��Ҫ����������Ǻ� */
//#define MSG_CANCLE_BY_CTRL_C 	/* �� ctrl-c ������ѶϢ */
#define LOG_MY_MESG		/* ѶϢ��¼�м�¼�Լ���������ѶϢ */
#define BIGGER_MOVIE 		/* �Ӵ�����ռ� (����) */
//#define ALWAYS_SHOW_BRDNOTE 	/* ÿ�ν��涼�� show �����滭�� */

#define USE_TRY extern sigjmp_buf bus_jump
#define BBS_TRY \
    	if (!sigsetjmp(bus_jump, 1)) { \
        	signal(SIGBUS, sigbus);

#define BBS_CATCH \
	} \
	else { \

#define BBS_END } \
	signal(SIGBUS, SIG_IGN);

#define BBS_RETURN(x) {signal(SIGBUS, SIG_IGN);return (x);}
#define BBS_RETURN_VOID {signal(SIGBUS, SIG_IGN);return;}

typedef int (*RECORD_FUNC_ARG)(void *, void *);
typedef int (*APPLY_FUNC_ARG)(void *, int, void *);

#endif 
/* _FUNCTIONS_H_ */
