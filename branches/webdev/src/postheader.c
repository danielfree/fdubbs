/*
 $Id: postheader.c 366 2007-05-12 16:35:51Z danielfree $
 ��ҳ����Ҫ���������£��ظ����ºͷ��ż�ʱ��ͷ����
 */

#include "bbs.h"

extern int numofsig;
extern int noreply;
extern int mailtoauther;
extern struct boardheader *getbcache();
#ifdef ENABLE_PREFIX
char prefixbuf[MAX_PREFIX][6];
int numofprefix;
#endif
void check_title(char *title) {
	char tempfilename[50];
	trim(title);
	if (!strncasecmp(title, "Re:", 3) && !HAS_PERM(PERM_SYSOPS)) {
		sprintf(tempfilename, "Re��%s", &title[3]);
		strcpy(title, tempfilename);
	}
}
#ifdef ENABLE_PREFIX
void set_prefix() {
	char filename[STRLEN], buf[128];
	int i;
	FILE *fp;

	setvfile(filename, currboard, "prefix");
	if ((fp = fopen(filename, "r"))> 0) {
		for (i = 0; i < MAX_PREFIX; i++) {
			if (!fgets(buf, STRLEN, fp)) {
				break;
			}
			if (i == 0 && (buf[0] == '\n' || buf[0] == ' ') )
			buf[0] = '\0';
			strtok(buf, " \r\n\t");
			ansi_filter(prefixbuf[i], buf);
			prefixbuf[i][4] = '\0';
		}
		numofprefix = i;
		fclose(fp);
	} else numofprefix = 0;
}

