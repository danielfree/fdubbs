/* �����ˮվ �����г����� 1999.12.19 */
#include <string.h>
#include <stdlib.h>

#include "bbs.h"
#define MAXBET 5000
typedef struct exchanges {
	char	title[STRLEN];
	int	value;
	char	information[STRLEN];
} EXCHANGES;
int gotomarket(char *title)
{
        if (!strcmp("guest", currentuser.userid)) return 1;
        modify_user_mode(MARKET);
        clear();
        set_safe_record();
        move(2,0);
        prints("��ӭ���� [[32m%s[37m]....\n\n",title);
	return 0;
}

int lending()
{
        int     id, canlending=0,maxnum = 0, num = 0;
	char	ans[STRLEN];
	time_t 	now=time(0);
	extern int gettheuserid();
	if(gotomarket("�����г�")) return 0;
	maxnum = currentuser.money - currentuser.bet - 1000;
	prints("��ӭʹ��[0;1;33m"BBSNAME"[37m�����г�[32m����ת��[37m����[m");
	prints("\n\n��Ŀǰ������ǣ�\nע������([32m%d[37m ��) ��վ��ʱ��([32m%d[37m Сʱ) [44;37m��ת���ʽ�([32m%d[37m Ԫ)[m[37m",
		(now - currentuser.firstlogin)/86400,currentuser.stay/3600,currentuser.money-currentuser.bet-1000);
	if ( currentuser.stay <= 36000 || now - currentuser.firstlogin  <= 2592000 || maxnum <= 0 ) {
		 prints("\n\nĿǰ�г��涨�� ����[32m����ת��[m�� ID ����߱�����ȫ��������\n    1. ���ʺ�ע���������� 30 ��;\n    2. ����վʱ������ 10 Сʱ;\n    3. ����ӵ�д��� 1000 Ԫ.\n      (ע��ָ����ȥ�����Ĳ�ֵ.)");
		prints("\n\n�����г��涨����Ŀǰ��û��[32m����ת��[m���ʸ� :P \n");
		pressanykey();
		return 0;
	}
        if (!gettheuserid(9,"����ת�ʵ�˭���ʻ��ϣ������������ʺ�: ",&id))
                return 0;
	if(!strcmp(currentuser.userid,lookupuser.userid)) {
		prints("\n�Ǻǣ�ת�ʸ��Լ����� �ţ�Ҳ�С�������վ���ṩ�������");
		pressanykey();
		return 0;
	}
        if( lookupuser.money+lookupuser.nummedals*1000 > 90000 ) {
                prints("\n�Բ��𣬶Է�Ŀǰ���������в���Ҫ����ת�ʣ�");
                pressanykey();
                return 0;
        }
	move(10,0);
	canlending = maxnum > 90000 ? 90000 : maxnum;
	prints("����ת�ʵ� [1;32m%s[m ���ʺţ�������������ת�� [1;33m%d[m Ԫ��",lookupuser.userid, canlending);
        getdata(12, 0, "ȷ��Ҫת�ʣ�������ת����Ŀ��������ֱ�ӻس�ȡ��ת��: ",ans, 6, DOECHO, YEA);
        num = atoi(ans);
        if ( num <= 0 || num > canlending ) {
                prints("\n�����д���Ŷ�� �������˰ɡ�����");
                pressanykey();
                return 0;
        }
	set_safe_record();
	if(currentuser.money - currentuser.bet - 1000 != maxnum) {
		prints("\n�Բ������Ŀ�ת���ʽ������仯��ȡ���˴ν��ף�");
		prints("\n������ִ�б����ס�");
		pressanykey();
		return 0;
	}
 	currentuser.money -= num;
	substitut_record(PASSFILE, &currentuser, sizeof(currentuser),usernum);
	lookupuser.money += num;
	substitut_record(PASSFILE, &lookupuser, sizeof(lookupuser), id);
	prints("\nлл�������Ѿ��� [1;32m%s[m ת�� [1;33m%d[m Ԫ��",lookupuser.userid,num);
	prints("\nΪ��ʾ�����ת����Ϊ�ĸ�л����վ�Ѿ����ż�֪ͨ������");
	sprintf(genbuf,"�� %s ת�� %d Ԫ",lookupuser.userid,num);
	gamelog(genbuf);
	sprintf(ans,"%s ���������� %d Ԫ����ת��",currentuser.userid,num);
	sprintf(genbuf,"\n %s :\n\t���ã�\n\t�������� %s ���������� %d Ԫ����ת���ʽ�\n\t������ Market ���������˽��Ϊ���ı�ʾ��л��\n\t���������Ϳ��Ի���������ת���ʽ�\n\n\t��Ȼ����Ҳ�����˳�һ�κ��ٽ��뱾վ��\n\tһ�����Ի���������ת���ʽ�\n  ",lookupuser.userid,currentuser.userid,num);
	autoreport(ans,genbuf,NA,lookupuser.userid);
	pressanykey();
	return 1;
}

