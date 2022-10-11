#include <pthread.h>
#include <unistd.h>
#include <iostream>
#include <cstdlib>
#include "Rack.cpp"


using namespace std;
//Variables de meta y cantidad actual de monedas
double goal;
double current = 0.0;
//variable del precio unitario de la moneda "DASHCOIN"
double DashPrice = 0.01000;
//funcion encargada de variar el precio de la moneda 
void* DataFluctiation (void* data){
    int i = 0;
    while(i < 10){
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
        cout <<"DashPrice: " << DashPrice << endl;
        i++;
        sleep(1); //esperar un segundo antes de variar el precio
    }
}
void* RackWorking (void* args){
    Rack* rack = (Rack*)args;
    double subtotal = 0.0;
    //Hilos que representan cada cpu
    pthread_t CPUS[rack -> cpus];
    //crear los CPUS y generar sus cargas de trabajo
<<<<<<< Updated upstream
    CPU CPUSdata[rack -> cpus] = {rack -> model};
    for(int i = 0; i < rack->cpus; i++){
        CPUSdata[i].data.min = rand()%100;
        CPUSdata[i].data.max = rand()%10000; 
        pthread_create(&CPUS[i],0,CpuProcessor, (void*)&CPUS[i]); //está como plahe holder actualizar después del commit de GUS
=======
    pthread_mutex_lock(&Racklock);
    cout << "El rack " << rack -> id << " Esta en Linea"<< endl;
    pthread_mutex_unlock(&Racklock);
    while(RackIsOnline){
        for(int i = 0; i < rack->cpus; i++){
            CPUSdata[i].data.min = rand()%10;
            CPUSdata[i].data.max = rand()%100; 
            pthread_create(&CPUS[i],0,cpuFunction, (void*)&CPUSdata[i]); //está como plahe holder actualizar después del commit de GUS
        }
    //esperar los hilos y añadir al subtotal del rack 
        void *exit_value;
        for(int i=0; i < rack->cpus ; i++){
            pthread_join(CPUS[i],&exit_value);
            //this is just for testing
            double selling = DashPrice;
            double produced = ((int)exit_value + 0.0)*selling;
            rack -> CoinsGenerated += produced;
            pthread_mutex_lock(&Racklock);
            cout<<"Cpu no: " << i << "del rack" << rack -> id << "produjo: " << (int) exit_value << "Dash tokens" <<endl;
            cout <<"lo cual vendio a un precio de: " << selling << " Lo cual da un resultado de :" << produced << "DashCoins" << endl;
            pthread_mutex_unlock(&Racklock);
        }
        pthread_mutex_lock(&Racklock);
        //añadir el subtotal a lo producido
        cout << "El rack " << rack -> id << "ha generado: " << rack -> CoinsGenerated << endl;
        current += rack-> CoinsGenerated;
        cout << "monedas actuales: "<<current << endl;
        pthread_mutex_unlock(&Racklock);
        rack -> CoinsGenerated = 0;
        //esperar a los demás racks antes de realizar la comparacion
        pthread_barrier_wait(&barrier);
        if(current >= goal){ //si ya se llegó o sobrepasó el límite, apagar el rack
            RackIsOnline = false;
            cout << "El rack " << rack -> id << "se ha apagado"<< endl;
        }
>>>>>>> Stashed changes
    }

}
void* CpuProcessor (void* args){

<<<<<<< Updated upstream
}

int main(){
=======
int main(){
    int racks;
>>>>>>> Stashed changes
    cout<<"Bienvenido al Pseudo-Simulador de Criptominado"<<endl;
    //solicitarle la cantidad de racks
    cout<<"Cuantas metas va a producir?"<<endl;
    cin >> goal; 
    cout<<"Cuantos Racks posee su instalación?"<<endl;
    int racks = 0;
    cin >> racks;
    //ARRAY DE Racks
    Rack racksincluidos[racks];
    //configurar los CPUS de cada RACK
    for(int i=0; i<racks; i++){
        cout<<"Cuantos CPUS tiene el Rack?"<<endl;
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
    sleep(5); //esperar 5 segundos antes de crear 
    //crear el hilo que fluctua los precios y ejecutarlo
    pthread_t StockValue;
<<<<<<< Updated upstream
    pthread_create(&StockValue, NULL, DataFluctiation, 0);
    pthread_join(StockValue, NULL);
    //crear los hilos de los racks
    pthread_t Racks[racks];
    for(int i = 0; i < racks; i++){
        pthread_create(&Racks[i],0,RackWorking,0);
=======
    pthread_create(&StockValue, NULL, DataFluctiation, NULL);
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
    emulate = false; //<- bandera del hilo que fluctua los datos
    pthread_barrier_destroy(&barrier);
    cout <<"se ha terminado el minado de datos, se ha alcanzado el objetivo de " << goal << endl;
    if(current - goal > 0){
        cout << "Se ha obtenido una ganancia extra de: " << current - goal << endl;
>>>>>>> Stashed changes
    }

    return 0;
    
}