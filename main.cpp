#include <drogon/drogon.h>
#include <functional>
#include <iostream>
#include <string>

// Returner true bare for kommandoene vi støtter.
bool erGyldigKommando(const std::string& kommando)
{
    return kommando == "fremover"
        || kommando == "bakover"
        || kommando == "venstre"
        || kommando == "hoyre"
        || kommando == "stopp";
}

int main()
{
    drogon::app().setDocumentRoot("./web");
    drogon::app().setHomePage("index.html");

    // Alle knappene sender til denne adressen.
    drogon::app().registerHandler(
        "/kommando",
        [](const drogon::HttpRequestPtr& request,
           std::function<void(const drogon::HttpResponsePtr&)>&& sendResponse)
        {
            auto response = drogon::HttpResponse::newHttpResponse();
            response->setContentTypeCode(drogon::CT_TEXT_PLAIN);

            // Be Drogon tolke innholdet som JSON.
            const auto data = request->getJsonObject();

            // Kontroller at vi fikk et objekt med et tekstfelt.
            if (!data || !data->isObject()
                || !(*data)["kommando"].isString())
            {
                response->setStatusCode(drogon::k400BadRequest);
                response->setBody("Forventet JSON med tekstfeltet kommando.");
                sendResponse(response);
                return;
            }

            // Hent kommandoteksten fra JSON-objektet.
            const std::string kommando =
                (*data)["kommando"].asString();

            // Avvis ukjente kommandoer.
            if (!erGyldigKommando(kommando))
            {
                response->setStatusCode(drogon::k400BadRequest);
                response->setBody("Ukjent kommando.");
                sendResponse(response);
                return;
            }

            // Her kobler vi senere inn motorstyringen.
            std::cout << "Kommando: " << kommando << std::endl;

            response->setBody("Mottatt: " + kommando);
            sendResponse(response);
        },
        {drogon::Post}
    );

    std::cout << "Serveren starter: http://127.0.0.1:8080\n";

    drogon::app().addListener("127.0.0.1", 8080);
    drogon::app().run();

    return 0;
}