#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include "Rack.cpp"

/***
 * @brief espacio de variables globales
 */ 
using namespace std;
//bandera
bool emulate = true; 
//Variables de meta y cantidad actual de monedas
double goal;
double current = 0.0;
//variable del precio unitario de la moneda "DASHCOIN"
double DashPrice = 0.1000;
//barrera empleada por los racks
pthread_barrier_t barrier;
pthread_mutex_t Racklock = PTHREAD_MUTEX_INITIALIZER;

//funcion encargada de variar el precio de la moneda 
void* DataFluctiation (void* data){
    while(emulate){
        int random = rand()%1000;
        double fluctuation = random/100000.0;
        
        int action = rand()%2;
        
        switch(action){
            case 0: 
            
            DashPrice -= fluctuation;
            if (DashPrice < 0 ){
                DashPrice = fluctuation;
            } 
            break;
            case 1: 
            DashPrice += fluctuation;
            break;
        }
        pthread_mutex_lock(&Racklock);
        cout <<"DashPrice: " << DashPrice << endl;
        pthread_mutex_unlock(&Racklock);
        sleep(2); //esperar un segundo antes de variar el precio
    }
    pthread_exit(0);
}
void* RackWorking (void* args){
    int i = 0;
    bool RackIsOnline = true;
    Rack* rack = (Rack*)args;
    //Hilos que representan cada cpu
    pthread_t CPUS[rack -> cpus];
    //crear los CPUS y generar sus cargas de trabajo
    cout << "El rack " << rack -> id << " Esta en Linea"<< endl;
    while(i < 0){
        CPU CPUSdata[rack -> cpus] = {rack -> model};
        for(int i = 0; i < rack->cpus; i++){
            CPUSdata[i].data.min = rand()%10;
            CPUSdata[i].data.max = rand()%10000; 
            pthread_create(&CPUS[i],0,cpuFunction, (void*)&CPUS[i]); //está como plahe holder actualizar después del commit de GUS
        }
    //esperar los hilos y añadir al subtotal del rack 
        void *exit_value;
        for(int i=0; i < rack->cpus ; i++){
            pthread_join(CPUS[i],&exit_value);
            cout << exit_value << endl;
        }
        // //añadir el subtotal a lo producido
        // cout << "El rack " << rack -> id << "ha generado: " << rack -> CoinsGenerated << endl;
        // current += rack-> CoinsGenerated;
        // cout << "monedas actuales: "<<current << endl;
        // pthread_mutex_unlock(&Racklock);
        // rack -> CoinsGenerated = 0;
        // //esperar a los demás racks antes de realizar la comparacion
        
        // if(current >= goal){ //si ya se llegó o sobrepasó el límite, apagar el rack
        //     RackIsOnline = false;
        //     cout << "El rack " << rack -> id << "se ha apagado"<< endl;
        // }
        i++; 
    }
    return 0;
}

int main(){
    
    //cout<<"Bienvenido al Pseudo-Simulador de Criptominado"<<endl;
    //solicitarle la cantidad de racks
    //cout<<"Cuantas DashCoins desea producir?"<<endl;
   // cin >> goal; 
   // cout<<"Cuantos Racks posee su instalación?"<<endl;
    int racks = 1;
    //cin >> racks;
    //inicializar la barrera para permitir que los Racks se esperen entre sí
 
    //ARRAY DE Racks
    Rack racksincluidos[racks];
    //configurar los CPUS de cada RACK
    // for(int i=0; i<racks; i++){
    //     racksincluidos[i].id = i+1;
    //     cout<<"Cuantos CPUS tiene el Rack "<< i+1 << "?" <<endl;
    //     cin >> racksincluidos[i].cpus;
    //     cout<<"Cuantos hilos poseen esos CPUS?"<<endl;
    //     cin >> racksincluidos[i].model.cores;
    // } 
    racksincluidos[0].id = 1;
    racksincluidos[0].cpus = 1;
    racksincluidos[0].model.cores = 1;
    //crear los hilos de los racks
    pthread_t Racks[racks];
    for(int i = 0; i < racks; i++){
        pthread_create(&Racks[i],NULL,RackWorking,(void *)&racksincluidos[i]);
    }
    //crear el hilo que fluctua los precios y ejecutarlo
    // pthread_t StockValue;
    // pthread_create(&StockValue, NULL, DataFluctiation, 0);
    // pthread_join(StockValue, NULL);
    for(int i = 0; i < racks; i++){
        pthread_join(Racks[i],0);
    }
    

    cout <<"se ha terminado el minado de datos, se ha alcanzado el objetivo de " << goal << endl;
    if(current - goal > 0){
        cout << "Se ha obtenido una ganancia extra de: " << current - goal << endl;
    }
    return 0;
    
}