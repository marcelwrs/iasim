//
// Created by marcel on 20/12/2019.
//
#include <iostream>
#include <chrono>
#include <random>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <cstdlib>
#include <ctime>
#include <string.h>
#include "options.cpp"

// Random variables
std::default_random_engine gen (cenario);
std::normal_distribution<double> los_rand (0.0,5.8);
std::normal_distribution<double> nlos_rand (0.0,8.7);
std::uniform_real_distribution<double> uniform_rand (0,1);
std::uniform_real_distribution<double> angle_rand (0,360);

// Initializes basic information
void initialize()
{
	float angle_BS = 0;
	float angle_UE = 0;
	// Creating codebook
	for(int l = 0; l<M_UE+N_UE; l++){
		fi_UE[l] = angle_UE;
		angle_UE = angle_UE + 360.0/(M_UE+N_UE);
		if (verbose) printf("fi_UE %f\n ", fi_UE[l]);
	}

	for(int k = 0; k<M_BS+N_BS; k++){
		if(algorithm == 4){ 
			if (verbose) printf("algorithm 4\n\n");
			fi_BS[k] = angle_BS + 7.5;
		} else
			fi_BS[k] = angle_BS;

		angle_BS = angle_BS + 360.0/(M_BS+N_BS);
		if (verbose) printf("fi_BS %f\n ", fi_BS[k]);
	}

	// Draw geolocation and sets fi_UB and fi_BU
	fi_BU = angle_rand (gen);
	fi_UB = fmod(fi_BU + 180, 360);
	if (verbose) printf("fi_BU %f ", fi_BU);
	if (verbose) printf("fi_UB %f\n ", fi_UB);
	// Calculate X and Y from dist and angle
	X=cos(fi_BU*pi/180)*dist;
	Y=sin(fi_BU*pi/180)*dist;
	if (verbose) printf("X= %f ", X);
	if (verbose) printf("Y= %f\n ", Y);
	
	float x;
	float y;

	// Draw geolocation error
	std::normal_distribution<double> dist_error_rand (mediaErroGPS, desvErroGPS);
	std::uniform_real_distribution<double> dist_angle_rand (0, 360);
	double angle_error = dist_angle_rand(gen);
	if (verbose) printf("angle_error = %f\n ", angle_error);
	double dist_error = fabs(dist_error_rand(gen));
	if (verbose) printf("dist_error = %f\n ", dist_error);

	// Calculate geolocation plus error
	x = X + dist_error*cos(angle_error*pi/180);
	y = Y + dist_error*sin(angle_error*pi/180);
	if (verbose) printf("x_error = %f\n ", x);
	if (verbose) printf("y_error = %f\n ", y);

	if(y>0 && x>0) fi_BU_erro = ((atan(y/x) * 180) / pi);
	if(y>0 && x<0) fi_BU_erro = ((atan(y/x) * 180) / pi) + 180;
	if(y<0 && x<0) fi_BU_erro = ((atan(y/x) * 180) / pi) + 180;
	if(y<0 && x>0) fi_BU_erro = ((atan(y/x) * 180) / pi) + 360;
	
	if (verbose) printf("fi_BU_erro %f\n  ", fi_BU_erro);

	dif_BS =360.0;
	for(int p = 0; p<M_BS+N_BS; p++){

		dif = fabs(fi_BS[p]-fi_BU_erro);

		if(dif_BS >= dif){
			dif_BS = dif;  
			fiBS = fi_BS[p];
		}

		if(algorithm == 4){
			if (verbose) printf("algoritmo 4\n\n");
			if(fabs(367.5-fi_BU_erro)<dif_BS) fiBS = fi_BS[0];
		} else
			if(fabs(360-fi_BU_erro)<dif_BS) fiBS = fi_BS[0];
	}
	if (verbose) printf("fiBSsssss %f\n  ", fiBS);

	// Path loss random params
	if (verbose) printf("dist %f\n ", dist);
	Pout = (1 - exp(((-0.033333333*dist) + 5.2)));
	if (Pout<0) Pout=0;
	Plos = ((1 - Pout)*(exp(-0.014903129*dist)));
	Pnlos = (1 - Pout - Plos);
	if (verbose) printf("Pout final %f ", Pout);
	if (verbose) printf("Plos final %f ", Plos);
	if (verbose) printf("Pnlos final %f\n ", Pnlos);

	real = uniform_rand(gen);
	normallos = los_rand(gen);
	normalnlos = nlos_rand(gen);

	if (verbose) printf("real %f\t ", real);
	if (verbose) printf("normallos %f\t ", normallos);
	if (verbose) printf("normalnlos %f\n ", normalnlos);
}


// Main
int
main (int argc, char *argv[])
{
	// Get options from command line
	readParams(argc, argv);

	initialize();

	exit(0);
}
