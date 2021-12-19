#include <iostream>
#include <fstream>

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
    Stitcher::Mode enMode = Stitcher::PANORAMA;

    vector<Mat> arImages = ReadImages({ "../Images/panarama1.jpg",
                                        "../Images/panarama2.jpg",
                                        "../Images/panarama3.jpg"});


    Mat oNewImage;

    Ptr<Stitcher> oStitcher = Stitcher::create(enMode);

    Stitcher::Status enStatus = oStitcher->stitch(arImages, oNewImage);

    if (enStatus != Stitcher::OK)
    {
        cout << "Can't stitch images\n";
        return -1;
    }

    namedWindow("Result", WINDOW_AUTOSIZE);
    moveWindow("Result", 100, 100);
    imshow("Result", oNewImage);
    waitKey(0);

    return 0;
}