///Librerias:
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <conio.h>
#include <pthread.h>
#include <sched.h>
#include <unistd.h>
#include <time.h>
#include "gotoxy.h"
#include "damas.h"

///Ficha master blanca = 3
///Ficha master roja = 4


///Prototipos:
void tituloDamas();
void llenarMatrix(int tablero[DIM][DIM]);
void desplazar_seleccionador ();
int desplazamiento(int *x, int *y, int tecla, int flag, int tablero[DIM][DIM]);
void cuadroEstructura();
void cuadroInfo();
void calcularMovimientosPosibles(int tablero[DIM][DIM], int *escape);
int HabilitadoMover(int tablero[DIM][DIM]);
void casilleroToMatrix(int x, int y, int *i, int *j);
void imprimirMovimientosPosibles(int i, int j, int tipo, int tablero[DIM][DIM], int **escape);
void posicion_matrix_cuadrado(int *i, int *j);
void matrixToGotoxy(int i, int j, int *x, int *y);
void moverFicha(int x, int y, int movimiento, int tablero[DIM][DIM], int fichaC, int ***escape);
void EscapeCase(int x, int y, int contador, int movimiento,int tablero[DIM][DIM]);
void borrarFichaMovida(int i, int j, int tablero[DIM][DIM]);
void EnterCase(int x, int y, int tablero[DIM][DIM],int fichaC);
void casilleroToMatrix(int x, int y, int *i, int *j);
void FichaToMatrix(int x, int y, int *i, int *j);
void reloj();
int existeFicha(int ficha,int color);
void * ejecutar(void *flag);

int turno = 1;
pthread_mutex_t llave;

int main() {

    /*HWND consoleWindow = GetConsoleWindow();
    SetWindowPos(consoleWindow, 0,540, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER );*/

    SetConsoleTitle("\"Damas in C\"");
    system("MODE 93,45");
    hidecursor(0);

    ///Llamadas
    ///introDamas();

    pthread_mutex_init(&llave,NULL);

    pthread_t hilo1, hilo2;

    pthread_create(&hilo1,NULL,ejecutar,(void *)0);
    pthread_create(&hilo2,NULL,ejecutar,(void *)1);

    pthread_join(hilo1,NULL);
    pthread_join(hilo2,NULL);

    return EXIT_SUCCESS;
}

void * ejecutar(void *flag) {

    pthread_mutex_lock(&llave);
    int *bandera = (int *)flag;
    pthread_mutex_unlock(&llave);
    pthread_mutex_destroy(&llave);
    int tablero[DIM][DIM] = {{0},{0}};

    //int turno = 2;

    if(bandera==0) {
        reloj();
    }
    indices();
    tablero_damas();
    llenarMatrix(tablero);
    imprimirFichas(tablero);
    cuadroEstructura();
    desplazar_seleccionador(tablero);

    return NULL;
}

void llenarMatrix(int tablero[DIM][DIM]) {
    int i,j;

    for(i=0; i<DIM; i++) {
        for(j=0; j<DIM; j++) {
            if(i<3) {
                if((i%2==0 && j%2==0) || (i%2!=0 && j%2!=0)) {
                    tablero[i][j] = 2;
                }
            } else if(i > 4) {
                if((i%2 == 0 && j%2 == 0) || (i%2 != 0 && j%2 != 0)) {
                    tablero[i][j] = 1;
                }
            }
        }
    }
}

void desplazar_seleccionador(int tablero[DIM][DIM]) {
    int x = 10;
    int y = 1;
    int tecla;
    int flag = 0;

    do {
        Sleep(100);
        cuadroInfo();
        do {
            fflush(stdin);
            tecla = getch();
            gotoxy(x,y);
            casillero_auxiliar(x,y,YELLOW);
        } while(tecla!=ARR && tecla!=ABA && tecla!=DER && tecla!=IZQ && tecla!=ENTER && tecla!= ESCAPE);

        gotoxy(x,y);
        flag = desplazamiento(&x,&y,tecla,flag,tablero);
        gotoxy(x,y);
        casillero(x,y,LIGHTBLUE_DIFFERENT);
    } while(flag==0);
}

