# delivery_pkg

2023년 1학기 창업연계융합설계 과목 수강 중 네비게이션 로봇 제작 프로젝트에서 만든 패키지 


기능 :
1. yaml파일에 나열된 방 번호에 따른 좌표를 가져와 활용한다.
2. 아두이노로부터 tof, encoder, hough_tranform 패키지 로부터 교차점의 x좌표, d435i로부터 imu정보를 topic으로 subscribe한다.
3. 아두이노에게 계산된 모터의 rpm, rqt_plot에게 PD제어가 적용된 우측 모터의 rpm을 publish한다.

dependencies : 
roscpp, rospy, std_msgs, roslib
