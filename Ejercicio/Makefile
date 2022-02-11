all:
	g++ Principal2.cpp  LBPDescriptor1.cpp lbp-library-master/histogram.cpp lbp-library-master/package_lbp/cslbp/CSLBP.cpp --std=c++17 -I/home/computacion/opencv/opencvi/include/opencv4/ -L/home/computacion/opencv/opencvi/lib/ -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_imgcodecs -lopencv_video -lopencv_videoio -lopencv_objdetect -o vision.bin

run:
	./vision.bin

saludo:
	echo "Hola Mundo desde C++!"