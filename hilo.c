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
	int *arreglos;
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
void generarNumerosArreglos(int *arreglos,int num_arreglos){
	int i=0;
	for(i=0;i<num_arreglos;i++){
		arreglos[i] = aleatorio(0,255);
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
	if(argc==4){
		double ti;
		int numeroelementos=atoi(argv[1]);
		int numerohilosini=atoi(argv[2]);
		int numerohilosfin=atoi(argv[3]);
		int tamanobloque;
		int residuo=0;
		void * valor_retorno;
		srand(time(0));
		long suma=0;

		ti=obtenerTiempoActual();	
	
		int *arreglos=malloc(numeroelementos*sizeof(int));
		memset(arreglos,0,numeroelementos*sizeof(int));
		generarNumerosArreglos(arreglos,numeroelementos);
		 
		double tf=obtenerTiempoActual()-ti;
		printf("tiempo en generar arreglo : %f \n", tf);
		
		
		
		for(int cant=numerohilosini; cant<=numerohilosfin; cant++){
			ti=obtenerTiempoActual();
			int numerohilos=cant;
			pthread_t *hilos =malloc(numerohilos*sizeof(pthread_t));
			int status, hilo,inicio=0;
			//validar tamano de bloque
			if(numeroelementos%numerohilos!=0){
				tamanobloque=numeroelementos/numerohilos;
				residuo=numeroelementos%numerohilos;
			}
			tamanobloque=numeroelementos/numerohilos;
			for(hilo=0;hilo<numerohilos;hilo++) {
				estructura *mi_argumento_estructura = malloc(sizeof(estructura));
				mi_argumento_estructura->inicio  = inicio ;
				if(hilo==(numerohilos-1)){
					mi_argumento_estructura->cant  = tamanobloque +residuo;
				}else{
					mi_argumento_estructura->cant  = tamanobloque ;
				}
				mi_argumento_estructura->arreglos = arreglos;
				
				status = pthread_create(&hilos[hilo], NULL,sumas, (void *)mi_argumento_estructura);
				if(status < 0){
					fprintf(stderr, "Error al crear el hilo : %d\n", hilo);
					exit(-1);	
				}
				inicio+=tamanobloque;	
			}

			//esperando la finalizacion de los hilos
			for(hilo=0;hilo<numerohilos;hilo++) {
				valor_retorno=0;
				int status1 = pthread_join(hilos[hilo], &valor_retorno);
				if(status1 < 0){
					fprintf(stderr, "Error al esperar por el hilo 1\n");
					exit(-1);
				}
				//printf("Suma parcial hilo %d: %ld\n", hilo, (long)valor_retorno);
				suma=suma+(long)valor_retorno;
			}	
			printf("Suma total : %ld con %d hilos\n", suma, cant);
			tf=obtenerTiempoActual()-ti;
			printf("tiempo de ejecucion: %f con %d hilos \n", tf,cant);
			suma=tf=ti=0;
		}
	}else if(argc==3){
		double ti=obtenerTiempoActual();
		int numeroelementos=atoi(argv[1]);
		int numerohilos=atoi(argv[2]);
		int tamanobloque;
		int residuo=0;
		void * valor_retorno;
		srand(time(0));
		int *arreglos=malloc(numeroelementos*sizeof(int));
		memset(arreglos,0,numeroelementos*sizeof(int));
		long suma=0; 

		pthread_t *hilos =malloc(numerohilos*sizeof(pthread_t));
		int status, hilo,inicio=0;
		generarNumerosArreglos(arreglos,numeroelementos);

		//validar tamano de bloque
		if(numeroelementos%numerohilos!=0){
			tamanobloque=numeroelementos/numerohilos;
			residuo=numeroelementos%numerohilos;
		}
		tamanobloque=numeroelementos/numerohilos;
		for(hilo=0;hilo<numerohilos;hilo++) {
			estructura *mi_argumento_estructura = malloc(sizeof(estructura));
			mi_argumento_estructura->inicio  = inicio ;
			if(hilo==(numerohilos-1)){
				mi_argumento_estructura->cant  = tamanobloque +residuo;
			}else{
				mi_argumento_estructura->cant  = tamanobloque ;
			}
			mi_argumento_estructura->arreglos = arreglos;
			
			status = pthread_create(&hilos[hilo], NULL,sumas, (void *)mi_argumento_estructura);
			if(status < 0){
				fprintf(stderr, "Error al crear el hilo : %d\n", hilo);
				exit(-1);	
			}
			inicio+=tamanobloque;	
		}

		//esperando la finalizacion de los hilos
		printf("Hilo principal esta esperando a que terminen los otros hilos\n");
		for(hilo=0;hilo<numerohilos;hilo++) {
			valor_retorno=0;
			int status1 = pthread_join(hilos[hilo], &valor_retorno);
			if(status1 < 0){
				fprintf(stderr, "Error al esperar por el hilo 1\n");
				exit(-1);
			}
			printf("Suma parcial hilo %d: %ld\n", hilo, (long)valor_retorno);
			suma=suma+(long)valor_retorno;
		}	
		printf("Suma total: %ld\n", suma);
		double tf=obtenerTiempoActual()-ti;
		printf("tiempo de ejecucion: %f \n", tf);
	}else{
		printf("por favor ingrese correctamente los parametros: \n 1. numero de elementos \n 2. numeros de hilos");
	}
		pthread_exit(NULL);
}

