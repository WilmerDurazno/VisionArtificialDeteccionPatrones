#include "LBPDescriptor1.hpp"

String recuperarNombreImagen(String cadena){
    String lec;
    stringstream salida(cadena);
    vector<string> cadenaArreglo;
    while(getline(salida,lec,'/')){
        cadenaArreglo.push_back(lec);
    }
    return cadenaArreglo[1];
}

void guardarHistograma(string nombre, vector<int> histograma)
{
    ofstream outFile(nombre);
    if(outFile.is_open()){
        for (int i = 0; i < histograma.size(); i++)
        {
            outFile << histograma[i] << ";";
        }
    //cout << "Archivo: " << nombre << " guardado" << endl;
        outFile << "\n";
        outFile.close();
    }
}


vector<int> test_image(String ruta)
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
	//lbp = new ELBP;     // 0-255
	//lbp = new VARLBP;   // 0-953.0
	lbp = new CSLBP;    // 0-15
	//lbp = new CSLDP;    // 0-15
	//lbp = new XCSLBP;   // 0-15
	//lbp = new SILTP;    // 0-80
	//lbp = new CSSILTP;  // 33-120
	//lbp = new SCSLBP;   // 0-15
	//lbp = new BGLBP;    // 0-239
	lbp->run(img_input, img_output);

	double min, max; cv::minMaxLoc(img_output, &min, &max); std::cout << "min: " << min << ", max: " << max <<endl;
	cv::normalize(img_output, img_output, 0, 255, cv::NORM_MINMAX, CV_8UC1);
    Mat histogramaLBP;
    show_histogram("gray_hist_LBP",img_output);
    histogram(img_output, histogramaLBP,256);

    //vector<uchar> histo(histograma.rows*histograma.cols);
    //if(histograma.isContinuous()){
      //  array=histograma.data;
    //}
    vector<int> histo;
    for(int i=0; i<histogramaLBP.cols;i++){
        //cout << histograma[i] << endl;
        int pixel=(int) histogramaLBP.at<uchar>(0,i);
        //cout << pixel << ", " ;
        histo.push_back(pixel);
    }
    cout << endl;
	cv::imshow("Output", img_output);
    //cout << histograma.type() << endl;
   // cout <<"Tamano: " <<histogramaLBP.size() << endl;
    //cout <<"Valores: "<<histogramaLBP << endl;
    //cout <<"Canales: " <<histograma.channels() << endl;
    //cout <<"Columna :" <<histograma.cols << " Fila: "<< histograma.rows << endl;
    //cout <<"Vector :" <<histo.size()<< endl;
    //for(int i=0; i<histo.size();i++){
        //cout << histograma[i] << endl;
        //int pixel=(int) histograma.at<uchar>(0,i);
        //histo.push_back(pixel);
      //  cout << histo[i] << ", ";
    //}
    //guardarHistograma("prueba.csv",histo);
    //cv::waitKey();
    return histo;
	//delete lbp;
}

vector<string> subirImagenes(string ruta)
{
    vector<string> archivos;
    for(const auto &entry:fs::directory_iterator(ruta)){
        archivos.push_back(entry.path());
    }
    sort(archivos.begin(),archivos.end());
    return archivos;
}



int main(int argc, char *argv[]){

    LBP * lbp;
    lbp= new CSLBP;

    string ruta="Bryan-Mendez";
    vector<string> archivos=subirImagenes(ruta);
    cout << archivos.size() << endl;
    cout << archivos[0] << endl;
    cout << "Cambiado" << endl;
    vector<int> ht;
    
    vector<int> histograma;    
    ofstream outFile("DescriptorHistograma-Mendez.csv");
    String nombreImagen;
    for(int i=0;i<archivos.size();i++){
        nombreImagen=recuperarNombreImagen(archivos[i]);
        //cout << nombreImagen << endl;
        histograma=test_image(archivos[i]);
        outFile << nombreImagen << ";";
        for (int j = 0; j < histograma.size(); j++)
        {
            outFile << histograma[j] << ";";
        }
        outFile << "\n";
        }
    outFile.close();
    
    delete lbp;
    return 0;
}
