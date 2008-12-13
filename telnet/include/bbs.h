/*
 $Id: bbs.h 365 2007-05-12 13:07:16Z danielfree $
 */

#ifndef  _BBS_H_
#define _BBS_H_

#ifndef BBSIRC

/* Global includes, needed in most every source file... */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <setjmp.h>
#include <signal.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <errno.h>
#include <time.h>
#include<strings.h>
#ifndef BSD44
#include <stdlib.h>
#endif
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/mman.h>
#include <dirent.h>
#include <syslog.h>

#ifdef DMALLOC
#include <dmalloc.h>
#endif

#include "types.h"
#include "config.h"             	/* User-configurable stuff */

#include "glossary.h"
#include "functions.h"				/* you can enable functions that you want */
#include "permissions.h"

#ifndef LOCK_EX
#define LOCK_EX         2       /* exclusive lock */
#define LOCK_UN         8       /* unlock */
#endif

#ifdef XINU
extern int errno; //������Ϣ���
#endif

#define randomize() srand((unsigned)time(NULL))	//���������ʼ��
#define YEA (1)        /* Booleans  (Yep, for true and false) */
#define NA  (0) 

#define DOECHO (1)     /* Flags to getdata input function */
#define NOECHO (0)

char * bfile();

extern FILE *ufp; /* External variable declarations */
extern long ti;

#endif /* BBSIRC */

/*Added by Ashinmarch on 12.24
 *to support multi-line msgs
 */
#define MAX_MSG_SIZE (240-1)
#define MAX_MSG_LINE 2
#define LINE_LEN 78
/*added end*/
#define MAXFRIENDS (300)
#define MAXREJECTS (32)
#define NUMPERMS   (32)
#define REG_EXPIRED	3600    	/* �������ȷ������ */
#ifdef FDQUAN
#define MAXGUEST	80	   	/* ��� guest �ʺ���վ���� */
#define QUERY_REALNAMES		//վ���ѯʱ,�Ƿ���ʾ��ʵ����
#else
#define MAXGUEST	500
#endif

#ifdef FDQUAN
#define MAX_ANON    300
#else
#define MAX_ANON	2500	//���WWW������
#endif

#define FILE_BUFSIZE        200    /* max. length of a file in SHM*/
#define FILE_MAXLINE         25    /* max. line of a file in SHM */
#define MAX_WELCOME          15    /* ��ӭ������ */
#define MAX_GOODBYE          15    /* ��վ������ */
#define MAX_ISSUE            15    /* ����վ������ */
#define MAX_DIGEST         1000    /* �����ժ�� */
#define MAX_POSTRETRY       100
#define MAX_PREFIX			  9    /* ������ǰ׺��*/
#define MORE_BUFSIZE       4096
#define GOOD_BRC_NUM	70	//�ղؼ������ղذ����� 2007-04-23 ��boards.c������
#ifdef BIGGER_MOVIE
#define MAXMOVIE		8
#else
#define MAXMOVIE		6    /* lines of  activity board  */
#endif

#define ACBOARD_BUFSIZE      512 /* max. length of each line for activity board*/
#define ACBOARD_MAXLINE      256 /* max. lines of  activity board  */
#define STRLEN               80    /* Length of most string data */
#define BM_LEN               60    /* Length of BM id length*/
#define NAMELEN              40    /* Length of username/realname */
#define IDLEN                12    /* Length of userids */

#ifdef MD5		  	   /* MD5 cryptographic support */
#define ENCPASSLEN         35
#else
#define ENCPASSLEN         14  /* Length of encrypted passwd field */
#endif  

#define PASSLEN		 	14    /* User's password length (13 chars) */

//#ifdef CODE_VALID
#define RNDPASSLEN             10        /* ������֤�İ��볤�� (���˷�Χ 4~10)*/
//#endif

#define MAXGOPHERITEMS     9999    /* max of gopher items */
#define PASSFILE     ".PASSWDS"    /* Name of file User records stored in */
#define ULIST_BASE   ".UTMP"       /* Names of users currently on line */
extern char ULIST[];

#ifndef BBSIRC 

#define FLUSH       ".PASSFLUSH"   /* Stores date for user cache flushing */
#define BOARDS      ".BOARDS"      /* File containing list of boards */
#define DOT_DIR     ".DIR"         /* Name of Directory file info */
#define THREAD_DIR  ".THREAD"      /* Name of Thread file info */
#define DIGEST_DIR  ".DIGEST"      /* Name of Digest file info */
#define MARKED_DIR  ".MARKEDDIR"
#define AUTHOR_DIR  ".AUTHORDIR"
#define KEY_DIR	    ".KEYDIR"
#define BOTTOM_DIR  ".BOTTOM"
#define KEYWORDS    ".KEYWORDS"
#define TRASH_DIR   ".TRASH"
#define JUNK_DIR    ".JUNK"

