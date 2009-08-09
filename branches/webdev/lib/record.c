#include "bbs.h"
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>

#define BUFSIZE (MAXUSERS + 244)

USE_TRY;

//	ѭ��д�ַ����ļ���ֱ��д��size���ֽ�
//	origsz,bp�Ƕ���ģ�
static int safewrite(int fd, const void *buf, int size)
{
	int cc, sz = size, origsz = size;
	const void *bp = buf;

	do {
		cc = write(fd, bp, sz);
		if ((cc < 0) && (errno != EINTR)) {
			report("safewrite err!", "");
			return -1;
		}
		if (cc > 0) {
			bp += cc;
			sz -= cc;
		}
	} while (sz > 0);
	return origsz;
}

//	���ļ�filename������,����-1
//	���򷵻�filename�д�ŵļ�¼��
long get_num_records(const char *filename, const int size)
{
	struct stat st;
	if (stat(filename, &st) == -1)
		return 0;
	return (st.st_size / size);
}

//����һ����¼����СΪsize,�׵�ַΪrecord
//	�ļ���Ϊfilename
int append_record(const char *filename, const void *record, int size)
{
	int fd;
	if ((fd = open(filename, O_WRONLY | O_CREAT, 0644)) == -1) {
		report("open file error in append_record()", "");
		return -1;
	}
	FLOCK(fd, LOCK_EX);
	lseek(fd, 0, SEEK_END);
	if (safewrite(fd, record, size) == -1)
		report("apprec write err!", "");
	FLOCK(fd, LOCK_UN);
	close(fd);
	return 0;
}

//ȡ�ü�¼�ľ��,�������rptr��
//	fd���ļ���������,size��ʾ��¼�Ĵ�С,id��ʾ��¼��λ��
//	���ɹ�ʱ,����-1 ; �ɹ�ʱ,����0
static int get_record_handle(int fd, void *rptr, int size, int id)
{
	if (lseek(fd, size * (id - 1), SEEK_SET) == -1)
		return -1;
	if (read(fd, rptr, size) != size)
		return -1;
	return 0;
}

//ȡ�ü�¼,filename��ʾ�ļ���,����������get_record_handle
int get_record(char *filename, void *rptr, int size, int id)
{
	int fd;
	int ret;

	if ((fd = open(filename, O_RDONLY, 0)) == -1)
		return -1;
	ret = get_record_handle(fd, rptr, size, id);
	close(fd);
	return ret;
}

//���ļ�filename�еĵ�id-1����¼����ȡ��СΪsize,����Ϊnumber�ļ�¼��
//	���ʧ��,����-1,���δ�ܶ�ȡnumber����¼,�򷵻ض�ȡ�ļ�¼��
int get_records(const char *filename, void *rptr, int size, int id,
		int number)
{
	int fd;
	int n;
	if ((fd = open(filename, O_RDONLY, 0)) == -1)
		return -1;
	if (lseek(fd, (off_t) (size * (id - 1)), SEEK_SET) == -1) {
		close(fd);
		return 0;
	}
	if ((n = read(fd, rptr, size * number)) == -1) {
		close(fd);
		return -1;
	}
	close(fd);
	return (n / size);
}

#ifndef THREAD_C

//	����Ϊfilename�ļ�¼�ļ�ִ��fptr����
int apply_record(char *filename, APPLY_FUNC_ARG fptr, int size, void *arg,
		int applycopy, int reverse)
{
	void *buf1, *buf2 = NULL;
	int i;
	int count;
	mmap_t m;

	BBS_TRY {
		// TODO: is nolock safe here?
		m.oflag = O_RDONLY;
		if (mmap_open(filename, &m) < 0)
			BBS_RETURN(0);
		mmap_lock(&m, LOCK_UN);
		count = m.size / size; //��¼����Ŀ
		if (reverse)
			buf1 = (char *)m.ptr + (count - 1) * size;
		else
			buf1 = m.ptr;
		for (i = 0; i < count; i++) {
			if (applycopy) {
				buf2 = malloc(size);
				memcpy(buf2, buf1, size);
			} else {
				buf2 = buf1;
			}
			if ((*fptr) (buf2, reverse ? count - i : i + 1, arg) == QUIT) {
				//ִ�к���fptr,bufΪ�������׵�ַ,argΪ��������
				mmap_close(&m); //��ֹ�ڴ�ӳ��,����û����,
				//���ڲ���Ҫ����
				if (applycopy)
					free(buf2);
				BBS_RETURN(QUIT);
			}
			if (reverse)
				buf1 -= size;
			else
				buf1 += size;
		}
	}
	BBS_CATCH {
	}
	BBS_END mmap_close(&m);

	if (applycopy)
		free(buf2);
	return 0;
}

