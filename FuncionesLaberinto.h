/*------------------------------------------
|  Universidad Nacional Autonoma de Mexico |
|  Facultad de Ingenieria                  |
|  Algoritmos y Estructura de Datos        |
|  Alumno: De La Rosa Salas Fernando       |
|  Profesor: Ing. Robles Muñoz Fernando    |
|  Primer Proyecto "Laberinto"             |
|  Semestre 2016-2                         |
------------------------------------------*/

//-----------------------------------------
// funciones para dimensiones del tablero |
//-----------------------------------------

//cuenta las coordenadas en el eje y
int coor_y(FILE *lb){
  char c;
  int esp=0;
  rewind(lb);
  while(c!='\n'){
    c=fgetc(lb);
    if(c=='1'||c=='0'){
      esp++;
    }
  }
  return esp;
}

//cuenta las coordenadas en el eje x
int coor_x(FILE *lb){
  char c; 
  int sal=0;
  rewind(lb);
  while(!feof(lb)){
    c=fgetc(lb);
    if(c=='\n'){
      sal++;
    }
  }
  return sal;
}

//reserva memoria para una matriz de char
char** resmemotabl(int ren,int col){
  char **a;
  int i;
  a=(char**)malloc(ren*sizeof(char*));
  for(i=0;i<ren;i++){
    a[i]=(char*)malloc(col*sizeof(char));
  }
  if(a==NULL){
    printf("No se pudo cargar la memoria del tablero\n");
    return NULL;
  }
  return a;
}

//volcado de memoria de txt a apuntodor de char
char** volca(FILE *lb,int ren,int col){
  char c,**al=resmemotabl(ren,col);
  int i=0,j=0;
  rewind(lb);
  for(i=0;i<ren;i++){
    for(j=0;j<col;j++){
      c=fgetc(lb);
      if(c!='\n'&&c!=' '){
	al[i][j]=c;
      }
      else{
	j--;
      }
    }
  }
  return al;
}

//imprime una matriz de char
void imprtabl(char **a,int ren,int col){
  int i,j;
  for(i=0;i<ren;i++){
    printf("\n");
    for(j=0;j<col;j++){
      printf("%c ",a[i][j]);
    }
  }
}

//busca posible salida del laberinto
int buscaSalida(char **a,int tamx, int tamy){
  int i,sal=0;
  for(i=0;i<tamy;i++){
    if(a[0][i]=='0'){
      sal++;
    }
  }
  for(i=0;i<tamy;i++){
    if(a[tamx-1][i]=='0'){
      sal++;
    }
  }
  for(i=0;i<tamx;i++){
    if(a[i][0]=='0'){
      sal++;
    }
  }
  for(i=0;i<tamx;i++){
    if(a[i][tamy-1]=='0'){
      sal++;
    }
  }
  return sal;
}

//--------------------------------------------
//funciones para manejo de listas ligadas    |
//--------------------------------------------

typedef struct coordenada{
  int cx,cy;//valores de las coord x,y
  struct nodo *sig;//apuntador al sig nodo
}nodo;

//ingresa al ultimo del camino recorrido
nodo* ingresa(nodo *inicio,int x,int y){
  nodo *actual=inicio;
  if(inicio==NULL){
    actual=malloc(sizeof(nodo));
    actual->cx=x;
    actual->cy=y;
    actual->sig=NULL;
    inicio=actual;
  }
  else{
    while(actual->sig != NULL){
      actual=actual->sig;
    }
    nodo *nuevo=malloc(sizeof(nodo));
    nuevo->cx=x;
    nuevo->cy=y;
    nuevo->sig=NULL;
    actual->sig=nuevo;
  }
  return inicio;
}

