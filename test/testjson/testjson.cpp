#include "json.hpp"
#include <string>

using json = nlohmann::json;

#include <iostream>
#include <vector>
#include <map>

using namespace std;

//json序列化示例1
string func1() {
    json js;
    js["msg_type"] = 2;
    js["from"] = "zhang san";
    js["msg"] = "lis si";
    js["msg"] = "hello, what are you doing now?";

    string sendBuf = js.dump();
    cout << js << endl;
    return sendBuf;
}

string func2() {
    json js;
    //添加数组
    js["id"] = { 1, 2, 3, 4, 5 };
    // 添加 key-value
    js["name"] = "zhang san";
    //添加对象
    js["msg"]["zhuang san"] = "hello world";
    js["msg"]["liu shuo"] = "hello china";
    //上面的语句等同于一次性添加数组对象
    js["msg"] = { {"zhan san", "hello"} , {"liu shuo", "hello china"} };

    return js.dump();
}

void func3() {
    json js;

    //直接序列化一个vector容器
    vector<int> vec;
    vec.push_back( 1 );
    vec.push_back( 2 );
    vec.push_back( 3 );
    js["list"] = vec;

    //直接序列化一个map容器
    map<int , string> m;
    m.insert( { 1, "黄山" } );
    m.insert( { 2, "华山" } );
    m.insert( { 3, "泰山" } );
    js["path"] = m;

    cout << js << endl;
}
int main() {
    string recvBuf = func1();
    //数据的反序列化 json字符串 ==》 反序列化
    json jsbuf = json::parse( recvBuf );
    cout << jsbuf["msg_type"] << endl;

    string recvBuf2 = func2();
    json jsbuf2 = json::parse( recvBuf2 );

    vector<int> v = jsbuf2["id"];
    cout << jsbuf2["id"];

    string recvBuf3 = func3();
    json jsbuf3 = json::parse( recvBuf3 );
    map<int , string> mymap = jsbuf3["path"];
    
    return 0;
}