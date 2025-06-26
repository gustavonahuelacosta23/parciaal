#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <windows.h>

 void play_audio(uint32_t sample_rate, uint32_t sample_count, float *audio);

int main() {
    FILE *musiquita = fopen("audio_list.raw", "rb");
    if (musiquita==NULL) {
        printf("No se pudo abrir el archivo.\n");
        return 1;
    }

    // Leo la  cantidad de pistas
    uint32_t total_pistas;
    fread(&total_pistas, sizeof(uint32_t), 1, musiquita);

    // Mostrar pistas
    printf("Hay %u pistas.\n", total_pistas);
    printf("Listado de pistas:\n");

    // Guardamos donde empieza cada audio
    long posiciones[100];

    for (int i = 0; i < total_pistas; i++) {
        uint32_t sample_rate, sample_count;
        char nombre[64];

        fread(&sample_rate, sizeof(uint32_t), 1, musiquita);
        fread(&sample_count, sizeof(uint32_t), 1, musiquita);
        fread(nombre, sizeof(char), 64, musiquita);

        posiciones[i] = ftell(musiquita); // guardo posición de los datos de audio

        printf("Pista %d: %s - %u muestras, %u Hz\n", i + 1, nombre, sample_count, sample_rate);
    }

    int opc;
    do {
        uint32_t sample_rate, sample_count;
        char nombre[64];
        printf("\nSelecciona pista (1 a %u, 0 para salir): ", total_pistas);
        scanf("%d", &opc);
        if (opc <= 0 || opc > (int)total_pistas) break;

        // para volver a leer los datos de la pista seleccionada
       fseek(musiquita, 4, SEEK_SET); 

        for (int i = 0; i < opc - 1; i++) {
            uint32_t sr, sc;
            fread(&sr, sizeof(uint32_t), 1, musiquita);
            fread(&sc, sizeof(uint32_t), 1, musiquita);
            fseek(musiquita, 64 + sc * sizeof(float), SEEK_CUR);
        }

        fread(&sample_rate, sizeof(uint32_t), 1, musiquita);
        fread(&sample_count, sizeof(uint32_t), 1, musiquita);
        fread(nombre, sizeof(char), 64, musiquita);

        float *datos = malloc(sample_count * sizeof(float));
        fread(datos, sizeof(float), sample_count, musiquita);

        printf("Reproduciendo:\n");
        play_audio(sample_rate, sample_count, datos);

        // Guardado
        char resp;
        printf("¿Desea guardar la pista en sound.raw? (s/n): ");
        scanf(" %c", &resp);
        if (resp == 's' || resp == 'S') {
            FILE *salida = fopen("sound.raw", "wb");
            if (salida) {
                fwrite(datos, sizeof(float), sample_count, salida);
                fclose(salida);
                printf("Guardada correctamente.\n");
            } else {
                printf("Error al guardar.\n");
            }
        }

        free(datos);
    } while (1);

    printf("eto est esto esto es todos amigos\n");
    fclose(musiquita);
    return 0;
}