//elimina ultima corrdenada ingresada
nodo* remover(nodo *inicio){
  nodo *actual=inicio;
  if(inicio==NULL){
    return 0;
  }
  else{
    nodo *anterior=malloc(sizeof(nodo));
    while(actual->sig!=NULL){
      anterior=actual;
      actual=actual->sig;
    }
    anterior->sig=actual->sig;
    free(actual);
  }
  return inicio;
}
//funcion elimina todo el camino
nodo* eliminarcamino(nodo *inicio){
  nodo *actual=inicio;
  if(inicio==NULL){
    return NULL;
  }
  else{
    nodo *next=inicio->sig;
    while(next!=NULL){
      free(actual);
      actual=next;
      next=actual->sig;
    }
  }
  free(actual);
  return NULL;
}

//--------------------------------------
//funciones que resuleven el laberinto |
//--------------------------------------

//0=camino disponible
//1=pared
//2=camino recorrido
//3=callejon sin salida

//regresa sobre los pasos recorridos, eliminando la ultima coordenada visitada
int* backtracking(char **a,int newx,int newy,nodo *inicio){
  int *newcr=malloc(2);
  inicio=remover(inicio);
  newcr[0]=newx; newcr[1]=newy;
  if(a[newx][newy-1]=='2'){
    newcr[0]=newx; newcr[1]=(newy-1);
    inicio=remover(inicio);
    return newcr;
  }
  if(a[newx][newy+1]=='2'){
    newcr[0]=newx; newcr[1]=(newy+1);
    inicio=remover(inicio);
    return newcr;
  }
  if(a[newx-1][newy]=='2'){
    newcr[0]=(newx-1); newcr[1]=newy;
    inicio=remover(inicio);
    return newcr;
  }
  if(a[newx+1][newy]=='2'){
    newcr[0]=(newx+1); newcr[1]=newy;
    inicio=remover(inicio);
    return newcr;
  }
  return newcr;
}

//resuelve el laberinto bloqueando casilla visitada
int* avanza(char **a,int newx,int newy,nodo *inicio){
  int *newcr=malloc(2);
  newcr[0]=newx; newcr[1]=newy;
  if(a[newx][newy-1]=='0'){
    newcr[0]=newx; newcr[1]=(newy-1);
    a[newx][newy-1]='2';
    return newcr;
  }
  if(a[newx][newy+1]=='0'){
    newcr[0]=newx; newcr[1]=(newy+1);
    a[newx][newy+1]='2';
    return newcr;
  }
  if(a[newx-1][newy]=='0'){
    newcr[0]=(newx-1); newcr[1]=newy;
    a[newx-1][newy]='2';
    return newcr;
  }
  if(a[newx+1][newy]=='0'){
    newcr[0]=(newx+1); newcr[1]=newy;
    a[newx+1][newy]='2';
    return newcr;
  }
  a[newx][newy]='3';
  if(a[newx][newy]=='3'){
    // inicio=remover(inicio);
    newcr=backtracking(a,newx,newy,inicio);
    return newcr;
  }
}

//-----------------------------------
//funciones para menejo de archivos |
//-----------------------------------

//imprime el laberiento desde el archivo txt
void imp_laberinto(FILE *lb){
  char c;
  rewind(lb);
  while(!feof(lb)){
    c=fgetc(lb);
    printf("%c",c);
  }
}

//guarda el camino en un archivo
void guardacamino(nodo *inicio){
  FILE *caminos=NULL;;
  if(inicio==NULL){
    printf("\nNo hay camino");
  }
  else{
    caminos=fopen("caminos.txt","w");
    if(caminos==NULL){
      printf("\nNo se pudo crear el archivo");
    }
    while(inicio!=NULL){
      fprintf(caminos,"%d %d, ",inicio->cx,inicio->cy);
      inicio=inicio->sig;
    }
    fprintf(caminos,"\n");
  }
}

//libera tablero utilizado
void libretab(char **a,int tamy){
  int i;
  for(i=0;i<tamy;i++){
    free(a[i]);
  }
  free(a);
}

//libera la memoria utilizada
void libre(char **a, int *b, nodo *c,int tamy){
  libretab(a,tamy);
  free(b);
  eliminarcamino(c);
}
