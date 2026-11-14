#include "artemis.h"
#include "constantes.h"
#include <math.h>

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
    return (angulo_grados >= ANGULO_REINGRESO_MIN_GRADOS) && (angulo_grados <= ANGULO_REINGRESO_MAX_GRADOS);
}

/* --- Simulacion fase a fase --- */

static double grados_a_radianes(double grados) {
    return grados * 3.14159265358979323846 / 180.0;
}

static void registrar_historial(Mision *m, double tiempo_total) {
    int segundo_actual = (int)tiempo_total;
    if (segundo_actual <= m->ultimo_segundo_registrado) {
        return;
    }
    if (m->cantidad_registros >= MAX_TURNOS) {
        return;
    }
    RegistroHistorial *r = &m->historial[m->cantidad_registros];
    r->tiempo_s = tiempo_total;
    r->altitud_m = m->altitud;
    r->velocidad_ms = m->velocidad;
    r->fase = m->fase;
    m->cantidad_registros++;
    m->ultimo_segundo_registrado = segundo_actual;
}

void mision_iniciar(Mision *m, double angulo_grados) {
    m->altitud = ALTITUD_INICIAL_M;
    m->velocidad = VELOCIDAD_INICIAL_MS;
    m->angulo_grados = angulo_grados;
    m->desaceleracion_actual_ms2 = 0.0;
    m->fallida = !angulo_en_ventana_segura(angulo_grados);
    m->velocidad_en_paracaidas = 0.0;
    m->fuerza_impacto_n = 0.0;
    m->fuerza_impacto_pesos = 0.0;
    m->cantidad_registros = 0;
    m->ultimo_segundo_registrado = -1;
    m->altitud_max_fase = ALTITUD_INICIAL_M;

    m->fuerza_rozamiento_n = 0.0;
    m->energia_cinetica_perdida_j = 0.0;
    m->energia_rozamiento_perdida_j = 0.0;

    if (angulo_grados < ANGULO_REINGRESO_MIN_GRADOS) {
        m->fase = FASE_REBOTE;
        m->aceleracion_frenado = 0.0;
    } else {
        double angulo_rad = grados_a_radianes(angulo_grados);
        double s = (ALTITUD_INICIAL_M - ALTITUD_FINAL_M) / sin(angulo_rad);
        m->fase = FASE_FRENADO;
        m->aceleracion_frenado = (VELOCIDAD_INICIAL_MS * VELOCIDAD_INICIAL_MS -
                                   VELOCIDAD_FINAL_MS * VELOCIDAD_FINAL_MS) / (2.0 * s);
        m->desaceleracion_actual_ms2 = m->aceleracion_frenado;
        m->fuerza_rozamiento_n = MASA_MODULO_KG * m->aceleracion_frenado;
    }

    registrar_historial(m, 0.0);
}

static void avanzar_frenado(Mision *m, double dt) {
    double angulo_rad = grados_a_radianes(m->angulo_grados);
    double v_antes = m->velocidad;
    double proporcion_v;

    m->velocidad -= m->aceleracion_frenado * dt;
    m->altitud -= (v_antes + m->velocidad) / 2.0 * sin(angulo_rad) * dt;

    proporcion_v = m->velocidad / VELOCIDAD_INICIAL_MS;
    m->fuerza_rozamiento_n = MASA_MODULO_KG * m->aceleracion_frenado * proporcion_v * proporcion_v;

    m->energia_cinetica_perdida_j = 0.5 * MASA_MODULO_KG *
        (VELOCIDAD_INICIAL_MS * VELOCIDAD_INICIAL_MS - m->velocidad * m->velocidad);
    m->energia_rozamiento_perdida_j = m->energia_cinetica_perdida_j +
        MASA_MODULO_KG * GRAVEDAD_MS2 * (ALTITUD_INICIAL_M - m->altitud);

    if (m->altitud <= ALTITUD_FINAL_M) {
        m->altitud = ALTITUD_FINAL_M;
        m->velocidad = VELOCIDAD_FINAL_MS;
        m->velocidad_en_paracaidas = m->velocidad;
        m->fase = FASE_PARACAIDAS;
        m->altitud_max_fase = ALTITUD_FINAL_M;
        m->desaceleracion_actual_ms2 = DECELERACION_PARACAIDAS_MS2;
        m->fuerza_rozamiento_n = 0.0;
        m->energia_cinetica_perdida_j = 0.5 * MASA_MODULO_KG *
            (VELOCIDAD_INICIAL_MS * VELOCIDAD_INICIAL_MS - VELOCIDAD_FINAL_MS * VELOCIDAD_FINAL_MS);
        m->energia_rozamiento_perdida_j = m->energia_cinetica_perdida_j +
            MASA_MODULO_KG * GRAVEDAD_MS2 * (ALTITUD_INICIAL_M - ALTITUD_FINAL_M);
    }
}

static void avanzar_paracaidas(Mision *m, double dt) {
    double v_antes = m->velocidad;

    if (m->velocidad > VELOCIDAD_IMPACTO_MS) {
        m->velocidad -= DECELERACION_PARACAIDAS_MS2 * dt;
        if (m->velocidad < VELOCIDAD_IMPACTO_MS) {
            m->velocidad = VELOCIDAD_IMPACTO_MS;
        }
        m->desaceleracion_actual_ms2 = DECELERACION_PARACAIDAS_MS2;
    } else {
        m->desaceleracion_actual_ms2 = 0.0;
    }
    m->altitud -= (v_antes + m->velocidad) / 2.0 * dt;

    if (m->altitud <= 0.0) {
        m->altitud = 0.0;
        m->fuerza_impacto_n = calcular_fuerza_promedio_amerizaje(MASA_MODULO_KG, m->velocidad, HUNDIMIENTO_M);
        m->fuerza_impacto_pesos = m->fuerza_impacto_n / (MASA_MODULO_KG * GRAVEDAD_MS2);
        m->velocidad = 0.0;
        m->desaceleracion_actual_ms2 = 0.0;
        m->fase = FASE_TERMINADA;
    }
}

static void avanzar_rebote(Mision *m, double dt) {
    m->velocidad = VELOCIDAD_REBOTE_MS;
    m->altitud += VELOCIDAD_REBOTE_MS * dt;
    m->desaceleracion_actual_ms2 = 0.0;

    if (m->altitud >= ALTITUD_REBOTE_MAX_M) {
        m->fase = FASE_TERMINADA;
    }
}

void avanzar_simulacion(Mision *m, double dt, double tiempo_total) {
    switch (m->fase) {
        case FASE_FRENADO:
            avanzar_frenado(m, dt);
            break;
        case FASE_PARACAIDAS:
            avanzar_paracaidas(m, dt);
            break;
        case FASE_REBOTE:
            avanzar_rebote(m, dt);
            break;
        case FASE_TERMINADA:
            break;
    }
    registrar_historial(m, tiempo_total);
}

