

#include "LBPDescriptor1.hpp"

LBPDescriptor::LBPDescriptor()
{
    cout << "Inicializa el descriptor ... " << endl;
}

/**
* Calculate a LBP8,1 feature vector for an image array.
* This function does not use interpolation. The input
* image is presented as a linear array, in raster-scan
* order. As a result, a newly allocated array of 256
* integers is returned.
**/
Prueba::Prueba()
{
}
vector<int> Prueba::calcularHistogramaLBP(Mat imagen)
{
    vector<int> histograma;
    int *data = (int *)malloc(imagen.rows * imagen.cols * sizeof(int));
    for (int i = 0, k = 0; i < imagen.rows; i++)
    {
        for (int j = 0; j < imagen.cols; j++, k++)
        {
            //data[k] = imagen.at<uchar>(i,j);
            *(data + k) = imagen.at<uchar>(i, j);
        }
    }
    int *histo = this->LBP8(data, imagen.rows, imagen.cols);
    for (int i = 0; i < 256; i++)
        histograma.push_back(histo[i]);
    free(data);
    return histograma;
}

double Prueba::calcularDistanciaEuclidea(vector<int> histoTrain, vector<int> histoTest)
{
    double d = 0.0;
    for (size_t i = 0; i < 256; i++)
    {
        d += pow(histoTrain[i] - histoTest[i], 2);
    }
    return sqrt(d);
}

vector<string> Prueba::subirImagenes(string ruta)
{
    vector<string> archivos;
    for(const auto &entry:fs::directory_iterator(ruta)){
        archivos.push_back(entry.path());
    }
    sort(archivos.begin(),archivos.end());
    return archivos;
}


vector<string> Prueba::cargarImagenes(const char *pathFolder)
{
    vector<string> files;
    
    DIR *dr;
    struct dirent *en;
    dr = opendir(pathFolder); //open all directory
                              // int i = 1;
    if (dr)
    {
        while ((en = readdir(dr)) != NULL)
        {
            files.push_back(en->d_name);
            // cout << i << endl;
            // i++;
        }
        closedir(dr); //close all directory
    }
    
    return files;
}
void Prueba::guardarHistograma(string nombre, vector<int> histograma)
{
    ofstream outFile(nombre);
    for (int i = 0; i < histograma.size(); i++)
    {
        outFile << histograma[i] << ";";
    }
    cout << "Archivo: " << nombre << " guardado" << endl;
    outFile.close();
}

void Prueba::calculaAciertoFallo(vector<string> clase, string path, Prueba *lbp, vector<int> clase1, vector<int> clase2)
{
    double distancia1 = 0.0;
    double distancia2 = 0.0;
    int aciertos = 0, fallos = 0;
    for (int i = 1; i < (clase.size() - 1); i++)
    {
        Mat bdImg = imread(clase[i], IMREAD_GRAYSCALE);
        //Mat bdImg = imread(path + "/" + clase[i], IMREAD_GRAYSCALE);
        vector<int> histoRiceP = lbp->calcularHistogramaLBP(bdImg);
        distancia1 = lbp->calcularDistanciaEuclidea(histoRiceP, clase1);
        distancia2 = lbp->calcularDistanciaEuclidea(histoRiceP, clase2);

        if ((distancia1 && distancia2) != 0)
        {
            if (distancia1 > distancia2)
            {
                aciertos = aciertos + 1;
            }
            else
            {
                fallos = fallos + 1;
            }
        }
    }
    cout << "Numero de Aciertos clase: " << path << ", es: " << aciertos << endl;
    cout << "Numero de Fallos clase: " << path << ", es: " << fallos << endl;
}
int *Prueba::LBP8(const int *data, int rows, int columns)
{
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
    int r, c, cntr;
    unsigned int value;
    int *result = (int *)malloc(256 * sizeof(int));
    memset(result, 0, 256 * sizeof(int));
    for (r = 0; r < rows - 2; r++)
    {
        for (c = 0; c < columns - 2; c++)
        {
            value = 0;
            cntr = *center - 1;
            compab_mask_inc(p0, 0);
            compab_mask_inc(p1, 1);
            compab_mask_inc(p2, 2);
            compab_mask_inc(p3, 3);
            compab_mask_inc(p4, 4);
            compab_mask_inc(p5, 5);
            compab_mask_inc(p6, 6);
            compab_mask_inc(p7, 7);
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

//---------------------
int *LBPDescriptor::LBP8(const int *data, int rows, int columns)
{
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
    int r, c, cntr;
    unsigned int value;
    int *result = (int *)malloc(256 * sizeof(int));
    memset(result, 0, 256 * sizeof(int));
    for (r = 0; r < rows - 2; r++)
    {
        for (c = 0; c < columns - 2; c++)
        {
            value = 0;
            cntr = *center - 1;
            compab_mask_inc(p0, 0);
            compab_mask_inc(p1, 1);
            compab_mask_inc(p2, 2);
            compab_mask_inc(p3, 3);
            compab_mask_inc(p4, 4);
            compab_mask_inc(p5, 5);
            compab_mask_inc(p6, 6);
            compab_mask_inc(p7, 7);
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

vector<int> LBPDescriptor::LBP8(Mat imagen)
{

    int *data = (int *)malloc(imagen.rows * imagen.cols * sizeof(int));
    for (int i = 0, s = 0; i < imagen.rows; i++)
    {
        for (int j = 0; j < imagen.cols; j++, s++)
        {
            data[s] = (int)imagen.at<uchar>(i, j);
            //*(data+s) = (int) imagen.at<uchar>(i,j);
        }
    }

    int *descriptor = LBP8(data, imagen.rows, imagen.cols);
    vector<int> desc;
    for (int i = 0; i < 256; i++)
    {
        desc.push_back(descriptor[i]);
    }

    free(data);

    return desc;
}