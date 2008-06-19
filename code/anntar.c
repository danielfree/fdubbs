#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/stat.h>

#define BBSDISKPATH "/export/disk1/bbs/0Announce/groups/"
#define WWWDISKPATH "/export/disk1/bbswww/htdocs/groups/"
#define TARWORKPATH "/bbsbackup/Anntar/"
#define TRASHPATH "/bbsbackup/anntrash/"
#define DOTNAMES ".Names"
#define DOTINDEX "index.htm"
#define MAXQUEUEDEEP 4096
#define MAXDIRFILES 1024
#define DIRPATHLEN 1024
#define STRLEN 255

char arrDirs[MAXQUEUEDEEP][DIRPATHLEN];
int intArrDirsHead;
int intArrDirsTail;

char arrFiles[MAXDIRFILES][STRLEN];
int intArrFilesCount;

//�ж�һ��·���Ƿ��������Ŀ¼������Ǵ��ڵ�Ŀ¼�򷵻�1�����򷵻�0��
int isDir(char *strFileName){
	struct stat st;
	return ( stat( strFileName, &st ) == 0 && S_ISDIR( st.st_mode ) );
}

//�ж�һ��·���Ƿ���ڣ�����Ǵ��ڵ��򷵻�1�����򷵻�0��
int isExist(char *strPath){
	struct stat st;
	return ( stat( strPath, &st ) == 0 );
}

//��һ������longIntת��Ϊһ������ΪintLength���ַ���strInt������ΪintLength
void longToStr( long longInt, char *strInt, int intLength){

	int intTail;

	strInt[ intLength ]='\0';
	while( intLength -- ){
		intTail = longInt % 10;
		longInt = longInt / 10;
		strInt[ intLength] = '0' + intTail;
	}

}

//��unix�ļ�ת��Ϊhtm�ļ�
convertFiles(char *strHtmDirName){

	char strBuf[STRLEN], strTemp[STRLEN], ch;
	int intCount;
	FILE *fin, *fout;

	for( intCount = 0; intCount < intArrFilesCount; intCount ++ ){
		//��ԭ���ļ��е����лس��滻Ϊ<br>�����пո��滻Ϊ&nbsp;����ʱ����HTMLEncoding��
		fin = fopen( arrFiles[ intCount ], "rt" );
		longToStr( intCount, strTemp, 6 );
		sprintf( strBuf, "%s/%s.htm", strHtmDirName, strTemp );
		fout = fopen ( strBuf, "wt" );
		if( fin && fout ){
			//��������һЩ�����ͷ��Ϣ
			fprintf( fout, "<html>\n<head>\n" );
			fprintf( fout, "<title>������ѧ���¹⻪վ�þ�����</title>\n" );
			fprintf( fout, "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=GB2312\">\n" );
			fprintf( fout, "</head>\n\n" );
			fprintf( fout, "<body>\n" );
			fprintf( fout, "\n<center><h1>������ѧ���¹⻪վ�þ�����</h1></center>\n\n" );
			while( !feof( fin ) ){
				ch = fgetc( fin );
				if( ch == '\n' ){
					fprintf( fout, "<br>\n" );
				}else if( ch == ' ' )
					fprintf( fout, "&nbsp;" );
				else if( ch == '\t' )
					fprintf( fout, "&nbsp;&nbsp;&nbsp;&nbsp;" );
				else
					fputc( ch, fout );
			}
			//��������һЩ�����β��Ϣ
			fprintf( fout, "\n<center><h1>������ѧ���¹⻪վ�þ�����</h1></center>\n" );
			fprintf( fout, "</body>\n</html>\n" );
		}
		fclose( fout );
		fclose( fin );
	}

	intArrFilesCount = 0;

}

