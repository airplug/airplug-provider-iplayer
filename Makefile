all:
	mkdir -p build
	gmcs 	-lib:lib -r:System.Xml.Linq -r:System.Core \
				-resource:res/help.txt \
				-out:build/iplayer-dl.exe \
				src/*.cs src/IPDL/*.cs src/Mono.Options/*.cs