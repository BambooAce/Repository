# 从右向左匹配 ：% 和 %% 操作符的示例
#!/bin/bash
#提取文件名，删除后缀。
#file_name="text.gif"
#name=${file_name%.*}
#echo file name is: $name
#输出结果：
#file name is: test
# ${VAR%.* }含义：从$VAR中删除位于 % 右侧的通配符左右匹配的字符串，通配符从右向左进行匹配。现在给变量 name 赋值，name=text.gif，那么通配符从右向左就会匹配到 .gif，所有从 $VAR 中删除匹配结果。
# % 属于非贪婪操作符，他是从左右向左匹配最短结果；%% 属于贪婪操作符，会从右向左匹配符合条件的最长字符串。
#file_name="text.gif.bak.2012"
#name=${file_name%.*}
#name2=${file_name%%.*}
#echo file name is: $name
#echo file name is: $name2
#输出结果：
#file name is: test.gif.bak    //使用 %
#file name is: test            //使用 %%
#操作符 %% 使用 .* 从右向左贪婪匹配到 .gif.bak.2012
#左向右匹配：# 和 ## 操作符示例
#!/bin/bash
#提取后缀，删除文件名。
#file_name="text.gif"
#suffix=${file_name#*.}
#echo suffix is: $suffix
#输出结果：
#suffix is: gif
# ${VAR#*.} 含义：从 $VAR 中删除位于 # 右侧的通配符所匹配的字符串，通配符是左向右进行匹配。
# 跟 % 一样，# 也有贪婪操作符 ## 。
#file_name="text.gif.bak.2012.txt"
#suffix=${file_name#*.}
#suffix2=${file_name##*.}
#echo suffix is: $suffix
#echo suffix is: $suffix2
#输出结果：
#suffix is: text.gif.bak.2012     //使用 #
#suffix2 is: txt                  //使用 ##
#操作符 ## 使用 *. 从左向右贪婪匹配到 text.gif.bak.2012


file_name="test.gif.gz"
name=${file_name%%.*}
name1=${file_name%.*}
na=${file_name#*.}
na1=${file_name##*.}
echo $file_name
echo $name
echo $name1
echo $na
echo $na1
