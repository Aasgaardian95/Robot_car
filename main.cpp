#include <drogon/drogon.h>
#include <functional>
#include <iostream>

int main()
{
    // Lever nettsidefiler fra undermappen web.
    drogon::app().setDocumentRoot("./web");

    // Bruk index.html som forside.
    drogon::app().setHomePage("index.html");

    // Motta kommandoen fra nettleseren.
    drogon::app().registerHandler(
        "/fremover",
        [](const drogon::HttpRequestPtr& request,
           std::function<void(const drogon::HttpResponsePtr&)>&& sendResponse)
        {
            std::cout << "Fremover" << std::endl;

            auto response = drogon::HttpResponse::newHttpResponse();
            response->setContentTypeCode(drogon::CT_TEXT_PLAIN);
            response->setBody("Fremover mottatt!");

            sendResponse(response);
        },
        {drogon::Post}
    );

    std::cout << "Serveren starter: http://127.0.0.1:8080\n";

    drogon::app().addListener("127.0.0.1", 8080);
    drogon::app().run();

    return 0;
}