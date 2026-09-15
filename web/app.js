const statusfelt = document.getElementById("status");

// Finn alle styreknappene.
const knapper = document.querySelectorAll("[data-kommando]");

// Koble hver knapp til den samme funksjonen.
for (const knapp of knapper) {
  knapp.addEventListener("click", () => {
    sendKommando(knapp.dataset.kommando);
  });
}

async function sendKommando(kommando) {
  statusfelt.textContent = "Sender: " + kommando;

  try {
    const svar = await fetch("/kommando", {
      method: "POST",

      headers: {
        "Content-Type": "application/json",
      },

      body: JSON.stringify({ kommando: kommando }),
    });

    const melding = await svar.text();

    if (!svar.ok) {
      statusfelt.textContent = "Avvist: " + melding;
      return;
    }

    statusfelt.textContent = melding;
  } catch (feil) {
    statusfelt.textContent = "Kommandoen kunne ikke bekreftes.";
  }
} // sendKommando avsluttes her.

// Kamerakoden ligger utenfor sendKommando.
// Den kjøres når app.js lastes.
const kamerabilde = document.getElementById("kamerabilde");

function hentKamerabilde() {
  kamerabilde.src = "/kamera.jpg?t=" + Date.now();
}

// Hent neste bilde 33 ms etter at forrige er ferdig lastet.
kamerabilde.addEventListener("load", () => {
  setTimeout(hentKamerabilde, 33);
});

// Hvis innlasting feiler, prøv igjen etter ett sekund.
kamerabilde.addEventListener("error", () => {
  setTimeout(hentKamerabilde, 1000);
});

// Start kameraoppdateringen uten å vente på et knappetrykk.
hentKamerabilde();
