# **[Unreal] ⚔️ 발더스게이트3 스타일 턴제 전투 게임**

## 📜 프로젝트 소개
이 프로젝트는 '발더스 게이트3'의 UI 및 턴제 전투 시스템 구현을 목표로 제작되었습니다.

## 🙋🏻‍♀️ 개발 인원
1명

## ⌛ 개발 기간
5개월

## 🎞️ 게임 영상
[언리얼 턴제 전투 (YUTUBE)](https://youtu.be/Pp9vFBqveSk)


## Foot IK 적용
![FootIk](https://github.com/user-attachments/assets/2bf8683e-e8ad-48e2-af82-1d212157c8a9)  



FeetComponent를 만들어 매 프레임마다 캐릭터의 발 위치를 기반으로  


LineTrace를 수행하여 지형의 높이와 기울기를 계산한다.  


이 정보를 바탕으로 힙 오프셋을 조정, 동시에 각 발의 위치와 회전을 계산 및 보간하여  


캐릭터의 발이 지형에 밀착되도록 한다.


애님블루프린트에서 본의 트랜스폼을 조정한 후, 2본 IK 노드를 활용해 본의 위치와 회전을 자연스럽게 보정한다.  


## 선제권 기반 턴제 전투
![선제권](https://github.com/user-attachments/assets/fd778f70-5c39-41d6-a3a1-73382a6b6438)  


선제권 숫자를 기반으로 상단에 오름차순으로 정렬되며  


현재 행동 중인 유닛의 초상화는 약간 더 크게 강조된다.  


유닛이 본인의 턴을 종료해야 다음 유닛에게 행동 기회가 주어진다.  

## 전투 중 이동
![이동](https://github.com/user-attachments/assets/a9fda9e6-2fe0-4028-becd-9d29da94d489)  


목표 지점을 클릭하면 캐릭터가 그곳으로 이동한다.  


턴 종료 버튼의 파란 테두리는 이동력을 뜻하며 캐릭터가 이동하는 만큼 줄어들게 된다.  


캐릭터와 목표 지점 사이의 거리가 현재 이동력으로 갈 수 있는지 계산하고  


갈 수 있다면 캐릭터는 해당 지점으로 이동한다.  

## 피격 방향 판별
![정면](https://github.com/user-attachments/assets/bd912e4b-ebae-4ab8-bb91-ffcceaff625f)
![좌측](https://github.com/user-attachments/assets/7f88d733-13fa-4d68-b45a-cd95df98f6ad)
![후면](https://github.com/user-attachments/assets/fa393183-6d3c-4819-83e8-68823a519434)
![우측](https://github.com/user-attachments/assets/b4b7db8c-0937-4b9b-8435-fd4d1a144620)  




