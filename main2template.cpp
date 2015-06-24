/**
	* @file main.cpp
	* @version 1.0
	* @date 17/04/2015
	* @author Alan Collante & Guillermo Navarro
	* @bug No bugs conocidos 
	
	main.cpp es un programa que de reconocimiento del sistema brailler, recibe una imagen de entrada y verifica si es un sistema brailler para
	posteriormente traducirlo a caracteres, utilizando varias tecnicas como el tamplate matching.

	*/



#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

using namespace std;
using namespace cv;

/** 
	* @brief main Recibe una imagen con sistema brailler de entrada y lo traduce a caracteres.
	*/

void getdb(vector<Mat> &abc)
{
  Mat temp,temp_g;
  
  temp= imread( "C://Users//Manuela//Desktop//inf//vision por comptuador//proyectofinal//a.jpg" );
  cvtColor( temp, temp, COLOR_BGR2GRAY );
  abc.push_back(temp);
  temp= imread( "C://Users//Manuela//Desktop//inf//vision por comptuador//proyectofinal//b.jpg" );
  cvtColor( temp, temp, COLOR_BGR2GRAY );
  abc.push_back(temp);
  temp= imread( "C://Users//Manuela//Desktop//inf//vision por comptuador//proyectofinal//c.jpg" );
  cvtColor( temp, temp, COLOR_BGR2GRAY );
  abc.push_back(temp);
  temp= imread( "C://Users//Manuela//Desktop//inf//vision por comptuador//proyectofinal//d.jpg" );
  cvtColor( temp, temp, COLOR_BGR2GRAY );
  abc.push_back(temp);
  temp= imread( "C://Users//Manuela//Desktop//inf//vision por comptuador//proyectofinal//e.jpg" );
  cvtColor( temp, temp, COLOR_BGR2GRAY );
  abc.push_back(temp);
  temp= imread( "C://Users//Manuela//Desktop//inf//vision por comptuador//proyectofinal//f.jpg" );
  cvtColor( temp, temp, COLOR_BGR2GRAY );
  abc.push_back(temp);
  temp= imread( "C://Users//Manuela//Desktop//inf//vision por comptuador//proyectofinal//g.jpg" );
  cvtColor( temp, temp, COLOR_BGR2GRAY );
  abc.push_back(temp);
  temp= imread( "C://Users//Manuela//Desktop//inf//vision por comptuador//proyectofinal//h.jpg" );
  cvtColor( temp, temp, COLOR_BGR2GRAY );
  abc.push_back(temp);
  temp= imread( "C://Users//Manuela//Desktop//inf//vision por comptuador//proyectofinal//i.jpg" );
  cvtColor( temp, temp, COLOR_BGR2GRAY );
  abc.push_back(temp);
  temp= imread( "C://Users//Manuela//Desktop//inf//vision por comptuador//proyectofinal//j.jpg" );
  cvtColor( temp, temp, COLOR_BGR2GRAY );
  abc.push_back(temp);
  temp= imread( "C://Users//Manuela//Desktop//inf//vision por comptuador//proyectofinal//k.jpg" );
  cvtColor( temp, temp, COLOR_BGR2GRAY );
  abc.push_back(temp);
  temp= imread( "C://Users//Manuela//Desktop//inf//vision por comptuador//proyectofinal//l.jpg" );
  cvtColor( temp, temp, COLOR_BGR2GRAY );
  abc.push_back(temp);
  temp= imread( "C://Users//Manuela//Desktop//inf//vision por comptuador//proyectofinal//m.jpg" );
  cvtColor( temp, temp, COLOR_BGR2GRAY );
  abc.push_back(temp);
  temp= imread( "C://Users//Manuela//Desktop//inf//vision por comptuador//proyectofinal//n.jpg" );
  cvtColor( temp, temp, COLOR_BGR2GRAY );
  abc.push_back(temp);
  temp= imread( "C://Users//Manuela//Desktop//inf//vision por comptuador//proyectofinal//o.jpg" );
  cvtColor( temp, temp, COLOR_BGR2GRAY );
  abc.push_back(temp);
  temp= imread( "C://Users//Manuela//Desktop//inf//vision por comptuador//proyectofinal//p.jpg" );
  cvtColor( temp, temp, COLOR_BGR2GRAY );
  abc.push_back(temp);
  temp= imread( "C://Users//Manuela//Desktop//inf//vision por comptuador//proyectofinal//q.jpg" );
  cvtColor( temp, temp, COLOR_BGR2GRAY );
  abc.push_back(temp);
  temp= imread( "C://Users//Manuela//Desktop//inf//vision por comptuador//proyectofinal//r.jpg" );
  cvtColor( temp, temp, COLOR_BGR2GRAY );
  abc.push_back(temp);
  temp= imread( "C://Users//Manuela//Desktop//inf//vision por comptuador//proyectofinal//s.jpg" );
  cvtColor( temp, temp, COLOR_BGR2GRAY );
  abc.push_back(temp);
  temp= imread( "C://Users//Manuela//Desktop//inf//vision por comptuador//proyectofinal//t.jpg" );
  cvtColor( temp, temp, COLOR_BGR2GRAY );
  abc.push_back(temp);
  temp= imread( "C://Users//Manuela//Desktop//inf//vision por comptuador//proyectofinal//u.jpg" );
  cvtColor( temp, temp, COLOR_BGR2GRAY );
  abc.push_back(temp);
  temp= imread( "C://Users//Manuela//Desktop//inf//vision por comptuador//proyectofinal//v.jpg" );
  cvtColor( temp, temp, COLOR_BGR2GRAY );
  abc.push_back(temp);
  temp= imread( "C://Users//Manuela//Desktop//inf//vision por comptuador//proyectofinal//w.jpg" );
  cvtColor( temp, temp, COLOR_BGR2GRAY );
  abc.push_back(temp);
  temp= imread( "C://Users//Manuela//Desktop//inf//vision por comptuador//proyectofinal//x.jpg" );
  cvtColor( temp, temp, COLOR_BGR2GRAY );
  abc.push_back(temp);
  temp= imread( "C://Users//Manuela//Desktop//inf//vision por comptuador//proyectofinal//y.jpg" );
  cvtColor( temp, temp, COLOR_BGR2GRAY );
  abc.push_back(temp);
  temp= imread( "C://Users//Manuela//Desktop//inf//vision por comptuador//proyectofinal//z.jpg" );
  cvtColor( temp, temp, COLOR_BGR2GRAY );
  abc.push_back(temp);
  return;
}

