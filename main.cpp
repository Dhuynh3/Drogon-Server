#include <drogon/WebSocketController.h>
#include <drogon/PubSubService.h>
#include <drogon/HttpAppFramework.h>


using namespace drogon;
using namespace std::chrono_literals;
using std::cout; using std::endl;
using std::chrono::duration_cast;
using std::chrono::milliseconds;
using std::chrono::seconds;
using std::chrono::system_clock;



class WebSocketChat : public drogon::WebSocketController<WebSocketChat>
{
public:
    virtual void handleNewMessage(const WebSocketConnectionPtr&,
        std::string&&,
        const WebSocketMessageType&) override;
    virtual void handleConnectionClosed(
        const WebSocketConnectionPtr&) override;
	
    virtual void handleNewConnection(const HttpRequestPtr&, const WebSocketConnectionPtr&) override;
	
    WS_PATH_LIST_BEGIN
        WS_PATH_ADD("/chat", Get);
    WS_PATH_LIST_END
private:
    PubSubService<std::string> chatRooms_;
};

struct Subscriber
{
    std::string chatRoomName_;
    drogon::SubscriberID id_;
};

void WebSocketChat::handleNewMessage(const WebSocketConnectionPtr& wsConnPtr, std::string&& message, const WebSocketMessageType& type) {
    // write your application logic here

	

    LOG_DEBUG << "new websocket message:" << message;
    if (type == WebSocketMessageType::Ping)
    {
        LOG_DEBUG << "recv a ping";
    }
    else if (type == WebSocketMessageType::Text)
    {
        auto& s = wsConnPtr->getContextRef<Subscriber>();
        chatRooms_.publish(s.chatRoomName_, message);
        
     
        
    }
}

void WebSocketChat::handleConnectionClosed(const WebSocketConnectionPtr& conn)
{
	
    LOG_DEBUG << "websocket closed!";
    auto& s = conn->getContextRef<Subscriber>();
    chatRooms_.unsubscribe(s.chatRoomName_, s.id_);
}

void WebSocketChat::handleNewConnection(const HttpRequestPtr& req, const WebSocketConnectionPtr& conn) {

    std::string IPSTRING;
    IPSTRING = req->getPeerAddr().toIp();

    LOG_INFO << "Path :" << req->path();

	LOG_INFO << "IP : " << IPSTRING;

    conn->send("sdf", WebSocketMessageType::Binary);
    
    Subscriber s;
    s.chatRoomName_ = req->getParameter("room_name");

    s.id_ = chatRooms_.subscribe(s.chatRoomName_,
        [conn](const std::string& topic,
            const std::string& message) {
                // Supress unused variable warning
                (void)topic;
                 conn->send(message);
        });

    conn->setContext(std::make_shared<Subscriber>(std::move(s)));
}



int main()
{

    app().registerHandler("/", [](const HttpRequestPtr&, std::function<void(const HttpResponsePtr&)>&& callback) {
       auto resp = HttpResponse::newHttpResponse();
       resp->setBody("Hello, World!");
    
       
       callback(resp);
    }, { Get });

	
    app().addListener("127.0.0.1", 8848).enableSendfile(true).enableSession(2min).run();
	
}