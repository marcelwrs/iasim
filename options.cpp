//
// Created by marcel on 20/12/2019.
//
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

/* Flag set by ‘--verbose’. */
static int verbose = 0;
static int trace = 0;

double Pt;
int dmax;
int npontos;
int cenario;
char* arqname;
double NF;
double TN;
double BW;
double divisor;
double dist;
double SNRmin;
int Tper;
int Tcanal;
int limite;
int tipoErro;
double mediaErroGPS;
double desvErroGPS;
int algorithm;
int cen;
int Tmove;

float lambda = 0.010714; //28GHz

int M_BS = 12;
int N_BS = 12;
int M_UE = 4;
int N_UE = 4;

float pi = 3.1415926535;

float real;
float normallos;
float normalnlos;
float ray;
float rayy;
float rayleigh;
float fi_BS[24]={};
float fi_UE[8]={};
double fi_BU;
double fi_UB;
float Pout;
float Plos;
float Pnlos;
float fiBS;
float somador_BS;
float somador_UE;
float dif_BS;
float fi_BU_erro;
float dif;
float X=0;
float Y=0;
float incX=0;
float incY=0;
float velocity;
int protocolo;
int startIA;
float decaimentoSNR;



int
readParams (int argc, char **argv)
{
	int opt = 0;

	static struct option long_options[] =
	{
		/* These options set a flag. */
		{"verbose", no_argument,       0, 'v'},
		{"trace",   no_argument,       0, 't'},
		{"add",     no_argument,       0, 'p'},
		{"append",  no_argument,       0, 'b'},
		{"delete",  required_argument, 0, 'd'},
		{"create",  required_argument, 0, 'c'},
		{"file",    required_argument, 0, 'f'},
		{0, 0, 0, 0}
	};

        int long_index = 0;
	while ((opt = getopt_long_only(argc, argv, "", long_options, &long_index )) != -1) {

		switch (opt)
		{
			case 'v':
				verbose = 1;
				break;

			case 't':
				trace = 1;
				break;

			case 'b':
				puts ("option -b\n");
				break;

			case 'c':
				printf ("option -c with value `%s'\n", optarg);
				break;

			case 'd':
				printf ("option -d with value `%s'\n", optarg);
				break;

			case 'f':
				printf ("option -f with value `%s'\n", optarg);
				break;

			case '?':
				/* getopt_long already printed an error message. */
				break;

			default:
				abort ();
		}
    }

    /* Print any remaining command line arguments (not options). */
    if (optind < argc)
    {
        printf ("non-option ARGV-elements: ");
        while (optind < argc)
            printf ("%s ", argv[optind++]);
        putchar ('\n');
    }

    return 0;
}