//digest mode
#define NORMAL_MODE       0		//һ��ģʽ
#define DIGIST_MODE       1		//��ժģʽ��G�ģ�
#define THREAD_MODE       2		//ͬ����ģʽ
#define MARK_MODE         3		//m��ģʽ
#define ORIGIN_MODE       4		//ԭ��ģʽ
#define AUTHOR1_MODE      5             //ͬ����ģ������ģʽ
#define AUTHOR2_MODE      6             //ͬ���߾�ȷ����ģʽ
#define KEYWORD_MODE      7		//��������ģʽ
#define TRASH_MODE       10		//����������ģʽ
#define JUNK_MODE	     11		//վ��������ģʽ
#define BOTTOM_MODE	     12		//??
#define ATTACH_MODE          13         //??
#define ANNPATH_SETMODE	0
#define ANNPATH_GETMODE	1

#ifdef ENABLE_NOTICE
#define NOTICE_MODE 13
#define NOTICE_DIR ".NOTICE"
#ifdef FDQUAN
#define MAX_NOTICE 6
#else
#define MAX_NOTICE 5
#endif
#endif

#define QUIT 			0x666    	/* Return value to abort recursive functions */

#define FILE_READ  		0x1        	/* Ownership flags used in fileheader structure */
#define FILE_OWND  		0x2        	/* accessed array */
#define FILE_VISIT 		0x4
#define FILE_MARKED 	0x8
#define FILE_DIGEST 	0x10      	/* Digest Mode*/
#define FILE_FORWARDED 	0x20  		/* Undelete file */
#define MAIL_REPLY 		0x20 		/* Mail Reply */
#define FILE_NOREPLY 	0x40      	/* No Allow Replay */
#define FILE_DELETED 	0x80

#define FILE_NOTICE     0x01
#define FILE_SUBDEL     0x02
#define FILE_LASTONE	0x04
#define FILE_IMPORTED   0x08

/*	����ı�־		*/
#define BOARD_VOTE_FLAG    	0x1		//ͶƱģʽ
#define BOARD_NOZAP_FLAG   	0x2		//no zap
#define BOARD_OUT_FLAG     	0x4		//ת��
#define BOARD_ANONY_FLAG   	0x8		//����ģʽ
#define BOARD_NOREPLY_FLAG 	0x10		//No reply board
#define BOARD_JUNK_FLAG	        0x20		//����������
#define BOARD_CLUB_FLAG         0x40		//���ֲ�����
#define BOARD_READ_FLAG		0x80		//�����ư���
#define BOARD_POST_FLAG         0x100		//postmask
#define BOARD_DIR_FLAG		0x200		//Ŀ¼
#define BOARD_PREFIX_FLAG	0x400	//ǰ׺
#define BOARD_CUSTOM_FLAG	0x80000000		//�ղؼ��Զ���Ŀ¼ defined by cometcaptor 2007-04-16 ��Ϊ��Ŀ¼���Զ��壬��ѡ�ñ�־�����λ���ͱ�׼�����Էֿ�
#define ZAPPED  		0x1         /* For boards...tells if board is Zapped */

/* these are flags in userec.flags[0] */
#define PAGER_FLAG   	0x1   /* true if pager was OFF last session */
#define CLOAK_FLAG   	0x2   /* true if cloak was ON last session */
#define SIG_FLAG     	0x8   /* true if sig was turned OFF last session */
#define BRDSORT_FLAG 	0x20  /* true if the boards sorted alphabetical */
#define BRDSORT_ONLINE 	0x40
#define GIVEUPBBS_FLAG  0x80  /* true if the user give up BBs now (2003.04.22 stephen)*/
#define ACTIVE_BOARD 	0x200 /* true if user toggled active movie board on */

#define MSG_SEPERATOR   "\
������������������������������������������������������������������������������"
#define MULTI_LOGINS	2	/* ͬʱ����վ ID �� */
#ifndef FDQUAN
#define IPMAXLOGINS		5	// ͬIPͬʱ��վID��
#endif
#define DEF_VALUE		"��� ��ӭ�������¹⻪վ! ���"
#define DEF_FILE		"etc/whatdate"

