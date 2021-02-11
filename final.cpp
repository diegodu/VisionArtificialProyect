#include "opencv2/opencv.hpp"
#include "opencv2/imgcodecs.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/videoio.hpp"
#include <opencv2/highgui.hpp>
#include <opencv2/video.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <dirent.h>
#include <opencv2/features2d/features2d.hpp> // Librería que contiene funciones para detección de descriptores como SIFT

#include <opencv2/xfeatures2d/nonfree.hpp> // Librería que contiene funciones para detección de descriptores (que no son libres) como SURF
#include <ctime>
#include <sstream>
#include <vector>
#include <time.h>
#include <dirent.h>
#include <list>
#include <map>
#include<string>
#include <fstream>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/video/video.hpp>
#include <opencv2/videoio/videoio.hpp>
#include <opencv2/imgcodecs/imgcodecs.hpp>
using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;
void eventoTrack(int v, void* pP) {

}

int main() {
    VideoCapture videoCapture(0);

    if (!videoCapture.isOpened()) {
        cout << "Can't find camera!" << endl;
        return -1;
    }

    Mat frame, frameOut, hsv, bw, ycrcb;
    int findBiggestContour(vector<vector<Point> >);

    Mat canny_output;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    Mat dilatado;
    int hMin = 0;
    int sMin = 0;
    int vMin = 0;

    int hMax = 255;
    int sMax = 180;
    int vMax = 240;

    int yMin = 0;
    int crMin = 0;
    int cbMin = 0;

    int yMax = 0;
    int crMax = 0;
    int cbMax = 0;
    //Mat mano;
    Mat mano = Mat(Size(290, 480), CV_8UC3, Scalar(0, 0, 0));
    Mat camara;
    string texto="Esperando...";
    while (true) {
        videoCapture >> frame;
      
       
        namedWindow("frame", WINDOW_AUTOSIZE);
        createTrackbar("H-Min", "frame", &hMin, 180, eventoTrack, NULL);
        createTrackbar("S-Min", "frame", &sMin, 255, eventoTrack, NULL);
        createTrackbar("V-Min", "frame", &vMin, 255, eventoTrack, NULL);
        createTrackbar("H-Max", "frame", &hMax, 180, eventoTrack, NULL);
        createTrackbar("S-Max", "frame", &sMax, 255, eventoTrack, NULL);
        createTrackbar("V-Max", "frame", &vMax, 255, eventoTrack, NULL);
        createTrackbar("Y-Min", "frame", &yMin, 255, eventoTrack, NULL);
        createTrackbar("Cr-Min", "frame", &crMin, 255, eventoTrack, NULL);
        createTrackbar("Cb-Min", "frame", &cbMin, 255, eventoTrack, NULL);
        createTrackbar("Y-Max", "frame", &yMax, 255, eventoTrack, NULL);
        createTrackbar("Cr-Max", "frame", &crMax, 255, eventoTrack, NULL);
        createTrackbar("Cb-Max", "frame", &cbMax, 255, eventoTrack, NULL);
        Point p1(350, 0), p2(350, 480);
        Scalar colorLine(0, 255, 0);
        int thicknessline = 2;
        line(frame, p1, p2, colorLine, thicknessline);

        imshow("frame", frame);

        for (int i = 0; i < frame.rows; i++) {
            for (int j = 350; j < frame.cols; j++) {

                mano.at<Vec3b>(i, j - 350) = frame.at<Vec3b>(i, j);


            }
        }


        camara = frame.clone();

        GaussianBlur(frame, frame, Size(19, 19), 0.0, 0);
        cvtColor(mano, hsv, COLOR_BGR2HSV);
        cvtColor(mano, ycrcb, COLOR_BGR2YCrCb);
        inRange(hsv, Scalar(hMin, sMin, vMin), Scalar(hMax, sMax, vMax), bw);
        inRange(ycrcb, Scalar(yMin, crMin, cbMin), Scalar(yMax, crMax, cbMax), ycrcb);
        dilate(ycrcb, dilatado, getStructuringElement(MORPH_CROSS, Size(5, 5)));

        imshow("ycrcb", ycrcb);
        imshow("output", bw);
        imshow("dilatado", dilatado);
        findContours(bw, contours, hierarchy, RETR_TREE, CHAIN_APPROX_SIMPLE, Point(0, 0));
        int s = findBiggestContour(contours);


        Mat drawing = Mat::zeros(mano.size(), CV_8UC1);
        drawContours(drawing, contours, s, Scalar(255), -1, 8, hierarchy, 0, Point());
        Mat manobgr = Mat(Size(drawing.cols, drawing.rows), CV_8UC3, Scalar(0, 0, 0));
        dilate(drawing, drawing, getStructuringElement(MORPH_CROSS, Size(3, 3)));
        for (int i = 0; i < drawing.rows; i++) {
            for (int j = 0; j < drawing.cols; j++) {
                if (drawing.at<uchar>(i, j) == 255) {
                    manobgr.at<Vec3b>(i, j) = mano.at<Vec3b>(i, j);
                }
            }
        }

        double aux = 1009;
        int tecla = waitKey(1);
       

        if (tecla == 97) {
           
            cout << "manobgr" << endl;
            //imwrite("imagenDB.png",manobgr);

            vector<string> imagenes;
            Mat imagenI = manobgr;
            cvtColor(imagenI, imagenI, COLOR_BGR2GRAY);
            resize(imagenI, imagenI, Size(200, 200));
            Mat imagenJ;
            Mat resta;
            DIR* dir;

            struct dirent* ent;

            string directorio = "Base";



            dir = opendir(directorio.c_str());



            while ((ent = readdir(dir)) != NULL) {

                if ((strcmp(ent->d_name, ".") != 0) && (strcmp(ent->d_name, "..") != 0) && (strcmp(ent->d_name, "imagenes.directory") != 0) && (strcmp(ent->d_name, ".directory") != 0)) {


                    imagenes.push_back(directorio + "/" + ent->d_name);

                }

            }



            for (int i = 0; i < imagenes.size(); i++) {

                for (int j = 0; j < imagenes.size(); j++) {



                    imagenJ = imread(imagenes[j]);


                    cvtColor(imagenJ, imagenJ, COLOR_BGR2GRAY);

                    resize(imagenJ, imagenJ, Size(200, 200));

                    resta = abs(imagenI - imagenJ);

                    int sumatoria = 0;

                    int cantidadElementos = 200 * 200;

                    for (int y = 0; y < 200; y++) {

                        for (int x = 0; x < 200; x++) {
                            int elementoActual = resta.at<uchar>(y, x);
                            sumatoria = sumatoria + elementoActual;

                        }

                    }

                    double promedio = (float)sumatoria / (float)cantidadElementos;

                    if (promedio < 100) {
                        if (promedio < aux) {
                            aux = promedio;
                            cout << "imagen" << imagenes[j] << endl;
                            texto = imagenes[j];
                            cout << "texto" << texto << endl;
                            cout << promedio << endl;


                            cout << "identificado" << endl;


                            //                   cout << frame.rows << frame.cols << endl;
                              //                 cout << "saleindo" << endl;

                        }


                    }

                }

            }
            cout << "prommmm" << aux << endl;

        }

        else if (tecla == 49) {
            cout << "savew" << endl;
            imwrite("Base/gesto1.png", mano);

        }
        else if (tecla == 50) {
            cout << "savew" << endl;
            imwrite("Base/gesto2.png", mano);

        }
        else if (tecla == 51) {
            cout << "savew" << endl;
            imwrite("Base/gesto3.png", mano);

        }
        else if (tecla == 27) {
            break;
        }
        else if (tecla == 98) {


            Mat image1Img = imread("Base/gesto1.png", IMREAD_COLOR);
            Mat image2Img = imread("Base/gesto2.png", IMREAD_COLOR);
            Mat image3Img = imread("Base/gesto3.png", IMREAD_COLOR);


            Mat scoreImg;
            double maxScore;

            matchTemplate(manobgr, image1Img, scoreImg, TM_CCOEFF_NORMED);
            minMaxLoc(scoreImg, 0, &maxScore);

            cout << "score imagen    " << maxScore * 10 << endl;

            //wxLogMessage(wxString::Format("score <%.2f>", maxScore));
            matchTemplate(manobgr, image2Img, scoreImg, TM_CCOEFF_NORMED);
            minMaxLoc(scoreImg, 0, &maxScore);

            cout << "score imagen    " << maxScore * 10 << endl;


            //wxLogMessage(wxString::Format("score <%.2f>", maxScore));
            matchTemplate(manobgr, image3Img, scoreImg, TM_CCOEFF_NORMED);
            minMaxLoc(scoreImg, 0, &maxScore);

            cout << "score imagen    " << maxScore * 10 << endl;
        }
        //namedWindow("Mano", WINDOW_AUTOSIZE);

        imshow("Mano", mano);
        imshow("manobgr", manobgr);
        imshow("drw", drawing);
        putText(camara, texto, Point(50, 50), FONT_HERSHEY_PLAIN, 2, Scalar(255, 5, 7), 3, 8, false);

        imshow("camara", camara);
        
    }
    

    return 0;
}

int findBiggestContour(vector<vector<Point> > contours) {
    int indexOfBiggestContour = -1;
    int sizeOfBiggestContour = 0;
    for (int i = 0; i < contours.size(); i++) {
        if (contours[i].size() > sizeOfBiggestContour) {
            sizeOfBiggestContour = contours[i].size();
            indexOfBiggestContour = i;
        }
    }
    return indexOfBiggestContour;
}