#ifndef INTERFAZ_H
#define INTERFAZ_H

#include "artemis.h"

void dibujar_simulacion(const Mision *m);

/* Dibuja el boton de pausa y devuelve 1 si se hizo click en el, 0 si no */
int dibujar_boton_pausa(int pausado);

#endif