#define ALL_PAGER       0x1
#define FRIEND_PAGER    0x2
#define ALLMSG_PAGER    0x4
#define FRIENDMSG_PAGER 0x8
#define LOGOFFMSG_PAGER 0x10   /* Amigo 2002.04.03 */

#define SHIFTMODE(usernum,mode) ((usernum<MAXUSERS)?mode:mode<<4)

#define SETFILEMODE(array,usernum,mode) \
     (array[usernum%MAXUSERS] |= ((usernum<MAXUSERS)?mode:mode<<4))

#define CLRFILEMODE(array,usernum,mode) \
          (array[usernum%MAXUSERS] &= ((usernum<MAXUSERS)?~mode:~(mode<<4)))

#define CHECKFILEMODE(array,usernum,mode) \
       (array[usernum%MAXUSERS] & ((usernum<MAXUSERS)?mode:mode<<4))
#define USERIDSIZE 		(16)
#define USERNAMESZ 		(24)
#define TERMTYPESZ 		(10)
/* END */

#endif /* BBSIRC */

#ifndef NO_STRUCT_H
#include "struct.h"
#endif

#include "modes.h"              /* The list of valid user modes */

#define DONOTHING       0       /* Read menu command return states */
#define FULLUPDATE      1       /* Entire screen was destroyed in this oper*/
#define PARTUPDATE      2       /* Only the top three lines were destroyed */
#define DOQUIT          3       /* Exit read menu was executed */
#define NEWDIRECT       4       /* Directory has changed, re-read files */
#define READ_NEXT       5       /* Direct read next file */
#define READ_PREV       6       /* Direct read prev file */
#define GOTO_NEXT       7       /* Move cursor to next */
#define DIRCHANGED      8       /* Index file was changed */
#define MODECHANGED     9       /* ... */
#define READ_AGAIN		10

#define I_TIMEOUT   (-2)         /* Used for the getchar routine select call */
#define I_OTHERDATA (-333)       /* interface, (-3) will conflict with chinese */

#define SCREEN_SIZE (23)         /* Used by read menu  */

#define BMLOGLEN			32
#define BMLOG_STAYTIME		0			//ͣ��ʱ��
#define BMLOG_INBOARD		1			//����
#define BMLOG_POST			2			//���ڷ���
#define BMLOG_DIGIST		3			//������ժ
#define BMLOG_UNDIGIST		4			//ȥ����ժ
#define BMLOG_MARK          5           //���m�� 
#define BMLOG_UNMARK        6           //ȥ��m���
#define BMLOG_WATER         7           //���ˮ��
#define BMLOG_UNWATER       8           //ȥ��ˮ��
#define BMLOG_CANNOTRE		9           //��ǲ���re
#define BMLOG_UNCANNOTRE	10          //ȥ������re
#define BMLOG_DELETE        11          //ɾ������(d/B/L/D)
#define BMLOG_UNDELETE      12          //�ָ�ɾ��(y/B/L)
#define BMLOG_DENYPOST      13          //���
#define BMLOG_UNDENY        14          //���
#define BMLOG_ADDCLUB       15          //������ֲ�
#define BMLOG_DELCLUB       16          //ȡ�����ֲ�
#define BMLOG_ANNOUNCE      17          //���뾫��
#define BMLOG_DOANN         18          //������
#define BMLOG_COMBINE       19          //�ϼ�����
#define BMLOG_RANGEANN		20          //L����B���뾫��
#define BMLOG_RANGEDEL      21          //L����Bɾ��
#define BMLOG_RANGEOTHER    23          //L����B��������
extern int scrint; /* Set when screen has been initialized */
/* Used by namecomplete *only* */

extern int digestmode; /*To control Digestmode*/

#ifndef NO_STRUCT_H				 /* NO_STRUCT_H */

extern struct userec currentuser;/*  user structure is loaded from passwd */
/*  file at logon, and remains for the   */
/*  entire session */

extern struct user_info uinfo; /* Ditto above...utmp entry is stored here
 and written back to the utmp file when
 necessary (pretty darn often). */
extern int usernum; /* Index into passwds file user record */
extern int utmpent; /* Index into this users utmp file entry */
extern int count_friends, count_users; /*Add by SmallPig for count users and friends*/

extern int t_lines, t_columns; /* Screen size / width */
extern struct userec lookupuser; /* Used when searching for other user info */

#endif					/* NO_STRUCT_H */
extern char someoneDIR[];
extern int nettyNN;
extern char netty_path[];
extern char netty_board[];
extern char currboard[]; /* name of currently selected board */
extern char currBM[]; /* BM of currently selected board */

extern int selboard; /* THis flag is true if above is active */