//ת��һ��Ŀ¼����Ŀ¼�е������ļ�תΪhtm��������Ŀ¼ѹ��������
convertDir(int intDirIndex){

	char strBuf[STRLEN], strHtmDirName[STRLEN], strName[STRLEN], strPath[STRLEN];
	char *ptr;
	FILE *fl, *indexFile;
	int intFlag;

	//����htmĿ¼
	longToStr( intDirIndex, strBuf, 6 );
	sprintf( strHtmDirName, "%s%s", TARWORKPATH, strBuf );
	sprintf( strBuf, "mkdir %s", strHtmDirName );
	system( strBuf );

	intFlag = 0;

	//��index.htm�ļ�
	sprintf( strBuf, "%s/%s", strHtmDirName, DOTINDEX );
	if( indexFile = fopen( strBuf, "wt" ) ){

		//��������һЩ�����ͷ��Ϣ
		fprintf( indexFile, "<html>\n<head>\n" );
		fprintf( indexFile, "<title>������ѧ���¹⻪վ�þ�����</title>\n" );
		fprintf( indexFile, "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=GB2312\">\n" );
		fprintf( indexFile, "</head>\n\n" );
		fprintf( indexFile, "<body>\n" );
		fprintf( indexFile, "\n<center><h1>������ѧ���¹⻪վ�þ�����</h1></center>\n\n" );

		//.Names�ж����ļ��б�
		sprintf( strBuf, "%s/%s", arrDirs[ intDirIndex ], DOTNAMES );
		if( fl = fopen( strBuf, "rt" ) ){

			while( !feof( fl ) ){

				//�õ���һ�е�����
				fgets( strBuf, STRLEN, fl );
				if( feof( fl ) ) break;

				//ȡ��һ���ļ���ʾ���������Ƿ���� (BM: ***) �޳�ֻ�а����ܹܡ�վ���ɼ�������
				strBuf[ strlen( strBuf ) - 1 ] = '\0';
				if( ( ptr = strstr( strBuf, "Name=" ) ) && strlen( strBuf ) > 5 
					&& !strstr( strBuf, "(BM: SYSOPS)" ) && !strstr( strBuf, "(BM: OBOARDS)" )
					&& !strstr( strBuf, "(BM: BMS)" ) && !strstr( strBuf, "(BM: SECRET)" ) ){
					strcpy( strName, ptr + 5 );

					//�õ������� Name= ����һ�е�����
					fgets( strBuf, STRLEN, fl );
					if( feof( fl ) ) break;
					strBuf[ strlen( strBuf ) - 1 ] = '\0';

					//ȡ��һ���ļ�������ʽӦ���� Path=~/ �� �����Ƿ���� .. �޳�ѭ��Ŀ¼
					if( ( ptr = strstr( strBuf, "Path=~/" ) ) && strlen( strBuf ) > 7 ){
						strcpy( strPath, ptr + 7 );
						if( !strstr( strPath, "/" ) && !strstr( strPath, "\\" ) && strPath[0] != '.' ){

							//����ȫ·��
							sprintf( strBuf , "%s/%s", arrDirs[ intDirIndex ], strPath );

							//�ж�����ļ��Ƿ���Ŀ¼�������Ŀ¼����������Ŀ¼�����У��������������ĵ������С�
							if( isDir( strBuf ) ){
								//��Ŀ¼��д�������Ŀ¼����
								strcpy( arrDirs[ intArrDirsTail ], strBuf );
								//��Ŀ¼��д��index.htm��
								longToStr( intArrDirsTail, strBuf, 6 );
								fprintf( indexFile, "<a href=""../%s/%s"">%s</a><br>\n", strBuf, DOTINDEX, strName );
								intArrDirsTail ++;
								intFlag = 1;	//��Ŀ¼��Ϊ��
							}else if( isExist( strBuf ) ){
								//���ĵ���д��������ĵ�����
								strcpy( arrFiles[ intArrFilesCount ], strBuf );
								//���ĵ���д��index.htm��
								longToStr( intArrFilesCount, strBuf, 6 );
								fprintf( indexFile, "<a href=""%s.htm"">%s</a><br>\n", strBuf, strName );
								intArrFilesCount ++;
								intFlag = 1;	//��Ŀ¼��Ϊ��
							}

						}
					}

				}

			}

			convertFiles( strHtmDirName );

		}

		intArrDirsHead ++;

		//�ر�.Names
		fclose( fl );

	}

	//��������һЩ�����β��Ϣ
	if( intFlag )
		fprintf( indexFile, "\n<center><h1>������ѧ���¹⻪վ�þ�����</h1></center>\n" );
	else
		fprintf( indexFile, "\n<center>Ŀǰû������</center>\n" );
	fprintf( indexFile, "</body>\n</html>\n" );

	//�ر�.index.htm
	fclose( indexFile );

}

