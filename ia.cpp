#include <iostream>
#include <chrono>
#include <random>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <cstdlib>
#include <ctime>
#include <string.h>

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
int trace;
int Tmove;
int nAdjacentes;

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
float velocity_USR;
int velocity_OBJ;
int protocolo;
int intervaloFixoIA;
float decaimentoTaxaRx;
float quedaTaxaRx;
float limiarFastIA;
int condCanal;
int fastIA;

#define LOS 1
#define NLOS 2
#define AUTO 3

//################################################################################################################

void read_params(int argc, char *argv[ ])
{

	Pt = atof(argv[1]);
	dmax = atoi(argv[2]);
	npontos =atoi(argv[3]);
	cenario = atoi(argv[4]);
	arqname=argv[5];
	NF = atof(argv[6]);
	TN = atof(argv[7]);
	BW = atof(argv[8]);
	divisor = atof(argv[9]);
	Tmove = atof(argv[10]);
	SNRmin = atof(argv[11]);
	Tper = atoi(argv[12]);
	Tcanal = atoi(argv[13]);
	limite = atoi(argv[14]);
	tipoErro = atoi(argv[15]);
	mediaErroGPS = atof(argv[16]);
	desvErroGPS = atof(argv[17]);
	algorithm = atoi(argv[18]);
	trace = atoi(argv[19]);
	velocity_USR = atof(argv[20]);
	velocity_OBJ = atof(argv[21]);
	protocolo = atoi(argv[22]);
	intervaloFixoIA = atoi(argv[23]);
	decaimentoTaxaRx = atof(argv[24]);
	quedaTaxaRx = atof(argv[25]);
	fastIA = atoi(argv[26]); 
	limiarFastIA = atof(argv[27]);
	condCanal = atoi(argv[28]);
	nAdjacentes = atoi(argv[29]);
}

//################################################################################################################

void create_scenario(std::default_random_engine gen,
                     std::normal_distribution<double> los_rand,
                     std::normal_distribution<double> nlos_rand,
                     std::uniform_real_distribution<double> uniform_rand,
                     std::normal_distribution<double> ray_randx,
		     std::normal_distribution<double> ray_randy,
                     std::uniform_real_distribution<double> angle_rand)
{
	somador_BS = 0;
	somador_UE = 0;
	//criando codebook
	if(algorithm == 4){ printf("algoritmo 4\n\n");
	
		for(int l = 0; l<M_UE+N_UE; l++){

			fi_UE[l] = somador_UE;
			somador_UE = somador_UE + 360.0/(M_UE+N_UE);
			printf("fi_UE %f\n ", fi_UE[l]);
		}

		for(int k = 0; k<M_BS+N_BS; k++){

			fi_BS[k] = somador_BS+7.5;
			somador_BS =somador_BS + 360.0/(M_BS+N_BS);
			printf("fi_BS %f\n ", fi_BS[k]);

		}

	}else{

		for(int l = 0; l<M_UE+N_UE; l++){

			fi_UE[l] = somador_UE;
			somador_UE = somador_UE + 360.0/(M_UE+N_UE);
			printf("fi_UE %f\n ", fi_UE[l]);
		}

		for(int k = 0; k<M_BS+N_BS; k++){

			fi_BS[k] = somador_BS;
			somador_BS =somador_BS + 360.0/(M_BS+N_BS);
			printf("fi_BS %f\n ", fi_BS[k]);

		}

	}


	// Sorteia posicao real
	fi_BU = angle_rand (gen);
	fi_UB = fmod(fi_BU + 180,360);
	printf("fi_BU %f ", fi_BU);
	printf("fi_UB %f\n ", fi_UB);

	X=cos(fi_BU*pi/180)*dist;
	Y=sin(fi_BU*pi/180)*dist;
	printf("X= %f ", X);
	printf("Y= %f\n ", Y);
	
	float x;
	float y;

	// Calculando o erro
	if (tipoErro==0) { //Uniforme
		float resultado=dist+mediaErroGPS+1;

		while (resultado > dist+mediaErroGPS || resultado < dist-mediaErroGPS){

			std::uniform_real_distribution<double> distribution_x(X-mediaErroGPS,X+mediaErroGPS);
			x = distribution_x(gen);
			printf("y_erro = %f\n ", x);

			std::uniform_real_distribution<double> distribution_y(Y-mediaErroGPS,Y+mediaErroGPS);	
			y = distribution_y(gen);
			printf("x_erro = %f\n ", y);

			resultado= sqrt(fabs(pow(x,2)+pow(y,2))); 
			printf("resultado = %f\n ", resultado);
		}
	} else if (tipoErro==1) { //Normal
		std::normal_distribution<double> pos (mediaErroGPS, desvErroGPS);
		std::uniform_real_distribution<double> angulo (0, 360);
		double ang = angulo(gen);
		printf("ang = %f\n ", ang);
		double distgps = fabs(pos(gen));
		printf("distgps = %f\n ", distgps);

		x = X + distgps*cos(ang*pi/180);
		y = Y + distgps*sin(ang*pi/180);
		printf("x_erro = %f\n ", x);
		printf("y_erro = %f\n ", y);

	} else {
		printf("Modelo de erro desconhecido.\n");
		exit(1);
	}


	if(y>0 && x>0){fi_BU_erro = ((atan(y/x) * 180) / pi);}

	if(y>0 && x<0){fi_BU_erro = ((atan(y/x) * 180) / pi) + 180;}

	if(y<0 && x<0){fi_BU_erro = ((atan(y/x) * 180) / pi) + 180;}

	if(y<0 && x>0){fi_BU_erro = ((atan(y/x) * 180) / pi) + 360;}
	
	printf("fi_BU_erro %f\n  ", fi_BU_erro);

	dif_BS =360.0;
	for(int p = 0; p<M_BS+N_BS; p++){

		dif = fabs(fi_BS[p]-fi_BU_erro);

		if(dif_BS >= dif){
			dif_BS = dif;  
			fiBS = fi_BS[p];
		}

		if(algorithm == 4){printf("algoritmo 4\n\n");
			
			if(fabs(367.5-fi_BU_erro)<dif_BS){fiBS = fi_BS[0];}
	
		}else{

			if(fabs(360-fi_BU_erro)<dif_BS){fiBS = fi_BS[0];}
		}

		
	}
	printf("fiBSsssss %f\n  ", fiBS);

	// Sorteia canal
	printf("dist %f\n ", dist);
	Pout = (1 - exp(((-0.033333333*dist) + 5.2)));

	if(Pout<0){Pout=0;}

	Plos = ((1 - Pout)*(exp(-0.014903129*dist)));
	Pnlos = (1 - Pout - Plos);
	printf("Pout final %f ", Pout);
	printf("Plos final %f ", Plos);
	printf("Pnlos final %f\n ", Pnlos);

	real = uniform_rand(gen);
	normallos = los_rand(gen);
	normalnlos = nlos_rand(gen);

	printf("real %f\t ", real);
	printf("normallos %f\t ", normallos);
	printf("normalnlos %f\n ", normalnlos);

	
}

