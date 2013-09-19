#include <stdio.h>

#include "tortuga.h"

int main ( int argc, char** argv )
{
    int i,x,y;
    inicializar();

    posicion_inicial(20,15);
    velocidad_tortuga(VELOCIDAD_LENTA);
    for( i = 0 ; i < 4 ; i++) {
        punta_lapiz(i, 2 + 2 * i);
        avanzar(4);
        girar(90);
    }

    velocidad_tortuga(VELOCIDAD_MEDIA);
    punta_lapiz(GRAY,5);
    posicion_inicial(20,15);
    angulo_inicial(90);
    for( i = 0 ; i < 6 ; i++) {
        avanzar(3);
        girar(360/6);
    }

    velocidad_tortuga(VELOCIDAD_RAPIDA);
    punta_lapiz(GREEN,30);
    posicion_inicial(20,15);
    angulo_inicial(180);
    for( i = 0 ; i < 36 ; i++) {
        avanzar(1);
        girar(360/36);
    }

    velocidad_tortuga(VELOCIDAD_MAXIMA);

    angulo_inicial(0);
    punta_lapiz(WHITE, 2);
    for( y = 2 ; y < 10 ; y += 1) {
        for( x = 2 ; x < 10 ; x+= 2) {
            if((x/2+y) % 2 == 0) estado_lapiz(PEN_DOWN);
            else estado_lapiz(PEN_UP);
            posicion_inicial(x,y);
            avanzar(1);
        }
    }
    finalizar();

    // all is well ;)
    printf("Exited cleanly\n");
    return 0;
}
