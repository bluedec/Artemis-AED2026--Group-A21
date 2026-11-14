#include "tiempo.h"

#define DELTA_REAL_MAX_S 0.25

void reloj_iniciar(Reloj *r, double escala) {
    r->escala = escala;
    r->acumulador = 0.0;
    r->tiempo_total = 0.0;
}

void reloj_avanzar(Reloj *r, double delta_real) {
    if (delta_real > DELTA_REAL_MAX_S) {
        delta_real = DELTA_REAL_MAX_S;
    }
    r->acumulador += delta_real * r->escala;
}

int reloj_consumir_paso(Reloj *r, double paso) {
    if (r->acumulador < paso) {
        return 0;
    }
    r->acumulador -= paso;
    r->tiempo_total += paso;
    return 1;
}
