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
double DashPrice = 0.0010;
//barrera empleada por los racks
pthread_barrier_t barrier;
pthread_mutex_t Racklock = PTHREAD_MUTEX_INITIALIZER;

//funcion encargada de variar el precio de la moneda 
void* DataFluctiation (void* data){
    while(emulate){
        int random = rand()%1000;
        double fluctuation = random/10000.0;
        
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
        cout<<"-----------------------------------------------------"<<endl;
        cout <<"System: !!New DashPrice: !! : " << DashPrice << endl;
        cout<<"-----------------------------------------------------"<<endl;
        pthread_mutex_unlock(&Racklock);
        sleep(1);
    }
    pthread_exit(0);
}
void* RackWorking (void* args){
    bool RackIsOnline = true;
    Rack* rack = (Rack*)args;
    double coinsGenerated;
    //Hilos que representan cada cpu
    pthread_t CPUS[rack -> cpus];
    CPU CPUSdata[rack -> cpus]  ;
    //crear los CPUS y generar sus cargas de trabajo
    pthread_mutex_lock(&Racklock);
    cout << "!!System: Rack: " << rack -> id << "is now Online"<< endl;
    pthread_mutex_unlock(&Racklock);
    while(RackIsOnline){
        sleep(1);
        for(int i = 0; i < rack->cpus; i++){
            CPUSdata[i].data.min = 1;
            CPUSdata[i].data.max = rand()%1001; 
            CPUSdata[i].cores = rack ->model.cores;
            while( CPUSdata[i].data.max == 0){ //evitar que sea 0
                CPUSdata[i].data.max = rand()%1001; 
            }
            pthread_create(&CPUS[i],0,cpuFunction,(void*)&CPUSdata[i]); //está como plahe holder actualizar después del commit de GUS
        }
    //esperar los hilos y añadir al subtotal del rack 
        void *exit_value;
        
        for(int i=0; i < rack->cpus ; i++){
            pthread_join(CPUS[i],&exit_value);
            //this is just for testing
            int* temp = ((int*)exit_value);
            coinsGenerated += ((int)temp+0.0)*DashPrice;

        }
        pthread_mutex_lock(&Racklock);
        cout<<"- - - - - - - - - - - - - - - - - - - - - - - - - - -"<<endl;
        current += coinsGenerated;
        cout<< "System: Rack No. "<<rack -> id << " Generated:  " << coinsGenerated << endl;
        cout<< "New Account Balance!!: " << current << "'- Coins'" << endl;
        cout<<"- - - - - - - - - - - - - - - - - - - - - - - - - - -"<<endl;
        pthread_mutex_unlock(&Racklock);
        coinsGenerated = 0;
        pthread_barrier_wait(&barrier);
        if(current >= goal){
            RackIsOnline = false;
        }
    }
    pthread_mutex_lock(&Racklock);
    cout << "!!System: Rack: " << rack -> id << " is now Offline"<< endl;
    pthread_mutex_unlock(&Racklock);
    return 0;
}

int main(){

    int racks;
    cout<<"Bienvenido al Pseudo-Simulador de Criptominado"<<endl;
    //solicitarle la cantidad de racks
    cout<<"Cuantas DashCoins desea producir?"<<endl;
    cin >> goal; 
    cout<<"Cuantos Racks posee su instalacion?"<<endl;
    cin >> racks;
 
    //ARRAY DE Racks
    Rack racksincluidos[racks];
    //configurar los CPUS de cada RACK
    for(int i=0; i<racks; i++){
        racksincluidos[i].id = i+1;
        cout<<"Cuantos CPUS tiene el Rack "<< i+1 << "?" <<endl;
        cin >> racksincluidos[i].cpus;
        cout<<"Cuantos hilos poseen esos CPUS?"<<endl;
        cin >> racksincluidos[i].model.cores;
    } 
    //testing de las estructura (para mostrarlo)
    cout<<"Racks a usar"<<endl;
    for(int i=0; i<racks; i++){
        cout<< "Rack id:" << racksincluidos[i].id << endl;
        cout<<"Cpus: " << racksincluidos[i].cpus << endl;
        cout<<"Hilos de los cpus: "<< racksincluidos[i].model.cores << endl;
    }
    sleep(2); //esperar 2 segundos antes de seguir 
    //crear el hilo que fluctua los precios y ejecutarlo
    pthread_t StockValue;
    pthread_create(&StockValue, NULL, DataFluctiation, 0);
    //inicializar la barrera para permitir que los Racks se esperen entre sí
    pthread_barrier_init(&barrier, NULL , racks);
    //crear los hilos de los racks
    pthread_t Racks[racks];
    for(int i = 0; i < racks; i++){
        pthread_create(&Racks[i],NULL,RackWorking,(void *)&racksincluidos[i]);
    }
    for(int i = 0; i < racks; i++){
        pthread_join(Racks[i],0);
    }
    emulate = false;
    pthread_barrier_destroy(&barrier);
    cout <<"se ha terminado el minado de datos, se ha alcanzado el objetivo de " << goal << endl;
    if(current - goal > 0){
        cout << "Se ha obtenido una ganancia extra de: " << current - goal << endl;
    }
    return 0;
    
}