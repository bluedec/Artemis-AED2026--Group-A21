# TFI Algoritmos y Estructuras de Datos — Simulador de Misiones Artemis

Resumen del enunciado oficial (ver PDF original en
`docs/Trabajo Final Integrador C.docx.pdf`). Este grupo tiene asignado el
**Escenario 4**; se deja el resto de escenarios como referencia breve.

## Modalidad y requisitos técnicos obligatorios

- Lenguaje C, ejecución nativa por consola/terminal (nada de web/GUI).
- Estructuras condicionales y repetitivas.
- Modularización (no todo en un archivo): `main.c` solo con `main()` y menú;
  `.h`/`.c` de lógica con constantes, structs y cálculos; `.h`/`.c` de
  validaciones e interfaz.
- Uso de arreglos y punteros.
- Validación exhaustiva de entradas (tipo y rango) — nunca romper ni ciclar
  infinito ante entrada inválida.
- Menú principal + ciclo de simulación por turnos/eventos + reporte final.
- Repositorio Git/GitHub con historial de commits de todos los integrantes
  (no vale subir todo en un commit final) y README.md con: integrantes,
  descripción del escenario, instrucciones de compilación/ejecución y
  registro de uso de IA.

## Escenario 4 — Reingreso Atmosférico, Frenado Aerodinámico y Amerizaje

Tras el sobrevuelo lunar, el módulo de tripulación Orion (m = 10.400,0 kg)
se separa del módulo de servicio y reingresa a la atmósfera terrestre a
vi = 11,0 km/s (11.000,0 m/s) a una altitud h = 120,0 km (120.000,0 m). El
escudo térmico disipa energía cinética mediante el trabajo de la fuerza de
rozamiento aerodinámico (Froz), reduciendo la rapidez a vf = 25,0 m/s al
desplegarse los paracaídas a h = 2,50 km (2.500,0 m). Finalmente amériza en
el Pacífico a vimpacto = 8,0 m/s.

Preguntas que el programa debe poder responder (cálculo analítico a partir
de los datos, con posibilidad de variar entradas del usuario):

a. Trabajo total `Wroz` de la fuerza de rozamiento entre 120.000 m y 2.500 m.
b. Si la fase de mayor desaceleración dura Δt = 400,0 s, la desaceleración
   media sobre la tripulación en m/s² y en múltiplos de g.
c. Si la cápsula se hunde d = 1,50 m en el agua antes de flotar, la fuerza
   promedio `Fprom` que ejerce el agua, en N y como múltiplo del peso.
d. Consecuencias si el ángulo de reingreso γ queda fuera de la ventana
   segura 5,5° ≤ γ ≤ 7,0° (γ < 5,5° → rebote atmosférico; γ > 7,0° →
   desaceleración excesiva).

### Datos y fórmulas (ver también CLAUDE.md)

- m = 10.400 kg, vi = 11.000 m/s @ 120.000 m, vf = 25 m/s @ 2.500 m,
  v_impacto = 8 m/s, d = 1,5 m, Δt = 400 s, g = 9,8 m/s².
- `W_roz = ½·m·(vf² − vi²) − m·g·(hi − hf)`
- `a_media = (vf − vi) / Δt`  (y en g: `a_media / g`)
- `F_prom = m·(v²/(2d) + g)`  (y en pesos: `F_prom / (m·g)`)
- Ventana segura de reingreso: 5,5° ≤ γ ≤ 7,0°.

## Otros escenarios (no asignados a este grupo, solo contexto)

1. Ascenso y Propulsión con el SLS (MRUV y desprendimiento de los SRB).
2. Inserción y Estabilidad en Órbita Terrestre Baja — LEO (MCU y gravitación).
3. Inyección Translunar — TLI e inercia en trayectoria libre (1.ª Ley de
   Newton y energía).

## Rúbrica (resumen)

Se evalúa: diseño algorítmico y modularización en C, validaciones e
interfaz de consola, fidelidad física/operativa al escenario asignado, uso
de Git/GitHub/README, y defensa técnica individual en coloquio (cualquier
integrante debe poder explicar cualquier línea de código).
