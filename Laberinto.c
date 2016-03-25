/*------------------------------------------
|  Universidad Nacional Autonoma de Mexico |
|  Facultad de Ingenieria                  |
|  Algoritmos y Estructura de Datos        |
|  Alumno: De La Rosa Salas Fernando       |
|  Profesor: Ing. Robles Muñoz Fernando    |
|  Proyecto: "Laberinto"                   |
|  Semestre 2016-2                         |
------------------------------------------*/

#include<stdio.h>
#include<stdlib.h>
#include"FuncionesLaberinto.h"

//recibe como parametros: nombre del archivo "argv[1]"
//coordenada x:"argv[2]" y:"argv[3]"
//y una opcion para mostrar el recorrido "argv[4]" 

int main(int argc, char *argv[]){
  //verifica falta de argumentos
  if(argc<4){
    printf("Faltan parametros\n");
    return 0;
  }
  //vereifica si se cargo el archivo
  FILE *lab=NULL;
  lab=fopen(argv[1],"r");
  if(lab==0){
    printf("El archivo no se pudo cargar\n");
    return 0;
  }
  //coorx/y=dimensiones del laberinto
  //x/y=coordenadas de inicio
  int coorx,coory,x=atoi(argv[2]),y=atoi(argv[3]);
  coorx=coor_x(lab);
  coory=coor_y(lab);
  //verifica coordenada de inicio valida
  if(x>coorx || x<0 || y>coory || y<0){
    printf("Coordenadas incorrectas\n");
    return 0;
  }
  //crea tablero
  char **tablero;
  tablero=resmemotabl(coorx,coory);
  if(tablero==NULL){
    return 0;
  }
  tablero=volca(lab,coorx,coory);
  if(tablero[x][y]=='1'){
    printf("Coordenada incorrecta\n");
    return 0;
  }
  //verifica laberinto con salida
  int salida;
  salida=buscaSalida(tablero,coorx,coory);
  if(salida==0){
    printf("Laberinto sin salida\n");
    return 0;
  }
  //newcr=nuevo par ordenado
  int *newcr=NULL,newx=x,newy=y;
  nodo *camino=NULL;
  //se inicia el laberinto
  tablero[x][y]='2';
  //verifica si se quiere mostrar los pasos seguidos
  if(argv[4]!=NULL){
    printf("Posicion inincial: [%d,%d]",x,y);
    imprtabl(tablero,coorx,coory);
    printf("\n");
  }
  camino=ingresa(camino,x,y);
  //se resuelve el laberinto
  while((newx!=(coorx-1) && newy!=(coory-1)) && (newx!=0 && newy!=0)){
    if(camino==NULL){
      camino=ingresa(camino,newx,newy);
    }
    newcr=avanza(tablero,newx,newy,camino);
    newx=newcr[0]; newy=newcr[1];
    camino=ingresa(camino,newx,newy);
    if(argv[4]!=NULL){
    printf("\nCoordenada: [%d,%d]",newx,newy);
    imprtabl(tablero,coorx,coory);
    printf("\n");
    }
  }
  if(argv[4]!=NULL){
    printf("\nFIN\n");
  }
  guardacamino(camino);
  //libera memoria utilizada
  libre(tablero,newcr,camino,coory);
  fclose(lab);
   return 0;
}
