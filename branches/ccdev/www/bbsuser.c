/* total written by roly 02.01.23 */
#define MY_CSS "/bbsold.css"
#include "libweb.h"

int main() {
	char buf[1024], *ptr;
  	init_all();
	printf("<style type=text/css>\n"
"	A:link {	COLOR: #ffffff; TEXT-DECORATION: none}\n"
"	A:visited {	COLOR: #ffffff; TEXT-DECORATION: none}\n"
"	A:active {	COLOR: #ffffff; TEXT-DECORATION: none}\n"
"	A:hover {	COLOR: #3000ff; TEXT-DECORATION: underline}\n"
"        BODY { FONT-SIZE: 12px;	}</style>");
	
	printf("<BODY background=/bbsuserbg.jpg topmargin=0 leftmargin=0>");
	if(!loginok) {
		printf(
"		<FORM action=bbslogin method=post target=_top name=frmInput>\n"
"			<TABLE  background=/bbsuserbg.jpg style='border-collapse: collapse' bordercolor='#111111' cellpadding='0' cellspacing='0'>\n"
"  				<TBODY>\n"
"  					  <TR>\n"
"    					<TD background=/bbsuserbg.jpg>\n"
"    					<img border='0' src='/bbsuser.jpg' width='80' height='16'></TD></TR>\n"
"					  <TR>\n"
"					    <TD  background=/bbsuserbg.jpg >�ʺ�: <INPUT style='HEIGHT: 20px' maxLength=12 size=8 name=id></TD></TR>\n"
"					  <TR>\n"
"					    <TD  background=/bbsuserbg.jpg >����: <INPUT style='HEIGHT: 20px' type=password maxLength=12 size=8 name=pw></TD></TR>\n"
"					  <TR><TD  width='132' height='45' background='/bbsusersign.jpg'><input style='width:40px; height:20px; Font-size=12px;BACKGROUND-COLOR:c0b0b0' type=submit value=��¼>\n"
"                      </TD></TR>\n"
"					  </TBODY></TABLE></FORM>\n");
	} else {
                char buf[256]="δע���û�";
		printf("<TABLE style='border-collapse: collapse' bordercolor='#111111' cellpadding='0' cellspacing='0'>\n"
"  				<TBODY>\n"
"  					  <TR>\n"
"    					<TD colSpan=2>\n"
"    					<img border='0' src='/bbsuser.jpg' width='80' height='16'></TD></TR>\n"
"					  <TR>\n"
"						<TD height='22'>�û�: <a href=bbsqry?userid=%s target=f3>%s</a></TD></TR>", currentuser.userid, currentuser.userid);
                
				if(currentuser.userlevel & PERM_REGISTER) strcpy(buf, cexpstr(countexp(&currentuser)));
                if(currentuser.userlevel & PERM_BOARDS) strcpy(buf, "����");
				if(currentuser.userlevel & PERM_XEMPT) strcpy(buf, "�����ʺ�");
                if(currentuser.userlevel & PERM_SYSOPS) strcpy(buf, "��վվ��");
                
				printf("<TR><TD height='22'>   ����: %s</TD></TR>", buf);
                printf("<TR><TD  width='132' height='45' background='/bbsusersign.jpg'>\n"
			"		<a href=bbslogout target=_top>ע�����ε�¼</a>\n"
			"	</TD></TR></TBODY></TABLE>\n");
	}
  	printf("</body>");
}

