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
### :bulb: 소수점

![image](https://user-images.githubusercontent.com/105347300/185043097-6ed8e4e3-fdd6-421f-9afc-67835cf20ad1.png)

 x축에 수직으로 나눈 픽셀값이 255인 픽셀 개수의 평균값
 
![image](https://user-images.githubusercontent.com/105347300/185043155-4b340789-907f-4a08-bfeb-fc672eedede3.png)

- x축으로 나눈 구간 10개의 평균을 구함. --> double avg(vector<double> v)   -->평균값이 3이하
- y축으로 나눈 구간 10개의 평균을 구함 -- > double avg(vector<double> v) --> 평균값이 2이하
- 객체의 size가 50이하이면 true


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


