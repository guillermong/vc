#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <algorithm>    // std::sort

using namespace cv;
using namespace std;

Mat src; Mat src_gray, lineas;
int thresh = 150;
int max_thresh = 255;


void getdb(vector<Mat> &abc);
vector<int> buscarverticales(vector<Point2f> puntos);
vector<int> buscarhorizontales(vector<Point2f> puntos);
void thresh_callback(int, void* );



/** @function main */
int main( int argc, char** argv )
{
  /// Load source image and convert it to gray
  src = imread( "C://Users//Manuela//Desktop//inf//vision por comptuador//proyectofinal//test.jpg", 1 );

  

  /// Convert image to gray and blur it
  cvtColor( src, src_gray, CV_BGR2GRAY );
  blur( src_gray, src_gray, Size(3,3) );

  /// Create Window
  char* source_window = "Source";
  namedWindow( source_window, CV_WINDOW_AUTOSIZE );
  imshow( source_window, src );

  createTrackbar( " Threshold:", "Source", &thresh, max_thresh, thresh_callback );
  thresh_callback( 0, 0 );

  waitKey(0);
  return(0);
}

/** @function thresh_callback */
void thresh_callback(int, void* )
{
  Mat threshold_output;
  vector<vector<Point> > contours;
  vector<Vec4i> hierarchy;

  /// Detect edges using Threshold
  threshold( src_gray, threshold_output, thresh, 255, THRESH_BINARY );
  lineas= threshold_output.clone();
  /// Find contours
  findContours( threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

  /// Approximate contours to polygons + get bounding rects and circles
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


  /// Draw polygonal contour + bonding rects + circles
  Mat drawing = Mat( threshold_output.size(), CV_8UC1, Scalar( 255 ) );
  cout <<contours.size()<<endl;
  Scalar color = Scalar( 0 );

  vector<vector<Point> > grupos;

  for( int i = contours.size()-1; i>=0 ; i-- )
     {
	   cout<<"centro:"<<center[i]<<" radio:"<<radius[i]<<endl;
	   if(radius[i] <4){
			//circle( drawing, center[i], 1, color, 2, 8, 0 );	          
			//drawContours( drawing, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
			//rectangle( drawing, boundRect[i].tl(), Point(boundRect[i].tl().x+2,boundRect[i].tl().y+3), color, 2, 8, 0 );
			for( int x =  boundRect[i].tl().x; x < boundRect[i].tl().x+3; x++)
			{
				for( int y =  boundRect[i].tl().y; y <  boundRect[i].tl().y+5; y++)
				{
					drawing.at<uchar>(y,x)=0;
				}
			}
	   }
	
     }

  
  vector<int> horizontales = buscarhorizontales(center);
  vector<int> verticales = buscarverticales(center);
  


  string probando = "abcdefghijklmnopqrstuvwxyz";
  vector<Mat> abc;
  getdb(abc);
  Mat ventana(50, 50, CV_8UC1,256);
  Mat result;
  int x;

  for(int h = 0 ; h < horizontales.size() ; h++ ){
	  
	  if(horizontales[h+1]-horizontales[h]>14)
		continue;
	  else{ 
		  x=horizontales[h];
		  h+=2;
	   }
	  for( int v = 0; v < verticales.size(); v=v+1)
	  {
					int y=verticales[v];
				
				
					if(v+1 < verticales.size())
						if((verticales[v+1]-y) <15) v++;
				
					cout<<"x,y="<<x<<","<<y<<endl;
					for( int i = 0; i < 25; i++){
						for( int j = 0; j < 16; j++){
							//IF (EN LA HORIZONTAL NO HAY MAS PUNTOS SE DESCARTA)
							//vector<puntos> lista;
							//buscarpuntos(lista,result);
							ventana.at<uchar>(i,j) = drawing.at<uchar>(x+i-3,y+j-3);
							drawing.at<uchar>(x+i-3,y+j-3)=255;
						}
					}

					//***********modificar desde aqui**************
					double min, max;
					int maxfinal = 0;
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
					imshow("Imagen4", drawing );

					cout<<max<<endl;
					if( maxfinal >= 0.8 ) 
					{
						string text = probando.substr(word,1);
						int fontFace = FONT_HERSHEY_SIMPLEX;
						double fontScale = 0.7;
						int thickness = 2;  
						cv::Point textOrg(y,x+10);

						cv::putText(src, text, textOrg, fontFace, fontScale, Scalar::all(255), thickness,8);
						rectangle( src, Point(y-5,x-10), Point(y+15,x+30), color, 2, 8, 0 );
					}
					waitKey(0);
			
		
	  }

	  /// Show in a window
	  namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
	  imshow( "Contours", src );
	  imshow( "lineas", lineas );
	}
}


vector<int> buscarhorizontales(vector<Point2f> puntos){

	vector<vector<Point2f>> ghor;
	vector<int> poshorizontales;

	while (puntos.size()>0){
		
		vector<Point2f> horizontal;
		int promedio=puntos[0].y;

		poshorizontales.push_back(promedio);
		horizontal.push_back(puntos[0]);
		line( lineas, Point( 0, promedio ), Point( src.cols, promedio ), Scalar( 125 ),  1, 8 );
		puntos.erase(puntos.begin() + 0);
		

		for(int i =0 ;i<puntos.size();i++){
			
			if( puntos[i].y < promedio+3 && puntos[i].y > promedio-3 ){
					horizontal.push_back(puntos[i]);
					puntos.erase(puntos.begin() + i);
					i--;
			}
		}
		ghor.push_back(horizontal);
	}
	sort(poshorizontales.begin(),poshorizontales.end());
	cout <<"Horizontales:"<< ghor.size()<<endl;
	//return ghor;
	return poshorizontales;
}

vector<int> buscarverticales(vector<Point2f> puntos){

	vector<vector<Point2f>> gver;
	vector<int> posverticales;

	while (puntos.size()>0){
		
		vector<Point2f> vertical;
		int promedio=puntos[0].x;

		posverticales.push_back(promedio);
		vertical.push_back(puntos[0]);
		line( lineas, Point( promedio, 0 ), Point( promedio, src.cols), Scalar( 125 ),  1, 8 );
		puntos.erase(puntos.begin() + 0);
		
		for(int i =0 ;i<puntos.size();i++){
			
			if( puntos[i].x < promedio+3 && puntos[i].x > promedio-3 ){
					vertical.push_back(puntos[i]);
					puntos.erase(puntos.begin() + i);
					i--;
			}
		}
		gver.push_back(vertical);
	}
	sort(posverticales.begin(),posverticales.end());
	/*for(int i =0 ;i<posverticales.size();i++){
		cout<<posverticales[i]<<endl;
	}
	*/
	cout <<"verticales:"<< gver.size()<<endl;
	//return gver;
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
