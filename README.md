# Handwriting_Calculator 🖍
![Footer](https://capsule-render.vercel.app/api?type=waving&color=auto&height=200&section=footer)

#Project Description
- OpenCV를 활용해 사용자가 필기체로 입력한 수식을 계산해주는 계산기 프로그램이다.

#작품 영상



https://user-images.githubusercontent.com/105347300/185037897-1304241e-e89f-455f-bf7e-3eaab437dd1e.mp4



youtube :  https://www.youtube.com/watch?v=RH3HuNRTRrU

# Project Description
- OpenCV를 활용해 사용자가 필기체로 입력한 수식을 계산해주는 계산기 프로그램이다.

# Project function
- GUI : 사용자의 수식을 입력 받아 저장하는 기능
- 문자인식 : 20개의 문자를 인식하는 기능
  0,1,2,3,4,5,6,7,8,9,.(소수점),A,n,s,+,-,/,x,(,)
- 계산기 기능 : 인식된 수식을 계산하여 결과를 출력하는 기능 ,ANS,CE,AC 기능

# Project algorithm
![image](https://user-images.githubusercontent.com/105347300/185038520-ed0d36f8-6631-4ec4-b6e5-09933acd10f1.png)

계산기 구현 방법
-----------------

### 식을 후위표기법으로 바꾸기 --- > string calc1();


  >1. 숫자가 나오면 그대로 문자열에 입력한다.

  >2. '(' 나오면 벡터에 push한다.

  >3. '*' '/' 나오면 벡터에 push한다.

  >4. '+' '-' 연산이 나오면 여는 괄호 이후부터 끝까지 문자열에 입력 , 여는 괄호가 없다면 벡터의 끝까지 출력하고 그 연산자를 문자열에 입력한다.

  >5. 닫는 괄호(')')가 나오면 여는 괄호('(')가 나올때까지 문자열에 입력한다.



### 후위표기법 계산하기 --> double calc2(string str);


  >1.문자열 앞에서부터 ' ' 공백을 기준으로 끊기

  >2. 숫자이면 벡터에 push

  >3. 연산자이면 벡터의 마지막 요소와 마지막 전 요소끼리 연산후 두개의 요소를 pop하고 결과값을 벡터에 push



# 계산기 code
-식을 후위표기법으로 변환
'''
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
'''
