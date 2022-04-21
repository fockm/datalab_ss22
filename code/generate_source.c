#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <argtable2.h>
#include <time.h>
#include <math.h>
#include <gsl/gsl_randist.h>
#if defined(__MACH__) && !defined(CLOCK_REALTIME)
#include <sys/time.h>
#define CLOCK_REALTIME 0
// clock_gettime is not implemented on older versions of OS X (< 10.12).
// If implemented, CLOCK_REALTIME will have already been defined.
int clock_gettime(int clk_id, struct timespec* t) {
  struct timeval now;
  int rv = gettimeofday(&now, NULL);
  if (rv) return rv;
  t->tv_sec  = now.tv_sec;
  t->tv_nsec = now.tv_usec * 1000;
  return 0;
}
#endif


struct param
{
    float f;
    float p;
};

struct datapoint
{
    float t;
    float v;
};

struct arg_file * output;
struct arg_dbl * sigma;
struct arg_int * N;
struct arg_dbl * deltaT;
struct arg_dbl * freq;
struct arg_dbl * phase;
struct arg_dbl * ampl;
struct arg_dbl * sigma;
struct arg_int * modrand;
struct datapoint * data;

void print_help(char ** argv,void *argtable[])
{
    printf("Usage: %s ", argv[0]);
    arg_print_syntax(stdout, argtable, "\n");
    arg_print_glossary(stdout, argtable, "  %-25s %s\n");
    exit(0);
}

double drand ( double low, double high )
{
    srand(time(NULL));
    return ( (float)rand() * ( high - low ) ) / (float)RAND_MAX + low;
}

void generate_data(double si,double Nr, double dT, double fr, double ph, double am, int mod)
{
    double n,s;
    double fre, phas;
    const gsl_rng_type * T;
    gsl_rng * r;
    struct timespec Time;
    
    gsl_rng_env_setup ();
    T = gsl_rng_default;
    r = gsl_rng_alloc (T);
    clock_gettime(CLOCK_REALTIME , &Time);
    gsl_rng_set (r, Time.tv_nsec);
    
    if (mod==0)
    {
        fre=fr;
        phas=ph;
    }
    else if (mod==1)
    {
        fre=drand(0.1,1000.0);
        phas=drand(0,2.*M_PI);
        printf("%f\n",fre);
        printf("%f\n",phas);
    }
    for (int i=0; i<Nr; i++)
    {
        n=gsl_ran_gaussian(r,si);
        s=am*sin((double)i *dT*fre *2. *M_PI + phas);
        data[i].t=(double) i*dT;
        data[i].v=n+s;
        
    }
}

void write_data(char name[],int Nr )
{
    FILE * f;
    
    if (output->count==0)
    {
        f=stdout;
    }else{
        f = fopen(name,"w");
        if (f == NULL)
        {
            fprintf(stderr,"Couldn't open %s\n",name);
            exit(1);
        }
    }
    for (int i=0; i<Nr; i++)
    {
        fprintf(f,"%1.7f %1.7f\n",data[i].t,data[i].v);
    }
    fclose(f);
}
int main(int argc, char **argv)
{
    output = arg_file0("o", "output","<file>", "output file");
    sigma = arg_dbl0("s", "sigma","<float>", "standard deviation sigma");
    deltaT = arg_dbl0("d", "deltaT","<float>", "delta T betwean measurements");
    N = arg_int0("N", "length","<int>", "number of elements");
    freq = arg_dbl0("f", "freq","<float>", "frequency");
    phase = arg_dbl0("p", "phase","<float>", "phase");
    ampl = arg_dbl0("a", "amplitude","<float>", "signal amplitude");
    modrand = arg_int0("r","random","<int>","if set to 1, generate random signal");
    struct arg_lit *help  = arg_lit0("h","help", "print this help and exit");
    
    struct arg_end *end = arg_end(20);
    void *argtable[] = {output,sigma,deltaT,N,freq,phase,ampl,modrand,help,end};
    
    int exitcode = arg_parse(argc,argv,argtable);
    
    
    if (help->count > 0 || N->count == 0 ) print_help(argv,argtable);
    
    data=(struct datapoint * ) malloc ( sizeof(struct datapoint * )*N->ival[0] );
    if (deltaT->count == 0)
    deltaT->dval[0]=1.;
    generate_data(sigma->dval[0],N->ival[0],deltaT->dval[0],freq->dval[0],phase->dval[0],ampl->dval[0],modrand->ival[0]);
    char name[128];
    sprintf(name,"%s",output->filename[0]);
    write_data(name,N->ival[0]);
    arg_freetable(argtable,sizeof(argtable)/sizeof(argtable[0]));
    free(data);
    return exitcode;
}
