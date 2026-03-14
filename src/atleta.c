#include "atleta.h"

void calcula_pace(Atleta *a) {
    if (a->distanciaKM > 0) {
        // Pace = (Tempo em minutos) / Distância em KM
        a->pace = (a->tempoSegundos / 60.0) / a->distanciaKM;
    } else {
        a->pace = 0;
    }
}
