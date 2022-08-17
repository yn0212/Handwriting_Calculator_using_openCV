#include "yn.h"
namespace jyn {
    int maxcnt(vector<double> v, double a) {// a보다 큰 값을 가진 벡터 개수
        int cnt = 0;
        for (int i = 0; i < 10; i++)
        {
            if (v[i] > a) {
                cnt++;
            }
        }
        return cnt;
    }
    int max(vector<double> v) { //최대값 인덱스 
        int maxindex = 0;
        for (int i = 0; i < 10; i++)
        {
            if (v[maxindex] < v[i]) {
                maxindex = i;
            }
        }
        return maxindex;
    }
    double avg(vector<double> v) { //벡터값의 평균 구하기
        double avg = 0;
        for (int i = 0; i < v.size(); i++)
        {
            avg += v[i];
        }
        avg = avg / v.size();
        return avg;
    }

    vector<double> divide_x(Mat img) { //분석x

        Mat dst(img.size(), img.depth(), Scalar(0));
        vector<double> sum;
        vector<double> avgx;
        sum.resize(10);
        avgx.resize(10);
        for (int k = 0; k < 10; k++)
        {
            for (int i = 0; i <= img.cols; i++) //x축으로 1/10 나눈 부분의 평균 픽셀값 개수 저장 ||||
            {
                for (int j = 0; j < img.rows; j++)
                {
                    if (i >= img.cols / 10 * k && i < img.cols / 10 * (k + 1)) {
                        if (img.at<uchar>(j, i) == 255) {
                            sum[k] += 1;
                        }
                    }
                }
            }
        }
        for (int i = 0; i < 10; i++)
        {
            avgx[i] = sum[i] / img.rows * (img.cols / 10);
            // cout << i << "번 x축 평균개수:" << avgx[i] << endl;     //x축1/10부분 픽셀값 255인 부분의 평균개수
        }
        return avgx;
    }
    vector<double> divide_y(Mat img) { //분석y
        Mat dst(img.size(), img.depth(), Scalar(0));
        vector<double> sum;
        vector<double> avgy;
        sum.resize(10);
        avgy.resize(10);
        for (int k = 0; k < 10; k++)
        {
            for (int i = 0; i <= img.rows; i++) //y축으로 1/10 나눈 부분의 평균 픽셀값 개수 저장 =
            {
                for (int j = 0; j < img.cols; j++)
                {
                    if (i >= img.rows / 10 * k && i < img.rows / 10 * (k + 1)) {
                        if (img.at<uchar>(i, j) == 255) {
                            sum[k] += 1;
                        }
                    }
                }
            }
        }
        for (int i = 0; i < 10; i++)
        {
            avgy[i] = sum[i] / img.cols * (img.rows / 10);
            //cout << i << "번 y축 평균개수:" << avgy[i] << endl;     //y축1/10부분 픽셀값 255인 부분의 평균개수
        }
        return avgy;
    }

    bool roundCV(Mat img, int start, int end, int type) {//값을 반올림해서 같은지 판별하는 함수 , type==1==avgx , 2==avgy
        vector<double> avgx = divide_x(img);
        vector<double> avgy = divide_y(img);
        int ox = 1;
        for (int i = start; i < end; i++)
        {
            if (type == 1) {
                if (cvRound(avgx[i]) == cvRound(avgx[i + 1])) {
                    continue;
                }
                else {
                    return false;
                }
            }
            else {
                if (cvRound(avgy[i]) == cvRound(avgy[i + 1])) {
                    continue;
                }
                else {
                    return false;
                }
            }
        }
        return true;
    }