int popshop(void)
{
	int no,num,maxnum,templog;
	char ans[10];
	EXCHANGES exchanges[10] = {
		{"��վ����",2},
		{"������",5},
		{"������",10000},
		{"������",16000},
		{"����������",4500},
		{"�ʺ����ñ���",45000},
		{"ǿ�ƺ���",54000}, //expired function 06.1.5
		{"�ӳ�����ʱ��",9000},//expired function 06.1.5
		{"������",45000}
		};
	if(gotomarket("�г��䵱��")) return 1;
	prints("���յ䵱����:");
	for(no = 0; no < 9; no ++) {
		move(5+no, 2);
		prints("(%2d) %s",no+1,exchanges[no].title);
		move(5+no, 20);
		prints("==> %6d Ԫ", exchanges[no].value);
	}
	move(16,0);
	prints("��Ŀǰ�������: (1)��վ����([32m%d[37m)  (2)������([32m%d[37m)  (3)������.([32m%d[37m)\n",
	    currentuser.numlogins,currentuser.numposts,currentuser.nummedals);
	getdata(18, 0, "����䵱��һ�", ans, 10, DOECHO, YEA);
	no = atoi(ans);
	if ( no < 1 || no > 9 ) {
		prints("\n�Ǻǣ����䵱�ˣ� �ǣ����ߡ�����ӭ���� ;)");
		pressanykey();
		return 0;
	}
	move(18, 30);
	prints("��ѡ��䵱��[32m%s[m����",exchanges[no-1].title);
if(no>3){
	set_safe_record();
	maxnum = exchanges[no-1].value;
	switch(no) {
		case 4:
			if(!HAS_PERM(PERM_CLOAK)) {
				num = 0;
				break;
			}
			break;
		case 5:
                        if(!HAS_PERM(PERM_SEECLOAK)) {
                                num = 0;
                                break;
                        }
			break;
		case 6:
                        if(!HAS_PERM(PERM_XEMPT)) {
                                num = 0;
                                break;
                        }
			break;
		case 7:
                        //if(!HAS_PERM(PERM_FORCEPAGE)) {
                        //        num = 0;
                        //        break;
                        //} 
                        num = 0;
			break;
		case 8:
                        //if(!HAS_PERM(PERM_EXT_IDLE)) {
                        //        num = 0;
                        //        break;
                        //}
                        num = 0;
			break;
		case 9:
                        if(!HAS_PERM(PERM_LARGEMAIL)) {
                                num = 0;
                                break;
                        }
			break;
	}
	prints("\n\n");
	if(!num) {
		prints("�Բ���, �㻹û������Ȩ��. ");
		pressanykey();
		return 0;
	}
        if(askyn("��ȷ��Ҫ�䵱��",NA,NA) == NA ) {
                move(23,0);clrtoeol();
                prints("���ڲ��䵱�ˣ� �����´������� Ҫ�ǵ�Ŷ��");
                pressanykey();
                return 0;
        }
	set_safe_record();
        switch(no) {
                case 4:
                        num = HAS_PERM(PERM_CLOAK);
			currentuser.userlevel &= ~PERM_CLOAK ;
                        break;
                case 5:
                        num = HAS_PERM(PERM_SEECLOAK);
                        currentuser.userlevel &= ~PERM_SEECLOAK ;
                        break;
                case 6:
                        num = HAS_PERM(PERM_XEMPT);
                        currentuser.userlevel &= ~PERM_XEMPT ;
                        break;
                case 7:
                        //num = HAS_PERM(PERM_FORCEPAGE);
                        //currentuser.userlevel &= ~PERM_FORCEPAGE ;
                        break;
                case 8:
                        //num = HAS_PERM(PERM_EXT_IDLE);
                        //currentuser.userlevel &= ~PERM_EXT_IDLE ;
                        break;
                case 9:
                        num = HAS_PERM(PERM_LARGEMAIL);
                        currentuser.userlevel &= ~PERM_LARGEMAIL ;
                        break;
	}
        if(!num) {
                prints("�Բ���, ������ݷ����˱仯, ��Ŀǰû������Ȩ��. ");
                pressanykey();
                return 0;
        }
} else {
	if( no == 1 )maxnum = currentuser.numlogins;
	else if ( no == 2) maxnum = currentuser.numposts;  
	else	maxnum = currentuser.nummedals;
	templog = maxnum;
	sprintf(genbuf,"����䵱������(���%d)��",maxnum);
	getdata(19, 0, genbuf,ans, 10, DOECHO, YEA);
	num = atoi(ans);
	if ( num <= 0 || num > maxnum ) {
		prints("�����д���Ŷ�� �������˰ɡ�����");
		pressanykey();
		return 0;
	}
        maxnum = num*exchanges[no-1].value;
	move(19,0);
	prints("�����Ƶ䵱%s[32m%d[m �ݣ� %s [33m%d[m Ԫ��\n",exchanges[no-1].title,num,"���Ի��",maxnum);
        if(askyn("��ȷ��Ҫ�䵱��",NA,NA) == NA ) {
                move(21,0);clrtoeol();
		prints("���ڲ��䵱�ˣ� �����´������� Ҫ�ǵ�Ŷ��");
                pressanykey();
                return 0;
        }
	set_safe_record();
	if (no == 1) {
		if(templog==currentuser.numlogins)
			currentuser.numlogins -= num;
		else templog = -1;
	} else if (no == 2)  {
		if(templog == currentuser.numposts)
			currentuser.numposts -= num;
		else templog = -1;
	} else {
		if(templog == currentuser.nummedals)
			 currentuser.nummedals -= num;
		else templog = -1;
	}
	if( templog == -1) {
		move(21,0); clrtoeol();
		prints("�Բ���, �ڽ��׹������������ݷ����˱仯.\nΪ��֤���׵���������, �˴ν���ȡ��.\n���������½��б�����.");
		pressanykey();
		return 0;
	}
}
	currentuser.money += maxnum;
	if( currentuser.money > 400000000 ){
		move(21,0); clrtoeol();
		prints("�Բ��𣬽������ݹ��󣬲�������������½��ף�");
		pressanykey();
		return 0;
	}
        substitut_record(PASSFILE, &currentuser, sizeof(currentuser), usernum);
	move(21,0); clrtoeol();
        prints("˳����ɽ��ף���ӭ�����ٴλݹˡ�;P");
	sprintf(genbuf,"�䵱%s %d �ݣ����� %d Ԫ.",exchanges[no-1].title,num,maxnum);
	gamelog(genbuf);
        pressanykey();
        return 1;
}
int doshopping()
{
        int no,hasperm=1,maxnum;
        char ans[10];
        EXCHANGES exchanges[10] = {
                {"������",40000},
                {"����������",10000},
                {"�ʺ����ñ���",100000},
                {"ǿ�ƺ���",120000},//expired 06.1.5
                {"�ӳ�����ʱ��",20000},//expired 06.1.5
                {"������",100000}
                };
        if(gotomarket("�г���������")) return 1;
        prints("������Ʒ����:");
        for(no = 0; no < 6; no ++) {
                move(5+no, 2);
                prints("(%2d) %s",no+1,exchanges[no].title);
                move(5+no, 20);
                prints("==> %6d Ԫ", exchanges[no].value);
        }
        move(16,0);
        prints("��Ŀǰ���� %d Ԫ (���� %d ��)\n",
            currentuser.money,currentuser.nummedals);
        getdata(18, 0, "���빺����һ����Ʒ��", ans, 10, DOECHO, YEA);
        no = atoi(ans);
        if ( no < 1 || no > 6 ) {
                prints("\n�Ǻǣ��������ˣ� �ǣ����ߡ�����ӭ���� ;)");
                pressanykey();
                return 0;
        }
        if ( no == 4 || no == 5 ) {
                prints("\nС�겻�ṩ����Ʒ��Ŷ :)");
                pressanykey();
                return 0;
       }
        set_safe_record();
        maxnum = exchanges[no-1].value;
        switch(no) {
                case 1:
                        hasperm = HAS_PERM(PERM_CLOAK);
                        break;
                case 2:
                        hasperm = HAS_PERM(PERM_SEECLOAK);
                        break;
                case 3:
                        hasperm = HAS_PERM(PERM_XEMPT);
                        break;
                case 4:
                        //hasperm = HAS_PERM(PERM_FORCEPAGE);
                        break;
                case 5:
                        //hasperm = HAS_PERM(PERM_EXT_IDLE);
                        break;
                case 6:
                        hasperm = HAS_PERM(PERM_LARGEMAIL);
                        break;
        }
        prints("\n\n");
        if(hasperm) {
                prints("���Ѿ�������Ȩ��, ����Ҫ�ٹ���. ");
                pressanykey();
                return 0;
        }
        if(currentuser.money < maxnum) {
                prints("\n�Բ���, ��û���㹻��Ǯ��������Ȩ��.");
                pressanykey();
                return 0;
        }
        if(askyn("��ȷ��Ҫ������",NA,NA) == NA ) {
                move(23,0);clrtoeol();
                prints("���ڲ����ˣ� �����´������� Ҫ�ǵ�Ŷ��");
                pressanykey();
                return 0;
        }
        set_safe_record();
        switch(no) {
                case 1:
                        hasperm = HAS_PERM(PERM_CLOAK);
                        currentuser.userlevel |= PERM_CLOAK ;
                        break;
                case 2:
                        hasperm = HAS_PERM(PERM_SEECLOAK);
                        currentuser.userlevel |= PERM_SEECLOAK ;
                        break;
                case 3:
                        hasperm = HAS_PERM(PERM_XEMPT);
                        currentuser.userlevel |= PERM_XEMPT ;
                        break;
                case 4://expired 06.1.5
                        //hasperm = HAS_PERM(PERM_FORCEPAGE);
                        //currentuser.userlevel |= PERM_FORCEPAGE ;
                        break;
                case 5://expired 06.1.5
                        //hasperm = HAS_PERM(PERM_EXT_IDLE);
                        //currentuser.userlevel |= PERM_EXT_IDLE ;
                        break;
                case 6:
                        hasperm = HAS_PERM(PERM_LARGEMAIL);
                        currentuser.userlevel |= PERM_LARGEMAIL ;
                        break;
        }
        if(hasperm) {
                prints("�ڽ��׽���ǰ���Ѿ���������Ȩ��, ����ȡ���˴ν���. ");
                pressanykey();
                return 0;
        }
        if(currentuser.money < maxnum) {
                prints("\n�Բ���, ��û���㹻��Ǯ��������Ȩ��.");
                pressanykey();
                return 0;
        }
	currentuser.money -= maxnum;
        substitut_record(PASSFILE, &currentuser, sizeof(currentuser), usernum);        move(23,0); clrtoeol();
        prints("˳����ɽ��ף���ӭ�����ٴλݹˡ�;P");
        sprintf(genbuf,"����[%s]������ %d Ԫ.",exchanges[no-1].title,maxnum);
        gamelog(genbuf);
        pressanykey();
        return 1;
}	

