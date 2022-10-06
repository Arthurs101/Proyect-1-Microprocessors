//referente a la struct del cpu
//parametros a pasar a los cpus
struct DataToWORK{
    int min; //minimo y maximo del intervalo a operar
    int max;
};
struct cpu{
    int id;
    int threads;
    DataToWORK data;
};

