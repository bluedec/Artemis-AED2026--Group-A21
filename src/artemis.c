#include "artemis.h"
#include "constantes.h"

double calcular_trabajo_rozamiento(ModuloOrion modulo) {
    double delta_ec = 0.5 * modulo.masa *
        (modulo.velocidad_final * modulo.velocidad_final -
         modulo.velocidad_inicial * modulo.velocidad_inicial);
    double delta_ep = modulo.masa * GRAVEDAD_MS2 *
        (modulo.altitud_inicial - modulo.altitud_final);
    return delta_ec - delta_ep;
}

double calcular_desaceleracion_media(double vi, double vf, double delta_t) {
    return (vf - vi) / delta_t;
}

double calcular_fuerza_promedio_amerizaje(double masa, double v_impacto, double hundimiento) {
    return masa * ((v_impacto * v_impacto) / (2.0 * hundimiento) + GRAVEDAD_MS2);
}

int angulo_en_ventana_segura(double angulo_grados) {
    return angulo_grados >= ANGULO_REINGRESO_MIN_GRADOS &&
           angulo_grados <= ANGULO_REINGRESO_MAX_GRADOS;
}
