#include <pthread.h>
#include <unistd.h>
#include <iostream>

#include "Rack.cpp"
#include "Cpu.cpp"

using namespace std;
//variable del precio unitario de la moneda "DASHCOIN"
double DashPrice = 0.00000001;

//funcion encargada de variar el precio de la moneda 
void* DataFluctiation (void* data){
    while(true){
        
        sleep(1); //esperar un segundo antes de variar el precio
    }
}
void* RackWorking (void* args){
    

}
void* CpuProcessor (void* args){

}

int main(){
    cout<<"Bienvenido al Pseudo-Simulador de Criptominado"<<endl;
    //solicitarle la cantidad de racks
    cout<<"Cuantos Racks posee su instalación?"<<endl;
    int racks = 0;
    cin >> racks;
    //ARRAY DE Racks
    Rack racksincluidos[racks];

    
    return 0;
    
}