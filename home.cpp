/**
	* @file main.cpp
	* @version 2.0
	* @date 10/04/2015
	* @author Alan Collante & Guillermo Navarro
	* @bug Puntos que esten proximos al marco de la imagen produce errores
	
	main.cpp es un programa para traducir una imagen con sistema braille a caracteres

	*/


#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>    // std::sort

using namespace cv;
using namespace std;
/** * src Imagen cargada */
Mat src; 
/** * src_gray Imagen cargada a escala gris */
Mat src_gray;
/** * lineas Imagen cargada a escala gris con las verticales y horizonatles */
Mat lineas;
/** * tresh Valor del treshold */
int thresh = 150;
/** * tresh Valor maximo del treshold */
int max_thresh = 255;

/** 
	* @brief Carga los caracteres en sistema braille a memoria
	* @param abc Vector vacia de imagenes.
	* @return abc Vector con las imagenes de los caracteres en sistema braille cargadas.
	*/
void getdb(vector<Mat> &abc);
/** 
	* @brief buscarverticales busca los verticales segun los puntos encontrados.
	* @param puntos Puntos encontrados en la imagen.
	* @return puntos retorna un vector con las verticales encontradas.
	*/
vector<int> buscarverticales(vector<Point2f> puntos, vector<Point2f> &puntos2);
/** 
	* @brief buscarhorizontales busca los horizontales segun los puntos encontrados.
	* @param puntos Puntos encontrados en la imagen.
	* @return puntos retorna un vector con las horizontales encontradas.
	*/
vector<int> buscarhorizontales(vector<Point2f> puntos , vector<Point2f> &puntos2);
/** 
	* @brief thresh_callback procesa la imagen cargada para traducirlo.
	* @return Imagen traducida.
	*/
void thresh_callback(int, void* );
void thresh_callback2(int, void* );
/** 
	* @brief main
	*/

int main( int argc, char** argv )
{
	/// Load source image
	//Mat abc = imread( "C://Users//Manuela//Desktop//inf//vision por comptuador//proyectofinal//abc.jpg", 1 );
	//imshow( "ABC", abc );

	//src = imread( "C://Users//Manuela//Desktop//inf//vision por comptuador//proyectofinal//test3.jpg", 1 );
	src = imread( "C://Users//Manuela//Desktop//inf//vision por comptuador//proyectofinal//test.jpg", 1 );

	src = imread( "C://Users//Manuela//Desktop//inf//vision por comptuador//proyectofinal//test13.jpg", 1 );
	//src = imread( "C://Users//Manuela//Desktop//inf//vision por comptuador//proyectofinal//test11.jpg", 1 );
	
	src = imread( "C://Users//Manuela//Desktop//inf//vision por comptuador//proyectofinal//test15.jpg", 1 );
	src = imread( "C://Users//Manuela//Desktop//inf//vision por comptuador//proyectofinal//test10.jpg", 1 );



	cvtColor( src, src_gray, CV_BGR2GRAY );
	blur( src_gray, src_gray, Size(3,3) );
	/// Create Window
	char* source_window = "Source";
	namedWindow( source_window, CV_WINDOW_AUTOSIZE );
	imshow( source_window, src );
	waitKey(0);
	createTrackbar( " Threshold:", "Source", &thresh, max_thresh, thresh_callback );
	//debug
	//thresh_callback( 0, 0 );
	//nodebug
	thresh_callback2( 0, 0 );

	waitKey(0);
	return(0);
}


