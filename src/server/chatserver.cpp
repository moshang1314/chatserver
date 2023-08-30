#include "chatserver.hpp"
#include "json.hpp"
#include "chatservice.hpp"

#include <string>
#include <iostream>

using namespace std::placeholders;
using json = nlohmann::json;

ChatServer::ChatServer( EventLoop* loop ,
    const InetAddress& listenAddr ,
    const string& nameArg )
    : _server( loop , listenAddr , nameArg ) , _loop( loop ) {
    //注册连接回调
    _server.setConnectionCallback( std::bind( &ChatServer::onConnection , this , _1 ) );
    //注册消息回调
    _server.setMessageCallback( std::bind( &ChatServer::onMessage , this , _1 , _2 , _3 ) );

    //设置线程数量
    _server.setThreadNum( 4 );
}

//启动服务
void ChatServer::start() {
    _server.start();
}

void ChatServer::onConnection( const TcpConnectionPtr &conn ) {
    // 客户端断开连接
    if (!conn->connected()) {
        ChatService::instance()->clientCloseException( conn );
        conn->shutdown();
    }
}

void ChatServer::onMessage( const TcpConnectionPtr &conn,
    Buffer* buffer,
    Timestamp time) {
    string buf = buffer->retrieveAllAsString();
    json js;
    //数据的反序列化
    try {
        js = json::parse( buf );
    }
    catch(...){
        return;
    }
    //js = json::parse( buf );
    // 解耦网络模块和业务模块的代码
    auto msgHandler = ChatService::instance()->getHandler( js["msgid"] );
    // 回调消息绑定好的事件处理函数
    msgHandler( conn , js , time );
}