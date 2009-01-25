#include <bbs.h>

// Returns the path of 'filename' under the home directory of 'userid'.
char *sethomefile(char *buf, const char *userid, const char *filename)
{
	sprintf(buf, "home/%c/%s/%s", toupper(userid[0]), userid, filename);
	return buf;
}

// Returns the path of board 'boardname'.
char *setbpath(char *buf, const char *boardname)
{
	strcpy(buf, "boards/");
	strcat(buf, boardname);
	return buf;
}

// Returns the path of DOT_DIR file under the directory of 'boardname'.
char *setwbdir(char *buf, const char *boardname)
{
	sprintf (buf, "boards/%s/" DOT_DIR, boardname);
	return buf;
}

// Returns the path of 'filename' under the directory of 'boardname'.
char *setbfile(char *buf, const char *boardname, const char *filename)
{
	sprintf(buf, "boards/%s/%s", boardname, filename);
	return buf;
}

// Returns the path of 'filename' under the mail directory of 'userid'.
char *setmfile(char *buf, const char *userid, const char *filename)
{
	sprintf(buf, "mail/%c/%s/%s", toupper(userid[0]), userid, filename);
	return buf;
}

// Returns the path of '.DIR' under the mail directory of 'userid'.
char *setmdir(char *buf, const char *userid)
{
	sprintf(buf, "mail/%c/%s/" DOT_DIR, toupper(userid[0]), userid);
	return buf;
}

//	���ļ�filenameӳ�䵽�ڴ���,
//	��ret_fdΪ��,����ָ���filename��������,����ס�ļ�,��*size��Ϊ�ļ���С
//	�ɹ�ʱ����1,����0
int safe_mmapfile(char *filename, int openflag, int prot, int flag,
		void **ret_ptr, size_t * size, int *ret_fd) {
	int fd;
	struct stat st;

	fd = open(filename, openflag, 0600);
	if (fd < 0)//δ�ɹ��� 
		return 0;
	if (fstat(fd, &st) < 0) { //δ�ɹ�����ļ�״̬
		close(fd);
		return 0;
	}
	if (!S_ISREG(st.st_mode)) { //�ǳ����ļ�,�����ļ�������ָ����ļ�����
		close(fd);
		return 0;
	}
	if (st.st_size <= 0) { //�ļ���СΪ0
		close(fd);
		return 0;
	}
	*ret_ptr = mmap(NULL, st.st_size, prot, flag, fd, 0);//ӳ�������ļ����ڴ���
	if (!ret_fd) {
		close(fd);
	} else {
		*ret_fd = fd;
		flock(fd, LOCK_EX);
	}
	if (*ret_ptr == NULL)
		return 0;
	*size = st.st_size;
	return 1;
}

