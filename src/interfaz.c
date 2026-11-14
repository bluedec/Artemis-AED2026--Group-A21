#include <stdlib.h>
#include "raylib.h"
#include "interfaz.h"
#include "constantes.h"

#define CANTIDAD_ESTRELLAS 80

typedef struct {
    int x;
    int y;
    int radio;
} Estrella;

static Estrella estrellas[CANTIDAD_ESTRELLAS];
static int estrellas_generadas = 0;

static float altitud_a_pixel(double altitud, double altitud_max) {
    double y = Y_MAR_PX - (altitud / altitud_max) * (Y_MAR_PX - Y_TECHO_PX);
    return (float)y;
}

/* 1.0 en el espacio (altitud_inicial o mas), 0.0 al llegar a los 2.5 km de apertura del paracaidas */
static float factor_espacio(double altitud) {
    if (altitud > ALTITUD_INICIAL_M) {
        altitud = ALTITUD_INICIAL_M;
    }
    if (altitud < ALTITUD_FINAL_M) {
        altitud = ALTITUD_FINAL_M;
    }
    return (float)((altitud - ALTITUD_FINAL_M) / (ALTITUD_INICIAL_M - ALTITUD_FINAL_M));
}

static void generar_estrellas(void) {
    int i;

    if (estrellas_generadas) {
        return;
    }

    srand(20261114);
    for (i = 0; i < CANTIDAD_ESTRELLAS; i++) {
        estrellas[i].x = rand() % VENTANA_ANCHO_PX;
        estrellas[i].y = rand() % Y_MAR_PX;
        estrellas[i].radio = 1 + rand() % 2;
    }
    estrellas_generadas = 1;
}

static void dibujar_estrellas(const Mision *m) {
    float opacidad = factor_espacio(m->altitud);
    int i;

    if (opacidad <= 0.0f) {
        return;
    }

    generar_estrellas();
    for (i = 0; i < CANTIDAD_ESTRELLAS; i++) {
        DrawCircle(estrellas[i].x, estrellas[i].y, (float)estrellas[i].radio, Fade(WHITE, opacidad));
    }
}

static void dibujar_mar(const Mision *m) {
    if (m->fase == FASE_FRENADO) {
        return;
    }
    DrawRectangle(0, Y_MAR_PX, VENTANA_ANCHO_PX, VENTANA_ALTO_PX - Y_MAR_PX, DARKBLUE);
}

static void dibujar_indicador_fase2(const Mision *m) {
    float y_linea;

    if (m->fase != FASE_FRENADO) {
        return;
    }

    y_linea = altitud_a_pixel(ALTITUD_FINAL_M, m->altitud_max_fase);
    DrawLine(0, (int)y_linea, VENTANA_ANCHO_PX, (int)y_linea, SKYBLUE);
    DrawText(TextFormat("%.1f km - apertura de paracaidas", ALTITUD_FINAL_M / 1000.0),
              20, (int)y_linea - 22, 18, SKYBLUE);
}

static Color color_cielo(const Mision *m) {
    const Color espacio = BLACK;
    const Color atmosfera_alta = (Color){20, 20, 45, 255};
    const Color atmosfera_baja = (Color){60, 90, 140, 255};

    if (m->altitud >= ALTITUD_FINAL_M) {
        return ColorLerp(atmosfera_alta, espacio, factor_espacio(m->altitud));
    }

    float factor = (float)(m->altitud / ALTITUD_FINAL_M);
    return ColorLerp(atmosfera_baja, atmosfera_alta, factor);
}

static void dibujar_capsula(const Mision *m, float y_capsula) {
    Color color = (m->fase == FASE_TERMINADA && m->fallida) ? MAROON : GRAY;
    DrawRectangle(X_CAPSULA_PX - CAPSULA_ANCHO_PX / 2, (int)y_capsula - CAPSULA_ALTO_PX / 2,
                  CAPSULA_ANCHO_PX, CAPSULA_ALTO_PX, color);
}

static void dibujar_plasma(const Mision *m, float y_capsula) {
    double factor;

    if (m->fase != FASE_FRENADO) {
        return;
    }

    factor = m->velocidad / VELOCIDAD_INICIAL_MS;
    if (factor < 0.0) {
        factor = 0.0;
    }
    if (factor > 1.0) {
        factor = 1.0;
    }

    DrawCircle(X_CAPSULA_PX, (int)y_capsula + CAPSULA_ALTO_PX / 2,
               (float)(PLASMA_RADIO_MAX_PX * factor), Fade(ORANGE, (float)factor));
}

