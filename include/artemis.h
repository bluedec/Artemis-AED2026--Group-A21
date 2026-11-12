#ifndef ARTEMIS_H
#define ARTEMIS_H

/* Estado del modulo Orion durante el reingreso */
typedef struct {
    double masa;
    double velocidad_inicial;
    double altitud_inicial;
    double velocidad_final;
    double altitud_final;
    double velocidad_impacto;
} ModuloOrion;

/* Calculos del escenario (ver docs/enunciado.md) */
double calcular_trabajo_rozamiento(ModuloOrion modulo);
double calcular_desaceleracion_media(double vi, double vf, double delta_t);
double calcular_fuerza_promedio_amerizaje(double masa, double v_impacto, double hundimiento);
int angulo_en_ventana_segura(double angulo_grados);

#endif
