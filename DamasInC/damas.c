#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include "gotoxy.h"
#include "damas.h"

void indices() {
    int a = 0;
    int b = 8;
    int c = 2;
    int d = 14;

    char letras[9]= {'A','B','C','D','E','F','G','H'};

    color(YELLOW);
///For para insertar los numeros.
    for(a=0; a < 8; a++) {
        gotoxy(3,c);
        printf("%i", b);
        b--;
        c = c+3;
    }
///For para insertar las letras.
    b = 8;
    for(a=0; a < 8; a++) {
        gotoxy(d,27);
        printf("%c", letras[a]);
        b--;
        d = d + 9;
    }
}

void tablero_damas() {
    int i,j;
    int x=19;
    int y=4;

    gotoxy(10,1);
    color(YELLOW);
    printf("%c",SUP_I);

    for(i=0; i<71; i++) {
        printf("%c",HORIZ);
    }

    printf("%c",SUP_D);

    for(i=2; i<25; i++) {
        gotoxy(10,i);
        printf("%c",VERTIC);
    }

    for(i=2; i<25; i++) {
        gotoxy(82,i);
        printf("%c",VERTIC);
    }

    gotoxy(10,25);
    printf("%c",INF_I);
    for(i=0; i<71; i++) {
        printf("%c",HORIZ);
    }

    printf("%c",INF_D);

    for(i=0; i<TABLERO; i++) {
        gotoxy(11,y);
        for(j=0; j<71; j++) {
            printf("%c",HORIZ);
        }
        y += 3;
    }

    for(i=0; i<TABLERO; i++) {
        gotoxy(x,20);
        for(j=2; j<25; j++) {
            gotoxy(x,j);
            printf("%c",VERTIC);
        }
        x += 9;
    }
    esquinas_tablero();
}

void esquinas_tablero() {
    int i,j;

    for(i=4; i<23; i+=3) {
        gotoxy(82,i);
        printf("%c",TRI_IZQ);
        gotoxy(10,i);
        printf("%c",TRI_DER);
    }

    for(i=19; i<82; i+=9) {
        gotoxy(i,1);
        printf("%c",TRI_ABJ);
        gotoxy(i,25);
        printf("%c",TRI_ARR);
    }

    for(i=4; i<25; i+=3) {
        j = 19;
        gotoxy(j,i);
        for(j=19; j<82; j+=9) {
            gotoxy(j,i);
            printf("%c",TRI_ESTRELLA);
        }
    }
}

void imprimirFichas(int tablero[DIM][DIM]) {
    int i,j;
    int x = 0;
    int y = 0;

    for(i=0; i<DIM; i++) {
        for(j=0; j<DIM; j++) {
            if(tablero[i][j] == 2) {
                x = 13 + (9*j);
                y = 2 + (3*i);
                ficha(x,y,RED);
            } else if(tablero[i][j] == 1) {
                x = 13 + (9*j);
                y = 2 + (3*i);
                ficha(x,y,WHITE);
            } else if(tablero[i][j] == 3){
                x = 13 + (9*j);
                y = 2 + (3*i);
                fichaMaster(x,y,WHITE);
            } else if(tablero[i][j] == 4){
                x = 13 + (9*j);
                y = 2 + (3*i);
                fichaMaster(x,y,RED);
            }
        }
    }
}

void ficha(int x, int y, int tono) {
    gotoxy(x,y);
    color(tono);
    printf("%c%c%c%c",FICHA,FICHA,SHADOW,SHADOW);
    gotoxy(x,y+1);
    printf("%c%c%c%c",SHADOW,SHADOW,FICHA,FICHA);
}

void fichaMaster(int x, int y, int tono) {
    gotoxy(x,y);
    color(tono);
    printf("%c%c%c%c",FICHA,FICHA,FICHA,FICHA);
    gotoxy(x,y+1);
    printf("%c%c%c%c",FICHA,FICHA,FICHA,FICHA);
}


void casillero(int x, int y, int tono) {
    int i;

    gotoxy(x,y);
    color(tono);

    printf("%c",SUP_I);

    for(i=0; i<8; i++) {
        printf("%c", HORIZ);
    }

    printf("%c", SUP_D);

    gotoxy(x,y+1);
    printf("%c", VERTIC);
    gotoxy(x,y+2);
    printf("%c", VERTIC);
    gotoxy(x+9,y+1);
    printf("%c", VERTIC);
    gotoxy(x+9,y+2);
    printf("%c", VERTIC);

    gotoxy(x,y+3);
    printf("%c", INF_I);

    for(i=0; i<8; i++) {
        printf("%c", HORIZ);
    }

    printf("%c"RESET, INF_D);
}

void casillero_auxiliar(int x, int y, int tono) {
    int i;

    gotoxy(x+1,y);
    color(tono);

    for(i=0; i<8; i++) {
        printf("%c", HORIZ);
    }

    gotoxy(x,y+1);
    printf("%c", VERTIC);
    gotoxy(x,y+2);
    printf("%c", VERTIC);
    gotoxy(x+9,y+1);
    printf("%c", VERTIC);
    gotoxy(x+9,y+2);
    printf("%c", VERTIC);

    gotoxy(x+1,y+3);

    for(i=0; i<8; i++) {
        printf("%c", HORIZ);
    }
    modificar_esquinas(x,y);
}