int desplazamiento(int *x, int *y, int tecla, int flag, int tablero[DIM][DIM]) {

    int i=0,j=0,escape;

    switch(tecla) {
    case ABA:
        (*y) += 3;
        if((*y)> 22) {
            (*y) = 1;
        }
        break;
    case ARR:
        (*y) -= 3;
        if((*y) < 1) {
            (*y) = 22;
        }
        break;
    case DER:
        (*x) += 9;
        if((*x) > 73) {
            (*x) = 10;
        }
        break;
    case IZQ:
        (*x) -= 9;
        if((*x) < 10) {
            (*x) = 73;
        }
        break;
    case ENTER:
        gotoxy(*x,*y);
        if(HabilitadoMover(tablero) == 0) {
            escape = 0;
            casilleroToMatrix(*x,*y,&i,&j);
            casillero(*x,*y,VIOLET);
            calcularMovimientosPosibles(tablero,&escape);
            if(escape == 0) {
                borrarFichaMovida(i,j,tablero);
            }
        }
        break;
    case ESCAPE:
        exit(0);
        break;
    }
    return flag;
}

void calcularMovimientosPosibles(int tablero[DIM][DIM], int *escape) {
    int i,j, tipo;

    posicion_matrix_cuadrado(&i,&j);
    gotoxy(20,40);

    i-=1;
    j-=1;

    tipo = tablero[i][j];
    imprimirMovimientosPosibles(i,j,tipo, tablero,&escape);

}

void imprimirMovimientosPosibles(int i, int j, int tipo, int tablero[DIM][DIM], int **escape) {
    int x,y,movimiento = 0;

    int a,b;

    matrixToGotoxy(i,j,&x,&y);

    if(tipo == 2) {
        if(j == 0) { ///Limite izquierda
            y += 3;
            x += 9;
            ficha(x,y,GREEN);
            moverFicha(x,y,movimiento, tablero, tipo,&escape);
        } else if(j == 7) { ///Limite derecha
            y += 3;
            x -= 9;
            ficha(x,y,GREEN);
            moverFicha(x,y, movimiento, tablero, tipo,&escape);
        } else if(i == 7) {
            tablero[i][j] = 3;
            fichaMaster(x,y,RED); ///AGREGAR FICHA MASTER
            ///Agregar funcion que tenga los movimientos particulares de una reina
        } else if(j<7 && j>0) { ///No esta al limite
            if(existeFicha(tablero[i+1][j-1],2) == 0 && existeFicha(tablero[i+1][j+1],2) == 1){
                y += 3;
                x += 9;
                ficha(x,y,GREEN);
                moverFicha(x,y,movimiento, tablero, tipo,&escape);
            }else if(existeFicha(tablero[i+1][j-1],2) == 1 && existeFicha(tablero[i+1][j+1],2) == 0){
                y += 3;
                x -= 9;
                ficha(x,y,GREEN);
                moverFicha(x,y, movimiento, tablero, tipo,&escape);
            }else {
                y += 3;
                x += 9;
                FichaToMatrix(x,y,&a,&b);
                if(tablero[a][b] == 0) {
                    ficha(x,y,BLACK);
                } else if(tablero[a][b] == 1) {
                    ficha(x,y,WHITE);
                } else if(tablero[a][b] == 2) {
                    gotoxy(x,y);
                    ficha(x,y,RED);
                }
                    x -= 18;
                    ficha(x,y,GREEN);
                    movimiento = 18;
                    moverFicha(x,y,movimiento, tablero, tipo,&escape);
                }
            }

    } else if(tipo == 1) {
        if(j == 0) { ///Limite izquierda
            y -= 3;
            x += 9;
            ficha(x,y,GREEN);
            moverFicha(x,y,movimiento,tablero, tipo,&escape);
        } else if(j == 7) { ///Limite derecha
            y -= 3;
            x -= 9;
            ficha(x,y,GREEN);
            moverFicha(x,y,movimiento,tablero,tipo,&escape);
        } else if(i == 0) {
            fichaMaster(x,y,WHITE); ///AGREGAR FICHA MASTER
            system("pause>null");
        } else if(j<7 && j>0) { ///No esta al limite
            if(existeFicha(tablero[i-1][j-1],1) == 0 && existeFicha(tablero[i-1][j+1],1) == 1){
                y -= 3;
                x += 9;
                ficha(x,y,GREEN);
                moverFicha(x,y,movimiento,tablero, tipo,&escape);
            }else if(existeFicha(tablero[i-1][j-1],1) == 1 && existeFicha(tablero[i-1][j+1],1) == 0){
                y -= 3;
                x -= 9;
                ficha(x,y,GREEN);
                moverFicha(x,y,movimiento,tablero,tipo,&escape);
            }else {
                y -= 3;
                x -= 9;
                FichaToMatrix(x,y,&a,&b);
                if(tablero[a][b] == 0) {
                    ficha(x,y,BLACK);
                } else if(tablero[a][b] == 1) {
                    ficha(x,y,WHITE);
                } else if(tablero[a][b] == 2) {
                    ficha(x,y,RED);
                }
                x += 18;
                gotoxy(x,y);
                ficha(x,y,GREEN);
                movimiento = -18;
                moverFicha(x,y,movimiento,tablero,tipo,&escape);
                }

        }
    }
}

