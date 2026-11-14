#include <stdio.h>
#include "raylib.h"
#include "constantes.h"
#include "artemis.h"
#include "simulador.h"
#include "tiempo.h"
#include "interfaz.h"

int main(void) {
    ModuloOrion modulo = {
        .masa = MASA_MODULO_KG,
        .velocidad_inicial = VELOCIDAD_INICIAL_MS,
        .altitud_inicial = ALTITUD_INICIAL_M,
        .velocidad_final = VELOCIDAD_FINAL_MS,
        .altitud_final = ALTITUD_FINAL_M,
        .velocidad_impacto = VELOCIDAD_IMPACTO_MS
    };
    double delta_t = DELTA_T_DESACELERACION_S;
    double hundimiento = HUNDIMIENTO_M;

    //double angulo = solicitar_angulo_reingreso();
    double angulo = 5.5;

    double trabajo = calcular_trabajo_rozamiento(modulo);
    double desaceleracion = calcular_desaceleracion_media(modulo.velocidad_inicial, modulo.velocidad_final, delta_t);
    double fuerza = calcular_fuerza_promedio_amerizaje(modulo.masa, modulo.velocidad_impacto, hundimiento);
    int ventana_segura = angulo_en_ventana_segura(angulo);

    /* 1) Reporte analitico (preguntas a-d del enunciado), por consola */
    printf("\n===== Reporte analitico (Situacion-Problema 4) =====\n");
    printf("%s\n", TextFormat("Trabajo de rozamiento: %.2f J", trabajo));
    printf("%s\n", TextFormat("Desaceleracion media (Dt=%.0f s): %.2f m/s2 (%.2f g)",
                               delta_t, desaceleracion, desaceleracion / GRAVEDAD_MS2));
    printf("%s\n", TextFormat("Fuerza promedio al amerizar: %.2f N (%.2f pesos)",
                               fuerza, fuerza / (modulo.masa * GRAVEDAD_MS2)));
    printf("%s\n", TextFormat("Angulo de reingreso: %s",
                               ventana_segura ? "dentro de la ventana segura" : "fuera de la ventana segura"));

    /* 2) Recien con todos los datos listos, corremos la simulacion animada en raylib */
    Mision mision;
    mision_iniciar(&mision, angulo);

    Reloj reloj;
    reloj_iniciar(&reloj, ESCALA_TIEMPO);

    InitWindow(VENTANA_ANCHO_PX, VENTANA_ALTO_PX, VENTANA_TITULO);
    SetTargetFPS(VENTANA_FPS);

    int pausado = 0;

    while (!WindowShouldClose()) {
        if (!pausado) {
            reloj_avanzar(&reloj, GetFrameTime());
            while (mision.fase != FASE_TERMINADA && reloj_consumir_paso(&reloj, DT_FISICA_S)) {
                avanzar_simulacion(&mision, DT_FISICA_S, reloj.tiempo_total);
            }
        }

        BeginDrawing();
        ClearBackground(BLACK);
        dibujar_simulacion(&mision);
        if (dibujar_boton_pausa(pausado)) {
            pausado = !pausado;
        }
        EndDrawing();
    }

    CloseWindow();

    /* 3) Reporte final de desempeño, por consola */
    printf("\n===== Reporte final de la simulacion =====\n");
    printf("%s\n", TextFormat("Resultado: %s", mision.fallida ? "fallida" : "exitosa"));
    if (mision.fuerza_impacto_n > 0.0) {
        printf("%s\n", TextFormat("El modulo Orion amerizo. Fuerza del agua: %.2f N (%.2f veces el peso)",
                                   mision.fuerza_impacto_n, mision.fuerza_impacto_pesos));
    }
    printf("%s\n", TextFormat("Registros de historial: %d", mision.cantidad_registros));

    return 0;
}