/*void buscarpuntos(vector<puntos> &lista, Mat img)
{
	for( int w = 0; w < img.rows; w++)
	{
		for( int h = 0; h < img.cols; h++)
		{
			int i;
		}
	}
	
}*/





int main( int argc, char** argv )
{
	/** 
		* Luego de recibir la imagen y traducirlo a una imagen de estala gris para eliminar informacion que no es necesaria, se ejecuta la funcion
		* de treshold para detectar los puntos del sistema braille, despues se itera la imagen buscando algun punto negro , cuando encuentra un punto
		* se estrablece una ventana del tamano del template, y se ejecuta la funcion template matching sobre la ventana, luego si el template matching
		* supera cierto umbral entonces se encuentra un caracter y se imprime en la imagen.
		* 
		* 
	*/
  /** * img Imagen de entrada  */
  Mat img;
  /** * img_g Imagen de entrada a la escala gris  */
  Mat img_g;
  Mat dst;


  img = imread( "C://Users//Manuela//Desktop//inf//vision por comptuador//proyectofinal//test.jpg" ); 
  cvtColor( img, img_g, COLOR_BGR2GRAY );
  imshow("Imagen0", img_g );
 
  string probando = "abcdefghijklmnopqrstuvwxyz";
  vector<Mat> abc;
  getdb(abc);

  Mat result;

  threshold( img_g, dst, 150, 255, 1 );

  imshow("Imagen4", dst );

  Mat ventana(50, 50, CV_8UC1);
  //Mat ventana= Mat::ones(50, 50, CV_8UC1);


  vector < vector<Point> > contours;



  for( int x = 0; x < dst.rows; x++)
  {
		for( int y = 0; y < dst.cols; y++)
		{
			if(dst.at<uchar>(x,y) == 0) {

				for( int i = 0; i < 25; i++){
					for( int j = 0; j < 14; j++){
						//IF (EN LA HORIZONTAL NO HAY MAS PUNTOS SE DESCARTA)
						//vector<puntos> lista;
						//buscarpuntos(lista,result);
						ventana.at<uchar>(i,j) = dst.at<uchar>(x+i,y+j);
						dst.at<uchar>(x+i,y+j)=255;
					}
				}

				//***********modificar desde aqui**************
				double min, max;
				double maxfinal = 0;
				int word = 0;

				imshow("ventana", ventana );
				

				for(int t1=0 ;t1 < abc.size(); t1++){


					matchTemplate( ventana, abc[t1], result, 4 );
					minMaxLoc(result, &min, &max);
					if(maxfinal <= max) {
						maxfinal= max;
						word=t1;
					}
				}
				
				//imshow("Imagen65", ventana );
				imshow("Imagen5", result );
				imshow("Imagen4", dst );


				if( maxfinal >= 0.55 ) 
				{
					string text = probando.substr(word,1);
					int fontFace = FONT_HERSHEY_SIMPLEX;
					double fontScale = 0.7;
					int thickness = 2;  
					cv::Point textOrg(y,x+10);

					cv::putText(img_g, text, textOrg, fontFace, fontScale, Scalar::all(255), thickness,8);

				}
				waitKey(0);
			}
		}
  }
  imshow("Imagen77", img_g );


  waitKey(0);
  return 0;
}