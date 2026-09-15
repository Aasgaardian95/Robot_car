#include <drogon/drogon.h>
#include <functional>
#include <iostream>

int main()
{
    // GET /: Send nettsiden til nettleseren.
    drogon::app().registerHandler(
        "/",
        [](const drogon::HttpRequestPtr& request,
           std::function<void(const drogon::HttpResponsePtr&)>&& sendResponse)
        {
            auto response = drogon::HttpResponse::newHttpResponse();
            response->setContentTypeCode(drogon::CT_TEXT_HTML);

            // En navngitt råstreng lar HTML-en inneholde både
            // anførselstegn, parenteser og linjeskift.
            response->setBody(R"HTML(
                <!DOCTYPE html>
                <html lang="nb">
                <head>
                    <meta charset="UTF-8">
                    <title>Robotbil</title>
                </head>
                <body>
                    <h1>Robotbil</h1>

                    <button onclick="sendFremover()">Fremover</button>
                    <p id="status">Venter på kommando.</p>

                    <script>
                        // Denne funksjonen kjører i nettleseren.
                        async function sendFremover() {
                            const statusfelt =
                                document.getElementById("status");

                            statusfelt.textContent = "Sender...";

                            try {
                                // Send kommandoen til C++-serveren.
                                const svar = await fetch("/fremover", {
                                    method: "POST"
                                });

                                if (!svar.ok) {
                                    throw new Error("Serverfeil");
                                }

                                // Vis teksten som serveren svarer med.
                                statusfelt.textContent = await svar.text();
                            } catch (feil) {
                                statusfelt.textContent =
                                    "Kommandoen kunne ikke bekreftes.";
                            }
                        }
                    </script>
                </body>
                </html>
            )HTML");

            sendResponse(response);
        },
        {drogon::Get}
    );

    // POST /fremover: Motta kommandoen fra knappen.
    drogon::app().registerHandler(
        "/fremover",
        [](const drogon::HttpRequestPtr& request,
           std::function<void(const drogon::HttpResponsePtr&)>&& sendResponse)
        {
            // Foreløpig tester vi ved å skrive til terminalen.
            std::cout << "Fremover" << std::endl;

            // Bekreft til nettleseren at kommandoen er mottatt.
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