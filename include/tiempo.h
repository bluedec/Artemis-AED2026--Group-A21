#ifndef TIEMPO_H
#define TIEMPO_H

/* Reloj de paso fijo: acumula tiempo real (escalado) y lo entrega en
 * porciones de tamaño constante, para que la fisica no dependa de los FPS. */
typedef struct {
    double escala;
    double acumulador;
    double tiempo_total;
} Reloj;

void reloj_iniciar(Reloj *r, double escala);
void reloj_avanzar(Reloj *r, double delta_real);
int reloj_consumir_paso(Reloj *r, double paso);

#endif