    int outline_cnt(Mat img) {
        vector<vector<Point>> contours;
        findContours(img, contours, RETR_LIST, CHAIN_APPROX_NONE);

        //cout << contours.size() << "개 : 부분 행렬의 외각선 개수" << endl;
        return contours.size();
    }
    Mat CenterOfGravity(Mat img, Mat centroids, int direction, int type) { //무게중심으로 객체 자르기 x방향:1_ , y방향:2 | , 양방향+: 3(type은 왼쪽 위부터 오른쪽으로 1~4)
        Mat dst(img.size(), img.depth(), Scalar(0));
        double x = centroids.at<double>(1, 0);
        double y = centroids.at<double>(1, 1);
        if (direction == 1) {
            if (type == 1) {
                dst = img(Rect(Point(0, 0), Size(img.cols, y)));
            }
            else {
                dst = img(Rect(Point(0, y), Size(img.cols, img.rows - y)));
            }
        }
        else if (direction == 2) {
            if (type == 1) {
                dst = img(Rect(Point(0, 0), Size(x, img.rows)));
            }
            else {
                dst = img(Rect(Point(x, 0), Size(img.cols - x, img.rows)));
            }
        }
        else if (direction == 3) {
            if (type == 1) {
                dst = img(Rect(0, 0, x, y));
            }
            else if (type == 2) {
                dst = img(Rect(x, 0, img.cols - x, y));
            }
            else if (type == 3) {
                dst = img(Rect(0, y, x, img.rows - y));
            }
            else {
                dst = img(Rect(x, y, img.cols - x, img.rows - y));
            }
        }
        //rectangle(dst, Rect(Point(0, 0), Size(x, img.rows)), Scalar(255), 1);
        //imshow("dst", dst);
        //waitKey()
        return dst;
    }
    Mat label(Mat img) {//부분 객체 레이블링, 무게중심 반환
        Mat labels, stats, centroids;
        Mat dst;
        img.copyTo(dst);
        int cnt = connectedComponentsWithStats(dst, labels, stats, centroids);//레이블링
        return centroids;
    }
    int pix_val(Mat img) { //픽셀값이 0보다큰 픽셀 개수 세기
        int pix = 0;
        for (int i = 0; i < img.rows; i++)
        {
            for (int j = 0; j < img.cols; j++)
            {
                if (img.at<uchar>(i, j) > 0) {
                    pix++;
                }
            }
        }
        return pix++;
    }
    bool divide(Mat img, Mat centroids, int type) { //나누기(type==1)와 문자 s(type==0) 인식 조건 
        if (type == 1) {
            if (pix_val(CenterOfGravity(img, centroids, 3, 1)) * 5 < pix_val(CenterOfGravity(img, centroids, 3, 2))) {
                return true;
            }
            else {
                return false;
            }
        }
        else { //문자 s이면
            if (pix_val(CenterOfGravity(img, centroids, 3, 1)) > pix_val(CenterOfGravity(img, centroids, 3, 2) * 2)) {
                return true;
            }
            else {
                return false;
            }
        }

    }
    bool four(Mat img) { //4판별
        vector<Point> p;
        p.push_back(Point(0, 0));
        p.push_back(Point(img.cols / 6 * 5, 0));
        p.push_back(Point(0, img.rows / 6 * 5));
        p.push_back(Point(img.cols / 6 * 5, img.rows / 6 * 5));
        for (int i = 0; i < p.size(); i++)
        {
            Mat m = img(Rect(p[i], Size(img.cols / 6, img.rows / 6)));
            for (int k = 0; k < m.rows; k++)
            {
                for (int j = 0; j < m.cols; j++)
                {
                    if (m.at<uchar>(k, j) > 0) {
                        return false;
                    }

                }
            }
            //rectangle(img, Rect(p[i], Size(img.cols / 6, img.rows / 6)), Scalar(200), 1);
            //imshow("dst", img);
            //waitKey();
        }

        return true;
    }
    bool zero(Mat img) {
        Mat m2 = img(Rect(Point(img.cols / 2 - 30, img.rows / 2 - 30), Size(img.cols / 5, img.rows / 5)));
        for (int k = 0; k < m2.rows; k++)
        {
            for (int j = 0; j < m2.cols; j++)
            {
                if (m2.at<uchar>(k, j) > 0) {
                    return false;
                }

            }
        }
        //rectangle(img, Rect(Point(img.cols / 2, img.rows / 2), Size(img.cols / 5, img.rows / 5)), Scalar(200), 1);
        //imshow("dst", img);
        //waitKey();

        return true;
    }
    bool plus_d(Mat img) {   // 더하기 인식 조건문 : 이 박스안에 객체있으면 false
        vector<Point> p;
        p.push_back(Point(0, 0));
        p.push_back(Point(img.cols / 4 * 3, 0));
        p.push_back(Point(0, img.rows / 4 * 3));
        p.push_back(Point(img.cols / 4 * 3, img.rows / 4 * 3));
        for (int i = 0; i < p.size(); i++)
        {
            Mat m = img(Rect(p[i], Size(img.cols / 4, img.rows / 4)));
            for (int k = 0; k < m.rows; k++)
            {
                for (int j = 0; j < m.cols; j++)
                {
                    if (m.at<uchar>(k, j) > 0) {
                        return false;
                    }

                }
            }
            rectangle(img, Rect(p[i], Size(img.cols / 4, img.rows / 4)), Scalar(200), 1);
            //imshow("dst", img);
            //waitKey();
        }

        return true;
    }
    bool mul(Mat img, Mat centroids) { //무게중심 위아래양옆의 끝에 객체 있으면 false 
        double x = centroids.at<double>(1, 0);
        double y = centroids.at<double>(1, 1);
        vector<Point> p;
        p.push_back(Point(x - (img.cols / 7), 0));
        p.push_back(Point(x - (img.cols / 7), img.rows / 7 * 6));
        p.push_back(Point(0, y - (img.rows / 7)));
        p.push_back(Point(img.cols / 7 * 6, y - (img.rows / 7)));
        for (int i = 0; i < p.size(); i++)
        {
            Mat m = img(Rect(p[i], Size(img.cols / 7, img.rows / 7)));
            for (int k = 0; k < m.rows; k++)
            {
                for (int j = 0; j < m.cols; j++)
                {
                    if (m.at<uchar>(k, j) > 0) {
                        return false;
                    }

                }
            }
            //rectangle(img, Rect(p[i], Size(img.cols / 7, img.rows / 7)), Scalar(200), 1); //확인
            //imshow("mul", img);
            //waitKey();
        }

        return true;

    }
    bool parentheses(Mat img, Mat centroids, int type) {// 괄호 type==1==왼쪽 괄호, type==2==오른쪽 인식
        double x = centroids.at<double>(1, 0);
        double y = centroids.at<double>(1, 1);
        if (type == 1) {
            Mat m = img(Rect(Point(x, y - img.rows / 7), Size(img.cols - x, img.rows / 3)));
            for (int k = 0; k < m.rows; k++)
            {
                for (int j = 0; j < m.cols; j++)
                {
                    if (m.at<uchar>(k, j) > 0) {
                        return false;
                    }

                }
            }
            //rectangle(img, Rect(Point(x + img.cols / 4, y - img.rows / 7), Size(img.cols - x, img.rows / 3)), Scalar(200), 1); //확인
            //imshow("mul", img);
            //waitKey();
        }
        else {
            Mat m = img(Rect(Point(0, y - img.rows / 7), Size(x, img.rows / 3)));
            for (int k = 0; k < m.rows; k++)
            {
                for (int j = 0; j < m.cols; j++)
                {
                    if (m.at<uchar>(k, j) > 0) {
                        return false;
                    }

                }
            }
            //rectangle(img, Rect(Point(0, y - img.rows / 7), Size(x, img.rows / 3)), Scalar(200), 1); //확인
            //imshow("mul", img);
            //waitKey();
        }

        return true;
        //circle(img, Point(x, y), 3, Scalar(200), -1);
        //imshow("center", img);
        //waitKey();
    }
    String message = "";
    double ans = 0;
    void calc_op(double* val1, double* val2, int op_index, string str) {
        if ((int)str[op_index] == 120) { //곱셈
            *val1 = (*val1) * (*val2);
        }
        else if ((int)str[op_index] == 45) { //-
            *val1 -= (*val2);
        }
        else if ((int)str[op_index] == 47) {// /
            *val1 /= (*val2);
        }
        else if ((int)str[op_index] == 43) { //덧셈
            *val1 += *val2;
        }
    }

