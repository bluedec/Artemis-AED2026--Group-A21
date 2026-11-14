#include "raylib.h"
#include "interfaz.h"
#include "constantes.h"

static float altitud_a_pixel(double altitud, double altitud_max) {
    double y = Y_MAR_PX - (altitud / altitud_max) * (Y_MAR_PX - Y_TECHO_PX);
    return (float)y;
}

/* El mar no depende de la mision: siempre es la misma franja azul abajo. */
static void dibujar_mar(void) {
    DrawRectangle(0, Y_MAR_PX, VENTANA_ANCHO_PX, VENTANA_ALTO_PX - Y_MAR_PX, DARKBLUE);
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

static void dibujar_hud(const Mision *m) {
    Color color_g = m->fallida ? RED : GREEN;

    DrawText(TextFormat("Altitud: %.0f m", m->altitud), 20, 20, 20, LIGHTGRAY);
    DrawText(TextFormat("Velocidad: %.1f m/s", m->velocidad), 20, 45, 20, LIGHTGRAY);
    DrawText(TextFormat("Desaceleracion: %.2f g", m->desaceleracion_actual_ms2 / GRAVEDAD_MS2), 20, 70, 20, color_g);

    /* Fuerza de rozamiento atmosferico: solo mientras frena en el reingreso.
     * Mision.fuerza_rozamiento_n se recalcula en cada paso de fisica a
     * partir de la velocidad actual, no es un valor fijo del inicio. */
    if (m->fase == FASE_FRENADO) {
        DrawText(TextFormat("Fuerza de rozamiento: %.0f N", m->fuerza_rozamiento_n), 20, 95, 20, LIGHTGRAY);
        DrawText(TextFormat("Energia cinetica perdida: %.2f GJ", m->energia_cinetica_perdida_j / 1.0e9),
                  20, 120, 20, LIGHTGRAY);
    }
}

/* Mensaje final: cuanta fuerza hizo el agua, o por que fallo la mision */
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

    dibujar_mar();
    dibujar_plasma(m, y_capsula);
    dibujar_paracaidas(m, y_capsula);
    dibujar_capsula(m, y_capsula);
    dibujar_hud(m);
    dibujar_mensaje_final(m);
}
