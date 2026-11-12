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

/* --- Configuracion de la ventana (raylib) --- */
#define VENTANA_ANCHO_PX 1300
#define VENTANA_ALTO_PX 900
#define VENTANA_TITULO "Artemis - Reingreso"
#define VENTANA_FPS 60

#endif 