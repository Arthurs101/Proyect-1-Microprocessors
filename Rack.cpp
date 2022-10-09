#include "Cpu.h"
//Estructura Referente al Rack
struct Rack{
    //cantidad de CPUS
    int cpus;
    //descripcion de los CPUS
    CPU model;
    //monedas generadas por el Rack
    double CoinsGenerated;
    //identificador del Rack
    int id;
};
