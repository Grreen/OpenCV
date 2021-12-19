#include <iostream>
#include <fstream>
#include <algorithm>

#include <opencv2/opencv.hpp>
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/stitching.hpp"

using namespace std;
using namespace cv;

vector<Mat> ReadImages(vector<string> arFilePaths)
{
    if (arFilePaths.empty())
        return vector<Mat>();

    vector<Mat> arImages;

    for (string wsFilePath : arFilePaths)
    {
        Mat oImage = imread(wsFilePath);
        if (!oImage.data)
        {
            cout << "Can't read image \"" << wsFilePath << "\"" << endl;
            return vector<Mat>();
        }

        arImages.push_back(oImage);
    }

    return arImages;
}

int main()
{
    Mat oImage = imread("../Images/parts2.jpg");

    if (!oImage.data)
    {
        cout << "Can't read image" << endl;
        return -1;
    }

    resize(oImage, oImage, Size(500, (int)(oImage.size().height * 500 / oImage.size().width)));

    namedWindow("Original Image", WINDOW_AUTOSIZE);
    moveWindow("Original Image", 100, 100);
    imshow("Original Image", oImage);
    waitKey(0);

    Mat oGrayImage;
    cvtColor(oImage, oGrayImage, COLOR_BGR2GRAY);

    Mat oThreshImage;
    threshold(oGrayImage, oThreshImage, 150, 255, THRESH_BINARY);
    namedWindow("Binary Image", WINDOW_AUTOSIZE);
    moveWindow("Binary Image", 100, 100);
    imshow("Binary Image", oThreshImage);
    waitKey(0);

    vector<vector<Point>> arContours;
    vector<Vec4i> arHierarchy;
    findContours(oThreshImage, arContours, arHierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

    Mat oCopyImage = oImage.clone();
    drawContours(oCopyImage, arContours, -1, Scalar(0, 255, 0), 2);
    namedWindow("None approximation", WINDOW_AUTOSIZE);
    moveWindow("None approximation", 100, 100);
    imshow("None approximation", oCopyImage);
    waitKey(0);

    vector<string> arFileNames;
    vector<double> match;

    for (unsigned int unIndex = 0; unIndex < arContours.size(); ++unIndex)
    {
        if (arContours[unIndex].size() < 3)
        {
            arContours.erase(arContours.begin() + unIndex);
            continue;
        }

        Mat oTempImage(oImage, boundingRect(arContours[unIndex]));
        string sFileName = "temp" + to_string(unIndex + 1) + ".jpg";
        arFileNames.push_back(sFileName);
        imwrite(sFileName, oTempImage);
    }

    if (arContours.size() < 2)
        return -1;

    for (unsigned int unFirstIndex = 0; unFirstIndex < arContours.size(); ++unFirstIndex)
    {
        for (unsigned int unSecondIndex = 1; unSecondIndex < arContours.size(); ++unSecondIndex)
        {
            if (unFirstIndex == unSecondIndex)
                continue;

            double dMatch = matchShapes(arContours[unFirstIndex], arContours[unSecondIndex], CONTOURS_MATCH_I2, 0);

            #ifdef _DEBUG
                std::cout << unFirstIndex << " _ " << unSecondIndex << " = " << dMatch << endl;
            #endif 

            match.push_back(dMatch);
        }
    }
    


    Mat tempimg1 = imread("temp2.jpg");
    Mat tempimg2 = imread("temp3.jpg");


    Mat oVImg(max(tempimg1.rows, tempimg2.rows), tempimg1.cols + tempimg2.cols, tempimg1.type());

    Mat q0(oVImg, Rect(0, 0, tempimg1.cols, tempimg1.rows));
    Mat q1(oVImg, Rect(tempimg1.cols, 0, tempimg2.cols, tempimg2.rows));

    tempimg1.copyTo(q0);
    tempimg2.copyTo(q1);

    namedWindow("Result", WINDOW_AUTOSIZE);
    moveWindow("Result", 100, 100);
    imshow("Result", oVImg);
    waitKey(0);

    return 0;

    Mat img1 = imread("temp2.jpg");
    Mat img2 = imread("temp3.jpg");

    // 1. Инициализация
    vector<KeyPoint> keypoints1, keypoints2;
    Mat descriptors1, descriptors2;
    Ptr<ORB> orb = ORB::create(800);

    //  2. Извлечение характерных точек
    orb->detect(img1, keypoints1);
    orb->detect(img2, keypoints2);

    //  3. Рассчитать дескриптор объекта
    orb->compute(img1, keypoints1, descriptors1);
    orb->compute(img2, keypoints2, descriptors2);

    //  4. Сопоставьте КРАТКИЕ дескрипторы двух изображений, используя BFMatch, расстояние Хэмминга в качестве эталона
    vector<DMatch> matches;
    BFMatcher bfMatcher(NORM_HAMMING);
    bfMatcher.match(descriptors1, descriptors2, matches);

    double max_dist = 0;
    double min_dist = 100;

    // Найдем максимальное и минимальное расстояние между ключевыми точками
    for (int i = 0; i < matches.size(); i++)
    {
        double dist = matches[i].distance;
        if (dist < min_dist) 
        {
            min_dist = dist;
        }
        if (dist > max_dist) 
        {
            max_dist = dist;
        }
    }

    // Нарисуем только "хорошие" совпадения (т.е. те, для которых расстояние меньше, чем 3*min_dist)
    std::vector< DMatch > goodMatches;

    for (int i = 0; i < matches.size(); i++)
    {
        if (matches[i].distance < 3 * min_dist) 
        {
            goodMatches.push_back(matches[i]);
        }
    }

    Mat imgMatches;
    drawMatches(img1, keypoints1, img2, keypoints2,
        goodMatches, imgMatches, Scalar::all(-1), Scalar::all(-1),
        std::vector< char >(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);

    namedWindow("Matches Image", WINDOW_AUTOSIZE);
    moveWindow("Matches Image", 100, 100);
    imshow("Matches Image", imgMatches);
    waitKey(0);

    return 0;
}
