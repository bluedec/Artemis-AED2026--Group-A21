#ifndef ARTEMIS_H
#define ARTEMIS_H

#include "constantes.h"

/* Estado del modulo Orion durante el reingreso */
typedef struct {
    double masa;
    double velocidad_inicial;
    double altitud_inicial;
    double velocidad_final;
    double altitud_final;
    double velocidad_impacto;
} ModuloOrion;

/* Calculos analiticos del escenario (ver docs/enunciado.md, preguntas a-d) */
double calcular_trabajo_rozamiento(ModuloOrion modulo);
double calcular_desaceleracion_media(double vi, double vf, double delta_t);
double calcular_fuerza_promedio_amerizaje(double masa, double v_impacto, double hundimiento);
int angulo_en_ventana_segura(double angulo_grados);

/* --- Simulacion fase a fase, para la animacion --- */

typedef enum {
    FASE_FRENADO,
    FASE_PARACAIDAS,
    FASE_REBOTE,
    FASE_TERMINADA
} FaseSimulacion;

/* Un registro del historial por cada segundo simulado */
typedef struct {
    double tiempo_s;
    double altitud_m;
    double velocidad_ms;
    FaseSimulacion fase;
} RegistroHistorial;

typedef struct {
    double altitud;
    double velocidad;
    double angulo_grados;

    double aceleracion_frenado;       /* a constante, calculada al iniciar FRENADO */
    double desaceleracion_actual_ms2; /* para el HUD: cuanto se desacelera en este instante */
    double fuerza_rozamiento_n;       /* fuerza de rozamiento instantanea, recalculada cada paso */
    double energia_cinetica_perdida_j; /* cuanta Ec se perdio desde el reingreso, recalculada cada paso */
    double altitud_max_fase;          /* techo de la escala de dibujo en la fase actual */

    FaseSimulacion fase;
    int fallida; /* 1 si gamma quedo fuera de la ventana segura (rebote o desaceleracion excesiva) */

    double velocidad_en_paracaidas; /* v al llegar a la altitud de apertura */
    double fuerza_impacto_n;        /* F_prom al amerizar, en N */
    double fuerza_impacto_pesos;    /* F_prom como multiplo del peso */

    RegistroHistorial historial[MAX_TURNOS];
    int cantidad_registros;
    int ultimo_segundo_registrado;
} Mision;

void mision_iniciar(Mision *m, double angulo_grados);
void avanzar_simulacion(Mision *m, double dt, double tiempo_total);

#endif