#include "yn.h"
namespace jyn {
    int maxcnt(vector<double> v, double a) {// a���� ū ���� ���� ���� ����
        int cnt = 0;
        for (int i = 0; i < 10; i++)
        {
            if (v[i] > a) {
                cnt++;
            }
        }
        return cnt;
    }
    int max(vector<double> v) { //�ִ밪 �ε��� 
        int maxindex = 0;
        for (int i = 0; i < 10; i++)
        {
            if (v[maxindex] < v[i]) {
                maxindex = i;
            }
        }
        return maxindex;
    }
    double avg(vector<double> v) { //���Ͱ��� ��� ���ϱ�
        double avg = 0;
        for (int i = 0; i < v.size(); i++)
        {
            avg += v[i];
        }
        avg = avg / v.size();
        return avg;
    }

    vector<double> divide_x(Mat img) { //�м�x

        Mat dst(img.size(), img.depth(), Scalar(0));
        vector<double> sum;
        vector<double> avgx;
        sum.resize(10);
        avgx.resize(10);
        for (int k = 0; k < 10; k++)
        {
            for (int i = 0; i <= img.cols; i++) //x������ 1/10 ���� �κ��� ��� �ȼ��� ���� ���� ||||
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
            // cout << i << "�� x�� ��հ���:" << avgx[i] << endl;     //x��1/10�κ� �ȼ��� 255�� �κ��� ��հ���
        }
        return avgx;
    }
    vector<double> divide_y(Mat img) { //�м�y
        Mat dst(img.size(), img.depth(), Scalar(0));
        vector<double> sum;
        vector<double> avgy;
        sum.resize(10);
        avgy.resize(10);
        for (int k = 0; k < 10; k++)
        {
            for (int i = 0; i <= img.rows; i++) //y������ 1/10 ���� �κ��� ��� �ȼ��� ���� ���� =
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
            //cout << i << "�� y�� ��հ���:" << avgy[i] << endl;     //y��1/10�κ� �ȼ��� 255�� �κ��� ��հ���
        }
        return avgy;
    }

