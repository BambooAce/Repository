#!/bin/bash
#
# PXE imaging
#
. ./funcs.sh
OS=(CE6 ThinProZ ThinPro3 WES HP_XPe)
OSNAME=(CE6.0 ThinProSmartZeroCore ThinPro3 WES2009 XPE)
DM_PATH=`echo $DM_FILE | sed -e 's/\/\//\//g'`
DIR=`dirname $DM_PATH`
FILE=`basename $DM_PATH`
TOOLSDIR=`dirname $DM_PATH | xargs dirname | xargs dirname | awk '{FS="/"}{print $NF}'`
PEBOOT="\\${TOOLSDIR}\\Tools\\Imaging\\WinPE\\sources"
WINPE="\\${TOOLSDIR}\\Tools\\PXEImagingTool\\WESTools"
MKPART=1
ERRORLOG=/tmp/deploy_error.log
DMSTATUS=0
DM_USER=`/bin/dmdecrypt $DM_USER`
DM_PASS=`/bin/dmdecrypt $DM_PASS`
num=0

#
# If we do not support this OS type PXE imaging, will report failed.
#
for os in ${OS[@]}
do
	if [ "$DM_OS" == "$os" ];then
		err="Not support ${OSNAME[${num}]} type."
		/bin/dmpost "$DM_TASKID" "$DM_MSSHOST" "$DM_MSSPORT" "$DM_CLIENTIP" "$DM_CLIENTMAC" "311" "$err" "Failed" "$DIR" "$FILE"
		sleep 5
		/sbin/dmreboot
	else
		num=$((num+1))
	fi
done

#
# If image is ThinPro, and protocol not support SMB
#
if [ "${DM_OS:0:4}" == "Thin" ];then
	MKPART=0
	if [ "$DM_PROTOCOL" == "SMB" ];then
		/bin/dmpost "$DM_TASKID" "$DM_MSSHOST" "$DM_MSSPORT" "$DM_CLIENTIP" "$DM_CLIENTMAC" "311" "For ThinPro not support SMB protocol." "Failed" "$DIR" "$FILE"
		sleep 5
		/sbin/dmreboot
	fi
fi

#
# Umount all partition mounted by mini Linux.
#
MOUNTED=`mount | grep ${DEVICE} | awk '{print $3}'`
if [ "$MOUNTED" != "" ];then
	for PART in $MOUNTED
	do
		umount $PART
	done
fi

#
# If OS type is WES, will make a fat32 partition.
# will check use SMB protocol or not, and disk size.
# use:
# 	parted -s ${DEVICE} mklabel msdos
# will make sure partition successfully.
#
# If ThinPro, will use dd command to deploy imaging
# After Imaging, will report to gateway.
#
if [ $MKPART -eq 1 ];then
	if [ "$DM_PROTOCOL" != "SMB" ];then
		DMSTATUS=311
		INFO="Not find SMB protocol, for WES need SMB."
		/bin/dmpost "$DM_TASKID" "$DM_MSSHOST" "$DM_MSSPORT" "$DM_CLIENTIP" "$DM_CLIENTMAC" "$DMSTATUS" "$INFO" "Failed" "$DIR" "$FILE"
		sleep 5
		/sbin/dmreboot
	fi
	disksize=$(cat /sys/block/$(basename ${DEVICE})/size)
	disksize=$((disksize / 2))
	disksize=$((disksize * 1024))
	if [ $DM_SOURCEIMGSIZE -gt $disksize ];then
		echo "Image size larger than disk, can not imaging........"
		DMSTATUS=201
		INFO="Image size larger than disk size."
		/bin/dmpost "$DM_TASKID" "$DM_MSSHOST" "$DM_MSSPORT" "$DM_CLIENTIP" "$DM_CLIENTMAC" "$DMSTATUS" "$INFO" "Failed" "$DIR" "$FILE"
		sleep 5
		/sbin/dmreboot
	fi
	echo "Begin to make partition......."
	parted -s ${DEVICE} mklabel msdos 2>/dev/null
	parted -s ${DEVICE} mkpart primary  1M 800M mkfs 1 fat32 2>/dev/null
	sleep 2
	partprobe
	partprobe
	if [ $? -eq 0 ];then
		mkdir /winpe
		mount ${DEVICE}1 /winpe
		if [ $? -eq 0 ];then
			cd /winpe/
			SHAREFOLDER=`echo ${DM_FILE} | awk '{FS="/"}{print $1}'`
			/bin/smbclient //${DM_HOST}/${SHAREFOLDER} -U "${DM_USER}" "${DM_PASS}" <<!
			cd ${PEBOOT}
			get boot.wim
			cd ${WINPE}
			get winPE.tar.gz
			quit
!
			sleep 2
			sync
			sync
			cd /
			if [ -e /winpe/winPE.tar.gz ] && [ -e /winpe/boot.wim ];then
				echo "[UPDATE]" > /winpe/update.ini
				echo "DM_PXE=1" >> /winpe/update.ini
				echo "FILE=${FILE}" >> /winpe/update.ini
				cat /tmp/dmtask | awk '{print $2}' >> /winpe/update.ini
				if [ -e /winpe/update.ini ];then
					echo "Download WinPE success and save PXE task is OK......."
					tar -xzvf /winpe/winPE.tar.gz -C /winpe/ 2>/dev/null
					sync
					mv /winpe/boot.wim /winpe/sources/
					echo "Extract WinPE complete........"
				fi
				sync
				sync
			else
				echo "Download WINPE failed!!!!!"
				echo "export DMSTATUS=311" > /tmp/cloneresult
				echo " Download WINPE tool failed." >> $ERRORLOG
			fi
			if [ -e $ERRORLOG ];then
				ERRORMSG=`cat $ERRORLOG`
				chmod +x /tmp/cloneresult
				source /tmp/cloneresult
				/bin/dmpost "$DM_TASKID" "$DM_MSSHOST" "$DM_MSSPORT" "$DM_CLIENTIP" "$DM_CLIENTMAC" "$DMSTATUS" "$ERRORMSG" "Failed" "$DIR" "$FILE"
			fi
			[ -e /winpe/winPE.tar.gz ] && rm -rf /winpe/winPE.tar.gz
			sync
			umount winpe
			echo "Prepare reboot.........."
			sleep 3
			/bin/dmreport "$DM_TASKID" "$DM_MSSHOST" "$DM_MSSPORT" "$DM_CLIENTIP" "$DM_CLIENTMAC" "WriteImage" "0" "0" "50"
		fi
	fi
else
	PXEdeploy_ddgz $1 $2 $3
	RES=$?
	if [ -e /etc/cloneresult ];then
		chmod +x /etc/cloneresult
		source /etc/cloneresult
	fi
	case $RES in
	6)
		errormsg="Failed to get image size, please check!"
		result="Failed"
	;;
	7)
		if [ -e /tmp/deploy_error.txt ];then
			errormsg=`cat /tmp/deploy_error.txt`
			errormsg+=`cat .dmget_error.log`
		else
			errormsg="Download image or imaging failed"
		fi
		result="Failed"
	;;
	8)
		errormsg="Failure: Image size is larger than disk size."
		result="Failed"
	;;
	*)
		errormsg="0"
		result="Finished"
	;;
	esac
	/bin/dmreport "$DM_TASKID" "$DM_MSSHOST" "$DM_MSSPORT" "$DM_CLIENTIP" "$DM_CLIENTMAC" "WriteImage" "0" "0" "100"
	sleep 5
	echo /bin/dmpost "$DM_TASKID" "$DM_MSSHOST" "$DM_MSSPORT" "$DM_CLIENTIP" "$DM_CLIENTMAC" "$DMSTATUS" "$errormsg" "$result" "$DIR" "$FILE"
	/bin/dmpost "$DM_TASKID" "$DM_MSSHOST" "$DM_MSSPORT" "$DM_CLIENTIP" "$DM_CLIENTMAC" "$DMSTATUS" "$errormsg" "$result" "$DIR" "$FILE"
fi
sync
sync
sleep 5