static void dibujar_paracaidas(const Mision *m, float y_capsula) {
    int y_tela;

    if (m->fase != FASE_PARACAIDAS) {
        return;
    }

    y_tela = (int)y_capsula - CAPSULA_ALTO_PX / 2 - PARACAIDAS_ALTURA_PX;
    DrawCircle(X_CAPSULA_PX, y_tela, PARACAIDAS_RADIO_PX, WHITE);
    DrawLine(X_CAPSULA_PX - CAPSULA_ANCHO_PX / 2, (int)y_capsula - CAPSULA_ALTO_PX / 2,
             X_CAPSULA_PX, y_tela, WHITE);
    DrawLine(X_CAPSULA_PX + CAPSULA_ANCHO_PX / 2, (int)y_capsula - CAPSULA_ALTO_PX / 2,
             X_CAPSULA_PX, y_tela, WHITE);
}

static const char *nombre_fase(FaseSimulacion fase) {
    switch (fase) {
        case FASE_FRENADO: return "Frenado atmosferico";
        case FASE_PARACAIDAS: return "Descenso con paracaidas";
        case FASE_REBOTE: return "Rebote atmosferico";
        case FASE_TERMINADA: return "Mision terminada";
    }
    return "";
}

static void dibujar_titulo_fase(const Mision *m) {
    const char *texto = TextFormat("Fase: %s", nombre_fase(m->fase));
    int ancho = MeasureText(texto, 28);
    DrawText(texto, (VENTANA_ANCHO_PX - ancho) / 2, 15, 28, GOLD);
}

static void dibujar_hud(const Mision *m) {
    Color color_g = m->fallida ? RED : GREEN;

    DrawText(TextFormat("Altitud: %.0f m", m->altitud), 20, 20, 20, LIGHTGRAY);
    DrawText(TextFormat("Velocidad: %.1f m/s", m->velocidad), 20, 45, 20, LIGHTGRAY);
    DrawText(TextFormat("Desaceleracion: %.2f g", m->desaceleracion_actual_ms2 / GRAVEDAD_MS2), 20, 70, 20, color_g);

    if (m->fase == FASE_FRENADO) {
        DrawText(TextFormat("Fuerza de rozamiento: %.0f N", m->fuerza_rozamiento_n), 20, 95, 20, LIGHTGRAY);
        DrawText(TextFormat("Energia cinetica perdida: %.2f GJ", m->energia_cinetica_perdida_j / 1.0e9),
                  20, 120, 20, LIGHTGRAY);
    }
}

static void dibujar_mensaje_final(const Mision *m) {
    const char *titulo;
    Color color_titulo;
    int ancho_titulo;

    if (m->fase != FASE_TERMINADA) {
        return;
    }

    if (m->fuerza_impacto_n > 0.0) {
        const char *detalle = TextFormat("Fuerza del agua sobre el modulo Orion: %.0f N (%.2f veces el peso)",
                                          m->fuerza_impacto_n, m->fuerza_impacto_pesos);
        int ancho_detalle = MeasureText(detalle, 20);

        titulo = "El modulo Orion amerizo";
        color_titulo = m->fallida ? ORANGE : GREEN;
        ancho_titulo = MeasureText(titulo, 32);
        DrawText(titulo, (VENTANA_ANCHO_PX - ancho_titulo) / 2, MENSAJE_FINAL_Y_PX, 32, color_titulo);
        DrawText(detalle, (VENTANA_ANCHO_PX - ancho_detalle) / 2, MENSAJE_FINAL_Y_PX + 40, 20, LIGHTGRAY);
    } else {
        titulo = "Mision fallida: rebote atmosferico";
        color_titulo = RED;
        ancho_titulo = MeasureText(titulo, 32);
        DrawText(titulo, (VENTANA_ANCHO_PX - ancho_titulo) / 2, MENSAJE_FINAL_Y_PX, 32, color_titulo);
    }
}

int dibujar_boton_pausa(int pausado) {
    Rectangle boton = { BOTON_PAUSA_X_PX, BOTON_PAUSA_Y_PX, BOTON_PAUSA_ANCHO_PX, BOTON_PAUSA_ALTO_PX };
    int mouse_encima = CheckCollisionPointRec(GetMousePosition(), boton);
    const char *texto = pausado ? "Reanudar" : "Pausa";
    int ancho_texto = MeasureText(texto, 20);

    DrawRectangleRec(boton, mouse_encima ? LIGHTGRAY : GRAY);
    DrawRectangleLinesEx(boton, 2, WHITE);
    DrawText(texto, (int)boton.x + (BOTON_PAUSA_ANCHO_PX - ancho_texto) / 2,
              (int)boton.y + (BOTON_PAUSA_ALTO_PX - 20) / 2, 20, BLACK);

    return mouse_encima && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

void dibujar_simulacion(const Mision *m) {
    float y_capsula = altitud_a_pixel(m->altitud, m->altitud_max_fase);

    ClearBackground(color_cielo(m));
    dibujar_estrellas(m);
    dibujar_mar(m);
    dibujar_indicador_fase2(m);
    dibujar_plasma(m, y_capsula);
    dibujar_paracaidas(m, y_capsula);
    dibujar_capsula(m, y_capsula);
    dibujar_titulo_fase(m);
    dibujar_hud(m);
    dibujar_mensaje_final(m);
}
