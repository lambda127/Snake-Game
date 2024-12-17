# Snake Game
2024-2 C 프로그래밍 실습 최종과제 문제 - 지렁이 게임(Snake Game)



01. 시스템적 기능
- 초기 레벨 설정 (완료)

  select_level() 함수
<img src=https://github.com/user-attachments/assets/86bd4ca7-f09e-49e4-9836-149289424ef3 width="50%" height="50%"/>

<img src=https://github.com/user-attachments/assets/7c798606-c597-48d1-8d55-aff4cfd7c54a width="20%" height="20%"/>

- 스코어에 따라 레벨 바꾸기 (완료)

  logic() 함수
  
<img src=https://github.com/user-attachments/assets/4c8253f6-a512-415f-a5d0-ee86aaca1865 width="50%" height="50%"/>

- 종료 상황 (완료)

  logic() 함수
  
<img src=https://github.com/user-attachments/assets/29c8d14d-ebad-4f91-be5e-a26360773ad2 width="50%" height="50%"/>

- 기록 표시 (완료)

  read_record() 함수
  
<img src=https://github.com/user-attachments/assets/d51d19a9-2d8e-4120-a4a2-5d018896c726 width="50%" height="50%"/>

- 기록 저장 (완료)

  write_record() 함수
  
<img src=https://github.com/user-attachments/assets/bb200842-93c4-4e51-ac0c-098e46f150ff width="50%" height="50%"/>


02. 이벤트성 기능
- 미스테리 박스
  
    = 점수 증가
    : 랜덤으로 1~50 만큼의 점수 증가 (수정 완료)
             
    = 점수 감소

    : 랜덤으로 1~50 만큼의 점수 감소, 점수가 0이하 일 떄는 감소하지 않음 (수정 완료)

    점수 감소의 경우 랜덤한 감소량이 점수를 0 이하로 만들면 0이하로 떨어지지 않게 해야 하려나? (헸음)

    = 몸통 길이 증가

    : 랜덤으로 1~5개 의 몸통길이 증가 (완료)

<img src=https://github.com/user-attachments/assets/f5ccee8e-0b71-4b80-a128-8d4b80d08af3 width="50%" height="50%"/>

- 폭탄 박스
    = 점수 감소

     : 현재 점수와 상관없이 50점 감소 (수정 완료)

     레벨에 따라 개수 변경
              
<img src=https://github.com/user-attachments/assets/e5e95f20-c2f4-4918-9e40-46f8af09e55b width="50%" height="50%"/>

<img src=https://github.com/user-attachments/assets/1252f9fe-c28a-4141-a7f1-fd82f58d3966 width="50%" height="50%"/>

<img src=https://github.com/user-attachments/assets/ce3e1d4a-9601-4b46-aace-fc539f5bc1b7 width="50%" height="50%"/>



- 부비트랩 (완료)
  
    : 아이템에 닿으면 닿았을 때 절단, 잘린 몸통은 남아 있음, 잘린 몸통에 닿으면 게임 종료
  
     => 연결리스크에서 분리, 분리된 노드부터 리스트에 담아서 draw할 때 표시
  
    snake가 끊어진 몸통에 닿았을 때 종료 작성햐야 함 (완료)
  
<img src=https://github.com/user-attachments/assets/76f282c6-0a16-467c-8f69-8c814c0fd190 width="50%" height="50%"/>

<img src=https://github.com/user-attachments/assets/6433eca9-dae3-4e93-b10d-2c988a2dc6a4 width="50%" height="50%"/>

<img src=https://github.com/user-attachments/assets/a18ac4d3-ed31-486e-9b9b-bff23193bcfe width="50%" height="50%"/>




03. 추가 내용 (계획서 작성 이후 디벨롭)
   
- 인트로 대화 추가
  
    지렁이와 대화
  
    게임 내 이벤트 요소 및 게임 방법 설명
  
    플레이어 이름 입력
  
    게임 진행 선택

    select_level() 함수에서 진행
             
<img src=https://github.com/user-attachments/assets/1ba961a4-a4a4-4c5d-8f26-b4ef074b9e5e width="50%" height="50%"/>

<img src=https://github.com/user-attachments/assets/313312ba-6d75-4db9-9274-1a0b687f40e1 width="50%" height="50%"/>

<img src=https://github.com/user-attachments/assets/be5948c0-f61a-4fa2-a9ca-22ed79ef07c5 width="50%" height="50%"/>

<img src=https://github.com/user-attachments/assets/dca13dd4-6764-43c1-9d2b-d0a2d89763cf width="50%" height="50%"/>

<img src=https://github.com/user-attachments/assets/627bc024-8d8e-4180-8a5c-f67addd1e444 width="50%" height="50%"/>


- 게임 종료 후

    게임 플레이 기록 표시 (이름, 점수)
  
    최고 기록 보유 플레이어 이름, 최고 기록 표시
  
    최고 기록 달성 시 최고 기록 변경을 표시
  
<img src=https://github.com/user-attachments/assets/c0a8e86c-9b15-4c61-be32-4772ea79b01e width="50%" height="50%"/>


    
