// Andrea Puccio SM3201456 //

#include <stdio.h>      
#include <stdlib.h>    
#include <unistd.h>    
#include "/opt/homebrew/opt/libomp/include/omp.h"  // Per il supporto OpenMP (parallelizzazione)
#include "scene.h"      
#include "ppm.h"        

int main(int argc, char* argv[]) {
    // Strutture dati principali
    Scene currentScene;       // Struttura per memorizzare la scena
    PPMImage outputImage;     // Struttura per l'immagine di output
    char sceneFilename[100];  // Nome del file della scena
    char outputFilename[100]; // Nome del file di output
    int imageWidth;           // Larghezza dell'immagine
    int imageHeight;          // Altezza dell'immagine
    
    // Input da riga di comando
    printf("Inserire: nome file scena, nome file immagine, larghezza altezza (es: scene.txt output.ppm 800 600): ");
    if(scanf("%s %s %d %d", sceneFilename, outputFilename, &imageWidth, &imageHeight) != 4) {
        printf("Errore: formato input non valido.\n");
        return 0;
    }

    // Caricamento della scena dal file
    int status = loadScene(sceneFilename, &currentScene);
    if(status != 0) {
        // Uscita in caso di errore nel caricamento
        return 0;  
    }
    printf("Scena aperta.\n");

    // Inizializzazione dell'immagine di output
    status = initializeImage(outputFilename, &outputImage, imageWidth, imageHeight);
    if (status != 0) {
        printf("Errore nella creazione.\n");
        return 0;
    }
    printf("Immagine creata.\n");
  
    // Misurazione del tempo di elaborazione
    double startTime = omp_get_wtime();

    // Elaborazione parallela dei pixel dell'immagine
    #pragma omp parallel for collapse(2)  
    // Parallelizzazione dei due cicli for annidati
    for(int row = 0; row < outputImage.width; row++) {
        for(int col = 0; col < outputImage.height; col++) {
            // Inizializzazione del colore con il background della scena
            Color pixelColor;
            pixelColor.red = currentScene.background.red;
            pixelColor.green = currentScene.background.green;
            pixelColor.blue = currentScene.background.blue;
            
            // Calcolo del colore finale del pixel
            Color* currentPixel = getPixelAt(&outputImage, row, col);
            *currentPixel = calculatePixelColor(&currentScene, pixelColor, row, col, 
                                              outputImage.width, outputImage.height);
        }
    }

    // Calcolo del tempo di elaborazione
    double endTime = omp_get_wtime();
    double elapsedMs = (endTime - startTime) * 1000.0;

    printf("Pixel elaborati.\n");

    // Pulizia e chiusura dell'immagine
    status = cleanupImage(&outputImage);
    if (status != 0) {
        printf("Errore nella chiusura dell'oggetto immagine.\n");
        return 0;
    }
    printf("Chiusura immagine.\n");

    // Stampa del tempo di esecuzione 
    printf("Tempo di esecuzione: %f ms\n", elapsedMs);
    return 0;
}