// Andrea Puccio SM3201456 //

#ifndef _PPM_H
#define _PPM_H

#include <stdio.h>   
#include <stdint.h>  
#include "scene.h"   

/**
 * Struttura che rappresenta un'immagine PPM
 * packed: garantisce che non ci sia padding tra i membri della struttura
 */

struct PPMImage {
    int width;              // Larghezza dell'immagine in pixel
    int height;             // Altezza dell'immagine in pixel
    int headerOffset;       // Offset in byte dall'inizio del file ai dati immagine
    int fileSize;           // Dimensione totale del file in byte
    FILE* fileHandle;       // Puntatore al file dell'immagine
    Color* imageData;       // Puntatore ai dati dell'immagine mappati in memoria
} __attribute__((packed));

// Definizione di tipo 
typedef struct PPMImage PPMImage;
typedef struct PPMImage* PPMImagePtr;

/**
 * Inizializza una nuova immagine PPM
 * @param filePath  Percorso del file da creare
 * @param image     Puntatore alla struttura immagine da inizializzare
 * @param width     Larghezza dell'immagine
 * @param height    Altezza dell'immagine
 * @return          0 se successo, -1 se errore
 */

int initializeImage(char* filePath, PPMImagePtr image, int width, int height);

/**
 * Ottiene il puntatore al pixel alle coordinate specificate
 * @param image  Puntatore all'immagine
 * @param x      Coordinata x
 * @param y      Coordinata y
 * @return       Puntatore al pixel o NULL se coordinate invalide
 */

Color* getPixelAt(PPMImagePtr image, int x, int y);

/**
 * Libera le risorse associate all'immagine
 * @param image  Puntatore all'immagine da liberare
 * @return       
 */
int cleanupImage(PPMImagePtr image);

#endif 