//#############################################################################################

void update_scenario(std::default_random_engine gen,
                     std::normal_distribution<double> los_rand,
                     std::normal_distribution<double> nlos_rand,
                     std::uniform_real_distribution<double> uniform_rand,
                     std::normal_distribution<double> ray_randx,
		     std::normal_distribution<double> ray_randy,
                     std::uniform_real_distribution<double> angle_rand,
		     float incX, float incY, int update)
{
	printf("antes X = %f\n ", X);
	printf("antes Y = %f\n ", Y);
	printf("incX = %f\n ", incX);
	printf("incY = %f\n ", incY);
	X= X + incX;
	Y= Y + incY;
	printf("depois X = %f\n ", X);
	printf("depois Y = %f\n ", Y);

	dist=sqrt(fabs(pow(X,2)+pow(Y,2)));

	if(Y>0 && X>0){fi_BU = ((atan(Y/X) * 180) / pi);}

	if(Y>0 && X<0){fi_BU = ((atan(Y/X) * 180) / pi) + 180;}

	if(Y<0 && X<0){fi_BU = ((atan(Y/X) * 180) / pi) + 180;}

	if(Y<0 && X>0){fi_BU = ((atan(Y/X) * 180) / pi) + 360;}

	fi_UB = fmod(fi_BU + 180,360);

	printf("fi_BU %f ", fi_BU);
	printf("fi_UB %f\n ", fi_UB);
	printf("dist %f\n ", dist);

	if(update==1){
		somador_BS = 0;
		somador_UE = 0;
		//criando codebook
		if(algorithm == 4){ printf("algoritmo 4\n\n");
	
			for(int l = 0; l<M_UE+N_UE; l++){

				fi_UE[l] = somador_UE;
				somador_UE = somador_UE + 360.0/(M_UE+N_UE);
				printf("fi_UE %f\n ", fi_UE[l]);
			}

			for(int k = 0; k<M_BS+N_BS; k++){

				fi_BS[k] = somador_BS+7.5;
				somador_BS =somador_BS + 360.0/(M_BS+N_BS);
				printf("fi_BS %f\n ", fi_BS[k]);

			}

		}else{

			for(int l = 0; l<M_UE+N_UE; l++){

				fi_UE[l] = somador_UE;
				somador_UE = somador_UE + 360.0/(M_UE+N_UE);
				printf("fi_UE %f\n ", fi_UE[l]);
			}

			for(int k = 0; k<M_BS+N_BS; k++){

				fi_BS[k] = somador_BS;
				somador_BS =somador_BS + 360.0/(M_BS+N_BS);
				printf("fi_BS %f\n ", fi_BS[k]);

			}

		}

	
		float x;
		float y;

		// Calculando o erro
		if (tipoErro==0) { //Uniforme
			float resultado=dist+mediaErroGPS+1;

			while (resultado > dist+mediaErroGPS || resultado < dist-mediaErroGPS){

				std::uniform_real_distribution<double> distribution_x(X-mediaErroGPS,X+mediaErroGPS);
				x = distribution_x(gen);
				printf("y_erro = %f\n ", x);

				std::uniform_real_distribution<double> distribution_y(Y-mediaErroGPS,Y+mediaErroGPS);	
				y = distribution_y(gen);
				printf("x_erro = %f\n ", y);

				resultado= sqrt(fabs(pow(x,2)+pow(y,2))); 
				printf("resultado = %f\n ", resultado);
			}
		} else if (tipoErro==1) { //Normal
			std::normal_distribution<double> pos (mediaErroGPS, desvErroGPS);
			std::uniform_real_distribution<double> angulo (0, 360);
			double ang = angulo(gen);
			printf("ang = %f\n ", ang);
			double distgps = fabs(pos(gen));
			printf("distgps = %f\n ", distgps);

			x = X + distgps*cos(ang*pi/180);
			y = Y + distgps*sin(ang*pi/180);
			printf("x_erro = %f\n ", x);
			printf("y_erro = %f\n ", y);

		} else {
			printf("Modelo de erro desconhecido.\n");
			exit(1);
		}


		if(y>0 && x>0){fi_BU_erro = ((atan(y/x) * 180) / pi);}

		if(y>0 && x<0){fi_BU_erro = ((atan(y/x) * 180) / pi) + 180;}

		if(y<0 && x<0){fi_BU_erro = ((atan(y/x) * 180) / pi) + 180;}

		if(y<0 && x>0){fi_BU_erro = ((atan(y/x) * 180) / pi) + 360;}
	
		printf("fi_BU_erro %f\n  ", fi_BU_erro);

		dif_BS =360.0;
		for(int p = 0; p<M_BS+N_BS; p++){

			dif = fabs(fi_BS[p]-fi_BU_erro);

			if(dif_BS >= dif){
				dif_BS = dif;  
				fiBS = fi_BS[p];
			}

			if(algorithm == 4){printf("algoritmo 4\n\n");
			
				if(fabs(367.5-fi_BU_erro)<dif_BS){fiBS = fi_BS[0];}
	
			}else{

				if(fabs(360-fi_BU_erro)<dif_BS){fiBS = fi_BS[0];}
			}

		
		}
		printf("fiBSsssss %f\n  ", fiBS);			
	
					
		
	}

	
	
}

//#########################################################################################################

