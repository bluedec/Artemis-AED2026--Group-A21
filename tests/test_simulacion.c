#include <stdio.h>
#include <math.h>
#include "artemis.h"
#include "tiempo.h"
#include "constantes.h"

/* Corre la mision completa alimentando el reloj con un FPS fijo (simula
 * los frames) para demostrar que el resultado fisico no depende de el. */
static void simular_con_fps(double angulo_grados, double fps, Mision *m) {
    Reloj reloj;
    int iteraciones = 0;
    const int limite_iteraciones = 1000000;

    mision_iniciar(m, angulo_grados);
    reloj_iniciar(&reloj, ESCALA_TIEMPO);

    while (m->fase != FASE_TERMINADA && iteraciones < limite_iteraciones) {
        reloj_avanzar(&reloj, 1.0 / fps);
        while (m->fase != FASE_TERMINADA && reloj_consumir_paso(&reloj, DT_FISICA_S)) {
            avanzar_simulacion(m, DT_FISICA_S, reloj.tiempo_total);
        }
        iteraciones++;
    }
}

static int comparar(const char *nombre, double esperado, double obtenido, double tolerancia) {
    double error = fabs(obtenido - esperado);
    int ok = error < tolerancia;
    printf("  %-40s esperado=%9.4f obtenido=%9.4f -> %s\n",
           nombre, esperado, obtenido, ok ? "OK" : "ERROR");
    return ok;
}

int main(void) {
    const double angulo = 6.25;
    Mision m30;
    Mision m144;
    Mision m_inicial;
    int todo_ok = 1;

    simular_con_fps(angulo, 30.0, &m30);
    simular_con_fps(angulo, 144.0, &m144);

    /* La fuerza de rozamiento se recalcula paso a paso con la velocidad
     * actual (cae con v^2); al reingresar (v = v_inicial) debe coincidir
     * con m*a_frenado, el valor maximo de toda la fase. */
    mision_iniciar(&m_inicial, angulo);

    printf("--- Verificacion fisica (gamma = %.2f grados) ---\n", angulo);
    todo_ok &= comparar("Desaceleracion de frenado (m/s2)", 56.0, m30.aceleracion_frenado, 1.0);
    todo_ok &= comparar("Desaceleracion de frenado (g)", 5.7, m30.aceleracion_frenado / GRAVEDAD_MS2, 0.1);
    todo_ok &= comparar("Velocidad al abrir el paracaidas (m/s)", VELOCIDAD_FINAL_MS, m30.velocidad_en_paracaidas, 0.1);
    todo_ok &= comparar("Fuerza de impacto (multiplo del peso)", 3.2, m30.fuerza_impacto_pesos, 0.1);
    todo_ok &= comparar("Fuerza de rozamiento al reingresar (N)",
                         MASA_MODULO_KG * m_inicial.aceleracion_frenado, m_inicial.fuerza_rozamiento_n, 0.01);

    /* La energia cinetica perdida durante el frenado no depende de gamma
     * (solo de vi y vf, que son fijos): siempre debe llegar al mismo total. */
    todo_ok &= comparar("Energia cinetica perdida en el frenado (J)",
                         0.5 * MASA_MODULO_KG * (VELOCIDAD_INICIAL_MS * VELOCIDAD_INICIAL_MS -
                                                  VELOCIDAD_FINAL_MS * VELOCIDAD_FINAL_MS),
                         m30.energia_cinetica_perdida_j, 1.0);

    printf("--- Independencia del FPS (30 fps vs 144 fps) ---\n");
    todo_ok &= comparar("Velocidad al abrir el paracaidas", m30.velocidad_en_paracaidas, m144.velocidad_en_paracaidas, 0.0001);
    todo_ok &= comparar("Fuerza de impacto (N)", m30.fuerza_impacto_n, m144.fuerza_impacto_n, 0.0001);
    todo_ok &= comparar("Cantidad de registros de historial", (double)m30.cantidad_registros, (double)m144.cantidad_registros, 0.5);

    printf("\n%s\n", todo_ok ? "TODAS LAS VERIFICACIONES PASARON" : "HAY VERIFICACIONES FALLIDAS");
    return todo_ok ? 0 : 1;
}
