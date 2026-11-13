#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simulador.h"

static double leer_double(const char *mensaje) {
    char buffer[256];
    char *fin;
    double valor;

    for (;;) {
        printf("%s", mensaje);
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            continue;
        }
        buffer[strcspn(buffer, "\n")] = '\0';

        valor = strtod(buffer, &fin);
        while (*fin == ' ' || *fin == '\t') {
            fin++;
        }

        if (buffer[0] == '\0' || *fin != '\0') {
            printf("Error: %s no es un numero valido. Intente nuevamente.\n", buffer);
            continue;
        }
        return valor;
    }
}

double solicitar_angulo_reingreso(void) {
    printf("\n===== Datos de la simulacion de reingreso =====\n");
    return leer_double("Angulo de reingreso (grados): ");
}