void print_prefixbuf(char *buf, int index) {
	int i;
	buf += sprintf(buf, "ǰ׺:");
	for (i = 0; i < numofprefix; i++ ) {
		if (i == 0 && prefixbuf[i][0] == '\0' )
		buf += sprintf(buf, "1.%s��%s",
				(index == i + 1)?"\033[1m":"",
				(index == i + 1)?"\033[m":"");
		else
		buf += sprintf(buf, " %d.%s%s%s", i + 1,
				(index == i + 1)?"\033[1;33m":"",
				prefixbuf[i],
				(index == i + 1)?"\033[m":"");
	}
	sprintf(buf, "[%d]:", index);
}
#endif
int post_header(struct postheader *header) {
	int anonyboard = 0;
#ifdef ENABLE_PREFIX
	int index = 0;
	char pbuf[128];
#endif
	char r_prompt[20], mybuf[256], ans[5];
	char titlebuf[STRLEN];
	//�ڻظ�ģʽ��Ͷ��ʱ��Ϊ�����buffer
	struct boardheader *bp;
#ifdef IP_2_NAME
	extern char fromhost[];
#endif
#ifdef RNDSIGN  //���ǩ����
	/*rnd_sign=0���������,=1�������*/
	int oldset, rnd_sign = 0;
#endif

	//�Ե�ǰǩ������Խ�紦��
	if (currentuser.signature > numofsig || currentuser.signature < 0) {
		currentuser.signature = 1;
	}
#ifdef RNDSIGN
	if (numofsig> 0) {
		if (DEFINE(DEF_RANDSIGN)) {
			oldset = currentuser.signature;
			srand((unsigned) time(0));
			currentuser.signature = (rand() % numofsig) + 1;//�������ǩ����
			rnd_sign = 1; //������ǩ����
		} else {
			rnd_sign = 0;
		}
	}
#endif

	/*�����ǰ�ǻظ�ģʽ�����ԭ����copy����ǰ�����У�����ǵ�ǰΪ�ظ�ģʽ
	 ����ǰ����Ϊ��*/
	if (header->reply_mode) {
		strcpy(titlebuf, header->title);
		header->include_mode = 'R'; //exchange 'R' and 'S' by Danielfree 07.04.05
	} else {
		titlebuf[0] = '\0';
#ifdef ENABLE_PREFIX
		set_prefix();
#endif
	}

	//bp��¼��ǰ���ڰ������Ϣ
	bp = getbcache(currboard);

	//����Ƿ������£������ȼ�����ڰ����Ƿ�����������ʱ������������⣩
	if (header->postboard) {
		anonyboard = bp->flag & BOARD_ANONY_FLAG;
	}

#ifdef IP_2_NAME
	if (anonyboard && (fromhost[0] < '1'||fromhost[0]> '9'))
	anonyboard = 0;
#endif

	// modified by roly 02.03.07
	// modified by iamfat 02.10.30
	header->chk_anony = (anonyboard) ? 1 : 0;
	//header->chk_anony = 0;
	//modifiy end
#ifdef ENABLE_PREFIX
	if (numofprefix> 0)
	index = 1;
#endif	
	while (1) {
#ifdef ENABLE_PREFIX
		if (header->reply_mode) {
			sprintf(r_prompt, "����ģʽ [[1m%c[m]", header->include_mode);
			move(t_lines - 4, 0);
		} else if (numofprefix == 0)
		move(t_lines - 4, 0);
		else
		move(t_lines - 5, 0);
#else
		if (header->reply_mode)
			sprintf(r_prompt, "����ģʽ [[1m%c[m]", header->include_mode);
		move(t_lines - 4, 0);
#endif
		//�����������
		clrtobot();

		//������Ӧ������ӡ����ǰ��Ϣ
		prints(
				"[m%s [1m%s[m      %s    %s%s\n",
				(header->postboard) ? "���������" : "�����ˣ�",
				header->ds,
				(anonyboard) ? (header->chk_anony == 1 ? "[1mҪ[mʹ������"
						: "[1m��[mʹ������") : "",
				(header->postboard) ? ((noreply) ? "[����[1;33m������[m�ظ�"
						: "[����[1;33m����[m�ظ�") : "",
				(header->postboard&&header->reply_mode) ? ((mailtoauther) ? ",��[1;33m����[m��������������]"
						: ",��[1;33m������[m��������������]")
						: (header->postboard) ? "]" : "");
#ifdef ENABLE_PREFIX
		if (!header->reply_mode && numofprefix) {
			if (bp->flag & BOARD_PREFIX_FLAG && !header->title[0]) {
				index = 0;
				print_prefixbuf(pbuf, index);
				while (1) {
					getdata(t_lines - 4, 0, pbuf, ans, 2, DOECHO, YEA);
					if (!ans[0])
					return NA;
					index = ans[0] - '0';
					if (index> 0 && index<= numofprefix) {
						print_prefixbuf(pbuf, index);
						break;
					}
				}
			} else {
				print_prefixbuf(pbuf, index);
			}
			move(t_lines - 4, 0);
			prints(pbuf);
		}

		//���ڻظ��ͷ��ţ�title��ʼ��Ϊ��.����ֻ���ڷ�������ʱ���Ż����"[�����趨����]"
		move(t_lines-3, 0 );
#endif
		prints("ʹ�ñ���: [1m%-50s[m\n",
				(header->title[0] == '\0') ? "[�����趨����]" : header->title);
#ifdef RNDSIGN
		//�ڻظ�ģʽ�»������Ӧ������ģʽ��Ϣ
		prints("ʹ�õ� [1m%d[m ��ǩ����     %s %s", currentuser.signature
				,(header->reply_mode) ? r_prompt : "", (rnd_sign == 1) ? "[���ǩ����]" : "");
#else
		prints("ʹ�õ� [1m%d[m ��ǩ����     %s", currentuser.signature,
				(header->reply_mode) ? r_prompt : "");
#endif
		//���ڷ������»���Ͷ�����
		if (titlebuf[0] == '\0') {
			//move����Ӧ���У�Ϊ������׼��
			move(t_lines - 1, 0);
			if (header->postboard == YEA || strcmp(header->title, "û����"))
				ansi_filter(titlebuf, header->title);

			//�ӵ�ǰ�л���û�����ŵ�titlebuf�У�������50-1���ֽ�(�˴����������û������ϣ�ֻ����Ӧenter)
			getdata(t_lines - 1, 0, "����: ", titlebuf, 50, DOECHO, NA);
			check_title(titlebuf);

			//���û�����Ϊ�յ�����£�����Ƿ���������ֱ��ȡ���������Ͷ���û������Լ�������ͷΪû����
			if (titlebuf[0] == '\0') {
				if (header->title[0] != '\0') {
					titlebuf[0] = ' ';
					continue;
				} else
					return NA;
			}

			//���û��趨title���Ƶ���Ӧ�ṹ��
			strcpy(header->title, titlebuf);
			continue;
		}

		trim(header->title); //add by money 2003.10.29.
		move(t_lines - 1, 0);

#ifdef ENABLE_PREFIX	
		sprintf(mybuf,
				"[1;32m0[m~[1;32m%d V[m��ǩ����%s [1;32mX[m���ǩ����,[1;32mT[m����%s%s%s,[1;32mQ[m����:",
				numofsig, (header->reply_mode) ? ",[1;32mS[m/[1;32mY[m/[1;32mN[m/[1;32mR[m/[1;32mA[m ����ģʽ" : " \033[1;32mF\033[mǰ׺",
				(anonyboard) ? "��[1;32mL[m����" : "",(header->postboard)?",[1;32mU[m����":"",(header->postboard&&header->reply_mode)?",[1;32mM[m����":"");
#else
		sprintf(
				mybuf,
				"[1;32m0[m~[1;32m%d V[m��ǩ����%s [1;32mX[m���ǩ����,[1;32mT[m����%s%s%s,[1;32mQ[m����:",
				numofsig,
				(header->reply_mode) ? ",[1;32mS[m/[1;32mY[m/[1;32mN[m/[1;32mR[m/[1;32mA[m ����ģʽ"
						: "", (anonyboard) ? "��[1;32mL[m����" : "",
				(header->postboard) ? ",[1;32mU[m����" : "",
				(header->postboard&&header->reply_mode) ? ",[1;32mM[m����"
						: "");
#endif
		//��ӡ����ʾ��Ϣ�����������û����붯����,�û��������2���ֽ�
		getdata(t_lines - 1, 0, mybuf, ans, 3, DOECHO, YEA);
		ans[0] = toupper(ans[0]);

		//�û���ǩ�������ã�����ȡ����ǰ����
		if ((ans[0] - '0') >= 0 && ans[0] - '0' <= 9) {
			//��Ч��ǩ����ѡ��
			if (atoi(ans) <= numofsig)
				currentuser.signature = atoi(ans);
		} else if (ans[0] == 'Q' || ans[0] == 'q') { //ȡ����ǰ����
			return -1;
		}
		//���ڻظ�ģʽ
		else if (header->reply_mode && (ans[0] == 'Y' || ans[0] == 'N'
				|| ans[0] == 'A' || ans[0] == 'R'||ans[0]=='S')) {
			header->include_mode = ans[0];
		} //�������ñ���
		else if (ans[0] == 'T') {
			titlebuf[0] = '\0';
		}//������������ر��� 
		else if (ans[0] == 'L' && anonyboard) {
			header->chk_anony = (header->chk_anony == 1) ? 0 : 1;
		}//���������У��ɷ�ظ��ĸ��� 
		else if (ans[0] == 'U' && header->postboard) {
			noreply = ~noreply;
		}//���ڻظ�����ʱ�������������� 
		else if (ans[0] == 'M' && header->postboard && header->reply_mode) {
			mailtoauther = ~mailtoauther;
		}//��ǩ�����Ĵ��� 
		else if (ans[0] == 'V') {
			setuserfile(mybuf, "signatures");
			if (askyn("Ԥ����ʾǰ����ǩ����, Ҫ��ʾȫ����", NA, YEA) == YEA)
				ansimore(mybuf);
			else {
				clear();
				ansimore2(mybuf, NA, 0, 18);
			}
#ifdef RNDSIGN
		}//���ǩ���� 
		else if (ans[0] == 'X') {
			if (rnd_sign == 0 && numofsig != 0) {
				oldset = currentuser.signature;
				srand((unsigned) time(0));
				currentuser.signature = (rand() % numofsig) + 1;
				rnd_sign = 1;
			} else if (rnd_sign == 1 && numofsig != 0) {
				rnd_sign = 0;
				currentuser.signature = oldset;
			}
			ans[0] = ' ';
#endif
		}
#ifdef ENABLE_PREFIX
		//�޸�ǰ׺
		else if (!header->reply_mode && numofprefix && ans[0] == 'F') {
			int i;
			getdata(t_lines - 1, 0, pbuf, ans, 3, DOECHO, YEA);
			i = ans[0] - '0';
			if (i >= 0 && i <= numofprefix &&
					!(i == 0 && (bp->flag & BOARD_PREFIX_FLAG)))
			index = i;
		}
#endif
		else {
			if (header->title[0] == '\0')
				return NA;
			else {
#ifdef ENABLE_PREFIX
				strcpy(header->prefix, index?prefixbuf[index - 1]:"");
#endif
				return YEA;
			}
		}
	}
}