int
payoff()
{
	if(gotomarket("ˮվ������")) return 0;
        prints("�����涨: �����������һ�λ���. \n\n");
	if(currentuser.bet == 0 ) {
		prints("�㲢û���ڱ��г���Ǯ���������軹Ǯ���Ǻ�");
		pressanykey();
		return 0;
	}
	if(currentuser.money < currentuser.bet) {
		prints("���Ǯ������������´��������ޡ�");
		pressanykey();
		return 0;
	}
	prints("���ڱ��������� %d Ԫ.\n\n", currentuser.bet);
	 if(askyn("�����ھ��뻹�������",NA,NA) == NA ) {
		prints("���ڲ����ˣ� �����´������� Ҫ�ǵ�Ŷ��");
		pressanykey();
		return 0;
	}
        currentuser.money -= currentuser.bet;
        sprintf(genbuf,"������� %d Ԫ.",currentuser.bet);
        gamelog(genbuf);
        currentuser.bet = 0;
        currentuser.dateforbet = 0;
        substitut_record(PASSFILE, &currentuser, sizeof(currentuser), usernum);
        prints("���Ѿ��������ڱ��г������Ǯ����ӭ�����ٴλݹˡ�;P");
        pressanykey();
        return 1;
}
	
int
borrow()
{
	time_t now = time(0);
	int tempbet,maxbet;
	char 	buf[STRLEN];
	char *dstr;
	if(gotomarket("ˮվ�����"))return 0;
	prints("�����涨: Ŀǰÿ�������Դ��� %d Ԫ.\n\n", MAXBET);
	if(!currentuser.bet)
		prints("��Ŀǰ��û���ڱ�������.\n\n");
	else {
		prints("���Ѿ��ڱ������� %d Ԫ.\n",currentuser.bet);
		dstr = getdatestring(currentuser.dateforbet, NA);
		sprintf(genbuf, "��������������������:  %14.14s(%s) \n\n", dstr, dstr + 23);
		prints(genbuf);
		if( currentuser.bet>=MAXBET) {
               		prints("�Բ���, ���Ĵ����Ѿ��ﵽ�涨��Ŀ, ���������ܴ������.");
                        pressanykey();
                        return 0;
                }

	}
	if(askyn("����������վ������",NA,NA) == NA ) return 0;
	maxbet = MAXBET-currentuser.bet;
	if( maxbet > 1000 ) {
		sprintf(genbuf,  "�����Դ���: ���� 1000 Ԫ, ��� %d Ԫ�����������أ�",maxbet);
		getdata(10, 0, genbuf, buf, 10, DOECHO, YEA);
		tempbet = atoi(buf);
	} else {
		sprintf(genbuf,"�����Դ��� %d Ԫ����ȷ��Ҫ������",maxbet);
		if( askyn(genbuf,YEA,NA) == NA) {
			prints("\n�ţ������ˣ� �Ǻã��´������� ;p");
			pressanykey();
			return 0;
		}
		tempbet = maxbet;
	}
	if ( (maxbet > 1000 && tempbet >= 1000 && tempbet <= maxbet)
		||  maxbet <= 1000 ) {
		currentuser.money += tempbet;
		currentuser.bet += tempbet;
		currentuser.dateforbet = now + 10*24*60*60;
		substitut_record(PASSFILE, &currentuser, sizeof(currentuser), usernum);
		dstr = getdatestring(currentuser.dateforbet, NA);
		sprintf(genbuf, "\n����վ�ܹ���� %d Ԫ������Ҫ�� %14.14s(%s) ������",currentuser.bet, dstr, dstr + 23);
		prints(genbuf);
		sprintf(genbuf,"%s ��� %d Ԫ.",currentuser.userid,tempbet);
		gamelog(genbuf);
		pressanykey();
		return 1;
        }
	prints("\n���������Ŀ����ȷ��ȡ���˴ν��ס�");
	pressanykey();
	return 0;
}

