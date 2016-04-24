if [ "$1" == "" ] ;then
	echo "Input a html"
	exit -1
fi

HTTP=`cat $1 | grep "\.jpg" | grep "src" | awk '{FS="\""}{print $2}' | grep "^http" | awk '{FS="?"}{print $1}'`
echo "$HTTP" > src.html
[ -e ./picture ] && rm -rf picture
mkdir ./picture
i=0
cd ./picture
for http in $HTTP
do
	curl $http -o "image${i}.jpg" 
	i=$((++i))
done


