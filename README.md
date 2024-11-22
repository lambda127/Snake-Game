# Snake Game
2024-2 C 프로그래밍 실습 최종과제 문제 - 지렁이 게임(Snake Game)



    01. 시스템적 기능
       - 초기 레벨 설정
       - 스코어에 따라 레벨 바꾸기
       - 종료 상황    (완료)
       - 기록 표시
       - 기록 저장
    
    02. 이벤트성 기능
       - 미스테리 박스
          = 점수 증가
             : 랜덤으로 1~50 만큼의 점수 증가    (수정 완료)
    
          = 점수 감소
             : 랜덤으로 1~50 만큼의 점수 감소, 점수가 0이하 일 떄는 감소하지 않음    (수정 완료)
    
          = 몸통 길이 증가
             : 랜덤으로 1~5개 의 몸통길이 증가    (완료)
    
       - 폭탄 박스
          = 점수 감소
             : 현재 점수와 상관없이 50점 감소    (수정)
    
       - 부비트랩
          : 아이템에 닿으면 닿았을 때 절단, 잘린 몸통은 남아 있음, 잘린 몸통에 닿으면 게임 종료
             => 연결리스크에서 분리, 분리된 노드부터 리스트에 담아서 draw할 때 표시
