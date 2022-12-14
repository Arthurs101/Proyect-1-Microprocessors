#include <iostream>
#include <pthread.h>
/**
 * @brief Espacio para variables de la 'clase' de CPU
 * 
 */
using namespace std;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER; // mutex
int totalSum; //total de la suma de primos
/**
 * @brief Estructura que representa los datos necesitados por cada Core
 */
struct DataToWork{
    int min; //minimo y maximo del intervalo a operar
    int max;
};

/**
 * @brief Estructura que contiene los datos necesarios para generar una CPU
 */
struct CPU{
    int id; //TODO revisar que se este usando
    int cores;
    DataToWork data;
};

/**
 * @brief Funcion que  determina si  un numero es primo o no 
 * 
 * @param n numero a determinar si es primo 
 * @return true si el numero es primo 
 */
bool isPrime(int n){
    int i;
    bool is_prime = true;

    // 0 and 1 are not prime numbers
    if (n == 0 || n == 1) {
        is_prime = false;
    }

    // loop to check if n is prime
    for (i = 2; i <= n/2; ++i) {
        if (n % i == 0) {
            is_prime = false;
            break;
        }
    }

    return is_prime;
}
/**
 * @brief La funcion computa los numeros primos en un rango y los suma, la funcion devuelve
 * la suma de estos en una variable
 * 
 * @param args estructura de tipo DataToWork
 * @return void* 
 */
void* coreFunction(void* args){
    DataToWork* data = (DataToWork*) args;
    int begin = data->min;
    int end = data->max;

    int sum = 0;
    for(int i = begin ; i <= end ; i++){
        if (isPrime(i)){
            sum += i;
        }
    }

    pthread_mutex_lock(&lock);
    totalSum += sum;
    pthread_mutex_unlock(&lock);
    pthread_exit(0);
}


/**
 * @brief Funcion que genera los cores 
 * @param args estructura CPU
 * @return void* Suma de todos los numeros primos
 */
void* cpuFunction(void* args){
    CPU* param = (CPU*) args;
    DataToWork dataForCores = param->data;
    int threadNum = param->cores;

    pthread_t threadId;
    pthread_attr_t attr;
    
    
    pthread_attr_init(&attr);

    for(int i = 0; i < threadNum; i++){
        pthread_create(&threadId, &attr, &coreFunction, (void*)&dataForCores);
    }
    for (int i = 0; i < threadNum; i++){
        pthread_join(threadId,0);
    }
    
    return (void *)totalSum;
    pthread_exit(0);
}

  