void moverFicha(int x, int y, int movimiento, int tablero[DIM][DIM], int fichaC, int ***escape) {
    int tecla;
    int flag = 0;
    int contador = 0;
    int a,b;

    do {
        do {
            tecla = getch();
            fflush(stdin);
        } while(tecla!=DER && tecla!= IZQ && tecla!=ENTER && tecla!=ESCAPE);

        if(tecla == DER || tecla == IZQ) {
            tecla = MOV;
        }

        switch (tecla) {

        case ENTER:
            EnterCase(x,y,tablero,fichaC);
            flag = -1;
            break;

        case ESCAPE:
            EscapeCase(x,y,contador,movimiento,tablero);
            flag = -1;
            (***escape) = 1;
            break;

        case MOV:
            if(contador%2 == 0) {
                FichaToMatrix(x,y,&a,&b);
                if(tablero[a][b] == 0) {
                    gotoxy(x,y);
                    ficha(x,y,BLACK);
                } else if(tablero[a][b] == 1) {
                    gotoxy(x,y);
                    ficha(x,y,WHITE);
                } else if(tablero[a][b] == 2) {
                    gotoxy(x,y);
                    ficha(x,y,RED);
                }
                x += movimiento;
                gotoxy(x,y);
                ficha(x,y,GREEN);
            } else {
                FichaToMatrix(x,y,&a,&b);
                if(tablero[a][b] == 0) {
                    gotoxy(x,y);
                    ficha(x,y,BLACK);
                } else if(tablero[a][b] == 1) {
                    gotoxy(x,y);
                    ficha(x,y,WHITE);
                } else if(tablero[a][b] == 2) {
                    gotoxy(x,y);
                    ficha(x,y,RED);
                }
                x -= movimiento;
                gotoxy(x,y);
                ficha(x,y,GREEN);
            }
            contador ++;
            break;
        }
    } while(flag == 0);
}

void EnterCase(int x, int y, int tablero[DIM][DIM],int fichaC) {
    int i=0,j=0;

    FichaToMatrix(x,y,&i,&j);

    if(turno == 1) {
        turno = 2;
    } else {
        turno = 1;
    }
    tablero[i][j] = fichaC;
    imprimirFichas(tablero);
}

void EscapeCase(int x, int y, int contador, int movimiento, int tablero[DIM][DIM]) {
    gotoxy(x,y);
    ficha(x,y,BLACK);
    if(contador%2 == 0) {
        x += movimiento;
        gotoxy(x,y);
        ficha(x,y,BLACK);
    } else {
        x -= movimiento;
        gotoxy(x,y);
        ficha(x,y,BLACK);
    }
    imprimirFichas(tablero);
}

int posicionActual(int tablero[DIM][DIM],int i, int j) {
    int posActual;

    posActual = tablero[i][j];

    return posActual;
}

void borrarFichaMovida(int i, int j, int tablero[DIM][DIM]) {
    int x, y;

    matrixToGotoxy(i,j,&x,&y);
    ficha(x,y,BLACK);
    tablero[i][j] = 0;

}

