#include <drogon/drogon.h>

#include <opencv2/core.hpp>
#include <opencv2/videoio.hpp>
#include <opencv2/imgcodecs.hpp>

#include <functional>
#include <iostream>
#include <mutex>
#include <string>
#include <vector>

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
    // Åpne kamera nummer 0 gjennom Windows DirectShow.
    cv::VideoCapture kamera(0, cv::CAP_DSHOW);

    // Avslutt hvis kameraet ikke kunne åpnes.
    if (!kamera.isOpened())
    {
        std::cerr << "Kunne ikke aapne kameraet.\n";
        return 1;
    }

    // Hindrer at flere forespørsler leser kameraet samtidig.
    std::mutex kameraLaas;

    // Lever HTML og JavaScript fra undermappen web.
    drogon::app().setDocumentRoot("./web");
    drogon::app().setHomePage("index.html");

    // GET /kamera.jpg: Hent og send ett kamerabilde.
    drogon::app().registerHandler(
        "/kamera.jpg",
        [&kamera, &kameraLaas](
            const drogon::HttpRequestPtr& request,
            std::function<void(const drogon::HttpResponsePtr&)>&& sendResponse)
        {
            auto response = drogon::HttpResponse::newHttpResponse();

            // Bildet skal ikke mellomlagres i nettleseren.
            response->addHeader("Cache-Control", "no-store");

            // Mat lagrer pikselverdiene til bildet.
            cv::Mat bilde;
            bool lest;

            {
                // Bare én forespørsel får lese kameraet om gangen.
                std::lock_guard<std::mutex> laas(kameraLaas);

                lest = kamera.read(bilde);

                // Låsen frigjøres automatisk når blokken avsluttes.
            }

            // Send en feilmelding hvis kameraet ikke leverte et bilde.
            if (!lest || bilde.empty())
            {
                response->setStatusCode(drogon::k503ServiceUnavailable);
                response->setContentTypeCode(drogon::CT_TEXT_PLAIN);
                response->setBody("Kunne ikke hente kamerabilde.");
                sendResponse(response);
                return;
            }

            // Komprimer bildet til JPEG-byter i minnet.
            std::vector<unsigned char> jpeg;

            if (!cv::imencode(".jpg", bilde, jpeg))
            {
                response->setStatusCode(drogon::k500InternalServerError);
                response->setContentTypeCode(drogon::CT_TEXT_PLAIN);
                response->setBody("Kunne ikke lage JPEG.");
                sendResponse(response);
                return;
            }

            // Fortell nettleseren at svaret inneholder et JPEG-bilde.
            response->setContentTypeCode(drogon::CT_IMAGE_JPG);

            // Kopier JPEG-bytene til HTTP-svaret.
            // Lengden må oppgis fordi dette er binære data.
            response->setBody(std::string(
                reinterpret_cast<const char*>(jpeg.data()),
                jpeg.size()
            ));

            sendResponse(response);
        },
        {drogon::Get}
    );

    // POST /kommando: Motta kommandoer fra styreknappene.
    drogon::app().registerHandler(
        "/kommando",
        [](const drogon::HttpRequestPtr& request,
           std::function<void(const drogon::HttpResponsePtr&)>&& sendResponse)
        {
            auto response = drogon::HttpResponse::newHttpResponse();
            response->setContentTypeCode(drogon::CT_TEXT_PLAIN);

            // Be Drogon tolke forespørselens innhold som JSON.
            const auto data = request->getJsonObject();

            // Kontroller at vi fikk et objekt med tekstfeltet kommando.
            if (!data || !data->isObject()
                || !(*data)["kommando"].isString())
            {
                response->setStatusCode(drogon::k400BadRequest);
                response->setBody(
                    "Forventet JSON med tekstfeltet kommando."
                );
                sendResponse(response);
                return;
            }

            // Hent kommandoteksten fra JSON-objektet.
            const std::string kommando =
                (*data)["kommando"].asString();

            // Avvis kommandoer vi ikke støtter.
            if (!erGyldigKommando(kommando))
            {
                response->setStatusCode(drogon::k400BadRequest);
                response->setBody("Ukjent kommando.");
                sendResponse(response);
                return;
            }

            // Her kobler vi senere inn motorstyringen.
            std::cout << "Kommando: " << kommando << std::endl;

            // Bekreft at kommandoen er mottatt.
            response->setBody("Mottatt: " + kommando);
            sendResponse(response);
        },
        {drogon::Post}
    );

    std::cout << "Kameraet er aapnet.\n";
    std::cout << "Serveren starter: http://127.0.0.1:8080\n";

    // Serveren er tilgjengelig fra denne PC-en.
    drogon::app().addListener("127.0.0.1", 8080);

    // Hold serveren i gang og behandle forespørsler.
    drogon::app().run();

    return 0;
}