    string calc1() {
        //후위 표기법으로 만들기
        vector<char> op;
        string num = "";
        //문자를 숫자로
        int start = 0;
        int ox = 0;
        int i, num_cnt = 0;
        for (i = 0; i < message.size(); i++) // 후위 표기법 만들기
        {
            if (((int)message[i] <= 57 && (int)message[i] > 47) && i == message.size() - 1) { // 맨마지막이고  숫자이면
                num_cnt++;
                string s = message.substr(start, num_cnt);// 숫자
                num += s; //연산자 앞 숫자 자른 문자열 숫자로 바꿔서 넣기
                num += " ";
            }
            else if (((int)message[i] <= 57 && (int)message[i] > 47) || message[i] == '.') { //숫자 이거나 소수점이면

                num_cnt++;
            }
            else { // 연산자이면
                if ((int)message[i - 1] <= 57 && (int)message[i - 1] > 47) { //연산자 앞이 숫자이면
                    string s = message.substr(start, num_cnt);//연산자 앞 숫자
                    num += s; //연산자 앞 숫자 자른 문자열 숫자로 바꿔서 넣기
                    num += " "; // 구분
                    num_cnt = 0;
                }
                start = i + 1; //연산자 뒷 숫자 
                if (op.empty()) {
                    op.push_back((char)message[i]);
                }
                else if (message[i] == '(' || message[i] == 'x' || message[i] == '/') {
                    op.push_back((char)message[i]);
                }
                else if (message[i] == '+' || message[i] == '-') { //여는 괄호있으면  (위의 모든 연산자 출력, (없으면 벡터의 끝까지 출력 후 스택에push
                    for (int j = 0; j < op.size(); j++) // 스택에 괄호 있으면
                    {
                        if (op[j] == '(') { //괄호 안  연산자 출력
                            if (op.size() - 1 == j) { // 괄호 위에 아무것도 없으면
                            }
                            else {
                                for (int k = op.size() - 1; k >= j + 1; k--)
                                {
                                    num += op[k];
                                    num += " "; // 구분

                                    op.pop_back();
                                }
                            }
                            ox++;
                        }
                    }
                    if (ox == 0) // 괄호 없으면 스택의 끝까지 출력
                    {
                        for (int k = op.size() - 1; k >= 0; k--)
                        {
                            num += op[k]; // 출력
                            num += " "; // 구분
                            op.pop_back();

                        }
                    }
                    op.push_back((char)message[i]); // push
                    ox = 0;
                }
                else if (message[i] == ')') { // 닫는 괄호 나오면 여는 괄호까지 모든 op비우기

                    for (int j = 0; j < op.size(); j++)
                    {
                        if (op[j] == '(') {
                            for (int k = op.size() - 1; k >= j + 1; k--)
                            {
                                num += op[k];//출력
                                num += " "; // 구분
                                op.pop_back();

                            }

                            op.pop_back();
                        }
                    }
                }
            }
        }
        if (i == message.size()) { // message의 식이 끝나면 벡터에 남아있는 연산자 모두 num에 출력
            for (int j = op.size() - 1; j >= 0; j--)
            {

                num += op[j];
                num += " "; // 구분
            }
        }
        cout << num << endl;
        return num;
    }
    double calc2(string str) { // 후위표기법 계산
        vector<double> v;
        int cur_position = 0; // 시작위치
        int position; // 공백 인덱스
        int cnt = 0;
        double val1, val2, value = 0;
        bool ox = false;
        while ((position = str.find(" ", cur_position)) != std::string::npos) {
            int len = position - cur_position;
            string result = str.substr(cur_position, len);
            for (int i = 0; i < result.size(); i++) {
                if (((int)result[i] <= 57 && (int)result[i] > 47) || result[i] == '.') { // 숫자이거나 소수점이면
                    ox = true;
                    cnt++;
                }
            }
            if (ox) { // 숫자이면
                v.push_back(stod(result));
                ox = false;
            }
            if (cnt == 0) { //연산자이면

                val2 = v.back();
                val1 = v[v.size() - 2];
                //cout << "val1:" << val1 << "val2:" << val2 << endl;
                calc_op(&val1, &val2, cur_position, str);
                v.pop_back();
                v[v.size() - 1] = val1;
            }
            cur_position = position + 1;
            cnt = 0;
        }
        value = v[0];
        cout << "답:" << v[0] << endl;
        //string result = str.substr(cur_position);
        //cout << result << std::endl;
        return value;
    }
    bool under(Mat img, Mat centroids) { //무게중심 기준 으로 아래쪽에 객체 없으면
        double x = centroids.at<double>(1, 0);
        double y = centroids.at<double>(1, 1);
        Point p(x, img.rows / 6 * 5);

        Mat m = img(Rect(p, Size(img.cols / 5, img.rows / 5)));
        for (int k = 0; k < m.rows; k++)
        {
            for (int j = 0; j < m.cols; j++)
            {
                if (m.at<uchar>(k, j) > 0) {
                    return false;
                }

            }
        }
        //rectangle(img, Rect(p, Size(img.cols / 5, img.rows / 5)), Scalar(200), 1); //확인
        //imshow("mul", img);
        //waitKey();

        return true;
    }
    void message_diviison(Mat img, vector<vector<Point>> contours, Mat centroids, Mat stats, Mat firstimg) { // 문자 구분
        vector<double> avgx = divide_x(img);
        vector<double> avgy = divide_y(img);
        Mat gravity_X;
        static int cntminus = 0, cntminus_p = 0;
        static int cnt = 0;
        static int pcnt;
        static bool ox = false;

        if (contours.size() == 1) {
            if (avg(avgx) < 2) {   //1은 x축으로 나눈 구간들의 평균이 3이하일때
                if (avg(avgy) < 2 || img.rows < 50) { //y축으로 나눈 구간들의 평균이 3이하 이고 객체 크기50이하 소수점이
                    if (under(img, centroids)) { // n
                        cout << "n" << endl;
                    }
                    else if (divide(img, centroids, 0)) { // s
                        cout << "s" << endl;
                        message += to_string(ans);
                        cout << "ans:" << ans << endl;
                    }
                    else {
                        message += ".";
                        cout << ".(소수점)" << endl;
                    }

                }
                else {
                    message += "1";
                    cout << "1" << endl;
                }
            }
            else if (avg(avgy) < 1) { //y축으로 나눈 구간들의 평균이 1이하일때 : 마이너스
                message += "-";
                cout << "-" << endl;
                cntminus = cnt;
            }
            else if (divide(img, centroids, 1)) {
                message += "/";
                cout << "/" << endl;
            }
            else if (plus_d(img)) {
                message += "+";
                cout << "+" << endl;
            }
            else if (mul(img, centroids)) {
                message += "x";
                cout << "x" << endl;
            }
            else if (parentheses(img, centroids, 1)) {
                message += "(";
                cout << "(" << endl;
            }
            else if (parentheses(img, centroids, 2)) {
                message += ")";
                cout << ")" << endl;
            }
            else if ((outline_cnt(CenterOfGravity(img, centroids, 2, 1))) == 2) { //  무게중심을 기준으로 x축에 수직인 직선으로 나눈 왼쪽 행렬의 외각선이 2개
                if (max(avgy) == 9 || roundCV(img, 7, 9, 1)) {
                    message += "2";
                    cout << "2" << endl;
                }
                else {
                    message += "5";
                    cout << "5" << endl;
                }

            }
            else if ((outline_cnt(CenterOfGravity(img, centroids, 2, 1))) == 3) {
                message += "3";
                cout << "3" << endl;

            }
            else if (roundCV(img, 5, 7, 1) || ((int)avgx[6] == (int)avgx[7] && (int)avgx[7] == (int)avgx[5] && (int)avgx[5] == (int)avgx[6]) || roundCV(img, 6, 8, 0)) {

                message += "7";
                cout << "7" << endl;
            }
        }
        if (contours.size() == 2) {

            if ((outline_cnt(CenterOfGravity(img, centroids, 1, 0))) == 2) { //y축방향으로 자르기,  무게중심으로 객체 잘랐을때 아래쪽부분의 외각선 개수
                if ((int)avgy[6] == (int)avgy[7] && (int)avgy[7] == (int)avgy[8] && (int)avgy[8] == (int)avgy[6] || roundCV(img, 6, 8, 0)) {
                    message += "9";
                    cout << "9" << endl;
                }
                else {
                    message += "6";
                    cout << "6" << endl;
                }
            }
            else if ((roundCV(img, 8, 9, 1) || (int)avgx[8] == (int)avgx[9]) && four(img)) {
                message += "4";
                cout << "4" << endl;
            }
            else if ((outline_cnt(CenterOfGravity(img, centroids, 1, 1))) == 2 || (int)avgy[6] == (int)avgy[7] && (int)avgy[7] == (int)avgy[8] && (int)avgy[8] == (int)avgy[6] || roundCV(img, 6, 8, 0)) {
                message += "9";
                cout << "9" << endl;
            }
            else if (((outline_cnt(CenterOfGravity(img, centroids, 1, 1))) == 1 && (outline_cnt(CenterOfGravity(img, centroids, 1, 0))) == 1) || (roundCV(img, 4, 6, 0) || ((int)avgy[6] == (int)avgy[5] && (int)avgy[5] == (int)avgy[4] && (int)avgy[4] == (int)avgy[6])) && zero(img)) {
                if (under(img, centroids)) {
                    cout << "A" << endl;
                }
                else {
                    message += "0";
                    cout << "0" << endl;
                }

            }

        }
        if (contours.size() == 3) {

            message += "8";
            cout << "8" << endl;

        }
        cnt++;
    }
    void ce_button(Mat img) {
        Mat labels, stats, centroids;
        int cnt = connectedComponentsWithStats(img, labels, stats, centroids);//레이블링la
        //cout << stats.rows<<"개의 레이블 배경포함 객체수"<<endl;
        vector<Rect> r;
        for (int i = 0; i < cnt; i++)      //바운딩 박스 정보 vector<Rect>r에 저장
        {
            int* p = stats.ptr<int>(i);
            r.push_back(Rect(p[0], p[1], p[2], p[3]));
        }
        Rect tmp;
        for (int i = 1; i < r.size(); i++)      // 벡터 r의 저장된 순서를 x좌표가 작은순부터 저장되도록 정렬
        {
            for (int j = 0; j < r.size() - i; j++)
            {
                if (r[j].x > r[j + 1].x) {
                    tmp = r[j];
                    r[j] = r[j + 1];
                    r[j + 1] = tmp;
                }
            }
        }
        int x = r[r.size() - 1].x;
        int y = r[r.size() - 1].y;
        int w = r[r.size() - 1].width;
        int h = r[r.size() - 1].height;
        Mat last = img(Rect(x, y, w, h));
        last = Scalar(0);
    }
    void Object_Recognition(Mat img, double& value, vector<Rect>& r) {  //객체 인식
        Mat labels, stats, centroids;
        int cnt = connectedComponentsWithStats(img, labels, stats, centroids);//레이블링
        //cout << stats.rows<<"개의 레이블 배경포함 객체수"<<endl;

        for (int i = 0; i < cnt; i++)      //바운딩 박스 정보 vector<Rect>r에 저장
        {
            int* p = stats.ptr<int>(i);
            r.push_back(Rect(p[0], p[1], p[2], p[3]));
        }
        Rect tmp;
        for (int i = 1; i < r.size(); i++)      // 벡터 r의 저장된 순서를 x좌표가 작은순부터 저장되도록 정렬
        {
            for (int j = 0; j < r.size() - i; j++)
            {
                if (r[j].x > r[j + 1].x) {
                    tmp = r[j];
                    r[j] = r[j + 1];
                    r[j + 1] = tmp;
                }
            }
        }
        for (int i = 1; i < r.size(); i++)
        {
            //cout << r[i] << endl;
        }

        for (int i = 1; i < r.size(); i++)   // 객체 외각선 검출
        {
            vector<vector<Point>> contours;
            findContours(img(r[i]), contours, RETR_LIST, CHAIN_APPROX_NONE);
            //cout << contours.size() << "개의 외각선" << endl;
            Mat dst;
            img(r[i]).copyTo(dst);
            //   divide_x(&dst);
            message_diviison(dst, contours, label(dst), stats, img);
        }
        //double value =calc();
        //string str = to_string(value);
        //putText(img, "=" + str, Point(r[cnt - 1].x + r[cnt - 1].width + 50, r[cnt - 1].y + r[cnt - 1].height - 10), FONT_HERSHEY_SIMPLEX, 3.0, Scalar(255), 1);
        string str = calc1();
        value = calc2(str);
    }
    void draw(Mat img) {
        line(img, Point(0, 100), Point(img.cols, 100), Scalar(255), 2);
        line(img, Point(1300, 0), Point(1300, img.rows), Scalar(255), 2);
        line(img, Point(1300, 200), Point(img.cols, 200), Scalar(255), 2);
        line(img, Point(1300, 300), Point(img.cols, 300), Scalar(255), 2);

        putText(img, "AC", Point(1350, 70), FONT_HERSHEY_SIMPLEX, 2.5, Scalar(255), 3);
        putText(img, "CE", Point(1350, 175), FONT_HERSHEY_SIMPLEX, 2.5, Scalar(255), 3);
        putText(img, "Ans", Point(1350, 275), FONT_HERSHEY_SIMPLEX, 2, Scalar(255), 3);
        putText(img, "=", Point(1350, 420), FONT_HERSHEY_SIMPLEX, 4, Scalar(255), 3);
    }
    void draw_q(Mat img, Mat dst, double value, int ox) {
        line(img, Point(0, 100), Point(img.cols, 100), Scalar(255), 2);
        line(img, Point(1300, 0), Point(1300, img.rows), Scalar(255), 2);
        line(img, Point(1300, 200), Point(img.cols, 200), Scalar(255), 2);
        line(img, Point(1300, 300), Point(img.cols, 300), Scalar(255), 2);

        putText(img, "AC", Point(1350, 70), FONT_HERSHEY_SIMPLEX, 2.5, Scalar(255), 3);
        putText(img, "CE", Point(1350, 175), FONT_HERSHEY_SIMPLEX, 2.5, Scalar(255), 3);
        putText(img, "Ans", Point(1350, 275), FONT_HERSHEY_SIMPLEX, 2, Scalar(255), 3);
        putText(img, "=", Point(1350, 420), FONT_HERSHEY_SIMPLEX, 4, Scalar(255), 3);
        if (ox == 1) { //=일때
            putText(img, message + " = ", Point(50, 80), FONT_HERSHEY_SIMPLEX, 3, Scalar(255), 3);
            dst = Scalar(0);

            putText(img, to_string(value), Point(200, 350), FONT_HERSHEY_SIMPLEX, 4, Scalar(255), 3);
        }
        else { //ans일때
            img = Scalar(0);
            draw(img);
            putText(img, "Ans = " + to_string(ans), Point(50, 80), FONT_HERSHEY_SIMPLEX, 3, Scalar(255), 3);

        }

    }
    static Point2f  pts(1400, 250), small(100, 50);                            // 4개 좌표 및 좌표 사각형 크기

