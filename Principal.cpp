#include "Deteccion.hpp"


int main(int argc, char *argv[]){

    Deteccion *d=new Deteccion();

    string ruta="images";
    vector<string> imagenes=d->subirImagenes(ruta);
    cout << imagenes.size() <<endl;
    //d->creacionSVM(imagenes);
    string svmRuta="svm.xml";
    //string imgPrediccion="casco.png";
    string imgPrediccion="sincasco.jpg";
    d->SVMPrediccion(imgPrediccion,svmRuta);


   
    delete d;
    return 0;

}