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

