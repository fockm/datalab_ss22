/*
 * =====================================================================================
 *
 *       Filename:  prober.c
 *
 *    Description:   
 *
 *        Version:  1.0
 *        Created:  09.07.2015 11:29:29
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Henning Fehrmann (), henning.fehrmann@aei.mpg.de
 *        Company:  AEI Hannover
 *      Copyright:  GPL v2.0 Copyright (c) 2015, Henning Fehrmann
 *
 * =====================================================================================
 */


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <argtable2.h>
#include <time.h>
#include <math.h>
#include <fftw3.h>
#include <dirent.h>
#include <errno.h>

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

struct datapoint * data;
struct param * tbank;
// numbere of data
int nod;
int notb;
int maxdata=1000;
int maxtb=1000;


struct arg_file *input;
struct arg_file *output;
struct arg_file *tbfile;
struct arg_dbl *freq;
struct arg_dbl *phase;
struct arg_int *method;


void print_help(char ** argv,void *argtable[])
{
	printf("Usage: %s ", argv[0]);
	arg_print_syntax(stdout, argtable, "\n");
        arg_print_glossary(stdout, argtable, "  %-25s %s\n");
	exit(0);
}

void load_file()
{
	FILE * f = fopen(input->filename[0],"r");
	if (f == NULL)
	{
		fprintf(stderr,"Couldn't open %s\n",input->filename[0]);
		exit(0);
	}
	nod=0;
	while (fscanf(f, "%f %f\n", &data[nod].t,&data[nod].v)!=EOF)
	{
		if (nod==maxdata-1)
		{
			maxdata+=1000;
			struct datapoint * moredata = (struct datapoint * )realloc(data,maxdata*sizeof(struct datapoint));
			if (moredata == NULL)
			{
				fprintf(stderr,"couldn't allocate more memory for data\n");
				exit(1);
			}
			data=moredata;
		}
		nod++;
	}
	fclose(f);
}

void print_data()
{
	for (int i=0; i<nod; i++)
	{
		printf("%g %g\n",data[i].t,data[i].v);
	}
}

float analyze(float fr, float ph)
{
  float sum=0.;
  for (int i=0; i<nod; i++)
  {
    sum+= data[i].v*sin(2.*M_PI*fr*data[i].t + ph);
  }
  return sum/=(float)nod;
}

float analyze2(float fr, float ph)
{
	float sum=0.;
	for (int i=0; i<nod; i++)
	{
		sum+=(sin(2.*M_PI*fr*data[i].t+ph)-data[i].v)*(sin(2.*M_PI*fr*data[i].t+ph)-data[i].v);
	}
	return sum/=(float)nod;
}

void load_templatebank()
{
	tbank = (struct param *) malloc(maxtb * sizeof(struct param));
	FILE * f = fopen(tbfile->filename[0],"r");
	if (f == NULL)
	{
		fprintf(stderr,"Couldn't open %s\n",tbfile->filename[0]);
		exit(0);
	}
	notb=0;
	while (fscanf(f, "%f %f\n", &tbank[notb].f,&tbank[notb].p)!=EOF)
	{
		if (notb==maxtb-1)
		{
			maxtb+=1000;
			struct param * moretb = (struct param * )realloc(tbank,maxtb*sizeof(struct param));
			if (moretb == NULL)
			{
				fprintf(stderr,"couldn't allocate more memory for template bank\n");
				exit(1);
			}
			tbank=moretb;
		}
		notb++;
	}
	fclose(f);
}

