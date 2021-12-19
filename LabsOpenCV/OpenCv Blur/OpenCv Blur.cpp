#include <iostream>
#include <algorithm>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;
using namespace std;

void BlurImage(Mat& oImage, unsigned int unKoef)
{
    if (!oImage.data)
        return;

    unsigned int unWidth    = oImage.cols;
    unsigned int unHeight   = oImage.rows;

    for (unsigned int unY = 0; unY < unHeight; ++unY)
    {
        for (unsigned int unX = 0; unX < unWidth; ++unX)
        {
            int nA = 0, nR = 0, nG = 0, nB = 0;
            unsigned int unCount = 0;

            for (int nSy = (int)(unY - unKoef); nSy <= (int)(unY + unKoef); ++nSy)
            {
                const int nDeltaY = min((int)unHeight - 1, max(0, (int)nSy));
                for (int nSx = (int)(unX - unKoef); nSx <= (int)(unX + unKoef); ++nSx)
                {
                    const int nDeltaX = min((int)unWidth - 1, max(0, (int)nSx));

                    Vec3b oColor = oImage.at<Vec3b>(Point(nDeltaX, nDeltaY));

                    nR += oColor[2];
                    nG += oColor[1];
                    nB += oColor[0];

                    ++unCount;
                }
            }

            if (unCount != 0)
            {
                nA = (nA / unCount) & 0xFF;
                nR = (nR / unCount) & 0xFF;
                nG = (nG / unCount) & 0xFF;
                nB = (nB / unCount) & 0xFF;
            }

            Vec3b oColor(nB, nG, nR);

            oImage.at<Vec3b>(Point(unX, unY)) = oColor;
        }
    }
}

void ViewGradients(const Mat& oOriginalImage) 
{
    Mat oGradientX  = oOriginalImage.clone();
    Mat oGradientY  = oOriginalImage.clone();
    Mat oGradientXY = oOriginalImage.clone();

    resize(oGradientX,  oGradientX,  Size(oOriginalImage.size().width - 1, oOriginalImage.size().height - 1));
    resize(oGradientY,  oGradientY,  Size(oOriginalImage.size().width - 1, oOriginalImage.size().height - 1));
    resize(oGradientXY, oGradientXY, Size(oOriginalImage.size().width - 1, oOriginalImage.size().height - 1));

    for (int nRowIndex = 0; nRowIndex < oOriginalImage.rows - 1; ++nRowIndex)
    {
        for (int nColumnIndex = 0; nColumnIndex < oOriginalImage.cols - 1; ++nColumnIndex)
        {
            oGradientX.at<Vec3b>(Point(nRowIndex, nColumnIndex)) = (oOriginalImage.at<Vec3b>(Point(nRowIndex, nColumnIndex + 1)) - oOriginalImage.at<Vec3b>(Point(nRowIndex, nColumnIndex)));
            oGradientY.at<Vec3b>(Point(nRowIndex, nColumnIndex)) = (oOriginalImage.at<Vec3b>(Point(nRowIndex + 1, nColumnIndex)) - oOriginalImage.at<Vec3b>(Point(nRowIndex, nColumnIndex)));
        }
    }

    oGradientXY = oGradientX + oGradientY;

    namedWindow("Gradient along the X-axis",    WINDOW_AUTOSIZE);
    moveWindow("Gradient along the X-axis",     100, 100);
    imshow("Gradient along the X-axis",         oGradientX);
    waitKey(0);

    namedWindow("Gradient along the Y-axis",    WINDOW_AUTOSIZE);
    moveWindow("Gradient along the Y-axis",     100, 100);
    imshow("Gradient along the Y-axis",         oGradientX);
    waitKey(0);
    
    namedWindow("Gradient along the XY-axis",   WINDOW_AUTOSIZE);
    moveWindow("Gradient along the XY-axis",    100, 100);
    imshow("Gradient along the XY-axis",        oGradientXY);
    waitKey(0);
}

int main()
{
    Mat oOriginalImage;
    oOriginalImage = imread("../Images/Green.jpg");

    if (!oOriginalImage.data)
    {
        cout << "Could not open or find the image" << std::endl;
        return -1;
    }

    resize(oOriginalImage, oOriginalImage, Size(500, (int)(oOriginalImage.size().height * 500 / oOriginalImage.size().width)));

    namedWindow("Image", WINDOW_AUTOSIZE);
    moveWindow("Image", 100, 100);

    int nBlurValue = 0; 
    createTrackbar("Blur level", "Image", &nBlurValue, 10);

    while (true)
    {
        Mat oBlurIMage = oOriginalImage.clone();
        BlurImage(oBlurIMage, nBlurValue);

        imshow("Image", oBlurIMage);

        int iKey = waitKey(50);

        if (iKey == 27) //Нажатие на ESC
        {
            destroyWindow("Image");
            break;
        }
    }

    ViewGradients(oOriginalImage);

    return 0;
}
