// Andrea Puccio SM3201456 //

#include <stdio.h>      
#include <stdlib.h>    
#include <unistd.h>     
#include <sys/mman.h>   
#include <sys/stat.h>   
#include "ppm.h"        

/**
 * Inizializza un'immagine PPM con le dimensioni specificate
 * @param filePath      Percorso del file da creare
 * @param pImage        Puntatore alla struttura immagine da inizializzare
 * @param imageWidth    Larghezza dell'immagine
 * @param imageHeight   Altezza dell'immagine
 * @return             0 se successo, -1 se errore
 */

int initializeImage(char* filePath, PPMImagePtr pImage, int imageWidth, int imageHeight) {
    // Apre il file in modalità scrittura/lettura
    FILE* filePtr = fopen(filePath, "w+");
    if (filePtr == NULL) {
        return -1;
    }
    
    // Scrive l'header PPM (formato P6, dimensioni e massimo valore)
    int headerBytes = fprintf(filePtr, "P6\n%d %d\n255\n", imageWidth, imageHeight);
    
    // Imposta la dimensione del file includendo header e dati immagine (3 byte per pixel RGB)
    if (ftruncate(fileno(filePtr), headerBytes + imageWidth * imageHeight * 3) != 0) {
        fclose(filePtr);
        return -1;
    }
    fclose(filePtr);

    // Riapre il file per il memory mapping
    pImage->fileHandle = fopen(filePath, "r+");
    if (pImage->fileHandle == NULL) {
        return -1;
    }

    // Ottiene la dimensione del file
    struct stat statBuffer;
    stat(filePath, &statBuffer);
    pImage->fileSize = statBuffer.st_size;

    // Legge l'header PPM per verificare il formato e le dimensioni
    if (fscanf(pImage->fileHandle, "P6\n%d %d\n255\n", &pImage->width, &pImage->height) != 2) {
        fclose(pImage->fileHandle);
        return -1;
    }
    
    // Memorizza la posizione dopo l'header
    pImage->headerOffset = ftell(pImage->fileHandle);

    // Mappa il file in memoria, saltando l'header
    pImage->imageData = (ColorPtr)((uint8_t*)mmap(
        NULL,                        // Indirizzo suggerito (NULL = sistema sceglie)
        pImage->fileSize,            // Dimensione del mapping
        PROT_READ | PROT_WRITE,      // Permessi di lettura e scrittura
        MAP_SHARED,                  // Condivisibile tra processi
        fileno(pImage->fileHandle),  // File descriptor
        0                            // Offset nel file
    ) + pImage->headerOffset);

    // Verifica se il mapping è riuscito
    if (pImage->imageData == MAP_FAILED) {
        fclose(pImage->fileHandle);
        return -1;
    }
    return 0;
}

/**
 * Ottiene il puntatore al pixel alle coordinate specificate
 * @param pImage  Puntatore all'immagine
 * @param x       Coordinata x
 * @param y       Coordinata y
 * @return        Puntatore al pixel o NULL se coordinate invalide
 */

Color* getPixelAt(PPMImagePtr pImage, int x, int y) {
    // Verifica validità dei parametri
    if (pImage == NULL || x >= pImage->width || y >= pImage->height || x < 0 || y < 0) {
        return NULL;
    }
    // Calcola e restituisce l'indirizzo del pixel
    return pImage->imageData + (y * pImage->width + x);
}

/**
 * Libera le risorse associate all'immagine
 * @param pImage  Puntatore all'immagine da liberare
 * @return        0 se successo, -1 se errore
 */
int cleanupImage(PPMImagePtr pImage) {
    if (pImage == NULL) {
        return -1;
    }
    // Rimuove il mapping e chiude il file
    munmap(pImage->imageData, pImage->fileSize);
    fclose(pImage->fileHandle);
    return 0;
}