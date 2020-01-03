#!/usr/bin/octave-cli -qf

## -*- texinfo -*-
## @deftypefn {Function File} {} ci (@var{infile}, @var{dist_type})
##
## Compute confidence interval according to [1] and [2].
##
## [1] Jain, R.; "The Art of Computer Systems Performance Analysis -
## Techniques for Experimental Design, Measurement, Simulation, and
## Modeling"; 1st edition; John Wiley & Sons, Inc.; 1991.
##
## [2] Waner, S.; Costenoble S. R.; "Confidence Intervals
## Miscellaneous on-line topics for Finite Mathematics 2e";
## http://people.hofstra.edu/Stefan_Waner/RealWorld/finitetopic1/confint.html.
##
## @end deftypefn

## Author: Kleber <kleber@gta.ufrj.br>
## Description: ci (Confidence Interval)

if (nargin < 4)
   printf ("Usage: ci <Input file> <Output file> <Number of variables> <Confidence level> [<Sample distribution>]\n");
   printf ("Confidence level: 90, 95, 99...\n");
   printf ("Sample distribution: 0 (Normal) or 1 (T-student - default)\n");
   printf ("\nExample: ci in.dat out.dat 3 99\n");
   exit(1);
endif;

arg_list = argv ();

sampleDist = 1;
if (nargin == 5)
  sampleDist = str2num(arg_list{5});
endif

##ci (arg_list{1}, arg_list{2}, str2num(arg_list{3}), str2num(arg_list{4}), sampleDist);
## function ci (INFILE, OUTFILE, NUM_VARS, CONF_LEVEL, SAMPLE_DIST)

INFILE = arg_list{1};
OUTFILE = arg_list{2};
NUM_VARS = str2num(arg_list{3});
CONF_LEVEL = str2num(arg_list{4});
SAMPLE_DIST = sampleDist;

	matrixOfValues = load (INFILE);
	FIDOUT = fopen (OUTFILE, "at", "native");
	alfa = 1 - CONF_LEVEL/100;

	[numOfRows, numOfCols] = size (matrixOfValues);
	if (numOfCols < (NUM_VARS + 1))
		printf("Variables in the file: %d\n", (numOfCols - 1));
		printf("Variables you asked for: %d\n", NUM_VARS);
		exit(1);
	endif;

	sampleSize = numOfRows;
	if ((SAMPLE_DIST == 0) && (sampleSize < 30))
		printf("Sample size too small for Normal distribution: %d\n", sampleSize);
		exit(1);
	endif;

	meanOfValues = mean(matrixOfValues,1);
	if ((SAMPLE_DIST == 1) || (sampleSize < 30))
	# using T-student distribution
		if (sampleSize < 30)
			printf("Small sample size: %d.\nIt should be used only when the population has a Normal distribution.\n", sampleSize);
		endif;
		# ci = (t[1 - alfa/2; n -1]) * s / sqrt(n)
		ciOfValues = tinv((1 - alfa/2),(size(matrixOfValues,1) - 1)) * std(matrixOfValues)/sqrt(size(matrixOfValues,1));
	else
	# using Normal distribution
		# ci = (z[1 - alfa/2]) * s / sqrt(n)
		ciOfValues = norminv(1 - alfa/2)*std(matrixOfValues)/sqrt(size(matrixOfValues,1));
	endif;

	fprintf(FIDOUT, "%.10f\t", meanOfValues(1,1));
	for i=2:(NUM_VARS + 1);
		fprintf(FIDOUT, "%.10f\t %.10f\t", meanOfValues(i), ciOfValues(i));
	endfor;
	fprintf(FIDOUT, "\n");

## endfunction;

