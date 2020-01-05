#! /usr/bin/tclsh

# Parametros gerais
set ic			95		;# Intervalo de confian�a

# Parametros da simula��o
set distMax		100
set cenario             100
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
set stop		1500  ;# tempo de simulacao
set move		75000
set fastIA		0	
set velocityOBJ	        10
set limFastIA		0
set tmove 		0
set nAdj		1

#set val_condCanal       {1 2} ;# 1-LOS | 2-NLOS | 3-AUTO
set val_condCanal       {3} ;# 1-LOS | 2-NLOS | 3-AUTO
set val_velocityUSR     {0}
#set val_velocityUSR     {1 3 5 7 9}
set val_mediaErroGPS	{5 10}
#set val_algorithm	{0 1 2 3 4}  ;# 0-Exaustivo | 1-GPS | 2-GPS refinado | 3-Proposta intercalado | 4-Proposta iterativo
set val_algorithm	{2 3}  ;# 0-Exaustivo | 1-GPS | 2-GPS refinado | 3-Proposta intercalado | 4-Proposta iterativo
set val_nAdj		{1 2 3 4 5}
#set val_protocolo       {{2 0.5}}  ;#  0-Não refaz IA | 1-Intervalo fixo | 2-Limiar de SNR | 3- Good
set val_protocolo       {{1 200 500 1000 1500}}  ;#  0-Não refaz IA | 1-Intervalo fixo | 2-Limiar de SNR | 3- Good
set val_distMax		{10 20 30 40 50 60 70 80 90 100 125 150}

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
		
			foreach nAdj $val_nAdj {

				foreach condCanal $val_condCanal {

					foreach mediaErroGPS $val_mediaErroGPS {

						foreach velocityUSR $val_velocityUSR {

							foreach protocolo $val_protocolo {

								set protoID [lindex $protocolo 0]
								set protocolo [lreplace $protocolo 0 0]

								foreach protoParam $protocolo {

									# only one frame
									set stop $protoParam

									foreach distMax $val_distMax {

										for {set run 1} {$run <= $cenario} {incr run} {

											puts "qsub -o /dev/null -e /dev/null -V -cwd -b y -shell n ./ia $Pt $distMax $npontos $run $dir/alg_$alg.nadj_$nAdj.condCanal_$condCanal.protoID_$protoID.protoParam_$protoParam.mediaErroGPS_$mediaErroGPS.velocityUSR_$velocityUSR.dist_$distMax.run_$run $NF $TN $BW $div $tmove $minSNR $Tper $Tcanal $stop $tipoErro $mediaErroGPS [expr (9.0*$mediaErroGPS)-30.0] $alg $log $velocityUSR $velocityOBJ $protoID $protoParam $protoParam $protoParam $fastIA $limFastIA $condCanal $nAdj"
											exec qsub -o /dev/null -e /dev/null -V -cwd -b y -shell n ./ia $Pt $distMax $npontos $run $dir/alg_$alg.nadj_$nAdj.condCanal_$condCanal.protoID_$protoID.protoParam_$protoParam.mediaErroGPS_$mediaErroGPS.velocityUSR_$velocityUSR.dist_$distMax.run_$run $NF $TN $BW $div $tmove $minSNR $Tper $Tcanal $stop $tipoErro $mediaErroGPS [expr (9.0*$mediaErroGPS)-30.0] $alg $log $velocityUSR $velocityOBJ $protoID $protoParam $protoParam $protoParam $fastIA $limFastIA $condCanal $nAdj
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}

	"PARSE" {

		foreach alg $val_algorithm {
		
			foreach nAdj $val_nAdj {

				foreach condCanal $val_condCanal {

					foreach mediaErroGPS $val_mediaErroGPS {

						foreach velocityUSR $val_velocityUSR {

							foreach protocolo $val_protocolo {

								set protoID [lindex $protocolo 0]
								set protocolo [lreplace $protocolo 0 0]

								foreach protoParam $protocolo {

									# only one frame
									set stop $protoParam

									foreach distMax $val_distMax {

										for {set run 1} {$run <= $cenario} {incr run} {

											exec cat results/alg_$alg.nadj_$nAdj.condCanal_$condCanal.protoID_$protoID.protoParam_$protoParam.mediaErroGPS_$mediaErroGPS.velocityUSR_$velocityUSR.dist_$distMax.run_$run >> results/agg.alg_$alg.nadj_$nAdj.condCanal_$condCanal.protoID_$protoID.protoParam_$protoParam.mediaErroGPS_$mediaErroGPS.velocityUSR_$velocityUSR.dist_$distMax 

										}
										exec ./ci.m results/agg.alg_$alg.nadj_$nAdj.condCanal_$condCanal.protoID_$protoID.protoParam_$protoParam.mediaErroGPS_$mediaErroGPS.velocityUSR_$velocityUSR.dist_$distMax results/out-alg_$alg.nadj_$nAdj.condCanal_$condCanal.protoID_$protoID.protoParam_$protoParam.mediaErroGPS_$mediaErroGPS.velocityUSR_$velocityUSR.dist_$distMax 13 $ic
										exec cat results/out-alg_$alg.nadj_$nAdj.condCanal_$condCanal.protoID_$protoID.protoParam_$protoParam.mediaErroGPS_$mediaErroGPS.velocityUSR_$velocityUSR.dist_$distMax >> results/out.dist-alg_$alg.nadj_$nAdj.condCanal_$condCanal.protoID_$protoID.protoParam_$protoParam.mediaErroGPS_$mediaErroGPS.velocityUSR_$velocityUSR
										exec cat results/out-alg_$alg.nadj_$nAdj.condCanal_$condCanal.protoID_$protoID.protoParam_$protoParam.mediaErroGPS_$mediaErroGPS.velocityUSR_$velocityUSR.dist_$distMax >> results/out.nadj-alg_$alg.condCanal_$condCanal.protoID_$protoID.protoParam_$protoParam.mediaErroGPS_$mediaErroGPS.velocityUSR_$velocityUSR.dist_$distMax

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
		
			foreach nAdj $val_nAdj {

				foreach condCanal $val_condCanal {

					foreach mediaErroGPS $val_mediaErroGPS {

						foreach velocityUSR $val_velocityUSR {

							foreach protocolo $val_protocolo {

								set protoID [lindex $protocolo 0]
								set protocolo [lreplace $protocolo 0 0]

								foreach protoParam $protocolo {

									# only one frame
									set stop $protoParam

									foreach distMax $val_distMax {

										for {set run 1} {$run <= $cenario} {incr run} {

											puts "./ia $Pt $distMax $npontos $run $dir/alg_$alg.nadj_$nAdj.condCanal_$condCanal.protoID_$protoID.protoParam_$protoParam.mediaErroGPS_$mediaErroGPS.velocityUSR_$velocityUSR.dist_$distMax.run_$run $NF $TN $BW $div $tmove $minSNR $Tper $Tcanal $stop $tipoErro $mediaErroGPS [expr (9.0*$mediaErroGPS)-30.0] $alg $log $velocityUSR $velocityOBJ $protoID $protoParam $protoParam $protoParam $fastIA $limFastIA $condCanal $nAdj"
											exec ./ia $Pt $distMax $npontos $run $dir/alg_$alg.nadj_$nAdj.condCanal_$condCanal.protoID_$protoID.protoParam_$protoParam.mediaErroGPS_$mediaErroGPS.velocityUSR_$velocityUSR.dist_$distMax.run_$run $NF $TN $BW $div $tmove $minSNR $Tper $Tcanal $stop $tipoErro $mediaErroGPS [expr (9.0*$mediaErroGPS)-30.0] $alg $log $velocityUSR $velocityOBJ $protoID $protoParam $protoParam $protoParam $fastIA $limFastIA $condCanal $nAdj
											exec cat results/alg_$alg.nadj_$nAdj.condCanal_$condCanal.protoID_$protoID.protoParam_$protoParam.mediaErroGPS_$mediaErroGPS.velocityUSR_$velocityUSR.dist_$distMax.run_$run >> results/agg.alg_$alg.nadj_$nAdj.condCanal_$condCanal.protoID_$protoID.protoParam_$protoParam.mediaErroGPS_$mediaErroGPS.velocityUSR_$velocityUSR.dist_$distMax 

										}
										exec ./ci.m results/agg.alg_$alg.nadj_$nAdj.condCanal_$condCanal.protoID_$protoID.protoParam_$protoParam.mediaErroGPS_$mediaErroGPS.velocityUSR_$velocityUSR.dist_$distMax results/out-alg_$alg.nadj_$nAdj.condCanal_$condCanal.protoID_$protoID.protoParam_$protoParam.mediaErroGPS_$mediaErroGPS.velocityUSR_$velocityUSR.dist_$distMax 13 $ic
										exec cat results/out-alg_$alg.nadj_$nAdj.condCanal_$condCanal.protoID_$protoID.protoParam_$protoParam.mediaErroGPS_$mediaErroGPS.velocityUSR_$velocityUSR.dist_$distMax >> results/out.dist-alg_$alg.nadj_$nAdj.condCanal_$condCanal.protoID_$protoID.protoParam_$protoParam.mediaErroGPS_$mediaErroGPS.velocityUSR_$velocityUSR

									}
								}
							}
						}
					}
				}
			}
		}
	}

	"PLOT" {

		foreach alg $val_algorithm {
		
			#foreach nAdj $val_nAdj {

				foreach condCanal $val_condCanal {

					foreach mediaErroGPS $val_mediaErroGPS {

						foreach velocityUSR $val_velocityUSR {

							foreach protocolo $val_protocolo {

								set protoID [lindex $protocolo 0]
								set protocolo [lreplace $protocolo 0 0]

								foreach protoParam $protocolo {

									# only one frame
									set stop $protoParam

									#foreach distMax $val_distMax {

			# Capaciodade Efetiva
			exec sed "s/<output>/dist-Cef-alg_$alg.tframe_$protoParam.mediaErroGPS_$mediaErroGPS.pdf/" plot-base.gnu | sed "s/<xlabel>/Distância (m)/" | sed "s/<ylabel>/Capacidade Efetiva (Gbps)/" | sed "s/<xmax>/*/" | sed "s/<ymax>/*/" > plot.gnu
			exec echo "set key top right" >> plot.gnu
			exec echo "plot \\" >> plot.gnu
			exec echo "\"results/out.dist-alg_$alg.nadj_1.condCanal_$condCanal.protoID_$protoID.protoParam_$protoParam.mediaErroGPS_$mediaErroGPS.velocityUSR_$velocityUSR\" u 1:18:19 w errorlines t \"alg=$alg 1\", \\" >> plot.gnu
			exec echo "\"results/out.dist-alg_$alg.nadj_2.condCanal_$condCanal.protoID_$protoID.protoParam_$protoParam.mediaErroGPS_$mediaErroGPS.velocityUSR_$velocityUSR\" u 1:18:19 w errorlines t \"alg=$alg 2\", \\" >> plot.gnu
			exec echo "\"results/out.dist-alg_$alg.nadj_3.condCanal_$condCanal.protoID_$protoID.protoParam_$protoParam.mediaErroGPS_$mediaErroGPS.velocityUSR_$velocityUSR\" u 1:18:19 w errorlines t \"alg=$alg 3\", \\" >> plot.gnu
			exec echo "\"results/out.dist-alg_$alg.nadj_4.condCanal_$condCanal.protoID_$protoID.protoParam_$protoParam.mediaErroGPS_$mediaErroGPS.velocityUSR_$velocityUSR\" u 1:18:19 w errorlines t \"alg=$alg 4\", \\" >> plot.gnu
			exec echo "\"results/out.dist-alg_$alg.nadj_5.condCanal_$condCanal.protoID_$protoID.protoParam_$protoParam.mediaErroGPS_$mediaErroGPS.velocityUSR_$velocityUSR\" u 1:18:19 w errorlines t \"alg=$alg 5\"" >> plot.gnu
			exec gnuplot plot.gnu

									#}
								}
							}
						}
					}
				}
			#}
		}
	}

	default {
		puts "Tipo de simula��o invalido! (LOCAL ou SUBMIT/PARSE)"
	}
}

