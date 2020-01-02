#! /usr/bin/tclsh

# Parametros gerais
set ic			95		;# Intervalo de confian�a

# Parametros da simula��o
set distMax		100
set cenario             50
set npontos		50   ;# deprecated
set dir			results
set div			4		;# Fator que divide o lambda do espa�amento das antenas
set Pt			30
set NF			5
set TN			-174
set BW			1000000000
set minSNR		-5
set Tper                1
set Tcanal              1
set tipoErro		1  ;# 0-Uniforme | 1-Distribui��o normal
set desvErroGPS		10
set log			0
set stop		120000  ;# tempo de simulacao
set move		75000
set fastIA		0	
set velocityOBJ	        10
set limFastIA		0

set val_condCanal       {1 2} ;# 1-LOS | 2-NLOS | 3-AUTO
set val_velocityUSR     {1 3 5 7 9}
set val_mediaErroGPS	{5 10}
set val_algorithm	{0 1 2 3 4}  ;# 0-Exaustivo | 1-GPS | 2-GPS refinado | 3-Proposta intercalado | 4-Proposta iterativo
set val_protocolo       {{2 0.5}}  ;#  0-Não refaz IA | 1-Intervalo fixo | 2-Limiar de SNR | 3- Good
set val_distMax		{25 100}

