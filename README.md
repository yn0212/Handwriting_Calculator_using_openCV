# Handwriting_Calculator :pencil2:
![Footer](https://capsule-render.vercel.app/api?type=waving&color=auto&height=200&section=footer)

# :pushpin:Project Description
OpenCV를 활용해 사용자가 필기체로 입력한 수식을 계산해주는 계산기 프로그램이다.
--------------------------------------------------
### 사용 방법
- 마우스 왼쪽 버튼 : 수식 쓰기
- 마우스 가운데 버튼 : Ans버튼 끌어와 수식에서 사용하기
- AC : 모두 삭제 
- CE : 한개의 문자 삭제
- Ans : 식의 값 Ans에 저장(마우스 왼쪽 클릭) , Ans 버튼 드래그해 식에서 사용(마우스 가운데 버튼 ) 

# :pushpin:Project video



https://user-images.githubusercontent.com/105347300/185037897-1304241e-e89f-455f-bf7e-3eaab437dd1e.mp4



youtube :  https://www.youtube.com/watch?v=RH3HuNRTRrU

# :pushpin:Project Description
- OpenCV를 활용해 사용자가 필기체로 입력한 수식을 계산해주는 계산기 프로그램이다.

# :pushpin:Project function
- GUI : 사용자의 수식을 입력 받아 저장하는 기능
- 문자인식 : 20개의 문자를 인식하는 기능
  0,1,2,3,4,5,6,7,8,9,.(소수점),A,n,s,+,-,/,x,(,)
- 계산기 기능 : 인식된 수식을 계산하여 결과를 출력하는 기능 ,ANS,CE,AC 기능

# :pushpin:Project algorithm
## 첫번째 구분 
![image](https://user-images.githubusercontent.com/105347300/185038520-ed0d36f8-6631-4ec4-b6e5-09933acd10f1.png)

## 두번째 구분
### 📢 :loudspeaker: 외각선 1개
#### :bulb: 소수점

![image](https://user-images.githubusercontent.com/105347300/185043097-6ed8e4e3-fdd6-421f-9afc-67835cf20ad1.png)

 x축에 수직으로 나눈 픽셀값이 255인 픽셀 개수의 평균값
 
![image](https://user-images.githubusercontent.com/105347300/185043155-4b340789-907f-4a08-bfeb-fc672eedede3.png)

- x축으로 나눈 구간 10개의 평균을 구함. --> double avg(vector<double> v)   -->평균값이 3이하
- y축으로 나눈 구간 10개의 평균을 구함 -- > double avg(vector<double> v) --> 평균값이 2이하
- 객체의 size가 50이하이면 true  

#### :bulb: 문자 n
- x축으로 나눈 구간 10개의 평균을 구함. --> double avg(vector<double> v)   -->평균값이 3이하

- y축으로 나눈 구간 10개의 평균을 구함 -- > '' --> 평균값이 2이하

- 객체의 size가 50이하이고

- 객체의 무게중심 을 기준으로 맨 아래 중앙에 객체가 없으면 true

#### :bulb: 문자 s
- x축으로 나눈 구간 10개의 평균을 구함. --> double avg(vector<double> v)   -->평균값이 3이하

- y축으로 나눈 구간 10개의 평균을 구함 -- > '' --> 평균값이 2이하

- 객체의 size가 50이하

- 객체를 무게중심을 기준으로 4등분 하고 , 왼쪽 위의 구간의 픽셀값255개수 x 5 < 오른쪽 위 구간 픽셀값255의 개수이면 true

#### :bulb: 숫자 1
                                                      
![image](https://user-images.githubusercontent.com/105347300/185043793-ca4033e8-f49d-4f17-8b9d-c872ce2ece46.png)
![image](https://user-images.githubusercontent.com/105347300/185043817-b5c1c7f8-0cfb-43cc-93cb-62d85afb1774.png)

- x축으로 나눈 구간 10개의 평균을 구함. --> double avg(vector<double> v)   -->평균값이 3이하이고

- 소수점,n,s가 아니면 true

#### :bulb: 문자 -

  ![image](https://user-images.githubusercontent.com/105347300/185043928-880f8e5b-9925-48a4-984b-5bdc1b6e4b4d.png)
- y축으로 나눈 구간들의 평균이 1이하일때 : 마이너스

#### :bulb: 나누기
- bool divide(Mat img,Mat centroids)

- 무게 중심으로 객체를 자른다. -->4개로 나눠짐  
 ![image](https://user-images.githubusercontent.com/105347300/185044020-428d8462-bca0-4ae7-bfb4-1fc3d1c6e5b2.png)

- 구간 1의 픽셀값이 255인 개수 x 5 < 구간2의 픽셀값이 255인개수이면 true  
  
#### :bulb: 더하기 +            
- bool plus_d(Mat img)                               
![image](https://user-images.githubusercontent.com/105347300/185044128-8ed406be-3158-4f8b-ac43-cff89a52b3d1.png)
- 이 구간 안에 객체가 없으면 true
- 구간 : 무게중심 기준 상대적인 구간 

#### :bulb: 곱하기 x
- bool mul(mat img,Mat centroids);                               
![image](https://user-images.githubusercontent.com/105347300/185044273-83d1524b-7ed3-4c1c-97b3-de93feb4981f.png)
- 무게중심을 기준으로 위 아래 양 옆 끝에 객체가 없으면 true                               
- 구간 : 무게중심 기준 상대적인 구간                                
                               
#### :bulb: 괄호 ( , )   
- bool parentheses(mat img, Mat centroids,int num);                               
![image](https://user-images.githubusercontent.com/105347300/185044406-bc7e847a-3d2d-46c8-b441-3b7a0bb682d8.png)

 ![image](https://user-images.githubusercontent.com/105347300/185044415-9af4023b-6750-4e69-a122-3e79d62c53a0.png)
- 객체의 무게 중심을기준으로 설정한 이 구간에 객체 없으면 true
- 구간 : 무게중심 기준 상대적인 구간                                    
                               
#### :bulb: 숫자 2    
 ![image](https://user-images.githubusercontent.com/105347300/185044917-237ac316-6b8e-4f0e-aecb-988f03c3fc6c.png)

 ![image](https://user-images.githubusercontent.com/105347300/185044477-ab5f2ab4-d560-4d2f-a2e2-60299a0409bc.png)
- 무게중심 기준으로 y축으로 자른 왼쪽 부분의 외각선 이 2개
![image](https://user-images.githubusercontent.com/105347300/185044524-618ce27b-37d4-4339-ad3f-af8881b5a0c9.png)
- y축 9번째 평균개수가 최대값 or x축 평균개수(7~9)번의 반올림값이 같으면 true

#### :bulb: 숫자 5
- 무게중심 기준으로 y축으로 자른 왼쪽 부분의 외각선 이 2개
- 숫자 2 가 아니면 true
                               
#### :bulb: 숫자 3                               
![image](https://user-images.githubusercontent.com/105347300/185045705-b68694a5-d10e-45a3-8106-00dcd928870b.png)

- 무게중심 기준으로 y축으로 자른 왼쪽 부분의 외각선 이 3개이면 true
                               
#### :bulb: 숫자 7                                                      
- (x축 평균 개수 5~7번의 반올림 값이 같음 && 정수값이 같음) or y축 평균개수 6~8번의 반올림값이 같음                               
                               
                               
                               
                               
:paperclip:계산기 구현 방법 
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


