#include "bbs.h"
#include <sys/param.h>
#include <sys/resource.h>
#include <pwd.h>
#include <unistd.h>

#ifndef MD5
#ifndef DES
/* nor DES, MD5, fatal error!! */
#error "(pass.c) you've not define DES nor MD5, fatal error!!"
#endif
#endif

static unsigned char itoa64[] = /* 0 ... 63 => ascii - 64 */
"./0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

//	ȡv�ĸ�6λ,����ȡֵ�����itoa64�������ҵ�
static void to64(char *s, long v, int n) {
	while (--n >= 0) {
		*s++ = itoa64[v & 0x3f];
		v >>= 6;
	}
}

// ��pw���м��ܲ���������
char *genpasswd(const char *pw)
{
	char salt[10];
	static char pwbuf[PASSLEN];
	struct timeval tv;
	if (strlen(pw) == 0)
		return "";

	srand(time(0) % getpid());
	gettimeofday(&tv, 0);

#ifdef MD5			/* use MD5 salt */

	strlcpy(&salt[0], "$1$", 3);
	to64(&salt[3], random(), 3);
	to64(&salt[6], tv.tv_usec, 3);
	salt[8] = '\0';
#endif
#ifdef DES			/* use DES salt */

	to64(&salt[0], random(), 3);
	to64(&salt[3], tv.tv_usec, 3);
	to64(&salt[6], tv.tv_sec, 2);
	salt[8] = '\0';
#endif

	strcpy(pwbuf, pw);
	return crypt(pwbuf, salt);//����pwbuf��salt����DES���ܵĽ��
}

#ifdef CHKPASSWDFORK
int checkpasswd(const char *passwd, const char *test) //�����½��̼������,���ⱻ����?
{ // test Ϊ���������,passwdΪsaltֵ
	int pfds[2], pid;
	char value = 'f';
	if( pipe(pfds) < 0) {
		return checkpasswd0(passwd, test);
	}
	pid = fork();
	if( pid == 0) {
		close(pfds[0]);
		if( checkpasswd0(passwd, test) ) {
			value = 't';
		}
		write(pfds[1], &value, 1);
		close(pfds[1]);
		exit(0);
	} else if( pid == -1 ) {
		return checkpasswd0(passwd, test);
	} else {
		close(pfds[1]);
		read(pfds[0], &value, 1);
		close(pfds[0]);
		return ( value == 't');
	}
}

int checkpasswd0(char *passwd, char *test)
#else
int checkpasswd(const char *passwd, const char *test) //�������
#endif											// test Ϊ�û����������ַ���
{ // passwd Ϊsaltֵ
	char *pw;
	static char pwbuf[PASSLEN];

	strlcpy(pwbuf, test, PASSLEN);
	pw = crypt(pwbuf, passwd);
	return (!strcmp(pw, passwd));
}