    void draw_rect(Mat img)                                // 4개 좌표 잇는 사각형 그리기
    {
        Rect img_rect(Point(0, 0), img.size());            // 입력영상 크기 사각형
        Rect rect(pts - small, pts + small);        // 좌표 사각형
        rect &= img_rect;                                // 교차 영역 계산
        rectangle(img, rect, Scalar(0), 1);    // 좌표 사각형 그리기
        putText(img, "Ans", Point(rect.x + (rect.width / 2) - 50, rect.y + (rect.height / 2) + 25), FONT_HERSHEY_SIMPLEX, 2, Scalar(255), 3);
        imshow("img", img);
    }


    void onMouse(int event, int x, int y, int flags, void* userdata) {
        Mat img = *(Mat*)userdata;
        static Point ptOld;
        Mat dst = img(Rect(0, 120, 1280, 380));
        static double value;
        vector<Rect> r;
        static int ck = -1;
        static bool ox = false;
        static int level = 0;
        Point curr_pt(x, y);                                    // 현재 클릭 좌표


        draw_rect(img.clone());                                    // 좌표 사각형 그리기

        if (event == EVENT_LBUTTONDOWN) {
            ptOld = Point(x, y);
            if (x >= 1300 && y >= 310) { // = 버튼 구현
                draw_rect(img);                                    // 좌표 사각형 그리기
                Object_Recognition(dst, value, r); //객체 인식
                img = Scalar(0);
                draw_q(img, dst, value, 1);
                pts = Point(1400, 250);
                draw_rect(img.clone());
                imshow("img", img);
                message = "";
                level = 1;

            }
            else if (x >= 1300 && y <= 100) { // ac버튼 구현
                img = Scalar(0);
                draw(img);
                pts = Point(1400, 250);
                draw_rect(img.clone());
                imshow("img", img);
                level = 2;
            }
            else if (x >= 1300 && (y < 300 && y>200)) { // ans구현
                if (level == 1) {
                    ans = value;
                    cout << "ans:" << ans << endl;
                    draw_q(img, dst, ans, 0);
                    imshow("img", img);

                }


            }
            else if (x >= 1300 && y <= 200) { //ce

                ce_button(dst);
                imshow("img", img);
            }


        }
        else if (event == EVENT_MOUSEMOVE) {
            if (flags & EVENT_FLAG_LBUTTON) {
                if (x < 1300 && y >= 100) {
                    draw_rect(img);
                    line(img, ptOld, Point(x, y), Scalar(255), 6);
                    imshow("img", img);
                    ptOld = Point(x, y);

                }
            }
        }
        else if (event == EVENT_MBUTTONDOWN) {
            Rect rect(pts - small, pts + small);    // 좌표 사각형들 선언
            if (rect.contains(curr_pt))  ck = 1;        // 선택 좌표 사각형 찾기

        }
        if (event == EVENT_MBUTTONUP)
            ck = -1;                                    // 선택 좌표번호 초기화

        if (ck >= 0) {                                    // 객체 선택시    
            pts = curr_pt;                            // 클릭 좌표를 선택 좌표에 저장
            draw_rect(img.clone());                        // 4개 좌표 연결 사각형 그리기

        }

    }

}