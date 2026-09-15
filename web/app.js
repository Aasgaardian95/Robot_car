// Finn knappen og statusfeltet i HTML-en.
const fremoverKnapp = document.getElementById("fremover-knapp");
const statusfelt = document.getElementById("status");

// Kjør sendFremover når brukeren klikker på knappen.
fremoverKnapp.addEventListener("click", sendFremover);

async function sendFremover() {
  statusfelt.textContent = "Sender...";

  try {
    // Send kommandoen til C++-serveren.
    const svar = await fetch("/fremover", {
      method: "POST",
    });

    if (!svar.ok) {
      throw new Error("Serverfeil");
    }

    // Vis bekreftelsen fra serveren.
    statusfelt.textContent = await svar.text();
  } catch (feil) {
    statusfelt.textContent = "Kommandoen kunne ikke bekreftes.";
  }
}
