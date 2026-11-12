#include <stdio.h>
#include "raylib.h"
#include "constantes.h"
#include "artemis.h"
#include "simulador.h"

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

    double angulo = solicitar_angulo_reingreso();

    double trabajo = calcular_trabajo_rozamiento(modulo);
    double desaceleracion = calcular_desaceleracion_media(modulo.velocidad_inicial, modulo.velocidad_final, delta_t);
    double fuerza = calcular_fuerza_promedio_amerizaje(modulo.masa, modulo.velocidad_impacto, hundimiento);
    int ventana_segura = angulo_en_ventana_segura(angulo);

    char texto_trabajo[128];
    char texto_desaceleracion[128];
    char texto_fuerza[128];
    char texto_angulo[128];

    snprintf(texto_trabajo, sizeof(texto_trabajo), "Trabajo de rozamiento: %.2f J", trabajo);
    snprintf(texto_desaceleracion, sizeof(texto_desaceleracion),
              "Desaceleracion media: %.2f m/s2 (%.2f g)", desaceleracion, desaceleracion / GRAVEDAD_MS2);
    snprintf(texto_fuerza, sizeof(texto_fuerza), "Fuerza promedio al amerizar: %.2f N", fuerza);
    snprintf(texto_angulo, sizeof(texto_angulo), "Angulo de reingreso: %s",
              ventana_segura ? "dentro de la ventana segura" : "fuera de la ventana segura");

    /* 2) Recien con todos los datos listos, corremos la simulacion en raylib */
    InitWindow(VENTANA_ANCHO_PX, VENTANA_ALTO_PX, VENTANA_TITULO);
    SetTargetFPS(VENTANA_FPS);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        DrawText("Mision Artemis - Resultados del reingreso", 190, 100, 24, LIGHTGRAY);
        DrawText(texto_trabajo, 190, 160, 20, LIGHTGRAY);
        DrawText(texto_desaceleracion, 190, 200, 20, LIGHTGRAY);
        DrawText(texto_fuerza, 190, 240, 20, LIGHTGRAY);
        DrawText(texto_angulo, 190, 280, 20, LIGHTGRAY);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