void plot_results(int algorithm, int protocolo, float decaimentoTaxaRx, float velocity_USR, float mediaErroGPS, float capacidade, float capacidadeEf, int tIA, int nIA, double totalSNR, float ZERO, float BEAM, float BEAMZERO)
{

	
	FILE* arq = fopen(arqname, "w");

	fprintf(arq, "%d ", dmax);
	printf("dmax %d ", dmax);

	fprintf(arq, "%d ", algorithm);
	printf("algorithm %d ", algorithm);

	fprintf(arq, "%d ", protocolo);
	printf("protocolo %d ", protocolo);

	fprintf(arq, "%f ", decaimentoTaxaRx);
	printf("decaimentoTaxaRx %f ", decaimentoTaxaRx);

	fprintf(arq, "%f ", velocity_USR);
	printf("velocity_USR %f ", velocity_USR);

	fprintf(arq, "%f ", mediaErroGPS);
	printf("velocity_USR %f ", mediaErroGPS);

	fprintf(arq, "%d ", nIA);
	printf("nIA %d ", nIA);

	fprintf(arq, "%f ", (double)tIA/(double)limite);
	printf("tutil %f ", (double)tIA/(double)limite);

	fprintf(arq, "%d ", tIA);
	printf("tIA %d ", tIA);

	fprintf(arq, "%f ", capacidade);
        printf("Cnominal %f ", capacidade);

	fprintf(arq, "%f ", capacidadeEf);
        printf("Cefetiva %f ", capacidadeEf);

	fprintf(arq, "%f ", ZERO);
        printf("media ZERO %f ", ZERO);

	fprintf(arq, "%f ", BEAM);
        printf("media BEAM %f ", BEAM);

	fprintf(arq, "%f\n", BEAMZERO);
        printf("media BEAMZERO %f\n", BEAMZERO);



	/*double cnominalSNR;
	if (limite <= tIA) {
		cnominalSNR = 0.0;
	} else {
		cnominalSNR = log2((totalSNR/(limite-tIA)) + 1.0);
	}
	fprintf(arq, "%f ", cnominalSNR);
        printf("CnominalSNR %f ", cnominalSNR);

	fprintf(arq, "%f\n", cnominalSNR * ((limite-tIA)/(double)limite));
        printf("CefetivaSNR %f\n", cnominalSNR * ((limite-tIA)/(double)limite));*/

	fclose(arq);
	
}

//#########################################################################################################

