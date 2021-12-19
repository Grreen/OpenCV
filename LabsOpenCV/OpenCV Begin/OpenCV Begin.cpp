#include <iostream>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{
    Mat oOriginalImage;
    oOriginalImage = imread("../Images/Green.jpg");

    if (!oOriginalImage.data)
    {
        cout << "Could not open or find the image" << std::endl;
        return -1;
    }

    resize(oOriginalImage, oOriginalImage, Size(500, (int)(oOriginalImage.size().height * 500 / oOriginalImage.size().width)));

    //Отображаем исходное изображение
    namedWindow("Original Image", WINDOW_AUTOSIZE);
    moveWindow("Original Image", 100, 100);
    imshow("Original Image", oOriginalImage);
    waitKey(0);

    //Отображаем черно-белую версию изображения
    Mat oBlackAndWhiteImage;
    cvtColor(oOriginalImage, oBlackAndWhiteImage, COLOR_RGBA2GRAY);
    namedWindow("Black and white Image", WINDOW_AUTOSIZE);
    moveWindow("Black and white Image", 100, 100);
    imshow("Black and white Image", oBlackAndWhiteImage);
    waitKey(0);

    //Отображаем YUV версию изображения
    Mat oYUVImage;
    cvtColor(oOriginalImage, oYUVImage, COLOR_BGR2YUV);
    namedWindow("YUV Image", WINDOW_AUTOSIZE);
    moveWindow("YUV Image", 100, 100);
    imshow("YUV Image", oYUVImage);
    waitKey(0);

    //Отображаем изображение в схеме HSV
    Mat oHSVImage;
    cvtColor(oOriginalImage, oHSVImage, COLOR_BGR2HSV);
    namedWindow("HSV Image", WINDOW_AUTOSIZE);
    moveWindow("HSV Image", 100, 100);
    imshow("HSV Image", oHSVImage);
    waitKey(0);

    //Отобразим края объектов, находящихся на изображении
    Mat oCannyImage;
    Canny(oOriginalImage, oCannyImage, 100, 200);
    namedWindow("Canny Image", WINDOW_AUTOSIZE);
    moveWindow("Canny Image", 100, 100);
    imshow("Canny Image", oCannyImage);
    waitKey(0);

    //Отобразим размытое изображение
    Mat oGausseImage;
    GaussianBlur(oOriginalImage, oGausseImage, Size(15, 15), 0);
    namedWindow("Gausse Image", WINDOW_AUTOSIZE);
    moveWindow("Gausse Image", 100, 100);
    imshow("Gausse Image", oGausseImage);
    waitKey(0);

    return 0;
}
