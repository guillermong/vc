#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <string>
#include <vector>

using namespace std;
using namespace cv;

Mat integralimage(Mat in){
	Mat Int = in.clone();

	for( int x = 0; x < in.rows; x++)
	{
		for( int y = 0; y < in.cols; y++)
		{
			uchar out = in.at<uchar>(x,y);

			if( x - 1 >= 0 ) out += in.at<uchar>(x - 1,y);
			if( y - 1 >= 0 ) out += in.at<uchar>(x, y - 1 );
			if( x - 1 >= 0 && y - 1 >= 0 ) out -= in.at<uchar>(x - 1,y - 1);
			Int.at<uchar>(x,y)= out;
		
		}
	}
	return Int;
}

Mat windo(Mat Int){
	
	Mat f = Int.clone();
	 //supresion de no maximos
	int c1=6;
	float p = 0.15;

	/*for(int r=c1/2;r< Int.rows-c1/2; r++){
		for(int c=c1/2;c< Int.cols-c1/2; c++){
			float s=0;
			for(int x=-c1/2;x< (c1/2)+1; x++){
				for(int y=c1/2;y< (c1/2)+1; y++){

						//s += Int.at<char>(r,c);
						s += Int.at<char>(r,c);
						
					}
			}
			//f.at<char>(r,c)= s/(c1*c1);
			if ( f.at<char>(r,c) <= (s*(1.0-p))/(c1*c1)) f.at<char>(r,c)= 255;
			else f.at<char>(r,c)= 0;
		}
	}
	*/
	/*for( int x = c1/2; x < Int.rows-c1/2; x++)
	{
		for( int y = c1/2; y < Int.cols-c1/2; y++)
		{
			f.at<char>(x,y)= Int.at<char>(x,y)- Int.at<char>(x+c1,y-c1-1) - Int.at<char>(x-c1-1,y+c1) + Int.at<char>(x+c1-1,y-c1-1);
		}
	}
	*/

	return f;
}


int main( int argc, char** argv )
{
  Mat img,img_g;
  img = imread( "C://Users//Manuela//Desktop//inf//vision por comptuador//proyectofinal//test.jpg" ); 
  cvtColor( img, img_g, COLOR_BGR2GRAY );
  imshow("Imagen0", img_g );

  //Mat integral = integralimage(img_g);
  //imshow("Imagen1", integral );
 // Mat bin = windo(integral);
 // imshow("Imagen3", bin );

  


  //*********************

  Mat dst;
  Mat temp= imread( "C://Users//Manuela//Desktop//inf//vision por comptuador//proyectofinal//b.jpg" ); 
  Mat template1;
  cvtColor( temp, template1, COLOR_BGR2GRAY );
  Mat result;
  threshold( img_g, dst, 150, 255, 1 );
  imshow("Imagen4", dst );
  imshow("Imagen7", template1 );
  Mat f;
  Mat ventana(50, 50, CV_8UC1);
  //Mat ventana= Mat::ones(50, 50, CV_8UC1);


  for( int x = 0; x < dst.rows; x++)
  {
		for( int y = 0; y < dst.cols; y++)
		{
			if(dst.at<uchar>(x,y) <= 100) {
				//for( int i = 0; i < ventana.rows; i++){
				//	for( int j = 0; j < ventana.cols; j++){
				for( int i = 0; i < 25; i++){
					for( int j = 0; j < 14; j++){
						//IF (EN LA HORIZONTAL NO HAY MAS PUNTOS SE DESCARTA)
						ventana.at<uchar>(i,j) = dst.at<uchar>(x+i,y+j);
						dst.at<uchar>(x+i,y+j)=255;
					}
				}

				matchTemplate( ventana, template1, result, 5 );

				imshow("Imagen65", ventana );
				imshow("Imagen5", result );
				imshow("Imagen4", dst );

				
				for( int w = 0; w < result.rows; w++){
					for( int h = 0; h < result.cols; h++){
						if(result.at<float>(w,h) >= 0.55 ) {
							cout<<"encontrado en pos "<<x <<" "<< y;

							string text = "y";
							int fontFace = FONT_HERSHEY_SIMPLEX;
							double fontScale = 1;
							int thickness = 3;  
							cv::Point textOrg(y,x );


							cv::putText(img_g, text, textOrg, fontFace, fontScale, Scalar::all(255), thickness,8);


						}
					}
				}
				waitKey(0);
			}
		}
  }
  

 // matchTemplate( dst, template1, result, 5 );
 //imshow("Imagen5", result );

  imshow("Imagen77", img_g );


  waitKey(0);
  return 0;
}