int HabilitadoMover(int tablero[DIM][DIM]) {
    int i=0,j=0;
    int flag = 1;

    posicion_matrix_cuadrado(&i,&j);
    if(tablero[i][j] == 1 && turno == 1) {
        if(j != 7 && j !=0) {
            if(tablero[i-1][j-1] == 2 && tablero[i-1][j+1] == 2) {
                flag = 0;
            } else if(existeFicha(tablero[i-1][j-1],1) == 1 && existeFicha(tablero[i-1][j+1],1) == 1) {
                flag = 0;
            } else if(existeFicha(tablero[i-1][j-1],1) == 1 && existeFicha(tablero[i-1][j+1],1) == 0) {
                flag = 0;
            } else if(existeFicha(tablero[i-1][j-1],1) == 0 && existeFicha(tablero[i-1][j+1],1) == 1) {
                flag = 0;
            }
        } else {
            if(existeFicha(tablero[i-1][j-1],1) == 1 && existeFicha(tablero[i-1][j+1],1) == 1) {
                flag = 0;
            }
        }
    } else if(tablero[i][j] == 2 && turno == 2) {
        if(j != 7 && j !=0) {
            if(tablero[i+1][j-1] == 1 && tablero[i+1][j+1] == 1) {
                flag = 0;
            } else if(existeFicha(tablero[i+1][j-1],2) == 1 && existeFicha(tablero[i+1][j+1],2) == 1) {
                flag = 0;
            } else if(existeFicha(tablero[i+1][j-1],2) == 1 && existeFicha(tablero[i+1][j+1],2) == 0) {
                flag = 0;
            } else if(existeFicha(tablero[i+1][j-1],2) == 0 && existeFicha(tablero[i+1][j+1],2) == 1) {
                flag = 0;
            }
        } else {
            if(existeFicha(tablero[i+1][j-1],2) == 1 && existeFicha(tablero[i+1][j+1],2) == 1) {
                flag = 0;
            }
        }
    }
    return flag;
}

void posicion_matrix_cuadrado(int *i, int *j) {
    (*i) = whereY();
    (*j) = whereX();

    (*j) -= 10;
    (*i) -= 1;

    if((*j)!=0) {
        (*j) /= 9;
    }
    if((*i)!=0) {
        (*i) /= 3;
    }
}

void cuadroInfo() {
///Texto puntuacion:
    if(turno == 1) {
        color(WHITE);
        gotoxy(8,31);
        printf("Turno Jugador: 1 %c%c"RESET,FICHA,FICHA);
    } else {
        color(RED);
        gotoxy(8,31);
        printf("Turno Jugador: 2 %c%c"RESET,FICHA,FICHA);
    }

    color(YELLOW);
    gotoxy(30,31);
    printf("Tiempo: ");
    gotoxy(72,31);
    printf("PUNTUACION:"RESET);



///Texto turno:
    color(WHITE);
    gotoxy(72,35);
    printf("JUGADOR %c%c: 7",FICHA,FICHA);


    color(RED);
    gotoxy(72,39);
    printf("JUGADOR %c%c: 9",FICHA,FICHA);

///Texto info:


}

void matrixToGotoxy(int i, int j, int *x, int *y) {
    (*x) = 13 + (9 * j);
    (*y) = 2 + (3 * i);
}

void FichaToMatrix(int x, int y, int *i, int *j) {
    (*j) = x - 13;
    if((*j) > 0) {
        (*j) /= 9;
    }

    (*i) = (y - 2);
    if((*i) > 0) {
        (*i) /= 3;
    }
}

void casilleroToMatrix(int x, int y, int *i, int *j) {

    (*j) = x - 10;
    if((*j) > 0) {
        (*j) /= 9;
    }

    (*i) = (y - 1);
    if((*i) > 0) {
        (*i) /= 3;
    }
}

