// Andrea Puccio SM3201456 //

#include <stdio.h>    
#include <stdlib.h>   
#include <unistd.h>   
#include <math.h>     
#include "scene.h"    

/**
 * Carica una scena 3D da un file
 * @param filePath  Percorso del file della scena
 * @param scene     Puntatore alla struttura scena da inizializzare
 * @return          0 se successo, -1 se errore
 */

int loadScene(char* filePath, ScenePtr scene) {
    // Apre il file della scena
    scene->fileHandle = fopen(filePath, "r");
    if (scene->fileHandle == NULL) {
        printf("Errore: impossibile aprire il file.\n");
        return -1;
    }

    // Legge l'intestazione del file che contiene:
    // - VP: punto di vista (viewpoint)
    // - BG: colore di sfondo (background)
    // - OBJ_N: numero di oggetti nella scena

    if (fscanf(
            scene->fileHandle,
            "VP %f %f %f\nBG %hhu %hhu %hhu\nOBJ_N %d\n",
            &scene->viewpoint.x,
            &scene->viewpoint.y,
            &scene->viewpoint.z,
            &scene->background.red,
            &scene->background.green,
            &scene->background.blue,
            &scene->objectCount
        ) != 7) {
        printf("Formato dell'intestazione non valido.\n");
        fclose(scene->fileHandle);
        return -1;
    }

    // Alloca memoria per gli array degli oggetti della scena
    scene->spheres = (Point3DPtr)malloc(scene->objectCount * sizeof(Point3D));
    scene->radii = (float*)malloc(scene->objectCount * sizeof(float));
    scene->sphereColors = (ColorPtr)malloc(scene->objectCount * sizeof(Color));
    
    // Verifica allocazione memoria
    if (!scene->spheres || !scene->radii || !scene->sphereColors) {
        printf("Errore di allocazione della memoria per gli oggetti.\n");
        fclose(scene->fileHandle);
        return -1;
    }

    // Legge i dati di ogni sfera:
    // - S: posizione (x,y,z), raggio, colore (r,g,b)
    for (int i = 0; i < scene->objectCount; i++) {
        if (fscanf(
                scene->fileHandle, 
                "S %f %f %f %f %hhu %hhu %hhu\n",
                &scene->spheres[i].x,
                &scene->spheres[i].y,
                &scene->spheres[i].z,
                &scene->radii[i],
                &scene->sphereColors[i].red,
                &scene->sphereColors[i].green,
                &scene->sphereColors[i].blue
            ) != 7) {
            printf("Errore di formattazione nei dati delle sfere.\n");
            fclose(scene->fileHandle);
            return -1;
        }
    }

    return 0;
}

/**
 * Calcola il colore di un pixel utilizzando ray tracing semplificato
 * @param scene         Puntatore alla scena
 * @param initialColor  Colore iniziale (background)
 * @param x,y          Coordinate del pixel
 * @param width,height Dimensioni dell'immagine
 * @return             Colore calcolato per il pixel
 */

Color calculatePixelColor(ScenePtr scene, Color initialColor, int x, int y, int width, int height) {
    // Distanza minima all'intersezione più vicina
    float minDistance = INFINITY;  

    // Calcola la direzione del raggio per il pixel corrente
    Point3D rayDirection;
    rayDirection.x = (((scene->viewpoint.x) / (width - 1))) * x - (scene->viewpoint.x / 2);
    rayDirection.y = (((scene->viewpoint.y) / (height - 1))) * y - (scene->viewpoint.y / 2);
    rayDirection.z = 1.0f;

    // Normalizza la direzione del raggio
    float rayLength = sqrtf(rayDirection.x * rayDirection.x + rayDirection.y * rayDirection.y + rayDirection.z * rayDirection.z);
    rayDirection.x /= rayLength;
    // Inverte Y per coordinate schermo
    rayDirection.y = -rayDirection.y / rayLength;  
    rayDirection.z /= rayLength;

    // Calcola il coefficiente a dell'equazione quadratica per l'intersezione raggio-sfera
    float a = rayDirection.x * rayDirection.x + 
              rayDirection.y * rayDirection.y + 
              rayDirection.z * rayDirection.z;

    // Per ogni sfera nella scena
    for (int i = 0; i < scene->objectCount; i++) {
        // Calcola i coefficienti b e c dell'equazione quadratica
        float b = -2.0f * (rayDirection.x * scene->spheres[i].x + rayDirection.y * scene->spheres[i].y +rayDirection.z * scene->spheres[i].z);
                          
        float c = (scene->spheres[i].x * scene->spheres[i].x +scene->spheres[i].y * scene->spheres[i].y +scene->spheres[i].z * scene->spheres[i].z) -(scene->radii[i] * scene->radii[i]);

        // Calcola il discriminante per verificare l'intersezione
        float discriminant = b * b - 4.0f * a * c;
        
        // Se c'è intersezione e è più vicina della precedente
        if (discriminant >= 0.0f) {
            float intersectionDist = (-b - sqrtf(discriminant)) / (2.0f * a);
            if (intersectionDist > 0.0f && intersectionDist < minDistance) {
                minDistance = intersectionDist;
                initialColor = scene->sphereColors[i];
            }
        }
    }
    return initialColor;
}