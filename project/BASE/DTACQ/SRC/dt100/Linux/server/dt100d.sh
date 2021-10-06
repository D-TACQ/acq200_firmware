#!/bin/bash
# dt100d - dt100 server daemon

cd ~dt100/server
#./make-boardmap doesn't work from here - move to rc.local
#./make-boardmap
java -jar DtacqServer.jar $* >>/var/log/dt100d.log &
echo root process $? >>/var/log/dt100d.log
wait