/*---   End of Addition     ---*/
/* search_record������Ԥ���Ż�,�Լ���ϵͳ���ô���,����ٶ�. ylsdd, 2001.4.24 */
/* COMMAN : use mmap to improve search speed */
//	��filename�ļ������� �ȽϺ���Ϊfptr,��������¼Ϊfarg
//	��������ΪO(n)���Կ��ǸĽ�
int search_record(char *filename, void *rptr, int size,
		RECORD_FUNC_ARG fptr, void *farg)
{
	int i;
	void *buf1;
	mmap_t m;

	BBS_TRY {
		// TODO: is nolock safe here?
		m.oflag = O_RDONLY;
		if (mmap_open(filename, &m) < 0)
			BBS_RETURN(0);
		for (i = 0, buf1 = m.ptr; i < m.size / size; i++, buf1 += size) {
			if ((*fptr) (farg, buf1)) {
				if (rptr)
					memcpy(rptr, buf1, size);
				mmap_close(&m);
				BBS_RETURN(i + 1);
			}
		}
	}
	BBS_CATCH {
	}
	BBS_END mmap_close(&m);

	return 0;
}

//	��filename�ļ���id����¼�滻Ϊrptr��ָ�������
int substitute_record(char *filename, void *rptr, int size, int id)
{
	/*     * add by KCN      */
	struct flock ldata;
	int retval;
	int fd;

	if ((fd = open(filename, O_WRONLY | O_CREAT, 0644)) == -1)
		return -1;
	/*
	 * change by KCN
	 * flock(fd,LOCK_EX) ;
	 */
	ldata.l_type = F_WRLCK;
	ldata.l_whence = 0;
	ldata.l_len = size;
	ldata.l_start = size * (id - 1);
	if ((retval = fcntl(fd, F_SETLKW, &ldata)) == -1) {//�Ի��ⷽʽ���ļ�
		//bbslog("user", "%s", "reclock error");
		close(fd);
		/*---	period	2000-10-20	file should be closed	---*/
		return -1;
	}
	if (lseek(fd, size * (id - 1), SEEK_SET) == -1) { //�޷����ļ���ָ��λ��
		// bbslog("user", "%s", "subrec seek err");
		/*---	period	2000-10-24	---*/
		ldata.l_type = F_UNLCK;
		fcntl(fd, F_SETLK, &ldata);
		close(fd);
		return -1;
	}
	//��infotech�޸�,ȥ��safewrite���ж�,�Ժ����ʱ����������
	//if (safewrite(fd, rptr, size) != size)
	safewrite(fd, rptr, size);
	//bbslog("user", "%s", "subrec write err");
	/*
	 * change by KCN
	 * flock(fd,LOCK_UN) ;
	 */

	ldata.l_type = F_UNLCK;
	fcntl(fd, F_SETLK, &ldata);
	close(fd);

	return 0;
}

int delete_record(const char *file, int size, int id,
		RECORD_FUNC_ARG check, void *arg)
{
	mmap_t m;
	int ret;

	if (id <= 0)
		return 0;
	BBS_TRY {
		m.oflag = O_RDWR;
		if (mmap_open(file, &m) < 0)
			BBS_RETURN(-1);
		ret = 0;
		if (id * size> m.size) {
			ret = -2;
		} else {
			if (check) {
				if (!(*check)((char *)m.ptr + (id - 1) * size, arg)) {
					for (id = 0; id * size < m.size; id++)
						if ((*check) ((char *)m.ptr + (id - 1) * size, arg))
							break;
					if (id * size >= m.size)
						ret = -2;
				}
			}
		}
		if (ret == 0) {
			memmove((char *)m.ptr + (id - 1) * size,
					(char *)m.ptr + id * size, m.size - size * id);
			mmap_truncate(&m, m.size - size);
		}
	}
	BBS_CATCH {
		ret = -3;
	}
	BBS_END mmap_close(&m);

	return ret;
}

int insert_record(const char *file, int size, RECORD_FUNC_ARG check, void *arg)
{
	if (check == NULL || arg == NULL)
		return -1;
	mmap_t m;
	m.oflag = O_RDWR;
	if (mmap_open(file, &m) < 0)
		return -1;
	void *iter, *end = (char *)m.ptr + m.size;
	if (mmap_truncate(&m, m.size + size) < 0)
		return -1;
	for (iter = m.ptr; iter != end; iter = (char *)iter + size) {
		if (check(iter, arg))
			break;
	}
	if (iter != end)
		memmove(iter + size, iter, 
				m.size - size - ((char *)iter - (char *)m.ptr));
	memcpy(iter, arg, size);
	mmap_close(&m);
	return 0;
}

#endif