# Scrip de simula��o
switch [lindex $argv 0] {

	"MAKE" {
		exec g++ -std=c++11  ia.cpp -o ia
	}

	"SUBMIT" {
  		# Cria o diretorio de simula�ao
		if {[file exists $dir]} {
			puts "Diret�rio $dir j� existe."
			set a 1
			set bkp_old "$dir-$a"
			while {[file exists $bkp_old]} {
				incr a
				set bkp_old $dir-$a
			}
			exec mv $dir $bkp_old
		}
		exec mkdir $dir

		foreach alg $val_algorithm {

			foreach condCanal $val_condCanal {

				foreach mediaErroGPS $val_mediaErroGPS {

					foreach velocityUSR $val_velocityUSR {

						foreach protocolo $val_protocolo {

							set protoID [lindex $protocolo 0]
							set protocolo [lreplace $protocolo 0 0]

							foreach protoParam $protocolo {

								foreach distMax $val_distMax {

									for {set run 1} {$run <= $cenario} {incr run} {

										puts "./ia $Pt $distMax $npontos $run $dir/alg_$alg.condCanal_$condCanal.protoID_$protoID.protoParam_$protoParam.mediaErroGPS_$mediaErroGPS.velocityUSR_$velocityUSR.dist_$distMax.run_$run $NF $TN $BW $div [expr ($distMax*750)/$velocityUSR] $minSNR $Tper $Tcanal [expr $stop*(20/$velocityUSR)] $tipoErro $mediaErroGPS [expr (9.0*$mediaErroGPS)-30.0] $alg $log $velocityUSR $velocityOBJ $protoID $protoParam $protoParam $protoParam $fastIA $limFastIA $condCanal"
										exec qsub -o /dev/null -e /dev/null -V -cwd -b y -shell n ./ia $Pt $distMax $npontos $run $dir/alg_$alg.condCanal_$condCanal.protoID_$protoID.protoParam_$protoParam.mediaErroGPS_$mediaErroGPS.velocityUSR_$velocityUSR.dist_$distMax.run_$run $NF $TN $BW $div [expr ($distMax*750)/$velocityUSR] $minSNR $Tper $Tcanal [expr $stop*(20/$velocityUSR)] $tipoErro $mediaErroGPS [expr (9.0*$mediaErroGPS)-30.0] $alg $log $velocityUSR $velocityOBJ $protoID $protoParam $protoParam $protoParam $fastIA $limFastIA $condCanal

									}
								}
							}
						}
					}
				}
			}
		}
	}

	"LOCAL" {
  		# Cria o diretorio de simula�ao
		if {[file exists $dir]} {
			puts "Diret�rio $dir j� existe."
			set a 1
			set bkp_old "$dir-$a"
			while {[file exists $bkp_old]} {
				incr a
				set bkp_old $dir-$a
			}
			exec mv $dir $bkp_old
		}
		exec mkdir $dir

		foreach alg $val_algorithm {

			foreach condCanal $val_condCanal {

				foreach mediaErroGPS $val_mediaErroGPS {

					foreach velocityUSR $val_velocityUSR {

						foreach protocolo $val_protocolo {

							set protoID [lindex $protocolo 0]
							set protocolo [lreplace $protocolo 0 0]

							foreach protoParam $protocolo {

								foreach distMax $val_distMax {

									for {set run 1} {$run <= $cenario} {incr run} {

										puts "./ia $Pt $distMax $npontos $run $dir/alg_$alg.condCanal_$condCanal.protoID_$protoID.protoParam_$protoParam.mediaErroGPS_$mediaErroGPS.velocityUSR_$velocityUSR.dist_$distMax.run_$run $NF $TN $BW $div [expr ($distMax*750)/$velocityUSR] $minSNR $Tper $Tcanal [expr $stop*(20/$velocityUSR)] $tipoErro $mediaErroGPS [expr (9.0*$mediaErroGPS)-30.0] $alg $log $velocityUSR $velocityOBJ $protoID $protoParam $protoParam $protoParam $fastIA $limFastIA $condCanal"
										exec ./ia $Pt $distMax $npontos $run $dir/alg_$alg.condCanal_$condCanal.protoID_$protoID.protoParam_$protoParam.mediaErroGPS_$mediaErroGPS.velocityUSR_$velocityUSR.dist_$distMax.run_$run $NF $TN $BW $div [expr ($distMax*750)/$velocityUSR] $minSNR $Tper $Tcanal [expr $stop*(20/$velocityUSR)] $tipoErro $mediaErroGPS [expr (9.0*$mediaErroGPS)-30.0] $alg $log $velocityUSR $velocityOBJ $protoID $protoParam $protoParam $protoParam $fastIA $limFastIA $condCanal

									}
								}
							}
						}
					}
				}
			}
		}
	}

	"CAT" {

		foreach alg $val_algorithm {

			foreach condCanal $val_condCanal {

				foreach mediaErroGPS $val_mediaErroGPS {

					foreach velocityUSR $val_velocityUSR {

						foreach protocolo $val_protocolo {

							set protoID [lindex $protocolo 0]
							set protocolo [lreplace $protocolo 0 0]

							foreach protoParam $protocolo {

								foreach distMax $val_distMax {

									for {set run 1} {$run <= $cenario} {incr run} {
							
										puts "cat results/alg_$alg.condCanal_$condCanal.protoID_$protoID.protoParam_$protoParam.mediaErroGPS_$mediaErroGPS.velocityUSR_$velocityUSR.dist_$distMax.run_$run >> results/alg_$alg.condCanal_$condCanal.protoID_$protoID.protoParam_$protoParam.mediaErroGPS_$mediaErroGPS.velocityUSR_$velocityUSR.dist_$distMax"
											
										exec cat results/alg_$alg.condCanal_$condCanal.protoID_$protoID.protoParam_$protoParam.mediaErroGPS_$mediaErroGPS.velocityUSR_$velocityUSR.dist_$distMax.run_$run >> results/alg_$alg.condCanal_$condCanal.protoID_$protoID.protoParam_$protoParam.mediaErroGPS_$mediaErroGPS.velocityUSR_$velocityUSR.dist_$distMax 
							
									}					

								}
							}
						}
					}
				}
			}
		}


		foreach alg $val_algorithm {

			foreach condCanal $val_condCanal {

				foreach mediaErroGPS $val_mediaErroGPS {

					foreach velocityUSR $val_velocityUSR {

						foreach protocolo $val_protocolo {

							set protoID [lindex $protocolo 0]
							set protocolo [lreplace $protocolo 0 0]

							foreach protoParam $protocolo {

								foreach distMax $val_distMax {

							
									puts "./ci.m results/alg_$alg.condCanal_$condCanal.protoID_$protoID.protoParam_$protoParam.mediaErroGPS_$mediaErroGPS.velocityUSR_$velocityUSR.dist_$distMax results/out-alg_$alg.condCanal_$condCanal.protoID_$protoID.protoParam_$protoParam.mediaErroGPS_$mediaErroGPS.velocityUSR_$velocityUSR.dist_$distMax 13 $ic"
									exec ./ci.m results/alg_$alg.condCanal_$condCanal.protoID_$protoID.protoParam_$protoParam.mediaErroGPS_$mediaErroGPS.velocityUSR_$velocityUSR.dist_$distMax results/out-alg_$alg.condCanal_$condCanal.protoID_$protoID.protoParam_$protoParam.mediaErroGPS_$mediaErroGPS.velocityUSR_$velocityUSR.dist_$distMax 13 $ic
								
								}
							}						
						}
					}
				}
			}
		}


		foreach alg $val_algorithm {

			foreach condCanal $val_condCanal {

				foreach mediaErroGPS $val_mediaErroGPS {

					foreach velocityUSR $val_velocityUSR {

						foreach protocolo $val_protocolo {

							set protoID [lindex $protocolo 0]
							set protocolo [lreplace $protocolo 0 0]

							foreach protoParam $protocolo {

								foreach distMax $val_distMax {
					
									puts "cat results/out-alg_$alg.condCanal_$condCanal.protoID_$protoID.protoParam_$protoParam.mediaErroGPS_$mediaErroGPS.velocityUSR_$velocityUSR.dist_$distMax >> results/out.velocity-alg_$alg.condCanal_$condCanal.protoID_$protoID.protoParam_$protoParam.mediaErroGPS_$mediaErroGPS.dist_$distMax"
									
									exec cat results/out-alg_$alg.condCanal_$condCanal.protoID_$protoID.protoParam_$protoParam.mediaErroGPS_$mediaErroGPS.velocityUSR_$velocityUSR.dist_$distMax >> results/out.velocity-alg_$alg.condCanal_$condCanal.protoID_$protoID.protoParam_$protoParam.mediaErroGPS_$mediaErroGPS.dist_$distMax
								
								}
							}
						}
					}
				}
			}
		}


		foreach alg $val_algorithm {

			foreach condCanal $val_condCanal {

				foreach mediaErroGPS $val_mediaErroGPS {

					foreach velocityUSR $val_velocityUSR {

						foreach protocolo $val_protocolo {

							set protoID [lindex $protocolo 0]
							set protocolo [lreplace $protocolo 0 0]

							foreach protoParam $protocolo {

								foreach distMax $val_distMax {
					
									puts "cat results/out-alg_$alg.condCanal_$condCanal.protoID_$protoID.protoParam_$protoParam.mediaErroGPS_$mediaErroGPS.velocityUSR_$velocityUSR.dist_$distMax >> results/out.protoParam-alg_$alg.condCanal_$condCanal.protoID_$protoID.mediaErroGPS_$mediaErroGPS.velocityUSR_$velocityUSR.dist_$distMax"
									
									exec cat results/out-alg_$alg.condCanal_$condCanal.protoID_$protoID.protoParam_$protoParam.mediaErroGPS_$mediaErroGPS.velocityUSR_$velocityUSR.dist_$distMax >> results/out.protoParam-alg_$alg.condCanal_$condCanal.protoID_$protoID.mediaErroGPS_$mediaErroGPS.velocityUSR_$velocityUSR.dist_$distMax
								
								}
							}
						}
					}
				}
			}
		}

	}



		

	"PLOT" {


		foreach condCanal $val_condCanal {

			foreach mediaErroGPS $val_mediaErroGPS {

				foreach protocolo $val_protocolo {

					set protoID [lindex $protocolo 0]
					set protocolo [lreplace $protocolo 0 0]

					foreach protoParam $protocolo {


						foreach distMax $val_distMax {
							

							# Tempo Útil
							exec sed "s/<output>/velocity-tdados-protoID_$protoID.protoParam_$protoParam.condCanal_$condCanal.mediaErroGPS_$mediaErroGPS.dist_$distMax.eps/" plot-base.gnu | sed "s/<xlabel>/Velocidade (mps)/" | sed "s/<ylabel>/Tempo Util (%)/" | sed "s/<xmin>/*/" | sed "s/<xmax>/*/" | sed "s/<ymin>/0/" | sed "s/<ymax>/1/" > plot.gnu
							exec echo "set key top left" >> plot.gnu
							exec echo "plot \\" >> plot.gnu
				
							exec echo "\"results/out.velocity-alg_0.condCanal_$condCanal.protoID_$protoID.protoParam_$protoParam.mediaErroGPS_$mediaErroGPS.dist_$distMax\" u 8:14:15 w errorlines t \"Exaustivo\", \\" >> plot.gnu
							exec echo "\"results/out.velocity-alg_1.condCanal_$condCanal.protoID_$protoID.protoParam_$protoParam.mediaErroGPS_$mediaErroGPS.dist_$distMax\" u 8:14:15 w errorlines t \"GPS Simples\", \\" >> plot.gnu
							exec echo "\"results/out.velocity-alg_2.condCanal_$condCanal.protoID_$protoID.protoParam_$protoParam.mediaErroGPS_$mediaErroGPS.dist_$distMax\" u 8:14:15 w errorlines t \"GPS Refinado\", \\" >> plot.gnu
							exec echo "\"results/out.velocity-alg_3.condCanal_$condCanal.protoID_$protoID.protoParam_$protoParam.mediaErroGPS_$mediaErroGPS.dist_$distMax\" u 8:14:15 w errorlines t \"Proposta Refinado\", \\" >> plot.gnu
							exec echo "\"results/out.velocity-alg_4.condCanal_$condCanal.protoID_$protoID.protoParam_$protoParam.mediaErroGPS_$mediaErroGPS.dist_$distMax\" u 8:14:15 w errorlines t \"Proposta Iterativo\", \\" >> plot.gnu
						
							exec gnuplot plot.gnu

						
							}
						}
					}
				}
			}



		foreach condCanal $val_condCanal {

			foreach mediaErroGPS $val_mediaErroGPS {

				foreach protocolo $val_protocolo {

					set protoID [lindex $protocolo 0]
					set protocolo [lreplace $protocolo 0 0]

					foreach protoParam $protocolo {


						foreach distMax $val_distMax {
							

							# Capaciodade Efetiva
							exec sed "s/<output>/velocity-Cef-protoID_$protoID.protoParam_$protoParam.condCanal_$condCanal.mediaErroGPS_$mediaErroGPS.dist_$distMax.eps/" plot-base.gnu | sed "s/<xlabel>/Velocidade (mps)/" | sed "s/<ylabel>/Capacidade Efetiva (Gbps)/" | sed "s/<xmin>/*/" | sed "s/<xmax>/*/" | sed "s/<ymin>/0/" | sed "s/<ymax>/18/" > plot.gnu
							exec echo "set key top left" >> plot.gnu
							exec echo "plot \\" >> plot.gnu
				
							exec echo "\"results/out.velocity-alg_0.condCanal_$condCanal.protoID_$protoID.protoParam_$protoParam.mediaErroGPS_$mediaErroGPS.dist_$distMax\" u 8:20:21 w errorlines t \"Exaustivo\", \\" >> plot.gnu
							exec echo "\"results/out.velocity-alg_1.condCanal_$condCanal.protoID_$protoID.protoParam_$protoParam.mediaErroGPS_$mediaErroGPS.dist_$distMax\" u 8:20:21 w errorlines t \"GPS Simples\", \\" >> plot.gnu
							exec echo "\"results/out.velocity-alg_2.condCanal_$condCanal.protoID_$protoID.protoParam_$protoParam.mediaErroGPS_$mediaErroGPS.dist_$distMax\" u 8:20:21 w errorlines t \"GPS Refinado\", \\" >> plot.gnu
							exec echo "\"results/out.velocity-alg_3.condCanal_$condCanal.protoID_$protoID.protoParam_$protoParam.mediaErroGPS_$mediaErroGPS.dist_$distMax\" u 8:20:21 w errorlines t \"Proposta Refinado\", \\" >> plot.gnu
							exec echo "\"results/out.velocity-alg_4.condCanal_$condCanal.protoID_$protoID.protoParam_$protoParam.mediaErroGPS_$mediaErroGPS.dist_$distMax\" u 8:20:21 w errorlines t \"Proposta Iterativo\", \\" >> plot.gnu
						
							exec gnuplot plot.gnu

						
							}
						}
					}
				}
			}



		foreach condCanal $val_condCanal {

			foreach mediaErroGPS $val_mediaErroGPS {

				foreach protocolo $val_protocolo {

					set protoID [lindex $protocolo 0]
					set protocolo [lreplace $protocolo 0 0]

					foreach protoParam $protocolo {


						foreach distMax $val_distMax {
							

							# Capaciodade Nominal
							exec sed "s/<output>/velocity-Cnom-protoID_$protoID.protoParam_$protoParam.condCanal_$condCanal.mediaErroGPS_$mediaErroGPS.dist_$distMax.eps/" plot-base.gnu | sed "s/<xlabel>/Velocidade (mps)/" | sed "s/<ylabel>/Capacidade Efetiva (Gbps)/" | sed "s/<xmin>/*/" | sed "s/<xmax>/*/" | sed "s/<ymin>/0/" | sed "s/<ymax>/18/" > plot.gnu
							exec echo "set key top left" >> plot.gnu
							exec echo "plot \\" >> plot.gnu
				
							exec echo "\"results/out.velocity-alg_0.condCanal_$condCanal.protoID_$protoID.protoParam_$protoParam.mediaErroGPS_$mediaErroGPS.dist_$distMax\" u 8:18:19 w errorlines t \"Exaustivo\", \\" >> plot.gnu
							exec echo "\"results/out.velocity-alg_1.condCanal_$condCanal.protoID_$protoID.protoParam_$protoParam.mediaErroGPS_$mediaErroGPS.dist_$distMax\" u 8:18:19 w errorlines t \"GPS Simples\", \\" >> plot.gnu
							exec echo "\"results/out.velocity-alg_2.condCanal_$condCanal.protoID_$protoID.protoParam_$protoParam.mediaErroGPS_$mediaErroGPS.dist_$distMax\" u 8:18:19 w errorlines t \"GPS Refinado\", \\" >> plot.gnu
							exec echo "\"results/out.velocity-alg_3.condCanal_$condCanal.protoID_$protoID.protoParam_$protoParam.mediaErroGPS_$mediaErroGPS.dist_$distMax\" u 8:18:19 w errorlines t \"Proposta Refinado\", \\" >> plot.gnu
							exec echo "\"results/out.velocity-alg_4.condCanal_$condCanal.protoID_$protoID.protoParam_$protoParam.mediaErroGPS_$mediaErroGPS.dist_$distMax\" u 8:18:19 w errorlines t \"Proposta Iterativo\", \\" >> plot.gnu
						
							exec gnuplot plot.gnu

						
							}
						}
					}
				}
			}
		}

		

	"PLOT2" {

		foreach alg $val_algorithm {


			foreach mediaErroGPS $val_mediaErroGPS {


				foreach distMax $val_distMax {
							

					# Capaciodade Efitiva
					exec sed "s/<output>/velocity-Cef-alg_$alg.condCanal_1.mediaErroGPS_$mediaErroGPS.dist_$distMax.eps/" plot-base.gnu | sed "s/<xlabel>/Velocidade (m\\/s)/" | sed "s/<ylabel>/Capacidade Efetiva (Gbps)/" | sed "s/<xmin>/*/" | sed "s/<xmax>/*/" | sed "s/<ymin>/0/" | sed "s/<ymax>/18/" > plot.gnu
					exec echo "set key bottom left" >> plot.gnu
					exec echo "plot \\" >> plot.gnu
			
					exec echo "\"results/out.velocity-alg_$alg.condCanal_1.protoID_1.protoParam_500.mediaErroGPS_$mediaErroGPS.dist_$distMax\" u 8:20:21 w errorlines t \"FIXO-500ms\", \\" >> plot.gnu
					exec echo "\"results/out.velocity-alg_$alg.condCanal_1.protoID_1.protoParam_1000.mediaErroGPS_$mediaErroGPS.dist_$distMax\" u 8:20:21 w errorlines t \"FIXO-1000ms\", \\" >> plot.gnu
					exec echo "\"results/out.velocity-alg_$alg.condCanal_1.protoID_1.protoParam_2000.mediaErroGPS_$mediaErroGPS.dist_$distMax\" u 8:20:21 w errorlines t \"FIXO-2000ms\", \\" >> plot.gnu
					exec echo "\"results/out.velocity-alg_$alg.condCanal_1.protoID_2.protoParam_0.3.mediaErroGPS_$mediaErroGPS.dist_$distMax\" u 8:20:21 w errorlines t \"LIMIAR-30\\%\", \\" >> plot.gnu
					exec echo "\"results/out.velocity-alg_$alg.condCanal_1.protoID_2.protoParam_0.4.mediaErroGPS_$mediaErroGPS.dist_$distMax\" u 8:20:21 w errorlines t \"LIMIAR-40\\%\", \\" >> plot.gnu
					#exec echo "\"results/out.velocity-alg_$alg.condCanal_1.protoID_2.protoParam_0.5.mediaErroGPS_$mediaErroGPS.dist_$distMax\" u 8:20:21 w errorlines t \"LIMIAR-50\\%\", \\" >> plot.gnu
					exec gnuplot plot.gnu

				
				}
			}
		}


		foreach alg $val_algorithm {


			foreach mediaErroGPS $val_mediaErroGPS {


				foreach distMax $val_distMax {
							

					# Capaciodade Nominal
					exec sed "s/<output>/velocity-Cnom-alg_$alg.condCanal_1.mediaErroGPS_$mediaErroGPS.dist_$distMax.eps/" plot-base.gnu | sed "s/<xlabel>/Velocidade (m\\/s)/" | sed "s/<ylabel>/Capacidade Efetiva (Gbps)/" | sed "s/<xmin>/*/" | sed "s/<xmax>/*/" | sed "s/<ymin>/0/" | sed "s/<ymax>/18/" > plot.gnu
					exec echo "set key bottom left" >> plot.gnu
					exec echo "plot \\" >> plot.gnu
			
					exec echo "\"results/out.velocity-alg_$alg.condCanal_1.protoID_1.protoParam_500.mediaErroGPS_$mediaErroGPS.dist_$distMax\" u 8:18:19 w errorlines t \"FIXO-500ms\", \\" >> plot.gnu
					exec echo "\"results/out.velocity-alg_$alg.condCanal_1.protoID_1.protoParam_1000.mediaErroGPS_$mediaErroGPS.dist_$distMax\" u 8:18:19 w errorlines t \"FIXO-1000ms\", \\" >> plot.gnu
					exec echo "\"results/out.velocity-alg_$alg.condCanal_1.protoID_1.protoParam_2000.mediaErroGPS_$mediaErroGPS.dist_$distMax\" u 8:18:19 w errorlines t \"FIXO-2000ms\", \\" >> plot.gnu
					exec echo "\"results/out.velocity-alg_$alg.condCanal_1.protoID_2.protoParam_0.3.mediaErroGPS_$mediaErroGPS.dist_$distMax\" u 8:18:19 w errorlines t \"LIMIAR-30\\%\", \\" >> plot.gnu
					exec echo "\"results/out.velocity-alg_$alg.condCanal_1.protoID_2.protoParam_0.4.mediaErroGPS_$mediaErroGPS.dist_$distMax\" u 8:18:19 w errorlines t \"LIMIAR-40\\%\", \\" >> plot.gnu
					#exec echo "\"results/out.velocity-alg_$alg.condCanal_1.protoID_2.protoParam_0.5.mediaErroGPS_$mediaErroGPS.dist_$distMax\" u 8:18:19 w errorlines t \"LIMIAR-50\\%\", \\" >> plot.gnu
					exec gnuplot plot.gnu

				
				}
			}
		}

		foreach alg $val_algorithm {


			foreach mediaErroGPS $val_mediaErroGPS {


				foreach distMax $val_distMax {
							

					# Tempo util
					exec sed "s/<output>/velocity-tdados-alg_$alg.condCanal_1.mediaErroGPS_$mediaErroGPS.dist_$distMax.eps/" plot-base.gnu | sed "s/<xlabel>/Velocidade (m\\/s)/" | sed "s/<ylabel>/Capacidade Efetiva (Gbps)/" | sed "s/<xmin>/*/" | sed "s/<xmax>/*/" | sed "s/<ymin>/0/" | sed "s/<ymax>/1/" > plot.gnu
					exec echo "set key bottom left" >> plot.gnu
					exec echo "plot \\" >> plot.gnu
			
					exec echo "\"results/out.velocity-alg_$alg.condCanal_1.protoID_1.protoParam_500.mediaErroGPS_$mediaErroGPS.dist_$distMax\" u 8:14:15 w errorlines t \"FIXO-500ms\", \\" >> plot.gnu
					exec echo "\"results/out.velocity-alg_$alg.condCanal_1.protoID_1.protoParam_1000.mediaErroGPS_$mediaErroGPS.dist_$distMax\" u 8:14:15 w errorlines t \"FIXO-1000ms\", \\" >> plot.gnu
					exec echo "\"results/out.velocity-alg_$alg.condCanal_1.protoID_1.protoParam_2000.mediaErroGPS_$mediaErroGPS.dist_$distMax\" u 8:14:15 w errorlines t \"FIXO-2000ms\", \\" >> plot.gnu
					exec echo "\"results/out.velocity-alg_$alg.condCanal_1.protoID_2.protoParam_0.3.mediaErroGPS_$mediaErroGPS.dist_$distMax\" u 8:14:15 w errorlines t \"LIMIAR-30\\%\", \\" >> plot.gnu
					exec echo "\"results/out.velocity-alg_$alg.condCanal_1.protoID_2.protoParam_0.4.mediaErroGPS_$mediaErroGPS.dist_$distMax\" u 8:14:15 w errorlines t \"LIMIAR-40\\%\", \\" >> plot.gnu
					#exec echo "\"results/out.velocity-alg_$alg.condCanal_1.protoID_2.protoParam_0.5.mediaErroGPS_$mediaErroGPS.dist_$distMax\" u 8:14:15 w errorlines t \"LIMIAR-50\\%\", \\" >> plot.gnu
					exec gnuplot plot.gnu

				
				}
			}
		}
	}
				

	"PLOT3" {

		foreach alg $val_algorithm {


			foreach mediaErroGPS $val_mediaErroGPS {


				foreach distMax $val_distMax {
							

					# Capaciodade Efitiva
					exec sed "s/<output>/velocity-Cef-alg_$alg.condCanal_2.mediaErroGPS_$mediaErroGPS.dist_$distMax.eps/" plot-base.gnu | sed "s/<xlabel>/Velocidade (m\\/s)/" | sed "s/<ylabel>/Capacidade Efetiva (Gbps)/" | sed "s/<xmin>/*/" | sed "s/<xmax>/*/" | sed "s/<ymin>/0/" | sed "s/<ymax>/18/" > plot.gnu
					exec echo "set key top right" >> plot.gnu
					exec echo "plot \\" >> plot.gnu
			
					exec echo "\"results/out.velocity-alg_$alg.condCanal_2.protoID_1.protoParam_500.mediaErroGPS_$mediaErroGPS.dist_$distMax\" u 8:20:21 w errorlines t \"FIXO-500ms\", \\" >> plot.gnu
					exec echo "\"results/out.velocity-alg_$alg.condCanal_2.protoID_1.protoParam_1000.mediaErroGPS_$mediaErroGPS.dist_$distMax\" u 8:20:21 w errorlines t \"FIXO-1000ms\", \\" >> plot.gnu
					exec echo "\"results/out.velocity-alg_$alg.condCanal_2.protoID_1.protoParam_2000.mediaErroGPS_$mediaErroGPS.dist_$distMax\" u 8:20:21 w errorlines t \"FIXO-2000ms\", \\" >> plot.gnu
					exec echo "\"results/out.velocity-alg_$alg.condCanal_2.protoID_2.protoParam_0.3.mediaErroGPS_$mediaErroGPS.dist_$distMax\" u 8:20:21 w errorlines t \"LIMIAR-30\\%\", \\" >> plot.gnu
					exec echo "\"results/out.velocity-alg_$alg.condCanal_2.protoID_2.protoParam_0.4.mediaErroGPS_$mediaErroGPS.dist_$distMax\" u 8:20:21 w errorlines t \"LIMIAR-40\\%\", \\" >> plot.gnu
					#exec echo "\"results/out.velocity-alg_$alg.condCanal_2.protoID_2.protoParam_0.5.mediaErroGPS_$mediaErroGPS.dist_$distMax\" u 8:20:21 w errorlines t \"LIMIAR-50\\%\", \\" >> plot.gnu
					exec gnuplot plot.gnu

				
				}
			}
		}


		foreach alg $val_algorithm {


			foreach mediaErroGPS $val_mediaErroGPS {


				foreach distMax $val_distMax {
							

					# Capaciodade Nominal
					exec sed "s/<output>/velocity-Cnom-alg_$alg.condCanal_2.mediaErroGPS_$mediaErroGPS.dist_$distMax.eps/" plot-base.gnu | sed "s/<xlabel>/Velocidade (m\\/s)/" | sed "s/<ylabel>/Capacidade Efetiva (Gbps)/" | sed "s/<xmin>/*/" | sed "s/<xmax>/*/" | sed "s/<ymin>/0/" | sed "s/<ymax>/18/" > plot.gnu
					exec echo "set key top right" >> plot.gnu
					exec echo "plot \\" >> plot.gnu
			
					exec echo "\"results/out.velocity-alg_$alg.condCanal_2.protoID_1.protoParam_500.mediaErroGPS_$mediaErroGPS.dist_$distMax\" u 8:18:19 w errorlines t \"FIXO-500ms\", \\" >> plot.gnu
					exec echo "\"results/out.velocity-alg_$alg.condCanal_2.protoID_1.protoParam_1000.mediaErroGPS_$mediaErroGPS.dist_$distMax\" u 8:18:19 w errorlines t \"FIXO-1000ms\", \\" >> plot.gnu
					exec echo "\"results/out.velocity-alg_$alg.condCanal_2.protoID_1.protoParam_2000.mediaErroGPS_$mediaErroGPS.dist_$distMax\" u 8:18:19 w errorlines t \"FIXO-2000ms\", \\" >> plot.gnu
					exec echo "\"results/out.velocity-alg_$alg.condCanal_2.protoID_2.protoParam_0.3.mediaErroGPS_$mediaErroGPS.dist_$distMax\" u 8:18:19 w errorlines t \"LIMIAR-30\\%\", \\" >> plot.gnu
					exec echo "\"results/out.velocity-alg_$alg.condCanal_2.protoID_2.protoParam_0.4.mediaErroGPS_$mediaErroGPS.dist_$distMax\" u 8:18:19 w errorlines t \"LIMIAR-40\\%\", \\" >> plot.gnu
					#exec echo "\"results/out.velocity-alg_$alg.condCanal_2.protoID_2.protoParam_0.5.mediaErroGPS_$mediaErroGPS.dist_$distMax\" u 8:18:19 w errorlines t \"LIMIAR-50\\%\", \\" >> plot.gnu
					exec gnuplot plot.gnu

				
				}
			}
		}

		foreach alg $val_algorithm {


			foreach mediaErroGPS $val_mediaErroGPS {


				foreach distMax $val_distMax {
							

					# Tempo util
					exec sed "s/<output>/velocity-tdados-alg_$alg.condCanal_2.mediaErroGPS_$mediaErroGPS.dist_$distMax.eps/" plot-base.gnu | sed "s/<xlabel>/Velocidade (m\\/s)/" | sed "s/<ylabel>/Capacidade Efetiva (Gbps)/" | sed "s/<xmin>/*/" | sed "s/<xmax>/*/" | sed "s/<ymin>/0/" | sed "s/<ymax>/1/" > plot.gnu
					exec echo "set key top right" >> plot.gnu
					exec echo "plot \\" >> plot.gnu
			
					exec echo "\"results/out.velocity-alg_$alg.condCanal_2.protoID_1.protoParam_500.mediaErroGPS_$mediaErroGPS.dist_$distMax\" u 8:14:15 w errorlines t \"FIXO-500ms\", \\" >> plot.gnu
					exec echo "\"results/out.velocity-alg_$alg.condCanal_2.protoID_1.protoParam_1000.mediaErroGPS_$mediaErroGPS.dist_$distMax\" u 8:14:15 w errorlines t \"FIXO-1000ms\", \\" >> plot.gnu
					exec echo "\"results/out.velocity-alg_$alg.condCanal_2.protoID_1.protoParam_2000.mediaErroGPS_$mediaErroGPS.dist_$distMax\" u 8:14:15 w errorlines t \"FIXO-2000ms\", \\" >> plot.gnu
					exec echo "\"results/out.velocity-alg_$alg.condCanal_2.protoID_2.protoParam_0.3.mediaErroGPS_$mediaErroGPS.dist_$distMax\" u 8:14:15 w errorlines t \"LIMIAR-30\\%\", \\" >> plot.gnu
					exec echo "\"results/out.velocity-alg_$alg.condCanal_2.protoID_2.protoParam_0.4.mediaErroGPS_$mediaErroGPS.dist_$distMax\" u 8:14:15 w errorlines t \"LIMIAR-40\\%\", \\" >> plot.gnu
					#exec echo "\"results/out.velocity-alg_$alg.condCanal_2.protoID_2.protoParam_0.5.mediaErroGPS_$mediaErroGPS.dist_$distMax\" u 8:14:15 w errorlines t \"LIMIAR-50\\%\", \\" >> plot.gnu
					exec gnuplot plot.gnu

				
				}
			}
		}
	}
				
	
	"PLOT1" {

			foreach protocolo $val_protocolo {

				set protoID [lindex $protocolo 0]
				set protocolo [lreplace $protocolo 0 0]

				foreach protoParam $protocolo {

					foreach mediaErroGPS $val_mediaErroGPS {			

					# Capaciodade Efitiva
					exec sed "s/<output>/dist-Cef-tframe_$protoParam.mediaErroGPS_$mediaErroGPS.eps/" plot-base.gnu | sed "s/<xlabel>/Distância (m)/" | sed "s/<ylabel>/Capacidade Efetiva (Gbps)/" | sed "s/<xmax>/*/" | sed "s/<ymax>/*/" > plot.gnu
					exec echo "set key top left" >> plot.gnu
					exec echo "plot \\" >> plot.gnu
					exec echo "\"results/out.dist-out-alg_0-tframe_$protoParam.mediaErroGPS_$mediaErroGPS\" u 1:18:19 w errorlines t \"Exaustivo\", \\" >> plot.gnu
					exec echo "\"results/out.dist-out-alg_1-tframe_$protoParam.mediaErroGPS_$mediaErroGPS\" u 1:18:19 w errorlines t \"GPS Simples\", \\" >> plot.gnu
					exec echo "\"results/out.dist-out-alg_2-tframe_$protoParam.mediaErroGPS_$mediaErroGPS\" u 1:18:19 w errorlines t \"GPS Refinado\", \\" >> plot.gnu
					exec echo "\"results/out.dist-out-alg_3-tframe_$protoParam.mediaErroGPS_$mediaErroGPS\" u 1:18:19 w errorlines t \"Proposta Refinado\", \\" >> plot.gnu
					exec echo "\"results/out.dist-out-alg_4-tframe_$protoParam.mediaErroGPS_$mediaErroGPS\" u 1:18:19 w errorlines t \"Proposta Iterativo\", \\" >> plot.gnu
					exec gnuplot plot.gnu

			
					}
				}
			}


			foreach protocolo $val_protocolo {

				set protoID [lindex $protocolo 0]
				set protocolo [lreplace $protocolo 0 0]

				foreach protoParam $protocolo {

					foreach mediaErroGPS $val_mediaErroGPS {			

				# Capacidade Nominal
					exec sed "s/<output>/dist-Cnom-tframe_$protoParam.mediaErroGPS_$mediaErroGPS.eps/" plot-base.gnu | sed "s/<xlabel>/Distância (m)/" | sed "s/<ylabel>/Capacidade Nominal (Gbps)/" | sed "s/<xmax>/*/" | sed "s/<ymax>/*/" > plot.gnu
					exec echo "set key top left" >> plot.gnu
					exec echo "plot \\" >> plot.gnu
					exec echo "\"results/out.dist-out-alg_0-tframe_$protoParam.mediaErroGPS_$mediaErroGPS\" u 1:16:17 w errorlines t \"Exaustivo\", \\" >> plot.gnu
					exec echo "\"results/out.dist-out-alg_1-tframe_$protoParam.mediaErroGPS_$mediaErroGPS\" u 1:16:17 w errorlines t \"GPS Simples\", \\" >> plot.gnu
					exec echo "\"results/out.dist-out-alg_2-tframe_$protoParam.mediaErroGPS_$mediaErroGPS\" u 1:16:17 w errorlines t \"GPS Refinado\", \\" >> plot.gnu
					exec echo "\"results/out.dist-out-alg_3-tframe_$protoParam.mediaErroGPS_$mediaErroGPS\" u 1:16:17 w errorlines t \"Proposta Refinado\", \\" >> plot.gnu
					exec echo "\"results/out.dist-out-alg_4-tframe_$protoParam.mediaErroGPS_$mediaErroGPS\" u 1:16:17 w errorlines t \"Proposta Iterativo\", \\" >> plot.gnu
					exec gnuplot plot.gnu

			
					}
				}
			}
		}


	"PLOT10" {

		foreach alg $val_algorithm {

			foreach velocityUSR $val_velocityUSR {


				foreach protocolo $val_protocolo {

					set protoID [lindex $protocolo 0]
					set protocolo [lreplace $protocolo 0 0]

					foreach protoParam $protocolo {


						foreach distMax $val_distMax {
							

					# Capaciodade Efitiva
					exec sed "s/<output>/mobilidade-alg_$alg.condCanal_1.protoID_$protoID.protoParam_$protoParam.mediaErroGPS_10.velocityUSR_$velocityUSR.dist_$distMax.eps/" plot-base.gnu | sed "s/<xlabel>/tempo \\(\\ms\\)/" | sed "s/<ylabel>//" | sed "s/<xmin>/0/" | sed "s/<xmax>/20000/" | sed "s/<ymin>/0/" | sed "s/<ymax>/30/" > plot.gnu
					exec echo "set key bottom left " >> plot.gnu
					exec echo "plot \\" >> plot.gnu		
					exec echo "\"results/alg_$alg.condCanal_1.protoID_$protoID.protoParam_$protoParam.mediaErroGPS_10.velocityUSR_$velocityUSR.dist_$distMax.run_1.log\" u 1:25 w lines lt 3 t \"\", \\" >> plot.gnu
					exec echo "\"results/alg_$alg.condCanal_1.protoID_$protoID.protoParam_$protoParam.mediaErroGPS_10.velocityUSR_$velocityUSR.dist_$distMax.run_1.log\" u 1:(\$2\*30) w lines lt 1 t \"\", \\" >> plot.gnu				
					exec gnuplot plot.gnu

						}
					}
				}
			}
		}
	}

	

	default {
		puts "Tipo de simula��o invalido! (LOCAL ou SUBMIT/PARSE)"
	}
}

