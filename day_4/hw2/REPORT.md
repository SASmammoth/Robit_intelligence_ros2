# Day4_hw2

> **광운대학교 컴퓨터정보공학부**  
> **작성자:** 장경민  
> **제출일:** 2026.09.18.

---

## 1. 개요 (Overview)
robot_description 패키지만으로 bringup 패키지를 만들고, CLI 명령으로 로봇의 모든 구동부가 움직이는지 확인하기

## 2. 체크포인트 (checkpoint)

모든 영상은 ./etc폴더 안에 저장되어 있음.

### 2.1 launch 한 번으로 bringup 실행, 모든 컨트롤러 active

<video src="./etc/hw2_Ch1.webm" controls width="600" autoplay loop muted></video>

---

### 2.2 카메라 타워 관절 4개에 명령을 보내 움직이기

<video src="./etc/hw2_Ch2.webm" controls width="600" autoplay loop muted></video>


---

### 2.3 팔 관절 6개를 컨트롤러 하나로 궤적 명령을 보내 움직이기

<video src="./etc/hw2_Ch3.webm" controls width="600" autoplay loop muted></video>

---


### 2.4 	베이스의 바퀴와 플리퍼에 명령을 보내 움직이기

<video src="./etc/hw2_Ch4.webm" controls width="600" autoplay loop muted></video>  


## 3. 간단한 설명


robot_description 는 일절 건들이지 않고, eclipse_bringup_JKM 패키지를 하나 만들어서 구현 하였다. 강의자료에 있는 controllers.yaml 과 bringup.launch.py 에서 약간의 수정 및 추가를 하여 컨트롤러를 구성하고 연결하여 만들었다. 2.4에서 로봇을 전진/후진 시키는 것은 프레임을 설정을 base_link에서 새로 만든 odom으로 바꾸어 시각화함.