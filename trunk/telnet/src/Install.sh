#! /bin/sh
# Install.sh a shell script to install whole BBS

BBS_HOME=/home/bbs
BBSUID=bbs
BBSGRP=bbs
INSTALL="/usr/bin/install -c"
TARGET=/home/bbs/bin

if [ -d ${BBS_HOME} ] ; then
        echo -n "Warning: ${BBS_HOME} already exists, overwrite whole bbs [N]?"
        read ans
        ans=${ans:-N}
        case $ans in
            [Yy]) echo "Installing new bbs to ${BBS_HOME}" ;;
            *) echo "Abort ..." ; exit ;;
        esac
	echo " "
	if [ -f "${BBS_HOME}/.BOARDS" ] ; then
        	if [ ".${FORCE_INSTALL}" = ".YES" ] ; then
                	echo "force install ........ that might be many error, be aware!  "
                	echo "Press <Enter> to continue ..."
                	read ans
        	else
                	echo "WARNING: You have \"make install\" before, so you wont be install again."
                	echo "To install the new binary, please try \"make update\"."
                	echo " "
                	echo "If you want to force the install, please try "
                	echo "% make install FORCE_INSTALL=YES"
			echo "If you only to update your bbs, please try "
			echo "% make update"
                	exit
        	fi
	fi
else
        echo "Making dir ${BBS_HOME}"
        mkdir ${BBS_HOME}
        chown -R ${BBSUID} ${BBS_HOME}
        chgrp -R ${BBSGRP} ${BBS_HOME}
fi

( cd ../../bbshome ; tar cf - * ) | ( cd ${BBS_HOME} ; tar xf - )

#
# Setup bbs directory tree
#
echo "Setup bbs directory tree ....."
mv ${BBS_HOME}/BOARDS ${BBS_HOME}/.BOARDS
touch ${BBS_HOME}/.hushlogin
touch ${BBS_HOME}/.visitlog
touch ${BBS_HOME}/.PASSWDS

#
# create empty mail and home directory 
#
echo "creating necessary empty directory (user mail, user home)"

alphabet="A B C D E F G H I J K L M N O P Q R S T U V W X Y Z"
dirs="mail home"
preboards="boards vote"
boards="BBS_Help bmsecurity BM_Home boardsecurity club GoneWithTheWind newcomers Notepad Notice SysOp syssecurity usersecurity vote"


for x in $dirs; \
do \
	echo "in $x .."
	echo -n "   "
	for y in $alphabet; \
	do \
		echo -n " $y"
		mkdir -p ${BBS_HOME}"/$x/$y"; \
	done
	echo " done.  "
done

for x in $preboards; \
do \
	echo "in $x .."
	echo -n "  "
	for y in $boards; \
	do \
		echo -n " $y"
		mkdir -p ${BBS_HOME}"/$x/$y"; \
	done
	echo " done.  "
done

echo " "

#
# build directorys
#
mkdir -p ${BBS_HOME}"/bm"
mkdir -p ${BBS_HOME}"/tmp"
mkdir -p ${BBS_HOME}"/reclog"
mkdir -p ${BBS_HOME}"/so"
mkdir -p ${BBS_HOME}"/bin"
mkdir -p ${BBS_HOME}"/etc"
mkdir -p ${BBS_HOME}"/logs"
mkdir -p ${BBS_HOME}"/0Announce/bbslist"


chown -R ${BBSUID} ${BBS_HOME}
chgrp -R ${BBSGRP} ${BBS_HOME}

if [ -f bbsd ] ; then
	${INSTALL} -m 550  -s -g ${BBSGRP} -o ${BBSUID}  bbsd       ${TARGET}
else
	${INSTALL} -m 550  -s -g ${BBSGRP} -o ${BBSUID}  bbs        ${TARGET}
	${INSTALL} -m 4555 -s -g bin       -o root       bbsrf      ${TARGET}
fi
${INSTALL} -m 550  -s -g ${BBSGRP} -o ${BBSUID} chatd	 ${TARGET}
${INSTALL} -m 550  -s -g ${BBSGRP} -o ${BBSUID} thread   ${TARGET}
${INSTALL} -m 550  -s -g ${BBSGRP} -o ${BBSUID} expire   ${TARGET}
${INSTALL} -m 550  -s -g ${BBSGRP} -o ${BBSUID} bbsnet   ${TARGET}
${INSTALL} -m 550  -s -g ${BBSGRP} -o ${BBSUID} miscd    ${TARGET}

if [ -f admintool.so ] ; then
${INSTALL} -m 550  -s -g ${BBSGRP} -o ${BBSUID} admintool.so ${TARGET}/../so
fi

echo "Install is over...."
echo "Check the configuration in ${BBS_HOME}/etc/sysconf.ini"
echo "Then login your BBS and create an account called SYSOP (case-sensitive)"