void introDamas() {
    int i;
    int vel = 30;
    int bandera = 1;

    for(i=0; i<40; i++) {
        Sleep(vel);
        if(i % 2 == 0) {
            color(RED);
            gotoxy(46+i,12);
            printf("%c",HORIZ);

            gotoxy(46-i,20);
            printf("%c"RESET,HORIZ);
        } else {
            gotoxy(46+i,12);
            printf("%c",HORIZ);

            gotoxy(46-i,20);
            printf("%c",HORIZ);
        }
    }

    gotoxy(86,12);
    printf("%c",SUP_D);

    gotoxy(6,20);
    printf("%c",INF_I);

    for(i=0; i<7; i++) {
        Sleep(vel);

        if(i % 2 == 0) {
            color(RED);
            gotoxy(86,13+i);
            printf("%c",VERTIC);

            gotoxy(6,19-i);
            printf("%c"RESET,VERTIC);
        } else {
            gotoxy(86,13+i);
            printf("%c",VERTIC);

            gotoxy(6,19-i);
            printf("%c",VERTIC);

        }
    }

    gotoxy(6,12);
    printf("%c",SUP_I);

    gotoxy(86,20);
    printf("%c",INF_D);

    for(i=0; i<40; i++) {
        Sleep(vel);

        if(i % 2 == 0) {
            color(RED);
            gotoxy(85-i,20);
            printf("%c",HORIZ);

            gotoxy(7+i,12);
            printf("%c"RESET,HORIZ);
        } else {
            gotoxy(85-i,20);
            printf("%c",HORIZ);

            gotoxy(7+i,12);
            printf("%c",HORIZ);
        }

    }

    tituloDamas();

    while(bandera == 1) {
        Sleep(600);
        gotoxy(31,30);
        printf("PRESIONE ENTER PARA CONTINUAR...");
        Sleep(600);
        gotoxy(31,30);
        printf("                                       ");

        if(GetAsyncKeyState(0x0D)) {
            bandera = 0;
            system("cls");
        }

    }
}

void tituloDamas() {
    int vel = 100;

    Sleep(vel);
    gotoxy(12,13);
    printf("  _____          __  __           _____     _____ _   _       _____ ");
    Sleep(vel);
    gotoxy(12,14);
    printf(" |  __ \\   /\\   |  \\/  |   /\\    / ____|   |_   _| \\ | |     / ____|");
    Sleep(vel);
    gotoxy(12,15);
    printf(" | |  | | /  \\  | \\  / |  /  \\  | (___       | | |  \\| |    | |     ");
    Sleep(vel);
    gotoxy(12,16);
    printf(" | |  | |/ /\\ \\ | |\\/| | / /\\ \\  \\___ \\      | | | . ` |    | |     ");
    Sleep(vel);
    gotoxy(12,17);
    printf(" | |__| / ____ \\| |  | |/ ____ \\ ____) |    _| |_| |\\  |    | |____ ");
    Sleep(vel);
    gotoxy(12,18);
    printf(" |_____/_/    \\_\\_|  |_/_/    \\_\\_____/    |_____|_| \\_|     \\_____|");

}

void reloj() {

    int hora, minuto, segundo;

    for (hora=0; hora<=24; hora++ ) {
        for (minuto=0; minuto<60; minuto++ ) {
            for (segundo=0; segundo<60; segundo++ ) {
                pthread_mutex_lock(&llave);
                // AGREGAMOS UN INTERVALO DE 1000 MS = 1 SEGUNDO
                Sleep(1000);
                // IMPRIMIR NUESTRO CRONOMETRO
                fflush(stdin);
                color(YELLOW);
                gotoxy(37,31);
                printf(" %.2i : %.2i : %i ", hora, minuto, segundo);
                pthread_mutex_unlock(&llave);
            }
        }
    }
}

int existeFicha(int ficha,int color) {

    int bandera = 3;

    if(color == 1) {

        if(ficha == 2) {
            bandera = 1;
        } else if(ficha == 0) {
            bandera = 1;
        } else if(ficha == 4) {
            bandera = 1;
        } else if (ficha == 3) {
            bandera = 0;
        } else if (ficha == 1) {
            bandera = 0;
        }
    }

    if(color == 2) {

        if(ficha == 1) {
            bandera = 1;
        } else if(ficha == 0) {
            bandera = 1;
        } else if(ficha == 3) {
            bandera = 1;
        } else if (ficha == 4) {
            bandera = 0;
        } else if (ficha == 2) {
            bandera = 0;
        }
    }

    return bandera;

}







































