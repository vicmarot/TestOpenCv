
//#include "stdafx.h"
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

using namespace std;
using namespace cv;


class OpImages {

    private:
        int histSize = 256;
        bool uniform = true; bool accumulate = false;
         // Draw the histograms for B, G and R
        int hist_w = 512; int hist_h = 400;
        int bin_w = cvRound( (double) hist_w/histSize );

    public:
        vector <Mat> Images;
        void InputImages ();
        void ImgHistogram ();
        void ImgHsv();
};

int main()
{
    OpImages OpImages;
    
    OpImages.InputImages();
    //OpImages.ImgHistogram();
    OpImages.ImgHsv();

    //Esperar a pulsar una tecla
    waitKey(0);

    return 0;
}

void OpImages::ImgHsv(){

    vector <Mat> InHsv, Sout, Brightness;
    Mat Out;
    char Title [50];
    double Max, Min;
    unsigned int IChoose;
    vector <double> Avg;

    cout << __PRETTY_FUNCTION__ << endl;

    for (unsigned int j=0; j<Images.size()-1;j++){

        cvtColor( Images.at(j), Out, COLOR_BGR2HSV);
        InHsv.push_back(Out);
        split(InHsv.at(j),Sout);
        Brightness.push_back(Sout.at(2));
        minMaxLoc(Brightness.at(j),&Min,&Max);
        Avg.push_back((Min+Max)/2); // quick aprox, the picture which more near be to 255/2 will be the best contrast

    }

    for (unsigned int j=0; j<3; j++){ // se pone 3 en el j<3 xq solo se quieren mirar las 3 primeras imagenes, si se quisiera mirar todas las entradsa se deberia poner Avg.size()
        Avg.at(j)=abs(Avg.at(j)-(256/2));
    }
    
    vector <double>::iterator result;
    result = min_element(Avg.begin(),Avg.end());
    IChoose = distance(Avg.begin(), result);
    cout << "result: " << *result << endl;
    cout << "The picture with best constrast is the image " << IChoose << "(Startint the count from 0)" << endl;
    cout << "The picture with best constrast is the image " << IChoose+1 << "(Startint the count from 1)" << endl;

    sprintf(Title,"Best image (contrast)");
    namedWindow(Title, CV_WINDOW_AUTOSIZE);
    imshow(Title, Images.at(IChoose) );

}

void OpImages::ImgHistogram (){

    /// Set the ranges ( for B,G,R) )
    float range[] = {0,256};
    const float* histRange = { range };
    vector <Mat> HistOut;
    vector <Mat> InGray;
    Mat Out, Out2, gray, gray2;
    cout << "start" << endl;
    cout << "size" << Images.size() << endl;
    for (unsigned int j=0; j<Images.size()-1;j++){

        cvtColor( Images.at(j), Out, COLOR_BGR2GRAY);
        InGray.push_back(Out);

    }

    cout << "grey" << endl;
    
    /*cvtColor( Images.at(0), gray, COLOR_BGR2GRAY);
    cvtColor( Images.at(0), gray2, COLOR_BGR2GRAY);
    //imshow("gray",gray);
    calcHist( &gray, 1, 0, Mat(), Out, 1, &histSize, &histRange, uniform, accumulate );
    normalize( Out, Out, 0, 1, NORM_MINMAX, -1, Mat() );
    calcHist( &gray, 1, 0, Mat(), Out2, 1, &histSize, &histRange, uniform, accumulate );
    normalize( Out2, Out2, 0, 1, NORM_MINMAX, -1, Mat() );
   // double base_base;*/


    for (unsigned int j=0; j<Images.size()-1;j++){

        calcHist( &InGray.at(j), 1, 0, Mat(), Out, 1, &histSize, &histRange, uniform, accumulate );
        normalize( Out, Out, 0, 1, NORM_MINMAX, -1, Mat() );
        HistOut.push_back(Out);
    }

    cout << "Hist" << endl;

    for( int compare_method = 0; compare_method < 4; compare_method++ )
        {
            double Comp12 = compareHist( HistOut.at(0), HistOut.at(1), compare_method );
            double Comp13 = compareHist( HistOut.at(0), HistOut.at(2), compare_method );
            cout << "Method " << compare_method<< endl << " 1-2=>" << Comp12<< endl <<
             "1-3=>" <<  Comp13 << endl;
        }
    
    /*    cout<< "hist" << Out << endl;
    
    Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );
    
    for( int i = 1; i < histSize; i++ )
    {
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(Out.at<float>(i-1)) ) ,
                    Point( bin_w*(i), hist_h - cvRound(Out.at<float>(i)) ),
                    Scalar( 255, 0, 0), 1, 8, 0  );
    }
    imshow("original",histImage);*/

}


void OpImages::InputImages ()
{
    char ImagePath[60];
    int i=0;
    while(1){
        sprintf(ImagePath,"/home/optiva/ProyectosRD/TestOpenCv/resources/Imagen%d.jpg",i+1);
        

        Images.push_back(imread(ImagePath));

        if( !Images.at(i).data ) {
                cout<< "No more images to load. Final image number:" << i+1 <<endl;
                //i--;
                break;
        }
        i++;
    }
}

/******










#include "stdafx.h"

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>

using namespace std;
using namespace cv;

//-------------  LUTs
int main()
{
	//Nombre del fichero
	char NombreImagen[] = "ivvi_684x684_gray.jpg";
	
	Mat imagen;
	Mat ImagenResultadoLUT;

	//Cargamos la imagen y se comprueba que lo ha hecho correctamente
	imagen = cv::imread(NombreImagen);
	if (!imagen.data) {
		cout << "Error al cargar la imagen: " << NombreImagen << endl;
		exit(1);
	}

	Mat lut(1,256,CV_8U);
	
	for (int i = 0; i < 256; i++){
		lut.at<uchar>(i) = 255 - i; //Función Inversa
	}
	LUT(imagen, lut, ImagenResultadoLUT);

	//Mostrar la imagen
	namedWindow("Original", CV_WINDOW_AUTOSIZE);
	imshow("Original", imagen );

	namedWindow("ImagenResultadoLUT", CV_WINDOW_AUTOSIZE);
	imshow("ImagenResultadoLUT", ImagenResultadoLUT);
	
	//Esperar a pulsar una tecla
	cvWaitKey(0);
	return 0;
}












*/