
#include "Deteccion.hpp"



Deteccion::Deteccion(){
    cout << "Inicializa el descriptor ... " << endl;
}

/**
* Calculate a LBP8,1 feature vector for an image array.
* This function does not use interpolation. The input
* image is presented as a linear array, in raster-scan
* order. As a result, a newly allocated array of 256
* integers is returned.
**/
int* Deteccion::LBP8(const int* data, int rows, int columns){
    const int
    *p0 = data,
    *p1 = p0 + 1,
    *p2 = p1 + 1,
    *p3 = p2 + columns,
    *p4 = p3 + columns,
    *p5 = p4 - 1,
    *p6 = p5 - 1,
    *p7 = p6 - columns,
    *center = p7 + 1;
    int r,c,cntr;
    unsigned int value;
    int* result = (int*)malloc(256*sizeof(int));
    memset(result, 0, 256*sizeof(int));
    for (r=0;r<rows-2;r++){
        for (c=0;c<columns-2;c++){
            value = 0;
            cntr = *center - 1;
            compab_mask_inc(p0,0);
            compab_mask_inc(p1,1);
            compab_mask_inc(p2,2);
            compab_mask_inc(p3,3);
            compab_mask_inc(p4,4);
            compab_mask_inc(p5,5);
            compab_mask_inc(p6,6);
            compab_mask_inc(p7,7);
            center++;
            result[value]++;
        }
        p0 += 2;
        p1 += 2;
        p2 += 2;
        p3 += 2;
        p4 += 2;
        p5 += 2;
        p6 += 2;
        p7 += 2;
        center += 2;
    }
    return result;
}

vector<int> Deteccion::LBP8(Mat imagen){

    int *data = (int *) malloc(imagen.rows*imagen.cols*sizeof(int));
    for(int i=0,s=0;i<imagen.rows;i++){
        for(int j=0;j<imagen.cols;j++,s++){
            data[s] = (int) imagen.at<uchar>(i,j);
            //*(data+s) = (int) imagen.at<uchar>(i,j);
        }
    }

    int *descriptor = LBP8(data, imagen.rows, imagen.cols);
    vector<int> desc;
    for(int i=0;i<256;i++){
        desc.push_back(descriptor[i]);
    }

    free(data);

    return desc;
}

vector<string> Deteccion::subirImagenes(string ruta)
{
    vector<string> archivos;
    for(const auto &entry:fs::directory_iterator(ruta)){
        archivos.push_back(entry.path());
    }
    sort(archivos.begin(),archivos.end());
    return archivos;
}

vector<int> ELBPCL(String ruta)
{
	cv::Mat img_output;
	//cv::Mat img_input = cv::imread("Bryan-Mendez/n02091635_2.jpg");
    cv::Mat img_input = cv::imread(ruta);
	cv::imshow("Input", img_input);
	cv::cvtColor(img_input, img_input, cv::COLOR_BGR2GRAY);
    //Mat histograma= Mat::zeros(img_input.size(), img_input.type());

	//cv::GaussianBlur(img_input, img_input, cv::Size(7, 7), 5, 3, cv::BORDER_CONSTANT);

	LBP *lbp;
	//lbp = new OLBP;     // 0-255
	lbp = new ELBP;     // 0-255
	//lbp = new VARLBP;   // 0-953.0
	//lbp = new CSLBP;    // 0-15
	//lbp = new CSLDP;    // 0-15
	//lbp = new XCSLBP;   // 0-15
	//lbp = new SILTP;    // 0-80
	//lbp = new CSSILTP;  // 33-120
	//lbp = new SCSLBP;   // 0-15
	//lbp = new BGLBP;    // 0-239
	lbp->run(img_input, img_output);

	double min, max; cv::minMaxLoc(img_output, &min, &max);
    //std::cout << "min: " << min << ", max: " << max <<endl;
	cv::normalize(img_output, img_output, 0, 255, cv::NORM_MINMAX, CV_8UC1);
    Mat histogramaLBP;
    show_histogram("gray_hist_LBP",img_output);
    histogram(img_output, histogramaLBP,256);

    vector<int> histo;
    for(int i=0; i<histogramaLBP.cols;i++){
        //cout << histograma[i] << endl;
        int pixel=(int) histogramaLBP.at<uchar>(0,i);
        //cout << pixel << ", " ;
        histo.push_back(pixel);
    }
    //cout << endl;
	//cv::imshow("Output", img_output);
    delete lbp;
    return histo;
}


void Deteccion::creacionSVM(vector<string> imagenes){

    float clases[5319];

    for(int i=0; i<5319; i++){
        if(i>2268   ){
            clases[i]=-1;
        }else{
            clases[i]=1;
        }
    }

    float puntos[5319][256];
    for(int i=0; i<imagenes.size();i++){
        Mat imagen= imread(imagenes[i],IMREAD_GRAYSCALE);
        vector<int> descLBP=LBP8(imagen);
        for(int j=0;j<256;j++){
            puntos[i][j]=descLBP[j];
        }
    }

    Mat matrizPuntos(5319, 256, CV_32F, puntos);
    Mat matrizClases(5319, 1, CV_32SC1, clases);


    // Creamos la máquina de soporte vectorial y la entrenamos
    Ptr<SVM> svm = SVM::create();
    // Indicamos el tipo de SVM, es decir, si se usará para clasificar 2 clases o más
    svm->setType(SVM::C_SVC); // Con este tipo de SVM podemos clasificar más de 2 clases
    svm->setKernel(SVM::DEGREE); // Se especifica el tipo de Kernel (función para definir zonas de clasificación)

    // Especificamos los criterios de entrenamiento (número de iteraciones, error, etc.)
    svm->setTermCriteria(TermCriteria(TermCriteria::MAX_ITER, 100, 1e-7));
    svm->train(matrizPuntos, ROW_SAMPLE, matrizClases);
    svm->save("svm.xml");
}




void Deteccion::SVMPrediccion(string imagen, string svmFile){

    Ptr<SVM> svm = SVM::create();
    svm=svm->load(svmFile);
    cout <<"Se cargo el SVM" << endl;

    int width=120;
    int height=120;
    Mat img=imread(imagen, IMREAD_GRAYSCALE);
    resize(img,img,Size(width,height));
    Mat imgColor=imread(imagen);
    
    vector<int> desc=LBP8(img);
    
    float puntos[1][256];

    for(int i=0; i<256;i++){
        puntos[0][i]=desc[i];
    }

    Mat imgData(1,256, CV_32F, puntos);

    float r =svm->predict(imgData);
    
    cout << "Prediccion: " << r << endl;
    cout << "Paso" << endl;

    if(r>0.0){
        putText(imgColor, "Usa casco", Point(30,30), FONT_HERSHEY_PLAIN, 1, Scalar(255,10,10),2);
    }else if(r<0.0){
        putText(imgColor, "NO usa Casco", Point(30,30), FONT_HERSHEY_PLAIN, 1, Scalar(10,10,255),2);
    }
    imshow("Img",img);
    imshow("Img Color", imgColor);

     waitKey(0);
     
}