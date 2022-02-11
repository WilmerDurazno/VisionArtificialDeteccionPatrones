
#include "LBPDescriptor1.hpp"

int main(int argc, char *argv[])
{
    
    Mat imgGlaciar = imread("glaciar.jpg");
    Mat imgGirasol = imread("girasol-m.jpg");
    
    cvtColor(imgGlaciar,imgGlaciar, COLOR_BGR2Lab);
    cvtColor(imgGirasol, imgGirasol, COLOR_BGR2Lab);
    namedWindow("Textura Glaciar", WINDOW_AUTOSIZE);
    namedWindow("Textura Girasol", WINDOW_AUTOSIZE);
    imshow("Textura Glaciar", imgGlaciar);
    imshow("Textura Girasol", imgGirasol);

    
    Prueba *lbp = new Prueba();
    //Calculamos el histograma de las imagenes ingresadas
    vector<int> histoGlaciar = lbp->calcularHistogramaLBP(imgGlaciar);
    vector<int> histoGirasol = lbp->calcularHistogramaLBP(imgGirasol);
    //guardamos el histograma en un archivo csv dependiendo su clase
    lbp->guardarHistograma("histogramaimgGlaciar.csv", histoGlaciar);
    lbp->guardarHistograma("histogramaimgGirasol.csv", histoGirasol);
    string pathGlaciar = "Glaciares";
    string pathGirasol = "Girasoles";

   
    vector<string> Pared = lbp->subirImagenes(pathGlaciar);
    vector<string> Tela= lbp->subirImagenes(pathGirasol);
    cout << Pared.size() << endl;
    //for(int i=0; i<Pared.size();i++){
        //cout << Pared[i] << endl;
    //}

    //Mat bdImg = imread(Pared[0], IMREAD_GRAYSCALE);
    //namedWindow("Arbol", WINDOW_AUTOSIZE);
    //imshow("Textura Arbol", bdImg);

    // Ricess
    
    cout << "Resultados primera imagen ingresada" << endl;
    lbp->calculaAciertoFallo(Pared, pathGlaciar, lbp, histoGlaciar, histoGirasol);
    cout << endl;
    cout << endl;
    //waitKey(0);
    //destroyAllWindows();
    
    cout << "Resulatados segunda imagen ingresada" << endl;
    lbp->calculaAciertoFallo(Tela, pathGirasol, lbp, histoGlaciar, histoGirasol);
    // Walls
    waitKey(0);
    destroyAllWindows();
    delete lbp;
    return 0;
    
}