void modificar_esquinas(int x, int y) {
    int flag = 0;

    if(x == 10 && y == 1 && flag == 0) {
        gotoxy(x,y);
        printf("%c",SUP_I);
        gotoxy(x+9,y);
        printf("%c",TRI_ABJ);
        gotoxy(x,y+3);
        printf("%c",TRI_DER);
        gotoxy(x+9,y+3);
        printf("%c",TRI_ESTRELLA);
        flag = 1;
    }
    if(x == 10 && y == 22 && flag == 0) {
        gotoxy(x,y);
        printf("%c",TRI_DER);
        gotoxy(x+9,y);
        printf("%c",TRI_ESTRELLA);
        gotoxy(x,y+3);
        printf("%c",INF_I);
        gotoxy(x+9,y+3);
        printf("%c",TRI_ARR);
        flag = 1;
    }
    if(x == 73 && y == 1 && flag == 0) {
        gotoxy(x,y);
        printf("%c",TRI_ABJ);
        gotoxy(x+9,y);
        printf("%c",SUP_D);
        gotoxy(x,y+3);
        printf("%c",TRI_ESTRELLA);
        gotoxy(x+9,y+3);
        printf("%c",TRI_IZQ);
        flag = 1;
    }
    if(x == 73 && y == 22 && flag == 0) {
        gotoxy(x,y);
        printf("%c",TRI_ESTRELLA);
        gotoxy(x+9,y);
        printf("%c",TRI_IZQ);
        gotoxy(x,y+3);
        printf("%c",TRI_ARR);
        gotoxy(x+9,y+3);
        printf("%c",INF_D);
        flag = 1;
    }
    if(x > 10 && x <73 && y == 1 && flag == 0) {
        gotoxy(x,y);
        printf("%c",TRI_ABJ);
        gotoxy(x+9,y);
        printf("%c",TRI_ABJ);
        gotoxy(x,y+3);
        printf("%c",TRI_ESTRELLA);
        gotoxy(x+9,y+3);
        printf("%c",TRI_ESTRELLA);
        flag = 1;
    }
    if(x > 10 && x < 73 && y == 22 && flag == 0) {
        gotoxy(x,y);
        printf("%c",TRI_ESTRELLA);
        gotoxy(x+9,y);
        printf("%c",TRI_ESTRELLA);
        gotoxy(x,y+3);
        printf("%c",TRI_ARR);
        gotoxy(x+9,y+3);
        printf("%c",TRI_ARR);
        flag = 1;
    }
    if(x == 10 && y > 1 && y < 22 && flag == 0) {
        gotoxy(x,y);
        printf("%c",TRI_DER);
        gotoxy(x+9,y);
        printf("%c",TRI_ESTRELLA);
        gotoxy(x,y+3);
        printf("%c",TRI_DER);
        gotoxy(x+9,y+3);
        printf("%c",TRI_ESTRELLA);
        flag = 1;
    }
    if(x == 73 && y > 1 && y < 22 && flag == 0) {
        gotoxy(x,y);
        printf("%c",TRI_ESTRELLA);
        gotoxy(x+9,y);
        printf("%c",TRI_IZQ);
        gotoxy(x,y+3);
        printf("%c",TRI_ESTRELLA);
        gotoxy(x+9,y+3);
        printf("%c",TRI_IZQ);
        flag = 1;
    }
    if(x > 10 && x < 73 && y > 1 && y < 22 && flag == 0) {
        gotoxy(x,y);
        printf("%c",TRI_ESTRELLA);
        gotoxy(x+9,y);
        printf("%c",TRI_ESTRELLA);
        gotoxy(x,y+3);
        printf("%c",TRI_ESTRELLA);
        gotoxy(x+9,y+3);
        printf("%c",TRI_ESTRELLA);
        flag = 1;
    }
}

void cuadroEstructura() {
    int i;
    int x = 6;
    int y = 30;

    color(YELLOW);

    gotoxy(x,y);
    printf("%c",SUP_I);

    for(i=0; i<80; i++) {
        printf("%c", HORIZ);
    }

    printf("%c", SUP_D);


    for(i=1; i<13; i++) {
        gotoxy(x,y+i);
        printf("%c", VERTIC);
        gotoxy(x+81,y+i);
        printf("%c", VERTIC);
        gotoxy(x+63,y+i);
        printf("%c", VERTIC);
    }


    gotoxy(x,y+i);
    printf("%c", INF_I);

    for(i=0; i<80; i++) {
        printf("%c", HORIZ);
    }

    printf("%c", INF_D);

    gotoxy(x,32);
    printf("%c",TRI_DER);
    for(i=0; i<62; i++) {
        printf("%c", HORIZ);
    }

    printf("%c",TRI_IZQ);

    gotoxy(69,43);
    printf("%c",TRI_ARR);

    gotoxy(69,30);
    printf("%c",TRI_ABJ);

    gotoxy(28,30);
    printf("%c",TRI_ABJ);

    gotoxy(28,31);
    printf("%c",VERTIC);

    gotoxy(28,32);
    printf("%c",TRI_ARR);


///Nose si dejarlo o ponerlo:
    /*gotoxy(64,30);
    printf("%c",TRI_ABJ);

    gotoxy(64,31);
    printf("%c",VERTIC);

    gotoxy(64,32);
    printf("%c",TRI_ARR);*/

}
