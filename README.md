
     P1906.1 Simulation Framework
=====================================


== Wiki page ==

Please, refer to the official wiki page for more details:
https://standardswiki.comsoc.org/wiki/NS-3_simulation_framework

== How installing the tool ==

1) download and compile the latest version of ns-3

hg clone http://code.nsnam.org/ns-3-dev

cd ns-3-dev

./waf configure

./waf 

2) download the p1906.1 simulation framework code
from https://github.com/ieee-p1906-1-reference-code/p1906-dev
and copy paste the p1906 folder within ns-3-dev/src folder

3) compile again the ns-3 simulator

./waf configure

./waf 


== Run the simple example == 

A simple example is stored into the ns-3-dev/p1906/example folder

cp ns-3-dev/p1906/example/first-example.cc scratch

./waf --run scratch/first-example


== Run MOTOR example ==

./waf --run scratch/motor-example


== Examine the API ==

Use your browser to open p1906/motor-model/p1906/html/index.html

Also see p1906/motor-model/p1906/motor-model/README.txt

Finally, have a look to the presentation into the docs folder 

