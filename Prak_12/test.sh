#!/bin/bash

rm a.c b.c c.c d.c e.c
echo 'rm a.c b.c c.c d.c e.c'

./client ../../../Dokumente/eclipse-installer/plugins/com.jcraft.jsch_0.1.54.v20170116-1932.jar a.c &
./client ../../../Dokumente/eclipse-installer/plugins/com.jcraft.jsch_0.1.54.v20170116-1932.jar b.c &
./client ../../../Dokumente/eclipse-installer/plugins/com.jcraft.jsch_0.1.54.v20170116-1932.jar c.c &
./client ../../../Dokumente/eclipse-installer/plugins/com.jcraft.jsch_0.1.54.v20170116-1932.jar d.c &
./client ../../../Dokumente/eclipse-installer/plugins/com.jcraft.jsch_0.1.54.v20170116-1932.jar e.c

rm a.c b.c c.c d.c e.c
echo 'rm a.c b.c c.c d.c e.c'

./client ../../../Dokumente/eclipse-installer/plugins/com.jcraft.jsch_0.1.54.v20170116-1932.jar a.c &
./client ../../../Dokumente/eclipse-installer/plugins/com.jcraft.jsch_0.1.54.v20170116-1932.jar b.c &
./client ../../../Dokumente/eclipse-installer/plugins/com.jcraft.jsch_0.1.54.v20170116-1932.jar c.c &
./client ../../../Dokumente/eclipse-installer/plugins/com.jcraft.jsch_0.1.54.v20170116-1932.jar d.c &
./client ../../../Dokumente/eclipse-installer/plugins/com.jcraft.jsch_0.1.54.v20170116-1932.jar e.c &

./client client.c a.c &
./client server.c b.c &
./client messages.h c.c &
./client ../prak09/client.c d.c &
./client ../prak11/client.c e.c


exit 0


