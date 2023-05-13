#include "queue.h"
#include "file_reader.h"
#include "file_writer.h"

// Dati in ingresso e requisiti:
// - un file di testo in codifica Unicode (UTF-8) contenente un testo in italiano
// strutturato in paragrafi separati da una o più linee vuote (stringa contenente solo il
// carattere '\n'). Ogni paragrafo è costituito da una o più stringhe terminate da '\n' (il
// testo in ingresso è tutto su una sola colonna);
// - il numero di colonne per pagina su cui distribuire il testo;
// - il numero di linee per ogni colonna;
// - larghezza di ogni colonna (numero di caratteri per colonna);
// - la distanza in caratteri tra una colonna di testo e l'altra.

// Dati in uscita e requisiti:
// - un file di testo in codifica Unicode (UTF-8) contenente il testo di input incolonnato e
// impaginato secondo i parametri espressi dall’utente. Le pagine sono separate dalla
// stringa “\n %%% \n”;
// - ogni paragrafo è separato da una linea vuota;
// - la prima colonna di testo è a sinistra;
// - tutte le righe, tranne l’ultima, di ogni paragrafo dovranno essere allineate ad
// entrambi i margini della colonna;
// - l’ultima riga di ogni paragrafo è solo allineata a sinistra;
// - le parole in una linea dovranno essere separate da almeno un carattere di spazio;
// - la sillabazione di eventuali parole troppo lunghe non è necessaria.

int main(int argc, char* argv[]) {

    if(argc < 6){
        printf("Wrong usage! Run %s -h for more informations\n", argv[0]);
        return 1;
    }

    const char* INPUT_FILE = argv[1];
    const char* OUTPUT_FILE;
    const int COLUMN_AMOUNT = atoi(argv[2]);
    const int COLUMN_HEIGHT = atoi(argv[3]);
    const int COLUMN_WIDTH = atoi(argv[4]);
    const int INTER_COL_SPACE = atoi(argv[5]);

    //default value
    OUTPUT_FILE = "out.txt";

    Queue* pages;

    pages = read_pages(INPUT_FILE, COLUMN_AMOUNT, COLUMN_HEIGHT, COLUMN_WIDTH);
    write_pages(OUTPUT_FILE, pages, INTER_COL_SPACE);
    
    return 0;
}