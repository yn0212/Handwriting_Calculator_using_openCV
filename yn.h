
#ifndef YN_H
#define YN_H
#include<opencv2/opencv.hpp>
#include<iostream>
#include <vector>
#include<string>
#include<stdlib.h>
using namespace std;
using namespace cv;
namespace jyn
{


    int maxcnt(vector<double> v, double a);
    int max(vector<double> v);
    double avg(vector<double> v);
    vector<double> divide_x(Mat img);
    vector<double> divide_y(Mat img);
    bool roundCV(Mat img, int start, int end, int type);
    int outline_cnt(Mat img);
    Mat label(Mat img);
    int pix_val(Mat img);
    bool divide(Mat img, Mat centroids);
    bool four(Mat img);
    bool zero(Mat img);
    bool plus_d(Mat img);
    bool mul(Mat img, Mat centroids);
    bool parentheses(Mat img, Mat centroids, int type);
    extern String message;
    extern double ans;
    void calc_op(double* val1, double* val2, int op_index, string str);
    string calc1();
    double calc2(string str);
    void message_diviison(Mat img, vector<vector<Point>> contours, Mat centroids, Mat stats, Mat firstimg);
    void ce_button(Mat img);
    void Object_Recognition(Mat img, double& value, vector<Rect>& r);
    void draw(Mat img);
    void onMouse(int event, int x, int y, int flags, void* userdata);
    extern Point2f  pts, small;                            // 4개 좌표 및 좌표 사각형 크기

    void draw_rect(Mat img);


}
#endif