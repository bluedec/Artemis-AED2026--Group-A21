#ifndef TIEMPO_H
#define TIEMPO_H

/* Reloj de paso fijo, independiente de los FPS */
typedef struct {
    double escala;
    double acumulador;
    double tiempo_total;
} Reloj;

void reloj_iniciar(Reloj *r, double escala);
void reloj_avanzar(Reloj *r, double delta_real);
int reloj_consumir_paso(Reloj *r, double paso);

#endif