//����һ����ľ�����
void convertBoard(char *strBoardName){

	char strBuf[STRLEN];
	FILE *indexFile;

	intArrFilesCount = 0;
	intArrDirsHead = 0;
	intArrDirsTail = 0;
	sprintf( strBuf, "%s%s", BBSDISKPATH, strBoardName );
	strcpy( arrDirs[0], strBuf );
	intArrDirsTail ++;

	while( intArrDirsHead < intArrDirsTail )
		convertDir(intArrDirsHead);

	sprintf( strBuf, "%s%s", TARWORKPATH, DOTINDEX );
	if( indexFile = fopen( strBuf, "wt" ) ){

		//�����Ŀ¼��index.htm
		fprintf( indexFile, "<html><head>\n" );
		fprintf( indexFile, "<title>������ѧ���¹⻪վ�þ�����</title>\n" );
		fprintf( indexFile, "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=GB2312\">\n" );
		fprintf( indexFile, "<script>\n" );
		fprintf( indexFile, "function redirect(){window.location = \"000000/index.htm\"}\n" );
		fprintf( indexFile, "</script></head>\n" );
		fprintf( indexFile, "<body onload=\"redirect()\"></body></html>\n" );

	}
	fclose( indexFile );

	sprintf( strBuf, "%sgzipfile.txt", TRASHPATH );
	indexFile = fopen( strBuf, "wt" );
	fclose( indexFile );

	//�����ɵľ��������ݴ����anntrash
	sprintf( strBuf, "tar cfX - %sgzipfile.txt * |gzip >%s%s.html.tgz", TRASHPATH, WWWDISKPATH, strBoardName );
	system( strBuf );

	//����AnntarĿ¼
	sprintf( strBuf, "rm -r %s/*", TARWORKPATH );
	system( strBuf );

}

//����һ���������а���
void convertPart(char *strPartName){

	char strBuf[STRLEN], strBoardName[STRLEN];
	FILE *fl;

	//��ls��Ŀ¼�е��ļ����б����TRASHPATH�е�list_txt�ļ���
	sprintf( strBuf, "ls %s%s > %sboard_txt", BBSDISKPATH, strPartName, TRASHPATH );
	system( strBuf );

	//��list_txt�ж����ļ��б�
	sprintf( strBuf, "%sboard_txt", TRASHPATH );
	if( fl = fopen( strBuf, "rt" ) )
		while( !feof( fl ) ){

			//ȡ��һ���ַ���
			fgets( strBuf, STRLEN, fl );
			if( feof( fl ) ) break;

			//�õ����������İ����������·����
			strBuf[ strlen(strBuf) - 1 ] = '\0';
			sprintf( strBoardName , "%s/%s", strPartName, strBuf );
			sprintf( strBuf , "%s%s", BBSDISKPATH, strBoardName );

			//�ж�����ļ��Ƿ���Ŀ¼�������Ŀ¼�����convertBoard�������������
			if( isDir( strBuf ) ) convertBoard( strBoardName );

		}

	//�ر�list_txt
	fclose(fl);

}

main(){

	char strBuf[STRLEN];

	//0���ľ���ֻ��ָ����һЩ����
	convertBoard( "system.faq/Announce" );
	convertBoard( "system.faq/BBS_Dev" );
	convertBoard( "system.faq/BBS_Help" );
	convertBoard( "system.faq/BM_Home" );
	convertBoard( "system.faq/Dispute" );
	convertBoard( "system.faq/New_Board" );
	convertBoard( "system.faq/Notice" );
	convertBoard( "system.faq/SysOp" );
	convertBoard( "system.faq/Test" );

//	convertBoard( "system.faq/Zzzzz" );

	//�������ľ��������������
//	convertPart( "system.faq" );
	convertPart( "ccu.faq" );
	convertPart( "campus.faq" );
	convertPart( "comp.faq" );
	convertPart( "rec.faq" );
	convertPart( "music.faq" );
	convertPart( "literal.faq" );
	convertPart( "sport.faq" );
	convertPart( "talk.faq" );
	convertPart( "news.faq" );
	convertPart( "sci.faq" );
//	convertPart( "other.faq" );
//	convertPart( "soc.faq" );

	//����anntrash
	sprintf( strBuf, "rm -r %s/*", TRASHPATH );
	system( strBuf );

}
