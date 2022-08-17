
#include "yn.h"
using namespace jyn;


int main(void) {
    Mat img(500, 1500, CV_8UC1, Scalar(0));
    line(img, Point(0, 100), Point(img.cols, 100), Scalar(255), 2);
    line(img, Point(1300, 0), Point(1300, img.rows), Scalar(255), 2);
    line(img, Point(1300, 200), Point(img.cols, 200), Scalar(255), 2);
    line(img, Point(1300, 300), Point(img.cols, 300), Scalar(255), 2);

    putText(img, "AC", Point(1350, 70), FONT_HERSHEY_SIMPLEX, 2.5, Scalar(255), 3);
    putText(img, "CE", Point(1350, 175), FONT_HERSHEY_SIMPLEX, 2.5, Scalar(255), 3);
    putText(img, "Ans", Point(1350, 275), FONT_HERSHEY_SIMPLEX, 2, Scalar(255), 3);
    putText(img, "=", Point(1350, 420), FONT_HERSHEY_SIMPLEX, 4, Scalar(255), 3);
    namedWindow("img");



    setMouseCallback("img", onMouse, &img);
    imshow("img", img);
    waitKey();

    return 0;
}