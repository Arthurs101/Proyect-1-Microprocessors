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
    CPU CPUSdata[rack -> cpus] = {rack -> model};
    for(int i = 0; i < rack->cpus; i++){
        CPUSdata[i].data.min = rand()%100;
        CPUSdata[i].data.max = rand()%10000; 
        pthread_create(&CPUS[i],0,CpuProcessor, (void*)&CPUS[i]); //está como plahe holder actualizar después del commit de GUS
    }

}
void* CpuProcessor (void* args){

}

int main(){
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
    //crear el hilo que fluctua los precios y ejecutarlo
    pthread_t StockValue;
    pthread_create(&StockValue, NULL, DataFluctiation, 0);
    pthread_join(StockValue, NULL);
    //crear los hilos de los racks
    pthread_t Racks[racks];
    for(int i = 0; i < racks; i++){
        pthread_create(&Racks[i],0,RackWorking,0);
    }

    return 0;
    
}