    bool roundCV(Mat img, int start, int end, int type) {//���� �ݿø��ؼ� ������ �Ǻ��ϴ� �Լ� , type==1==avgx , 2==avgy
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

        //cout << contours.size() << "�� : �κ� ����� �ܰ��� ����" << endl;
        return contours.size();
    }
    Mat CenterOfGravity(Mat img, Mat centroids, int direction, int type) { //�����߽����� ��ü �ڸ��� x����:1_ , y����:2 | , �����+: 3(type�� ���� ������ ���������� 1~4)
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
    Mat label(Mat img) {//�κ� ��ü ���̺�, �����߽� ��ȯ
        Mat labels, stats, centroids;
        Mat dst;
        img.copyTo(dst);
        int cnt = connectedComponentsWithStats(dst, labels, stats, centroids);//���̺�
        return centroids;
    }
    int pix_val(Mat img) { //�ȼ����� 0����ū �ȼ� ���� ����
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
    bool divide(Mat img, Mat centroids, int type) { //������(type==1)�� ���� s(type==0) �ν� ���� 
        if (type == 1) {
            if (pix_val(CenterOfGravity(img, centroids, 3, 1)) * 5 < pix_val(CenterOfGravity(img, centroids, 3, 2))) {
                return true;
            }
            else {
                return false;
            }
        }
        else { //���� s�̸�
            if (pix_val(CenterOfGravity(img, centroids, 3, 1)) > pix_val(CenterOfGravity(img, centroids, 3, 2) * 2)) {
                return true;
            }
            else {
                return false;
            }
        }

    }
    bool four(Mat img) { //4�Ǻ�
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
    bool plus_d(Mat img) {   // ���ϱ� �ν� ���ǹ� : �� �ڽ��ȿ� ��ü������ false
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
    bool mul(Mat img, Mat centroids) { //�����߽� ���Ʒ��翷�� ���� ��ü ������ false 
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
            //rectangle(img, Rect(p[i], Size(img.cols / 7, img.rows / 7)), Scalar(200), 1); //Ȯ��
            //imshow("mul", img);
            //waitKey();
        }

        return true;

    }
    bool parentheses(Mat img, Mat centroids, int type) {// ��ȣ type==1==���� ��ȣ, type==2==������ �ν�
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
            //rectangle(img, Rect(Point(x + img.cols / 4, y - img.rows / 7), Size(img.cols - x, img.rows / 3)), Scalar(200), 1); //Ȯ��
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
            //rectangle(img, Rect(Point(0, y - img.rows / 7), Size(x, img.rows / 3)), Scalar(200), 1); //Ȯ��
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
        if ((int)str[op_index] == 120) { //����
            *val1 = (*val1) * (*val2);
        }
        else if ((int)str[op_index] == 45) { //-
            *val1 -= (*val2);
        }
        else if ((int)str[op_index] == 47) {// /
            *val1 /= (*val2);
        }
        else if ((int)str[op_index] == 43) { //����
            *val1 += *val2;
        }
    }

    string calc1() {
        //���� ǥ������� �����
        vector<char> op;
        string num = "";
        //���ڸ� ���ڷ�
        int start = 0;
        int ox = 0;
        int i, num_cnt = 0;
        for (i = 0; i < message.size(); i++) // ���� ǥ��� �����
        {
            if (((int)message[i] <= 57 && (int)message[i] > 47) && i == message.size() - 1) { // �Ǹ������̰�  �����̸�
                num_cnt++;
                string s = message.substr(start, num_cnt);// ����
                num += s; //������ �� ���� �ڸ� ���ڿ� ���ڷ� �ٲ㼭 �ֱ�
                num += " ";
            }
            else if (((int)message[i] <= 57 && (int)message[i] > 47) || message[i] == '.') { //���� �̰ų� �Ҽ����̸�

                num_cnt++;
            }
            else { // �������̸�
                if ((int)message[i - 1] <= 57 && (int)message[i - 1] > 47) { //������ ���� �����̸�
                    string s = message.substr(start, num_cnt);//������ �� ����
                    num += s; //������ �� ���� �ڸ� ���ڿ� ���ڷ� �ٲ㼭 �ֱ�
                    num += " "; // ����
                    num_cnt = 0;
                }
                start = i + 1; //������ �� ���� 
                if (op.empty()) {
                    op.push_back((char)message[i]);
                }
                else if (message[i] == '(' || message[i] == 'x' || message[i] == '/') {
                    op.push_back((char)message[i]);
                }
                else if (message[i] == '+' || message[i] == '-') { //���� ��ȣ������  (���� ��� ������ ���, (������ ������ ������ ��� �� ���ÿ�push
                    for (int j = 0; j < op.size(); j++) // ���ÿ� ��ȣ ������
                    {
                        if (op[j] == '(') { //��ȣ ��  ������ ���
                            if (op.size() - 1 == j) { // ��ȣ ���� �ƹ��͵� ������
                            }
                            else {
                                for (int k = op.size() - 1; k >= j + 1; k--)
                                {
                                    num += op[k];
                                    num += " "; // ����

                                    op.pop_back();
                                }
                            }
                            ox++;
                        }
                    }
                    if (ox == 0) // ��ȣ ������ ������ ������ ���
                    {
                        for (int k = op.size() - 1; k >= 0; k--)
                        {
                            num += op[k]; // ���
                            num += " "; // ����
                            op.pop_back();

                        }
                    }
                    op.push_back((char)message[i]); // push
                    ox = 0;
                }
                else if (message[i] == ')') { // �ݴ� ��ȣ ������ ���� ��ȣ���� ��� op����

                    for (int j = 0; j < op.size(); j++)
                    {
                        if (op[j] == '(') {
                            for (int k = op.size() - 1; k >= j + 1; k--)
                            {
                                num += op[k];//���
                                num += " "; // ����
                                op.pop_back();

                            }

                            op.pop_back();
                        }
                    }
                }
            }
        }
        if (i == message.size()) { // message�� ���� ������ ���Ϳ� �����ִ� ������ ��� num�� ���
            for (int j = op.size() - 1; j >= 0; j--)
            {

                num += op[j];
                num += " "; // ����
            }
        }
        cout << num << endl;
        return num;
    }
    double calc2(string str) { // ����ǥ��� ���
        vector<double> v;
        int cur_position = 0; // ������ġ
        int position; // ���� �ε���
        int cnt = 0;
        double val1, val2, value = 0;
        bool ox = false;
        while ((position = str.find(" ", cur_position)) != std::string::npos) {
            int len = position - cur_position;
            string result = str.substr(cur_position, len);
            for (int i = 0; i < result.size(); i++) {
                if (((int)result[i] <= 57 && (int)result[i] > 47) || result[i] == '.') { // �����̰ų� �Ҽ����̸�
                    ox = true;
                    cnt++;
                }
            }
            if (ox) { // �����̸�
                v.push_back(stod(result));
                ox = false;
            }
            if (cnt == 0) { //�������̸�

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
        cout << "��:" << v[0] << endl;
        //string result = str.substr(cur_position);
        //cout << result << std::endl;
        return value;
    }
    bool under(Mat img, Mat centroids) { //�����߽� ���� ���� �Ʒ��ʿ� ��ü ������
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
        //rectangle(img, Rect(p, Size(img.cols / 5, img.rows / 5)), Scalar(200), 1); //Ȯ��
        //imshow("mul", img);
        //waitKey();

        return true;
    }
    void message_diviison(Mat img, vector<vector<Point>> contours, Mat centroids, Mat stats, Mat firstimg) { // ���� ����
        vector<double> avgx = divide_x(img);
        vector<double> avgy = divide_y(img);
        Mat gravity_X;
        static int cntminus = 0, cntminus_p = 0;
        static int cnt = 0;
        static int pcnt;
        static bool ox = false;

        if (contours.size() == 1) {
            if (avg(avgx) < 2) {   //1�� x������ ���� �������� ����� 3�����϶�
                if (avg(avgy) < 2 || img.rows < 50) { //y������ ���� �������� ����� 3���� �̰� ��ü ũ��50���� �Ҽ�����
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
                        cout << ".(�Ҽ���)" << endl;
                    }

                }
                else {
                    message += "1";
                    cout << "1" << endl;
                }
            }
            else if (avg(avgy) < 1) { //y������ ���� �������� ����� 1�����϶� : ���̳ʽ�
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
            else if ((outline_cnt(CenterOfGravity(img, centroids, 2, 1))) == 2) { //  �����߽��� �������� x�࿡ ������ �������� ���� ���� ����� �ܰ����� 2��
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

            if ((outline_cnt(CenterOfGravity(img, centroids, 1, 0))) == 2) { //y��������� �ڸ���,  �����߽����� ��ü �߶����� �Ʒ��ʺκ��� �ܰ��� ����
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
        int cnt = connectedComponentsWithStats(img, labels, stats, centroids);//���̺�la
        //cout << stats.rows<<"���� ���̺� ������� ��ü��"<<endl;
        vector<Rect> r;
        for (int i = 0; i < cnt; i++)      //�ٿ�� �ڽ� ���� vector<Rect>r�� ����
        {
            int* p = stats.ptr<int>(i);
            r.push_back(Rect(p[0], p[1], p[2], p[3]));
        }
        Rect tmp;
        for (int i = 1; i < r.size(); i++)      // ���� r�� ����� ������ x��ǥ�� ���������� ����ǵ��� ����
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
    void Object_Recognition(Mat img, double& value, vector<Rect>& r) {  //��ü �ν�
        Mat labels, stats, centroids;
        int cnt = connectedComponentsWithStats(img, labels, stats, centroids);//���̺�
        //cout << stats.rows<<"���� ���̺� ������� ��ü��"<<endl;

        for (int i = 0; i < cnt; i++)      //�ٿ�� �ڽ� ���� vector<Rect>r�� ����
        {
            int* p = stats.ptr<int>(i);
            r.push_back(Rect(p[0], p[1], p[2], p[3]));
        }
        Rect tmp;
        for (int i = 1; i < r.size(); i++)      // ���� r�� ����� ������ x��ǥ�� ���������� ����ǵ��� ����
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

        for (int i = 1; i < r.size(); i++)   // ��ü �ܰ��� ����
        {
            vector<vector<Point>> contours;
            findContours(img(r[i]), contours, RETR_LIST, CHAIN_APPROX_NONE);
            //cout << contours.size() << "���� �ܰ���" << endl;
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
        if (ox == 1) { //=�϶�
            putText(img, message + " = ", Point(50, 80), FONT_HERSHEY_SIMPLEX, 3, Scalar(255), 3);
            dst = Scalar(0);

            putText(img, to_string(value), Point(200, 350), FONT_HERSHEY_SIMPLEX, 4, Scalar(255), 3);
        }
        else { //ans�϶�
            img = Scalar(0);
            draw(img);
            putText(img, "Ans = " + to_string(ans), Point(50, 80), FONT_HERSHEY_SIMPLEX, 3, Scalar(255), 3);

        }

    }
    static Point2f  pts(1400, 250), small(100, 50);                            // 4�� ��ǥ �� ��ǥ �簢�� ũ��

    void draw_rect(Mat img)                                // 4�� ��ǥ �մ� �簢�� �׸���
    {
        Rect img_rect(Point(0, 0), img.size());            // �Է¿��� ũ�� �簢��
        Rect rect(pts - small, pts + small);        // ��ǥ �簢��
        rect &= img_rect;                                // ���� ���� ���
        rectangle(img, rect, Scalar(0), 1);    // ��ǥ �簢�� �׸���
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
        Point curr_pt(x, y);                                    // ���� Ŭ�� ��ǥ


        draw_rect(img.clone());                                    // ��ǥ �簢�� �׸���

        if (event == EVENT_LBUTTONDOWN) {
            ptOld = Point(x, y);
            if (x >= 1300 && y >= 310) { // = ��ư ����
                draw_rect(img);                                    // ��ǥ �簢�� �׸���
                Object_Recognition(dst, value, r); //��ü �ν�
                img = Scalar(0);
                draw_q(img, dst, value, 1);
                pts = Point(1400, 250);
                draw_rect(img.clone());
                imshow("img", img);
                message = "";
                level = 1;

            }
            else if (x >= 1300 && y <= 100) { // ac��ư ����
                img = Scalar(0);
                draw(img);
                pts = Point(1400, 250);
                draw_rect(img.clone());
                imshow("img", img);
                level = 2;
            }
            else if (x >= 1300 && (y < 300 && y>200)) { // ans����
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
            Rect rect(pts - small, pts + small);    // ��ǥ �簢���� ����
            if (rect.contains(curr_pt))  ck = 1;        // ���� ��ǥ �簢�� ã��

        }
        if (event == EVENT_MBUTTONUP)
            ck = -1;                                    // ���� ��ǥ��ȣ �ʱ�ȭ

        if (ck >= 0) {                                    // ��ü ���ý�    
            pts = curr_pt;                            // Ŭ�� ��ǥ�� ���� ��ǥ�� ����
            draw_rect(img.clone());                        // 4�� ��ǥ ���� �簢�� �׸���

        }

    }

}