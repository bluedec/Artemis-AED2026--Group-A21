#ifndef CONSTANTS_H
#define CONSTANTS_H

/* --- Datos del escenario (Reingreso, Frenado y Amerizaje) --- */
#define MASA_MODULO_KG 10400.0        /* m: masa del modulo Orion */
#define VELOCIDAD_INICIAL_MS 11000.0  /* vi: velocidad al reingresar */
#define ALTITUD_INICIAL_M 120000.0    /* hi: altitud al reingresar */
#define VELOCIDAD_FINAL_MS 25.0       /* vf: velocidad al desplegar paracaidas */
#define ALTITUD_FINAL_M 2500.0        /* hf: altitud al desplegar paracaidas */
#define VELOCIDAD_IMPACTO_MS 8.0      /* v_impacto: velocidad al amerizar */
#define HUNDIMIENTO_M 1.5             /* d: profundidad que se hunde antes de flotar */
#define DELTA_T_DESACELERACION_S 400.0 /* duracion de la fase de mayor desaceleracion */
#define GRAVEDAD_MS2 9.8              /* g: aceleracion de la gravedad */

/* --- Ventana segura de reingreso --- */
#define ANGULO_REINGRESO_MIN_GRADOS 5.5
#define ANGULO_REINGRESO_MAX_GRADOS 7.0

/* --- Paso fijo de la simulacion (desacopla la fisica de los FPS) --- */
#define DT_FISICA_S 0.1    /* paso de integracion, en segundos simulados */
#define ESCALA_TIEMPO 20.0 /* segundos simulados por cada segundo real */

/* --- Fase de paracaidas --- */
#define DECELERACION_PARACAIDAS_MS2 2.0 /* frenado bajo el paracaidas hasta llegar a v_impacto */

/* --- Rebote atmosferico (gamma por debajo de la ventana segura) --- */
/* El enunciado solo exige que la capsula "rebote" y la mision se pierda;
 * no hay una formula fisica para ese ascenso, asi que alcanza con una
 * velocidad de salida simple y un umbral para considerarla fuera de escena. */
#define VELOCIDAD_REBOTE_MS 3000.0
#define ALTITUD_REBOTE_MAX_M (ALTITUD_INICIAL_M * 1.1)

/* --- Historial de la simulacion --- */
/* Un registro por segundo simulado. El vuelo completo (frenado: entre
 * ~175 s en el limite superior de la ventana y ~225 s en el inferior,
 * mas paracaidas: ~305 s, fijo) nunca supera los ~530 s, asi que 700
 * deja margen sin desperdiciar memoria. */
#define MAX_TURNOS 700

/* --- Configuracion de la ventana (raylib) --- */
#define VENTANA_ANCHO_PX 1300
#define VENTANA_ALTO_PX 900
#define VENTANA_TITULO "Artemis - Reingreso"
#define VENTANA_FPS 60

/* --- Disposicion de la animacion (coordenadas en pixeles) --- */
#define Y_TECHO_PX 60
#define Y_MAR_PX (VENTANA_ALTO_PX - 60)
#define X_CAPSULA_PX (VENTANA_ANCHO_PX / 2)
#define CAPSULA_ANCHO_PX 40
#define CAPSULA_ALTO_PX 26
#define PLASMA_RADIO_MAX_PX 50
#define PARACAIDAS_RADIO_PX 45
#define PARACAIDAS_ALTURA_PX 70

/* --- Boton de pausa --- */
#define BOTON_PAUSA_X_PX 20
#define BOTON_PAUSA_Y_PX 155
#define BOTON_PAUSA_ANCHO_PX 130
#define BOTON_PAUSA_ALTO_PX 36

/* --- Mensaje final (amerizaje o mision fallida) --- */
#define MENSAJE_FINAL_Y_PX 200

#endif