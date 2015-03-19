#/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
#/*
# *  Copyright © 2014 by IEEE.
# *
# *  This source file is an essential part of IEEE P1906.1,
# *  Recommended Practice for Nanoscale and Molecular
# *  Communication Framework.
# *  Verbatim copies of this source file may be used and
# *  distributed without restriction. Modifications to this source
# *  file as permitted in IEEE P1906.1 may also be made and
# *  distributed. All other uses require permission from the IEEE
# *  Standards Department (stds-ipr@ieee.org). All other rights
# *  reserved.
# *
# *  This source file is provided on an AS IS basis.
# *  The IEEE disclaims ANY WARRANTY EXPRESS OR IMPLIED INCLUDING
# *  ANY WARRANTY OF MERCHANTABILITY AND FITNESS FOR USE FOR A
# *  PARTICULAR PURPOSE.
# *  The user of the source file shall indemnify and hold
# *  IEEE harmless from any damages or liability arising out of
# *  the use thereof.
# *
# * Author: Giuseppe Piro - Telematics Lab Research Group
# *                         Politecnico di Bari
# *                         giuseppe.piro@poliba.it
# *                         telematics.poliba.it/piro
# */


set -x
set -e



for nodeDistance in 1.00000000000000e-06    0.00101109090909091     0.00202118181818182     0.00303127272727273     0.00404136363636364     0.00505145454545455     0.00606154545454546     0.00707163636363637     0.00808172727272727     0.00909181818181818     0.0101019090909091      0.0111120000000000      0.0121220909090909      0.0131321818181818      0.0141422727272727      0.0151523636363636      0.0161624545454545      0.0171725454545455      0.0181826363636364      0.0191927272727273      0.0202028181818182      0.0212129090909091      0.0222230000000000      0.0232330909090909      0.0242431818181818      0.0252532727272727      0.0262633636363636      0.0272734545454545      0.0282835454545455      0.0292936363636364      0.0303037272727273      0.0313138181818182      0.0323239090909091      0.0333340000000000      0.0343440909090909      0.0353541818181818      0.0363642727272727      0.0373743636363636      0.0383844545454546      0.0393945454545455      0.0404046363636364      0.0414147272727273      0.0424248181818182      0.0434349090909091      0.0444450000000000      0.0454550909090909      0.0464651818181818      0.0474752727272727      0.0484853636363636      0.0494954545454546      0.0505055454545455      0.0515156363636364      0.0525257272727273      0.0535358181818182      0.0545459090909091      0.0555560000000000      0.0565660909090909      0.0575761818181818      0.0585862727272727      0.0595963636363636      0.0606064545454546      0.0616165454545455      0.0626266363636364      0.0636367272727273      0.0646468181818182      0.0656569090909091      0.0666670000000000      0.0676770909090909      0.0686871818181818      0.0696972727272727      0.0707073636363636      0.0717174545454546      0.0727275454545455      0.0737376363636364      0.0747477272727273      0.0757578181818182      0.0767679090909091      0.0777780000000000      0.0787880909090909      0.0797981818181818      0.0808082727272727      0.0818183636363636      0.0828284545454546      0.0838385454545455      0.0848486363636364      0.0858587272727273      0.0868688181818182      0.0878789090909091      0.0888890000000000      0.0898990909090909      0.0909091818181818      0.0919192727272727      0.0929293636363636      0.0939394545454546      0.0949495454545455      0.0959596363636364      0.0969697272727273      0.0979798181818182      0.0989899090909091      0.100000000000000

do


	./waf --run "scratch/mol-example --nodeDistance=${nodeDistance}" &> tmp_out

	grep testcapacity tmp_out | awk '{print $6, $8}' > tmp_select

	sed 's/,/ /g' tmp_select > tmp_select2
	mv tmp_select2 tmp_select
	sed 's/)/ /g' tmp_select > tmp_select2
	mv tmp_select2 tmp_select 
	cat tmp_select >> RES_MOL

	rm tmp_*

done
