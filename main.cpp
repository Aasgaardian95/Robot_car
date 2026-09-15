#include <drogon/drogon.h>
#include <functional>
#include <iostream>

int main()
{
    // Bestem hva som skjer når nettleseren ber om forsiden "/".
    drogon::app().registerHandler(
        "/",
        [](const drogon::HttpRequestPtr& request,
           std::function<void(const drogon::HttpResponsePtr&)>&& sendResponse)
        {
            // Opprett et HTTP-svar.
            auto response = drogon::HttpResponse::newHttpResponse();

            // Fortell nettleseren at innholdet er HTML.
            response->setContentTypeCode(drogon::CT_TEXT_HTML);

            // HTML-koden som nettleseren skal vise.
            response->setBody(R"(
                <!DOCTYPE html>
                <html lang="nb">
                <head>
                    <meta charset="UTF-8">
                    <title>Robotbil</title>
                </head>
                <body>
                    <h1>Robotbil</h1>
                    <p>Nettsiden leveres av C++-programmet ditt!</p>
                </body>
                </html>
            )");

            // Send svaret tilbake til nettleseren.
            sendResponse(response);
        },
        {drogon::Get}
    );

    std::cout << "Serveren starter: http://127.0.0.1:8080\n";

    // Lytt etter forbindelser fra denne PC-en på port 8080.
    drogon::app().addListener("127.0.0.1", 8080);

    // Hold serveren i gang og behandle forespørsler.
    drogon::app().run();

    return 0;
}