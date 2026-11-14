#ifndef INTERFAZ_H
#define INTERFAZ_H

#include "artemis.h"

void dibujar_simulacion(const Mision *m, double tiempo_mision_s);

/* Dibuja el boton de pausa y devuelve 1 si se hizo click en el, 0 si no */
int dibujar_boton_pausa(int pausado);

/* Dibuja el boton de reinicio (solo si la mision termino) y devuelve 1 si se hizo click en el, 0 si no */
int dibujar_boton_reinicio(const Mision *m);

#endif
