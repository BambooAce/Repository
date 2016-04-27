#include <vector>
#include <list>
#include <deque>
#include <queue>
#include <string>
#include <iostream>
#include <iterator>
// utility pair
#include <utility>
#include <map>
using namespace std;
/*
 *
 * 尽管不能直接将一种容器内的元素复制给另一种容器，但系统允许通过传递一对迭代器（第 3.4 节）间接实现该实现该功能。
 * 使用迭代器时，不要求容器类型相同。容器内的元素类型也可以不相同，只要它们相互兼容，能够将要复制的元素转换为所构建的新容器的元素类型，即可实现复制。
 */
int main()
{
//    std::list<string> svec;
//    svec.push_front("C");
//    svec.push_back("A");
//    svec.push_back("B");
//    pair<int, string> pa(45,"Hello");
//    std::cout << pa.first << std::endl;
//    std::cout << pa.second << std::endl;
//    pair<string,string>infor = make_pair("name","Ace");
//    pair<string, vector<int> > paa;
//    pair<string, void *>paaa;
//    std::cout << infor.first << infor.second << std::endl;
//    //   svec.clear();
//    //    svec.pop_front();
//    //    svec.pop_back();
//    std::list<string>::iterator iter= svec.begin();
//    //    std::string temp = svec.back();
//    //    std::cout << temp << std::endl;

//    while(iter != svec.end())
//    {
//        std::cout<< *iter <<std::endl;
//        if (*iter == "C")
//        {
//            svec.erase(iter);
//            iter = svec.begin();
//        }
//        ++iter;
//    }
//    priority_queue<string>a;
//    iter = svec.begin();
//    std::list<string> svec1(iter, svec.end());
//    std::vector<string> vec(iter, svec.end());
//    //
//    //
//    std::deque<string> que(iter, svec.end());
//    //
//    //
//    //
//    std::list<string>::iterator listiter = svec1.begin();
//    std::cout << "size"<<svec1.size() << std::endl;
//    while(listiter != svec1.end())
//    {
//        std::cout <<"dfsdfs"<< *listiter <<std::endl;
//        ++listiter;
//    }
    class A
    {
    public:
        A():a(0)
        {}
        ~A()
        {}
    private:
        int a;
    };

    map<int, string>a;
    a[1] = "hello";
    a[-2] = "world";
    a[2] = "HOO";
    a[0] = "BOO";
    pair<int, string>p(4,"sfsdfd");
    a.insert(p);
    map<int, string>::const_iterator aiter = a.begin();
    while(aiter != a.end())
    {
        std::cout<<aiter->first<<" "<< aiter->second<<std::endl;
        aiter++;
    }
    A bb;
//    map<A, string>bbA;
//    bbA[bb] = "asc";
 //   pair<A *, string>ddddd (&bb,"ced");















    return 0;
}