int main(int argc, char *argv[ ])
{


	// Ler os parametros
	read_params(argc, argv);

	// Gerador de cenarios aleatorios
	std::default_random_engine gen (cenario);
	std::normal_distribution<double> los_rand (0.0,5.8);
	std::normal_distribution<double> nlos_rand (0.0,8.7);
	std::uniform_real_distribution<double> uniform_rand (0,1);
	std::normal_distribution<double> ray_randx (0,1);
	std::normal_distribution<double> ray_randy (0,1);
	std::uniform_real_distribution<double> angle_rand (0,360);

	FILE* tracefd;
	if(trace) {
		char str[800];
		strcpy(str, arqname);
		strcat(str, ".log");
		tracefd = fopen(str, "w");
	}
		
	


	// Executa IA
	float Psix_BS=0;
	float Psiy_BS=0;
	float AF_BS=0;
	float Psix_UE=0;
	float Psiy_UE=0;
	float AF_UE=0;
	float AF=0;
	float AFpuro=0;
	float dif1=0;		
	float dif2=0;
	float SNR = 0.0;
	float PL=0;
	float valorPL1=0;
	int tIA = 0;
	int Tupdate=(Tcanal/Tper);
	int fail=1; 
	int primeira=0;
	float resto=0;

	int Tsimulacao=0;
	int beamUSR=0;
	int beamBS=0;
	float USRbeam=0;
	float BSbeam=0;
	double totalCap=0;
	double totalSNR=0;
	int contDADOS=0;
	int contIA=0;
	int tempoIA=1;
	int tempoDADOS=0;
	float SNRmax=0;
	float BeamUSRdados=0;
	float BeamBSdados=0;
	int restoIA;
	int contador=0;
	int contadortot=0;
	float adjacente=-(360/(M_BS+N_BS));
	float tamFeixe=360/(M_BS+N_BS); // tamanho do feixe de acordo com o nro de elementos da UPA

	int modificador; // usado nos algoritmos
	if (algorithm == 2) // case GPS refinado
		modificador=-nAdjacentes; // usado nos algoritmos
	else if (algorithm == 3) // case proposta refinado
		modificador=-2*nAdjacentes; // usado nos algoritmos

	int fase=1;
	int alg=1;
	int contfase=0;
	int contalg=0;
	float afmax=0;
	float afmaxDADOS=0;
	int cria=1;
	float max=0;
	int mult=100;
	int cont_ray=0;
	float NbeamUSR=0;
	float NbeamBS=0;
	float x_update=0;
	float y_update=0;
	int cont_move=0;
	int continterupcao=0;
	int update=0;
	int continterupcaoIA=0;
	int contadorIA=0;
	int nIA=0;
	float SNRmovel=0;
	int contX=0;
	int godTsimulacao=0;
	int interIA=0;
	float interSNRmax=0;
	float interBSbeam=0;
	float interUSRbeam=0;
	int interCONT=0;
	int verificaIA=0; 
	float SNRmovelll=0;
	float SNRmaxx=0;
	int contx=0;
	int teste;
	int Tout=0;
	int ZERO=0;
	int certo=0;
	int certoZERO=0;
	float BeamMelhorUSR=0;
	float BeamMelhorBS=0;

	//###############rayleigh####################################
	
	std::uniform_real_distribution<double> rayleigh_rand (20,100);
	ray = rayleigh_rand (gen);

	int linha=1000000;
	FILE *distr_ray;
	float vet_ray [linha]={0.0};
	char line[1000];
	sprintf(line, "rayleigh_v%d.txt", (int)velocity_OBJ);
	printf(line, "rayleigh_v%d.txt", (int)velocity_OBJ);
	distr_ray =fopen(line,"r");
	printf("%s\n", line);
	for(int r=0 ;r<linha; r++){

		fscanf(distr_ray, "%f%*[^\n]%*c",&vet_ray[r]);
		//printf("%f\t", vet_ray[r]);
	}


	//###############distancia#################################

	if (velocity_USR > 0.0) {
		std::uniform_real_distribution<double> dist_rand (0,dmax);
		dist = dist_rand (gen);
	} else {
		dist = dmax;
	}

	//########################################################
			
	while(Tsimulacao < limite){


		if(cria==1){
			if(algorithm == 4){M_BS=3; N_BS=3; adjacente=-(360/(M_BS+N_BS));}

			// Cria o cenário
			create_scenario(gen, los_rand, nlos_rand, uniform_rand, ray_randx, ray_randy, angle_rand);
	
			if(cria==1){cria=0;}			
		
		} else {

			if (godTsimulacao <= 1){

				update_scenario(gen, los_rand, nlos_rand, uniform_rand, ray_randx, ray_randy, angle_rand, incX, incY, update);
				if(update==1){update=0;}

				x_update=X;
				y_update=Y;

				printf("x_update %f\n ",x_update);
				printf("y_update %f\n ",y_update);
			}	
		}

		// Calculo AF do BS
		Psix_BS = (((2.0*pi)/lambda)*(lambda/divisor)*(cos(fi_BU*pi/180))) - (((2.0*pi)/lambda)*(lambda/divisor)*(cos(BSbeam*pi/180)));
		Psiy_BS = (((2.0*pi)/lambda)*(lambda/divisor)*(sin(fi_BU*pi/180))) - (((2.0*pi)/lambda)*(lambda/divisor)*(sin(BSbeam*pi/180)));
		if(fabs(fi_BU-BSbeam) < 0.01 || fabs(fi_BU-(BSbeam + 360.0)) < 0.01){AF_BS=(M_BS*N_BS);}
		else{AF_BS= (sin((M_BS/2.0)*Psix_BS)/sin(Psix_BS/2.0))*(sin((N_BS/2.0)*Psiy_BS)/sin(Psiy_BS/2.0));}
		

		// Calculo AF do UE
		Psix_UE = (((2.0*pi)/lambda)*(lambda/divisor)*(cos(fi_UB*pi/180))) - (((2.0*pi)/lambda)*(lambda/divisor)*(cos(USRbeam*pi/180)));
		Psiy_UE = (((2.0*pi)/lambda)*(lambda/divisor)*(sin(fi_UB*pi/180))) - (((2.0*pi)/lambda)*(lambda/divisor)*(sin(USRbeam*pi/180)));
		if(fabs(fi_UB-USRbeam) < 0.01 || fabs(fi_UB-(USRbeam + 360.0)) < 0.01){AF_UE=(M_UE*N_UE);}
		else{AF_UE= (sin((M_UE/2.0)*Psix_UE)/sin(Psix_UE/2.0))*(sin((N_UE/2.0)*Psiy_UE)/sin(Psiy_UE/2.0));}

		printf("BSbeam %f\n ",BSbeam);
		printf("USRbeam %f\n ",USRbeam);
		
		//###################fast fading###################################
		if(BSbeam>=0){NbeamBS=((BSbeam/(360/(M_BS+N_BS)))+1);}else{NbeamBS=(((360+BSbeam)/(360/(M_BS+N_BS)))+1);}
		if(USRbeam>=0){NbeamUSR= ((USRbeam/(360/(M_UE+N_UE)))+1);}else{NbeamUSR=(((360+USRbeam)/(360/(M_UE+N_UE)))+1);}
		printf("NbeamBS %f\n ",NbeamBS);
		printf("NbeamUSR %f\n ",NbeamUSR);

		if (godTsimulacao <= 1){	
			if(protocolo==3){cont_ray++;}
			if(tempoIA==0){cont_ray++;}
			int valor_r = (((NbeamBS*10)+ NbeamUSR)*ray)+cont_ray;
			rayleigh=vet_ray[valor_r];
			printf("rayleigh %f\n ", rayleigh);
		}

		//###################################################################

		AF = fabs(AF_BS*AF_UE);
		AFpuro=AF;
		if(AF>afmax){afmax=AF;}
		printf("AF %f\n ",AF);
		printf("AFmax %f\n ",afmax);
		AF = fabs(AF_BS*AF_UE)*rayleigh;
		

		//####################shadowing#############################################

		if (godTsimulacao <= 1){
		
			float R;
			float dcor = 10;
			float Delta_d = sqrt(fabs(pow(incX,2)+pow(incY,2)));
			R = exp((-Delta_d)/dcor);
			// Update LOS shadowing
			float update_normallos = los_rand(gen);
			normallos = (R*normallos) + (sqrt(1- pow(R,2))*update_normallos);
			// Update NLOS shadowing
			float update_normalnlos = nlos_rand(gen);
			normalnlos = (R*normalnlos) + (sqrt(1- pow(R,2))*update_normalnlos);
		}
		

		//##########################################################################

		switch (condCanal)
		{
			case LOS:
				//PLos
				printf("Plos ");
				PL = pow(10.0,((61.4 + 20*log10(dist) + normallos)*-1.0)/10.0);
				printf("normal %f ", normallos);
				printf("PL %f\n ", 10*log10(PL));
				break;

			case NLOS:
				printf("Pnlos ");
				PL =pow(10.0,((72 + 29.2*log10(dist) + normalnlos)*-1.0)/10.0);
				printf("normal %f ", normalnlos);
				printf("PL %f\n ", 10*log10(PL));
				break;

			case AUTO:
				if(0<=real && real<Pout){           //Pout
					printf("Pout ");
					PL = 0;
				} else if(Pout<=real && real<(Pout+Plos)){ //PLos
					printf("Plos ");
					PL = pow(10.0,((61.4 + 20*log10(dist) + normallos)*-1.0)/10.0);
					printf("normal %f ", normallos);
					printf("PL %f\n ", 10*log10(PL));
				} else if((Pout+Plos)<=real && real<=1){   //Pnlos
					printf("Pnlos ");
					PL =pow(10.0,((72 + 29.2*log10(dist) + normalnlos)*-1.0)/10.0);
					printf("normal %f ", normalnlos);
					printf("PL %f\n ", 10*log10(PL));

				}
				break;

			default:
				printf("Erro: condicao do canal desconhecida.\n");
			exit(1);
		}

		SNR= (PL*pow(AF,2)*pow(10.0,Pt/10.0))/(M_UE*N_UE*M_BS*N_BS*pow(10.0,NF/10.0)*pow(10.0, TN/10.0)*BW);
		printf("SNR %f\n ",SNR);


		if(Tsimulacao==0){SNR=0;}

		if(SNRmax<=SNR){SNRmax=SNR; BeamUSRdados=USRbeam; BeamBSdados=BSbeam;}
		//if(SNRmax<=SNR){SNRmax=((SNRmax*0.8)+(SNR*0.2)); BeamUSRdados=USRbeam; BeamBSdados=BSbeam;}
		printf("SNRmax %f\n ",SNRmax);
		printf("BeamBSdados %f\n ",BeamBSdados);
		printf("BeamUSRdados %f\n\n\n\n ",BeamUSRdados);

		//############################random-point##################################
		
				
		cont_move ++;
		int restoMV;
		if (velocity_USR > 0.0)
			restoMV = cont_move % int(Tmove/velocity_USR);
		else
			restoMV = -1;
		float modulo=200;
		float ang_update;
		float pointy;
		float HIP;

		if((Tsimulacao==0 || fabs(dist)>dmax) && velocity_USR > 0.0){

                        std::uniform_real_distribution<double> angupdate_rand ((fi_BU+90.0+40.0),(fi_BU+90.0+140.0));
                        ang_update = angupdate_rand (gen);
                        if(ang_update>360.0){ang_update=ang_update-360.0;}
//			std::uniform_real_distribution<double> angupdate_rand (0,360);
//			ang_update = angupdate_rand (gen);
			printf("ang_update = %f %03.15e %03.15e\n ", ang_update, fabs(dist), (double)dmax);

			HIP = velocity_USR/1000;
			
			incX=cos(ang_update*pi/180)*HIP;

			incY=sin(ang_update*pi/180)*HIP;

			printf("incX = %f\n ", incX);
			printf("incY = %f\n ", incY);
			if (interIA==1){tempoDADOS=0; interIA=0; tempoIA=1; SNRmax=0; update=1; cont_ray=0; interCONT=0; interSNRmax=0;continue;}

			if (tempoDADOS==1){

				if (protocolo == 1){continterupcaoIA=0; tempoDADOS=0; interIA=0; tempoIA=1; update=1; continterupcao=0; cont_ray=0; SNRmax=0;continue;}
				else if(protocolo == 2){tempoDADOS=0; interIA=0; tempoIA=1; update=1; SNRmovel=0; continterupcao=0; cont_ray=0; SNRmax=0;continue;}
			}			
		
			
							
		} else if(restoMV==0){

			std::uniform_real_distribution<double> pointup_rand (0,100);
			float point_rand = pointup_rand (gen);

			if(point_rand <= 33.33){ang_update=ang_update+90.0;}
			else if(point_rand > 33.33 && point_rand<=66.66){ang_update=ang_update-90.0;}
			else if(point_rand >= 66.66){ang_update=ang_update;}
			
			if(ang_update>360.0){ang_update=ang_update-360.0;}
			else if(ang_update<0.0){ang_update=360.0+ang_update;}

			HIP = velocity_USR/1000;
			
			incX=cos(ang_update*pi/180)*HIP;

			incY=sin(ang_update*pi/180)*HIP;

			cont_move=0;
		
		}
		
		//##########################################################################
		
		if(tempoIA==1){SNRmovelll=0; SNRmaxx=0; contx=0;}
		else{
			SNRmaxx=SNRmax;
			contx++;
			if(contx==1){SNRmovelll=SNRmax;}
			else {SNRmovelll= (SNRmovelll*0.95) + (SNR*0.05);}
		}		
		
		if(trace) {fprintf(tracefd,"%5d %d %d %d %f %f %f %f %f %f %f %f %f %d %d %d %f %f %d %d %d %f %f\n",Tsimulacao, tempoIA, tempoDADOS, interIA, BSbeam, USRbeam, NbeamBS, NbeamUSR, AFpuro, 10*log10(SNR), 10*log10(SNRmax), BeamBSdados, BeamUSRdados, contDADOS, contadorIA, contfase, totalCap, log2(SNRmax + 1), certo, ZERO, certoZERO, BeamMelhorBS, BeamMelhorUSR);}

		

		if(protocolo==3){// GOD
			if (contIA != 0){ restoIA = contIA % ((M_BS+N_BS)*(M_UE+N_UE));}

			if(restoIA==0){restoIA=1; contIA=0; beamUSR=0; beamBS=0; totalCap=totalCap+(log2(SNRmax + 1)); if(log2(SNRmax + 1)<14.0){contX++;} SNRmax=0; Tsimulacao++; contDADOS++; godTsimulacao=0; }
			if (Tsimulacao==0){ Tsimulacao++; contDADOS++;}

			USRbeam=fi_UE[beamUSR];
			BSbeam=fi_BS[beamBS];
		
			if(beamUSR<(M_UE+N_UE-1)){beamUSR=beamUSR+1;}else{beamBS=beamBS+1; beamUSR=0;}
		
			contIA++;
			godTsimulacao++;
			
	

		}else{
			
			Tsimulacao++;

			 if(tempoDADOS==1){
				 if(SNR < pow(10.0,10/10)){contador=1; contadortot++;}else{contador=0;}
			}


			if(tempoIA==1){
				if(verificaIA==0){SNRmax=0;}
				verificaIA=1;
				contadorIA++;
				continterupcaoIA++;
				Tout++;

				if (algorithm == 0) { // Exaustivo
			
					if (contIA != 0){ restoIA = contIA % ((M_BS+N_BS)*(M_UE+N_UE));}

					printf("restoIA %d\n ",restoIA);

					if(restoIA==0) { beamUSR=0; beamBS=0; nIA++; }
					if((SNRmax<=pow(10.0,SNRmin/10))&&(restoIA==0)&&(teste==0)&&(Tout>1000)){Tout=0; update=1; teste=1; continue;}
					if((SNRmax>=pow(10.0,SNRmin/10))&&(restoIA==0)){verificaIA=0; USRbeam=BeamUSRdados; BSbeam=BeamBSdados; restoIA=1; contIA=0; tempoIA=0; tempoDADOS=1; beamUSR=0; beamBS=0; SNR=SNRmax; continue;
					}else{
						printf("passei aqui \n\n\n ");
						USRbeam=fi_UE[beamUSR];
						BSbeam=fi_BS[beamBS];
		
						if(beamUSR<(M_UE+N_UE-1)){
			
							beamUSR=beamUSR+1;
						printf("passei aqui \n\n\n ");
		
						}else{
	
							beamBS=beamBS+1;
							printf("beamBS %d\n\n ",beamBS);
							beamUSR=0;
						printf("passei aqui \n\n\n ");
						}
		
						contIA++;
						teste=0;
						printf("contIA %d\n ",contIA);
					}

			
				}else if(algorithm == 1){ //GPS

			
					if (contIA != 0){restoIA = contIA % (M_UE+N_UE);}

					printf("restoIA %d\n ",restoIA);

					if(restoIA==0) { beamUSR=0; beamBS=0;  nIA++;}
					if((SNRmax<=pow(10.0,SNRmin/10))&&(restoIA==0)&&(teste==0)&&(Tout>1000)){Tout=0; update=1; teste=1; continue;}
					if((SNRmax>=pow(10.0,SNRmin/10))&&(restoIA==0)){verificaIA=0; USRbeam=BeamUSRdados; BSbeam=BeamBSdados; restoIA=1; contIA=0; tempoIA=0; tempoDADOS=1; beamUSR=0; beamBS=0; SNR=SNRmax; continue;
					}else{
		
						USRbeam=fi_UE[beamUSR];
						BSbeam = fiBS;
		
						if(beamUSR<(M_UE+N_UE-1)){
			
							beamUSR=beamUSR+1;
		
						}else{
	
							beamUSR=0;
						}
		
						contIA++;
						teste=0;
						printf("contIA %d\n ",contIA);
					}
			
				
		
				/* OLD VERSION
				}else if(algorithm == 2){ //GPS Refinado

					if (contIA != 0){restoIA = contIA % ((M_UE+N_UE)*3);}

					printf("restoIA %d\n ",restoIA);
				
					// terminou IA
					if(restoIA==0) { beamUSR=0; beamBS=0;  adjacente=-(360/(M_BS+N_BS)); nIA++;}

					// terminou com falha
					if((SNRmax<=pow(10.0,SNRmin/10))&&(restoIA==0)&&(teste==0)&&(Tout>1000)){Tout=0; update=1; teste=1; continue;}

					// terminou IA com sucesso
					if((SNRmax>=pow(10.0,SNRmin/10))&&(restoIA==0)){

						verificaIA=0; USRbeam=BeamUSRdados; BSbeam=BeamBSdados; restoIA=1; contIA=0; tempoIA=0; tempoDADOS=1; beamUSR=0; beamBS=0; SNR=SNRmax; adjacente=-(360/(M_BS+N_BS)); continue; 

					} else {
		
						USRbeam=fi_UE[beamUSR];
						BSbeam = (fiBS+adjacente);
						printf("adjacente %f\n ",adjacente);
						if(beamUSR<(M_UE+N_UE-1)){
			
							beamUSR=beamUSR+1;
		
						}else{
							adjacente=(adjacente+(360/(M_BS+N_BS)));
							beamUSR=0;
						}
		
						contIA++;
						teste=0;
						printf("contIA %d\n ",contIA);
					}
			
				*/
		
				}else if(algorithm == 2){ //GPS Refinado

					if (contIA != 0){restoIA = contIA % ((M_UE+N_UE)*(2*nAdjacentes + 1));}

					printf("restoIA %d\n ",restoIA);
				
					// terminou IA
					if(restoIA==0) { beamUSR=0; beamBS=0; modificador=-nAdjacentes; nIA++;}

					// terminou com falha
					if((SNRmax<=pow(10.0,SNRmin/10))&&(restoIA==0)&&(teste==0)&&(Tout>1000)){Tout=0; update=1; teste=1; continue;}

					// terminou IA com sucesso
					if((SNRmax>=pow(10.0,SNRmin/10))&&(restoIA==0)){
						verificaIA=0; USRbeam=BeamUSRdados; BSbeam=BeamBSdados; restoIA=1; contIA=0; 
						tempoIA=0; tempoDADOS=1; beamUSR=0; beamBS=0; SNR=SNRmax; 
						continue; 

					// não terminou
					} else {
		
						USRbeam=fi_UE[beamUSR];
						BSbeam = fiBS + modificador*tamFeixe;
						printf("modificador %d\n ",modificador);
						if(beamUSR<(M_UE+N_UE-1)){
			
							beamUSR=beamUSR+1;
		
						}else{
							modificador++;
							beamUSR=0;
						}
		
						contIA++;
						teste=0;
						printf("contIA %d\n ",contIA);
					}
			
				
		
				/* OLD
				}else if(algorithm == 3){ //Proposta refinado
			

					if (contIA != 0){restoIA = contIA % ((M_UE+N_UE)*mult);}

					printf("restoIA %d\n ",restoIA);
					
					if(restoIA==0) { beamUSR=0; beamBS=0;  adjacente=-(360/(M_BS+N_BS)); alg=1; nIA++;}
					if((SNRmax<=pow(10.0,SNRmin/10))&&(restoIA==0)&&(teste==0)&&(Tout>1000)){Tout=0; update=1; teste=1; continue;}
					if((SNRmax>=pow(10.0,SNRmin/10))&&(restoIA==0)){verificaIA=0; USRbeam=BeamUSRdados; BSbeam=BeamBSdados; restoIA=1; contIA=0; tempoIA=0; tempoDADOS=1; beamUSR=0; beamBS=0; SNR=SNRmax; adjacente=-(360/(M_BS+N_BS)); alg=1; continue; }else{
		
						if(alg==1){printf("parte1111111\n ");
							USRbeam=fi_UE[beamUSR];
							BSbeam = (fiBS+(adjacente*2));
							printf("adjacente %f\n ",adjacente*2);
							if(beamUSR<(M_UE+N_UE-1)){
			
								beamUSR=beamUSR+1;
		
							}else{
								adjacente=(adjacente+(360/(M_BS+N_BS)));
								beamUSR=0;
							}
							contalg++;
							if(contalg==((M_UE+N_UE)*3)){
								contalg=0; alg=0; adjacente=-(360/(M_BS+N_BS)); max=SNRmax;
								if(SNRmax>=pow(10.0,SNRmin/10)){fiBS=BeamBSdados; mult=5;}else{mult=7;}
							}

						}else{printf("parte2222222222\n ");
					
							if(max>=pow(10.0,SNRmin/10)){
								USRbeam=fi_UE[beamUSR];
								BSbeam = (fiBS+(adjacente));
								printf("adjacente %f\n ",adjacente);
								if(beamUSR<(M_UE+N_UE-1)){
			
									beamUSR=beamUSR+1;
		
								}else{
									adjacente=(adjacente+(720/(M_BS+N_BS)));
									beamUSR=0;
								}
							}else{

								USRbeam=fi_UE[beamUSR];
								BSbeam = (fiBS+(adjacente*3));
								printf("adjacente %f\n ",adjacente);
								if(beamUSR<(M_UE+N_UE-1)){
			
									beamUSR=beamUSR+1;
		
								}else{
									adjacente=(adjacente+((720/3)/(M_BS+N_BS)));
									beamUSR=0;
								}
							}
				
						}
						contIA++;
						teste=0;
						printf("contIA %d\n ",contIA);
					}
				*/


				}else if(algorithm == 3){ //Proposta refinado
			

					// fim da 1a fase
					if((fase == 1) && (contfase == ((M_UE+N_UE)*(2*nAdjacentes + 1)))){
						contfase=0; 

						if(SNRmax>=pow(10.0,SNRmin/10)){ // sucesso na 1a fase
							fase=2; max=SNRmax;
							fiBS=BeamBSdados; modificador=-1;
						} else { // falha na 1a fase
							modificador = -2*nAdjacentes;
							// muitos slots sem sucesso
							if((teste==0)&&(Tout>1000)){
								beamUSR=0; beamBS=0; modificador=-2*nAdjacentes; fase=1; nIA++;
								Tout=0; update=1; teste=1; 
								continue;
							}
						}
					// fim da 2a fase
					} else if ((fase == 2) && (contfase == ((M_UE+N_UE)*2))) {

						// terminou IA
						beamUSR=0; beamBS=0; modificador=-2*nAdjacentes; fase=1; nIA++;
						verificaIA=0; USRbeam=BeamUSRdados; BSbeam=BeamBSdados; restoIA=1; contIA=0; 
						tempoIA=0; tempoDADOS=1; beamUSR=0; beamBS=0; SNR=SNRmax; fase=1; 
						continue; 
					
					}

					// 1a fase
					if(fase==1){printf("parte1111111\n ");

						USRbeam=fi_UE[beamUSR];
						BSbeam = fiBS + modificador*tamFeixe;
						printf("modificador %d\n ",modificador);
						if(beamUSR<(M_UE+N_UE-1)){
		
							beamUSR=beamUSR+1;
	
						}else{
							modificador = modificador + 2;
							beamUSR=0;
						}

					// 2a fase
					}else{printf("parte2222222222\n ");
				
						USRbeam=fi_UE[beamUSR];
						BSbeam = fiBS + modificador*tamFeixe;
						printf("modificador %d\n ",modificador);
						if(beamUSR<(M_UE+N_UE-1)){
	
							beamUSR=beamUSR+1;

						}else{
							modificador = 1;
							beamUSR=0;
						}
			
					}
					contfase++;
					contIA++;
					teste=0;
					printf("contIA %d\n ",contIA);

				/* OLD
				}else if(algorithm == 4){ //Proposta iterativo


					if (contIA != 0){restoIA = contIA % ((M_UE+N_UE)*7);}

					printf("restoIA %d\n ",restoIA);

					if(restoIA==0) { beamUSR=0; beamBS=0;  adjacente=-(360/(M_BS+N_BS)); alg=1; nIA++;}
					if((SNRmax<=pow(10.0,SNRmin/10))&&(restoIA==0)&&(teste==0)&&(Tout>1000)){Tout=0; update=1; teste=1; continue;}
					if((SNRmax>=pow(10.0,SNRmin/10))&&(restoIA==0)){verificaIA=0; USRbeam=BeamUSRdados; BSbeam=BeamBSdados; restoIA=1; contIA=0; tempoIA=0; tempoDADOS=1; beamUSR=0; beamBS=0; SNR=SNRmax; adjacente=-(360/(M_BS+N_BS)); alg=1; continue; }else{
		
						if(alg==1){printf("parte1111111\n ");
							M_BS=3; 
							N_BS=3;
							USRbeam=fi_UE[beamUSR];
							BSbeam = (fiBS+(adjacente));
							printf("adjacente %f\n ",adjacente);
							if(beamUSR<(M_UE+N_UE-1)){
			
								beamUSR=beamUSR+1;
		
							}else{
								adjacente=(adjacente+(360/(M_BS+N_BS)));
								beamUSR=0;
							}
							contalg++;
							if(contalg==((M_UE+N_UE)*3)){
								contalg=0; adjacente=-(360/(M_BS+N_BS)); beamUSR=0; beamUSR=0;
					
								if(SNRmax>=pow(10.0,SNRmin/10)){alg=0; fiBS=BeamBSdados;}
							}

						}else{printf("parte2222222222\n ");
							M_BS=12;
							N_BS=12;
							USRbeam=fi_UE[beamUSR];
							BSbeam = (fiBS+((adjacente*3)/8));
							printf("adjacente %f\n ",adjacente);
							if(beamUSR<(M_UE+N_UE-1)){
			
								beamUSR=beamUSR+1;
		
							}else{
								adjacente=(adjacente+(((360/(M_BS+N_BS))*8)/3));
								beamUSR=0;
							}
					
				
						}
						contIA++;
						teste=1;
						printf("contIA %d\n ",contIA);
					}
				
				*/
				}

			}//fim tempo IA ######################################################################################

	
			else if (tempoDADOS==1){

				if((contDADOS+contIA)<=limite){
					afmaxDADOS=0;
					if(BeamMelhorUSR==USRbeam && BeamMelhorBS==BSbeam){certo++;}
					if(SNR < pow(10.0,10/10)){ZERO++; SNR=0; printf("SNR zerou \n"); 
						if(BeamMelhorUSR==USRbeam && BeamMelhorBS==BSbeam){certoZERO++;}
					}
					USRbeam=BeamUSRdados;
					BSbeam=BeamBSdados;
					//totalSNR = totalSNR + SNR;
					totalCap=totalCap+(log2(SNR +1));
					printf("SNR %f\n ",SNR);
					contDADOS++;
				}else{printf("acabou simulação \n ");}
			
				if (protocolo == 1){
	
					continterupcao ++;
					int restointerupcao = (continterupcaoIA + continterupcao) % intervaloFixoIA;
					printf("continterupcao %d ", continterupcao);
					printf("restointerupcao %d ", restointerupcao);
					printf("intervaloFixoIA %d\n ", intervaloFixoIA);
					if (restointerupcao==0){interUSRbeam=USRbeam; interBSbeam=BSbeam; continterupcaoIA=0; tempoDADOS=0; interIA=1; tempoIA=0; update=0; continterupcao=0; cont_ray=0;}
	

				}else if(protocolo == 2){
				//################ Média Móvel ######################
					if(SNRmovel <= 0){SNRmovel=SNRmax;}
					SNRmovel= (SNRmovel*0.8) + (SNR*0.2);
				//###################################################
			
					if((log2(SNRmax +1)*(1.0 - decaimentoTaxaRx)) >  log2(SNRmovel +1)){interUSRbeam=USRbeam; interBSbeam=BSbeam; tempoDADOS=0; interIA=1; tempoIA=0; update=0; SNRmovel=0; continterupcao=0; cont_ray=0;}

				}else if(protocolo == 4){
				//################ Média Móvel ######################
					if(SNRmovel <= 0){SNRmovel=SNRmax;}
					SNRmovel= (SNRmovel*0.8) + (SNR*0.2);
				//###################################################
			
					if((log2(SNRmax +1)-(quedaTaxaRx)) >  log2(SNRmovel +1)){interUSRbeam=USRbeam; interBSbeam=BSbeam; tempoDADOS=0; interIA=1; tempoIA=0; update=0; SNRmovel=0; continterupcao=0; cont_ray=0;}
					if(0.5 >  log2(SNRmovel +1)){interUSRbeam=USRbeam; interBSbeam=BSbeam; tempoDADOS=0; interIA=1; tempoIA=0; update=0; SNRmovel=0; continterupcao=0; cont_ray=0;}	
				}
			}//fim tempo DADOS ####################################################################################################


			if (interIA==1){

				if (!fastIA) { tempoDADOS=0; interIA=0; tempoIA=1; SNRmax=0; update=1; cont_ray=0; interCONT=0; interSNRmax=0; continue; }

				if(interCONT>7){
				
					if((log2(SNRmax +1)*(limiarFastIA)) > log2(interSNRmax +1)){tempoDADOS=0; interIA=0; tempoIA=1; SNRmax=0; update=1; cont_ray=0; interCONT=0; interSNRmax=0;continue;}
					else{tempoDADOS=1; interIA=0; tempoIA=0; update=0; USRbeam=BeamUSRdados; BSbeam=BeamBSdados; interCONT=0; interSNRmax=0;continue;}
				}
				if(interSNRmax<=SNR){interSNRmax=SNR; BeamUSRdados=USRbeam; BeamBSdados=BSbeam;}

				if (interCONT==0) {USRbeam=interUSRbeam-(360/(M_UE+N_UE)); BSbeam=interBSbeam-(360/(M_BS+N_BS));}
				else if (interCONT==1) {USRbeam=interUSRbeam+(360/(M_UE+N_UE)); BSbeam=interBSbeam-(360/(M_BS+N_BS));}
				else if (interCONT==2) {USRbeam=interUSRbeam-(360/(M_UE+N_UE)); BSbeam=interBSbeam+(360/(M_BS+N_BS));}
				else if (interCONT==3) {USRbeam=interUSRbeam+(360/(M_UE+N_UE)); BSbeam=interBSbeam+(360/(M_BS+N_BS));}
				else if (interCONT==4) {USRbeam=interUSRbeam; BSbeam=interBSbeam+(360/(M_BS+N_BS));}
				else if (interCONT==5) {USRbeam=interUSRbeam; BSbeam=interBSbeam-(360/(M_BS+N_BS));}
				else if (interCONT==6) {USRbeam=interUSRbeam+(360/(M_UE+N_UE)); BSbeam=interBSbeam;}
				else if (interCONT==7) {USRbeam=interUSRbeam-(360/(M_UE+N_UE)); BSbeam=interBSbeam;}
				if (USRbeam<0){USRbeam=360.0+USRbeam;}
				if (BSbeam<0){BSbeam=360.0+BSbeam;}
				if (USRbeam>360.0){USRbeam=USRbeam-360.0;}
				if (BSbeam>360.0){BSbeam=BSbeam-360.0;}
				interCONT ++;
				contadorIA++;
			

			
							
			}//fim interIA ###############################################################################################

		}

	}
	printf("contDADOS %d\n ",contDADOS);
	//printf("contador %d\n ",contador);
	printf("totalCap %f\n ",totalCap);

	float avgcapacidade;
	float avgcapacidadeEf;
	float avgZERO;
	float avgBEAM;
	if (contDADOS > 0) {
		avgcapacidade=(totalCap/contDADOS);
		avgcapacidadeEf=(totalCap/Tsimulacao);
		avgZERO=(float)ZERO/(float)contDADOS;
		avgBEAM=(float)certo/(float)contDADOS;
	} else {
		avgcapacidade=0.0;
		avgcapacidadeEf=0.0;
		avgZERO=0.0;
		avgBEAM=0.0;
	}
	float avgBEAMZERO;
	if (ZERO > 0)
		avgBEAMZERO=(float)certoZERO/(float)ZERO;
	else
		avgBEAMZERO=0.0;
	
	//if(contIA>=limite){avgcapacidade=0; }
	printf("avgcapacidade %f\n ",avgcapacidade);
	plot_results(algorithm, protocolo, decaimentoTaxaRx, velocity_USR, mediaErroGPS, avgcapacidade, avgcapacidadeEf, contadorIA, nIA, totalSNR, avgZERO, avgBEAM, avgBEAMZERO);
	
	if(trace) { fclose(tracefd);}

	return(0);

}
