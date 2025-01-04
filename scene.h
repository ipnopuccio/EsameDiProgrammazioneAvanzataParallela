// Andrea Puccio SM3201456 //

#ifndef _SCENE_H
#define _SCENE_H

// Inclusione delle librerie necessarie
#include <stdio.h>    
#include <stdint.h>   

/**
 * Struttura che rappresenta un colore RGB
 * packed: garantisce che non ci sia padding tra i componenti del colore
 */

struct Color {
    uint8_t red;      
    uint8_t green;    
    uint8_t blue;     
} __attribute__((packed));

typedef struct Color Color;
typedef struct Color* ColorPtr;

/**
 * Struttura che rappresenta un punto nello spazio 3D
 */

struct Point3D {
    float x;          
    float y;          
    float z;          
};

typedef struct Point3D Point3D;
typedef struct Point3D* Point3DPtr;

/**
 * Struttura che rappresenta una scena 3D contenente sfere
 */
struct Scene {
    FILE* fileHandle;      // Handle del file della scena
    Point3D viewpoint;     // Punto di vista della camera
    Color background;      // Colore di sfondo della scena
    int objectCount;       // Numero di oggetti (sfere) nella scena
    Point3DPtr spheres;    // Array delle posizioni delle sfere
    float* radii;          // Array dei raggi delle sfere
    ColorPtr sphereColors; // Array dei colori delle sfere
};

typedef struct Scene Scene;
typedef struct Scene* ScenePtr;

/**
 * Carica una scena da un file
 * @param filePath  Percorso del file della scena
 * @param scene     Puntatore alla struttura scena da inizializzare
 * @return          0 se successo, -1 se errore
 */

int loadScene(char* filePath, ScenePtr scene);

/**
 * Calcola il colore di un pixel usando ray tracing
 * @param scene         Puntatore alla scena
 * @param initialColor  Colore iniziale (background)
 * @param x            Coordinata X del pixel
 * @param y            Coordinata Y del pixel
 * @param width        Larghezza dell'immagine
 * @param height       Altezza dell'immagine
 * @return             Colore calcolato per il pixel
 */

Color calculatePixelColor(ScenePtr scene, Color initialColor, int x, int y, int width, int height);

#endif 