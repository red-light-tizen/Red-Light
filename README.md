## 팀명 및 팀원
* 나상혁 - 팀장, 기획, S/W 설계, S/W 개발
* 김건우 - 디자인, H/W 설계, H/W 개발, 기술조언
* 김혜령 - 디자인
* 윤석진 - S/W 개발

## 프로젝트 제목
* Red Light
* 행동 불능 시에도 사용 가능한 응급 호출 서비스

## 프로젝트 배경 혹은 목적
* 행동불능 상태에서도 위험을 알릴 방법이 필요
* 사용자의 조작 없이도 위험정보를 알릴 수 있는 제품

## 파일 리스트
* inc/bluetooth-common.h
* inc/bluetooth-receiver.h
* inc/bluetooth-sender.h
* inc/event-bus.h
* inc/red-light.h
* inc/utils.h
* src/bluetooth-common.c
* src/bluetooth-receiver.c
* src/bluetooth-sender.c
* src/event-bus.c
* src/red-light.c

## 코드 기여자
* inc/bluetooth-common.h 윤석진
* inc/bluetooth-receiver.h 윤석진
* inc/bluetooth-sender.h 윤석진
* inc/event-bus.h 나상혁
* inc/red-light.h
* inc/utils.h 나상혁
* src/bluetooth-common.c add_bluetooth_device_authorization 나상혁
* src/bluetooth-common.c 윤석진
* src/bluetooth-receiver.c 윤석진
* src/bluetooth-sender.c 윤석진
* src/event-bus.c 나상혁
* src/red-light.c

## 보드
* RPI3: 센서 연동, https://github.com/red-light-tizen/Red-Light
* x86 PC: 상태 모니터링, https://github.com/red-light-tizen/DesktopApplication

## 구현사항
* UART, SPI 사용
* 클라우드 사용하지 않음
* 카메라 사용하지 않음
* 이미지 분석 사용하지 않음