extern char genbuf[1024]; /* generally used global buffer */

extern jmp_buf byebye; /* Used for exception condition like I/O error*/

extern char save_title[]; /* These are used by the editor when inserting */
extern int save_gid;
extern char save_filename[]; /* header information */
extern int in_mail;
extern int dumb_term;
extern int showansi;
extern time_t uptime; /* save user last key-in time, up every 1min */

#define Min(a,b) ((a<b)?a:b)
#define Max(a,b) ((a>b)?a:b)

/*SREAD Define*/
#define SR_BMBASE       (10)
#define SR_BMDEL		(11)
#define SR_BMMARK       (12)
#define SR_BMDIGEST     (13)
#define SR_BMIMPORT     (14)
#define SR_BMTMP        (15)
#define SR_BMUNDERLINE 	(16) 
/*SREAD Define*/

#ifndef EXTEND_KEY
#define EXTEND_KEY0
#define KEY_TAB         9
#define KEY_ESC         27
#define KEY_UP          0x0101
#define KEY_DOWN        0x0102
#define KEY_RIGHT       0x0103
#define KEY_LEFT        0x0104
#define KEY_HOME        0x0201
#define KEY_INS         0x0202
#define KEY_DEL         0x0203
#define KEY_BACKSPACE   0x7f
#define KEY_END         0x0204
#define KEY_PGUP        0x0205
#define KEY_PGDN        0x0206
#endif	//EXTEND_KEY	��չ��
/* edwardc.990706 move shmkeys from sysconf.ini */

struct _shmkey { //�����ڴ�������ֵ
	char key[16];
	int value;
};

const static struct _shmkey shmkeys[] = { { "BCACHE_SHMKEY", 30000 }, {
		"UCACHE_SHMKEY", 30010 }, { "UTMP_SHMKEY", 30020 }, {
		"ACBOARD_SHMKEY", 30030 }, { "ISSUE_SHMKEY", 30040 }, {
		"GOODBYE_SHMKEY", 30050 }, { "WELCOME_SHMKEY", 30060 }, {
		"STAT_SHMKEY", 30070 },
#ifdef ALLOWSWITCHCODE
		{	"CONV_SHMKEY", 30080},
#endif
#ifdef CERTIFYMODE
		{	"KEYWORDS_SHMKEY",30090},
#endif
		{ "ACACHE_SHMKEY", 30005 }, { 0, 0 } };

#define SEM_COUNTONLINE 30000

#define Ctrl(c)         ( c & 037 )		//���Կ��ǽ��������д��inline����
#define isprint2(c)     ( (c & 0x80) || isprint(c) )	//����Ŀɴ�ӡ�ַ�
//����ASCII�����пɴ�ӡ�ַ��͵�һλΪ1���ַ�

#ifdef  SYSV						//SYSV��û�ж������º���
//#define bzero(tgt, len)         memset( tgt, 0, len )
#define bcopy(src, tgt, len)    memcpy( tgt, src, len)

#define usleep(usec)            {               \
    struct timeval t;                           \
    t.tv_sec = usec / 1000000;                  \
    t.tv_usec = usec % 1000000;                 \
    select( 0, NULL, NULL, NULL, &t);           \
}

#endif  /* SYSV */

/* =============== ANSI EDIT ================== */
#define   ANSI_RESET    	"\033[0m"
#define   ANSI_REVERSE  	"\033[7m\033[4m"
extern int editansi;
extern int KEY_ESC_arg;
/* ============================================ */

/* Added by deardragon 1999.11.2 */
extern char datestring[];
/* Added End. */

/* added by iamfat 2002.08.29 */
//extern int technician;
/* added end. */

//added by iamfat 2003.02.27
#define DEBUG(x) {file_append("debug", #x"\n");x;}	//����ģʽ
#endif /* of _BBS_H_ */

#define FLOCK(x,y) flock(x,y)

#define SHM_HOMEDIR "tmp"

//add by infotech,supporting for 5 BMS
#define BMMAXNUM		(5)				//ÿ���������������
#define BMNAMELISTLIMIT					//��ʾ��������ʱʹ��...
#define BMNAMELISTLEN	(56)		//��ʾ��������б�������
#define BMNAMEMAXLEN	(56)		//ÿ�����������ʱ�����
//���BMNAMELISTLEN��BMNAMEMAXLEN��ͬ,��ʾ�����б�ʱ,����...����ʾ��������
//BMNAMELISTLENһ��ҪС��BMNAMEMAXLEN,�Ҿ�С��56,�����ڴ�Խ��
