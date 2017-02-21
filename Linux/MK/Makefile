HHH = sdfsdf
ZZZ=hello
all:
	echo $HHH
	echo $ZZZ
	@echo ${HHH}
	@echo ${ZZZ}
	echo $$HHH
ifdef HHH
	if [ -n "${HHH}" ];then echo "ls";fi;
	if [ -n "${ZZZ}" ];then echo "{ZZZ}";fi;
	if [ -n "$(HHH)" ];then echo "(HHH)";fi;
	if [ -n "$(ZZZ)" ];then echo "(ZZZ)";fi;
	
	if [ -n "$HHH" ];then echo "hhh";fi;
	if [ -n "$ZZZ" ];then echo "zzz";fi;
	if [ -n "$$HHH" ];then echo "ls1";fi;
	if [ -n "$$ZZZ" ];then echo "ls2";fi;

	if [ -n $$HHH ];then echo "ls1";fi;
	if [ -n $$ZZZ ];then echo "ls2";fi;
	if [ 23 -lt 44 ];then echo "sdfsdfdf";fi

else
	ls -al 
endif


