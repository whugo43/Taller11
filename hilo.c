#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>

//estructura con la informacion necesario a enviar a cada hilo creado
typedef struct mi_estructuraTDA{
	int inicio;
	int cant;
	long *arreglos;
} estructura;

//genera numeros enteros aleatorios entre max y min 
int aleatorio (int min, int max){
	return (rand() % (max - min +1)) + min;
}


double obtenerTiempoActual (){
	struct timespec tsp;
	clock_gettime (CLOCK_REALTIME, &tsp);
	double secs= (double)tsp.tv_sec;
	double nano= (double)tsp.tv_nsec / 1000000000.0;
	return secs +nano;
}

//se llena el array de un conjunto de valores aleatorios 
void generarNumerosArreglos(long *arreglos,int num_arreglos){
	int i=0;
	for(i=0;i<num_arreglos;i++){
		arreglos[i] = aleatorio(1,200);
	}
}

//suma un bloque especifico del arreglo
void * sumas(void *arg){
	estructura *argumentos = (estructura *)arg;		//convertimos al tipo de dato correcto
	//printf(" valores : inicio %d cant %d \n",argumentos->inicio,argumentos->cant );
	int i = 0;
	long suma=0;
	int max=argumentos->cant+argumentos->inicio;
	for(i = argumentos->inicio; i < max; i++){
		suma=suma+(argumentos->arreglos)[i];
		//printf(" %d %ld %ld\n",i, (argumentos->arreglos)[i],suma );
	}
	return (void *)suma;
}


int main(int argc, char *argv[]){
	if(argc==3){
		double ti=obtenerTiempoActual();
		int numeroelementos=atoi(argv[1]);
		int numerohilos=atoi(argv[2]);
		int tamanobloque;
		int residuo=0;
		void * valor_retorno;
		srand(time(0));
		long *arreglos=malloc(numeroelementos*sizeof(long));
		memset(arreglos,0,numeroelementos*sizeof(long));
		long suma=0; 

		pthread_t *hilos =malloc(numerohilos*sizeof(pthread_t));
		int status, hilo,inicio=0;
		generarNumerosArreglos(arreglos,numeroelementos);

		//validar tamano de bloque
		if(numeroelementos%numerohilos!=0){
			tamanobloque=numeroelementos/numerohilos;
			residuo=numeroelementos%numerohilos;
		}

	}else{
		printf("por favor ingrese correctamente los parametros: \n 1. numero de elementos \n 2. numeros de hilos");
	}
		pthread_exit(NULL);
}

