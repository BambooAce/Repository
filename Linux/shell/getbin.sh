rm -rf bin.tar.gz
rm -rf ./bin

if [ -e "bin" ] && [ -d "bin" ];then
	echo "bin dirtory has found"
else
	mkdir bin
	echo "bin dirtory was build"
fi

cd /bin
allfile=$(ls)
for file in $allfile
do
	if [ -e $file ] && [ -f $file ];then
		cp $file ~/bin
	fi
done 

cd /usr/sbin
allfile=$(ls)
for file in $allfile
do
	if [ -e $file ];then
		cp $file ~/bin
	fi
done 

cd ~

tar -czf  bin.tar.gz ./bin

if [ -e "bin.tar.gz" ];then
	echo "bin.tar.gz success"
else
	echo "bin.tar.gz fail"
fi	

if [ -e "bin.tar.gz" ];then
	ftp -n<<!
	open 192.168.0.103
	user root root
	binary
	put bin.tar.gz
	close
	bye
!
	echo "upload success"
fi
