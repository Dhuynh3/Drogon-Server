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
  
};



void WebSocketChat::handleNewMessage(const WebSocketConnectionPtr& wsConnPtr, std::string&& message, const WebSocketMessageType& type) {
  
}

void WebSocketChat::handleConnectionClosed(const WebSocketConnectionPtr& conn)
{
	
}

void WebSocketChat::handleNewConnection(const HttpRequestPtr& req, const WebSocketConnectionPtr& conn) {

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