void fftw_test()
{

	load_file();
	fftw_plan p;
	double * mag = (double *) malloc(nod*sizeof(double));
	double * in = (double * )fftw_malloc(nod*sizeof(double));
	if (in == NULL)
	{
		fprintf(stderr,"Couldn't allocate in for the fourier transform\n");
		exit(1);
	}
	fftw_complex * result = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * nod);
	if (result == NULL)
	{
		fprintf(stderr,"Couldn't allocate result for the fourier transform\n");
		exit(1);
	}
	for (int i=0; i<nod; i++)
	{
		in[i]=data[i].v;
	}
	p = fftw_plan_dft_r2c_1d(nod, in, result , FFTW_ESTIMATE );
	fftw_execute(p);

  FILE * g;
  g = fopen(output->filename[0],"w");
  if (g == NULL)
  {
    fprintf(stderr,"Couldn't open %s\n",output->filename[0]);
    exit(1);
  }
  for (int i=0; i<nod; i++)
	{
		mag[i] = (result[i][0]*result[i][0])+(result[i][1]*result[i][1]);
		fprintf(g,"%g %g %g\n",((1/data[1].t)/nod)*(i),0.0,(result[i][0]*result[i][0])+(result[i][1]*result[i][1]));
  }
  fclose(g);
	

	fftw_destroy_plan(p);

	fftw_free(result);
	fftw_free(in);
}



int main(int argc, char **argv)
{
	input = arg_file0("i", "input","<file>", "input file");
  output = arg_file0("o", "output","<file>", "output file");
	tbfile = arg_file0("t", "templatebank","<file>", "template bank file");
	freq = arg_dbl0("f", "freq","<float>", "frequency");
	phase = arg_dbl0("p", "phase","<float>", "phase");
	method = arg_int0("m","method","<int>","select method to analyze signal 1, 2 or 3");

//	input = arg_filen(NULL,NULL,"file",1,argc+2, "input file");;
//	struct arg_int *wtime = arg_int0("t","scalar",NULL, "max run time in seconds, default 300");
//	struct arg_lit *help  = arg_lit0("h","help", "print this help and exit");
	struct arg_lit *help  = arg_lit0("h","help", "print this help and exit");
	struct arg_end *end = arg_end(20);
	void *argtable[] = {input,output,freq,phase,help,tbfile, method, end};

	int exitcode = arg_parse(argc,argv,argtable);

	if (help->count > 0 ) print_help(argv,argtable);
	if ( input->count == 0) print_help(argv,argtable);
  
	data = (struct datapoint * )malloc(maxdata*sizeof(struct datapoint));
	if (data == NULL)
	{
		fprintf(stderr,"couldn't allocate memory for data\n");
		exit(1);
	}
	load_file();

	if ( tbfile->count > 0)
	{
    load_templatebank();
    
    if ( output->count == 0) print_help(argv,argtable);

		if (method->ival[0]==1)
		{
      FILE * g;
      g = fopen(output->filename[0],"w");
      if (g == NULL)
      {
        fprintf(stderr,"Couldn't open %s\n",output->filename[0]);
        exit(1);
      }
      for (int i=0; i<notb; i++)
      {
        //printf("%g %g %g\n",tbank[i].f,tbank[i].p,analyze(tbank[i].f,tbank[i].p));
        fprintf(g,"%g %g %g\n",tbank[i].f,tbank[i].p,analyze(tbank[i].f,tbank[i].p));
			}
      fclose(g);
		}
		else if (method->ival[0]==2)
		{
      FILE * g;
      g = fopen(output->filename[0],"w");
      if (g == NULL)
      {
        fprintf(stderr,"Couldn't open %s\n",output->filename[0]);
        exit(1);
      }
      for (int i=0; i<notb; i++)
      {
        //printf("%g %g %g\n",tbank[i].f,tbank[i].p,analyze2(tbank[i].f,tbank[i].p));
        fprintf(g,"%g %g %g\n",tbank[i].f,tbank[i].p,analyze2(tbank[i].f,tbank[i].p));
      }
      fclose(g);
		}
		else if (method->ival[0]==3)
		{	
//			char name[128];
//			sprintf(name,"%s",input->filename[0]);
			fftw_test();
    }
		
	}else{
    if (method->ival[0]==1)
    {
      printf("%g %g %g\n",freq->dval[0],phase->dval[0],analyze(freq->dval[0],phase->dval[0]));
    }
    else if (method->ival[0]==2)
    {
      printf("%g %g %g\n",freq->dval[0],phase->dval[0],analyze2(freq->dval[0],phase->dval[0]));
    }
    else if (method->ival[0]==3)
    {
      fftw_test();
    }
	}
	arg_freetable(argtable,sizeof(argtable)/sizeof(argtable[0]));
	free(data);
	if (tbank != NULL) free(tbank);
	return exitcode;
}