int inmoney(unsigned int money)
{
	set_safe_record();
        if(currentuser.money + money < 400000000)currentuser.money += money ;
	else currentuser.money = 400000000;
        substitut_record(PASSFILE, &currentuser, sizeof(currentuser), usernum);
        return currentuser.money;
}

void demoney(unsigned int money)
{
	set_safe_record();
	if(currentuser.money > money ) currentuser.money -= money;
	else currentuser.money = 0;
        substitut_record(PASSFILE, &currentuser, sizeof(currentuser), usernum);
}

check_money(unsigned int money)
{
  set_safe_record();
  if(currentuser.money < money)
    {
        move(22, 0);
        clrtobot();
        prints("��Ǹ������������ע %d Ԫ, ��Ϊ����������ֻ�� %d Ԫ��",
		money,currentuser.money);
	pressanykey();
        return 1;
    }
    return 0;
}
void
show_money(int m, char *welcome,int Clear)
{
	if(Clear) clear();
	if(welcome) {
                ansimore(welcome, NA);
        }
        move(22, 0);
        clrtobot();
        set_safe_record();
        prints("[0;1;37;44m                  �������ֽ�: [36m%-18d[37mѺע���: [36m%-20d[m  ", currentuser.money, m);
}

int get_money(int m, char *welcome)
{
   unsigned int	money;
   char	buf[5];
   do {
      show_money(m,welcome,YEA);
      getdata(23,16,"��ҪѺע����Ǯ(1 - 2000)? ", buf, 5, DOECHO, YEA);
      if(buf[0] == '\0') return 0;
      money = abs(atoi(buf));
      if ( money <= 0) return 0;
      if(check_money(money))return 0;
   } while ((money < 1) || (money > 2000));
   demoney(money);
   show_money(money,NULL,YEA);
   return money;
}