void thresh_callback(int, void* )
{
  Mat threshold_output;
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;

  threshold( src_gray, threshold_output, thresh, 255, THRESH_BINARY );
  imshow( "drawing", threshold_output );
  waitKey(0);
  lineas= threshold_output.clone();
  findContours( threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

  vector<vector<Point> > contours_poly( contours.size() );
  vector<Rect> boundRect( contours.size() );
  vector<Point2f>center( contours.size() );
  vector<float>radius( contours.size() );

  for( int i = 0; i < contours.size(); i++ )
  { 
	   approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
       boundRect[i] = boundingRect( Mat(contours_poly[i]) );
       minEnclosingCircle( (Mat)contours_poly[i], center[i], radius[i] );
  }

  Mat drawing = Mat( threshold_output.size(), CV_8UC1, Scalar( 255 ) );
  Scalar color = Scalar( 0,123,46 );


  vector<int> horizontales = buscarhorizontales(center, center);
  vector<int> verticales = buscarverticales(center,center);
  imshow( "lineas", lineas );
  waitKey(0);

  for( int i = center.size()-1; i>=0 ; i-- )
     {
	   if(radius[i] <4){
			for( int x =  center[i].x; x < center[i].x+3; x++)
			{
				for( int y =  center[i].y; y <  center[i].y+5; y++)
				{
					if(x<drawing.cols && y < drawing.rows)
						drawing.at<uchar>(y,x)=0;
				}
			}
	   }
	
     }


  
  string probando = "abcdefghijklmnopqrstuvwxyz";
  vector<Mat> abc;
  getdb(abc);
  Mat ventana(50, 50, CV_8UC1,256);
  Mat result;
  int x;

  for(int h = 0 ; h < horizontales.size() ; h++ ){
	  if(h+1<horizontales.size())
	  if(horizontales[h+1]-horizontales[h]>14)
		continue;
	  else{ 
		  x=horizontales[h];
		  h+=2;
	   }
	  for( int v = 0; v < verticales.size(); v=v+1)
	  {
					int y=verticales[v];			
					if(v+1 < verticales.size()){
						if((verticales[v+1]-y) <15) v++;
						cout<<" ";
					}
					for( int i = 0; i < 30; i++){
						for( int j = 0; j < 16; j++){

							if(x+i-3 < drawing.rows-1   && x+i-3 >= 0 && y+i-5 < drawing.cols-1  && y+i-5>=0){
								ventana.at<uchar>(i,j) = drawing.at<uchar>(x+i-3,y+j-4);
								drawing.at<uchar>(x+i-3,y+j-4)=255;
							}
						}
					}
					double min, max;
					int maxfinal = 0;
					int word = 0;

					imshow( "drawing", drawing );
					imshow( "ventana", ventana );
					

					for(int t1=0 ;t1 < abc.size(); t1++){
						matchTemplate( ventana, abc[t1], result, 4 );
						minMaxLoc(result, &min, &max);
						if(maxfinal <= max) {
							maxfinal= max;
							word=t1;
						}
					}

					if( maxfinal >= 0.8 ) 
					{
						string text = probando.substr(word,1);
						cout<<text;
						int fontFace = FONT_HERSHEY_SIMPLEX;
						double fontScale = 0.5;
						int thickness = 2;  
						cv::Point textOrg(y,x+10);
						cv::putText(src, text, textOrg, fontFace, fontScale, Scalar(255,54,66), thickness,8);
						rectangle( src, Point(y-5,x-10), Point(y+15,x+30), color, 2, 8, 0 );
					}
					waitKey(0);
		
	  }
	  /// Show in a window
	  namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
	  imshow( "Contours", src );
	  
	}
}


void thresh_callback2(int, void* )
{
  Mat threshold_output;
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;

  threshold( src_gray, threshold_output, thresh, 255, THRESH_BINARY );
  //imshow( "drawing", threshold_output );

  lineas= threshold_output.clone();
  findContours( threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

  vector<vector<Point> > contours_poly( contours.size() );
  vector<Rect> boundRect( contours.size() );
  vector<Point2f>center( contours.size() );
  vector<float>radius( contours.size() );

  for( int i = 0; i < contours.size(); i++ )
  { 
	   approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
       boundRect[i] = boundingRect( Mat(contours_poly[i]) );
       minEnclosingCircle( (Mat)contours_poly[i], center[i], radius[i] );
  }

  Mat drawing = Mat( threshold_output.size(), CV_8UC1, Scalar( 255 ) );
  Scalar color = Scalar( 0,123,46 );


  vector<int> horizontales = buscarhorizontales(center, center);
  vector<int> verticales = buscarverticales(center,center);
  //imshow( "lineas", lineas );
  //waitKey(0);

  for( int i = center.size()-1; i>=0 ; i-- )
     {
	   if(radius[i] <4){
			for( int x =  center[i].x; x < center[i].x+3; x++)
			{
				for( int y =  center[i].y; y <  center[i].y+5; y++)
				{
					if(x<drawing.cols && y < drawing.rows)
						drawing.at<uchar>(y,x)=0;
				}
			}
	   }
	
     }


  
  string probando = "abcdefghijklmnopqrstuvwxyz";
  vector<Mat> abc;
  getdb(abc);
  Mat ventana(50, 50, CV_8UC1,256);
  Mat result;
  int x;

  for(int h = 0 ; h < horizontales.size() ; h++ ){
	  if(h+1<horizontales.size())
	  if(horizontales[h+1]-horizontales[h]>14)
		continue;
	  else{ 
		  x=horizontales[h];
		  h+=2;
	   }
	  for( int v = 0; v < verticales.size(); v=v+1)
	  {
					int y=verticales[v];			
					if(v+1 < verticales.size()){
						if((verticales[v+1]-y) <15) v++;
						cout<<" ";
					}
					for( int i = 0; i < 30; i++){
						for( int j = 0; j < 16; j++){

							if(x+i-3 < drawing.rows-1   && x+i-3 >= 0 && y+i-5 < drawing.cols-1  && y+i-5>=0){
								ventana.at<uchar>(i,j) = drawing.at<uchar>(x+i-3,y+j-4);
								drawing.at<uchar>(x+i-3,y+j-4)=255;
							}
						}
					}
					double min, max;
					int maxfinal = 0;
					int word = 0;

					//imshow( "drawing", drawing );
					//imshow( "ventana", ventana );
					

					for(int t1=0 ;t1 < abc.size(); t1++){
						matchTemplate( ventana, abc[t1], result, 4 );
						minMaxLoc(result, &min, &max);
						if(maxfinal <= max) {
							maxfinal= max;
							word=t1;
						}
					}

					if( maxfinal >= 0.8 ) 
					{
						string text = probando.substr(word,1);
						cout<<text;
						int fontFace = FONT_HERSHEY_SIMPLEX;
						double fontScale = 0.5;
						int thickness = 2;  
						cv::Point textOrg(y,x+10);
						cv::putText(src, text, textOrg, fontFace, fontScale, Scalar(0,0,255), thickness,8);
						rectangle( src, Point(y-5,x-10), Point(y+15,x+30), color, 2, 8, 0 );
					}
					//waitKey(0);
		
	  }
	  /// Show in a window
	  namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
	  imshow( "Contours", src );
	  
	}
}



vector<int> buscarhorizontales(vector<Point2f> puntos,vector<Point2f> &puntos2){

	vector<int> poshorizontales;
	int divisor,suma;

	while (puntos.size()>0){
		
		vector<Point2f> horizontal;
		suma=puntos[0].y;
		divisor=1;
		int promedio=suma/divisor;

		horizontal.push_back(puntos[0]);	
		puntos.erase(puntos.begin() + 0);
		
		for(int i =0 ;i<puntos.size();i++){	
			if( puntos[i].y < promedio+3 && puntos[i].y > promedio-3 ){
					suma+=puntos[i].y;
					divisor++;
					promedio=suma/divisor;

					horizontal.push_back(puntos[i]);
					puntos.erase(puntos.begin() + i);
					i--;
			}
		}
		line( lineas, Point( 0, promedio ), Point( src.cols, promedio ), Scalar( 125 ),  1, 8 );
		poshorizontales.push_back(promedio);
	}
	sort(poshorizontales.begin(),poshorizontales.end());
	//ARREGLAR
	/*for(int j =1 ; j<poshorizontales.size();j++)
		if( poshorizontales[j] < poshorizontales[j-1]+14){
			poshorizontales[j]=poshorizontales[j-1]+10;
		}
	*/

	for(int i =0 ;i<puntos2.size();i++){
		for(int j =0 ; j<poshorizontales.size();j++){
			if( puntos2[i].y < poshorizontales[j]+3 && puntos2[i].y > poshorizontales[j]-3 ){
				puntos2[i].y=poshorizontales[j];
			}
		}
	}

	return poshorizontales;
}

vector<int> buscarverticales(vector<Point2f> puntos, vector<Point2f> &puntos2){

	vector<int> posverticales;
	int divisor,suma;

	while (puntos.size()>0){
		
		vector<Point2f> vertical;
		suma=puntos[0].x;
		divisor=1;
		int promedio=suma/divisor;
	
		vertical.push_back(puntos[0]);		
		puntos.erase(puntos.begin() + 0);
		
		for(int i =0 ;i<puntos.size();i++){			
			if( puntos[i].x < promedio+3 && puntos[i].x > promedio-3 ){	
					suma+=puntos[i].x;
					divisor++;
					promedio=suma/divisor;

					vertical.push_back(puntos[i]);
					puntos.erase(puntos.begin() + i);
					i--;
			}
		}
		line( lineas, Point( promedio, 0 ), Point( promedio, src.cols), Scalar( 125 ),  1, 8 );
		posverticales.push_back(promedio);
	}
	sort(posverticales.begin(),posverticales.end());

	//ARREGLAR
	/*for(int j =1 ; j<posverticales.size();j++)
		if( posverticales[j] < posverticales[j-1]+9){
			posverticales[j]=posverticales[j-1]+9;
		}
	*/	

	for(int i =0 ;i<puntos2.size();i++){
		for(int j =0 ; j< posverticales.size();j++){
			if( puntos2[i].x < posverticales[j]+3 && puntos2[i].x > posverticales[j]-3 ){
				puntos2[i].x=posverticales[j];
			}
		}
	}

	return posverticales;
}


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
