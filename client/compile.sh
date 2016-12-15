#!/bin/bash
cd src
if [ ! -d ../bin ]; then
	mkdir ../bin
fi
javac cards/client/*.java -Xdiags:verbose -Xlint:unchecked -d ../bin/
