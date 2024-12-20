# **[UE4] ⚔️ 발더스게이트3 스타일 턴제 전투 게임**


## 📜 프로젝트 소개
이 프로젝트는 **'발더스 게이트3'** 의 UI 및 턴제 전투 시스템 구현을 목표로 제작되었습니다.

## 🙋🏻‍♀️ 개발 인원
1명

## ⌛ 개발 기간
5개월

## 🎞️ 게임 영상
[언리얼 턴제 전투 (YUTUBE)](https://youtu.be/o7kSz3OnQvM))




## 📍 Foot IK 적용
![FootIk](https://github.com/user-attachments/assets/2bf8683e-e8ad-48e2-af82-1d212157c8a9)  


**FeetComponent**를 만들어 매 프레임마다 캐릭터의 발 위치를 기반으로  


**LineTrace**를 수행하여 지형의 높이와 기울기를 계산한다.  


이 정보를 바탕으로 힙 오프셋을 조정, 동시에 각 발의 위치와 회전을 계산 및 보간하여 캐릭터의 발이 지형에 밀착되도록 한다.

![anim_bp](https://github.com/user-attachments/assets/f635f85f-2959-4925-b352-731752e2ae11)


애님블루프린트에서 **본의 트랜스폼을 조정**한 후, **2본 IK 노드**를 활용해 본의 위치와 회전을 자연스럽게 보정한다.  


## 📍 선제권 기반 턴제 전투
![선제권](https://github.com/user-attachments/assets/fd778f70-5c39-41d6-a3a1-73382a6b6438)  


랜덤으로 정해진 **선제권 숫자**를 기반으로 상단에 오름차순으로 정렬되며  


현재 행동 중인 유닛의 초상화는 약간 더 크게 강조된다.  


유닛이 본인의 턴을 종료해야 다음 유닛에게 행동 기회가 주어진다.  


## 📍 전투 중 이동
![이동](https://github.com/user-attachments/assets/a9fda9e6-2fe0-4028-becd-9d29da94d489)  


목표 지점을 클릭하면 캐릭터가 그곳으로 이동한다.  


**턴 종료 버튼의 파란 테두리**는 **이동력**을 뜻하며 캐릭터가 이동하는 만큼 줄어들게 된다.  


캐릭터와 목표 지점 사이의 거리가 현재 이동력으로 갈 수 있는지 계산하고  


갈 수 있다면 캐릭터는 해당 지점으로 이동한다.


## 📍 피격 방향 판별
![정면](https://github.com/user-attachments/assets/bd912e4b-ebae-4ab8-bb91-ffcceaff625f)
![좌측](https://github.com/user-attachments/assets/7f88d733-13fa-4d68-b45a-cd95df98f6ad)
![후면](https://github.com/user-attachments/assets/fa393183-6d3c-4819-83e8-68823a519434)
![우측](https://github.com/user-attachments/assets/b4b7db8c-0937-4b9b-8435-fd4d1a144620)  

캐릭터를 기준으로 적 무기의 위치를 계산한 후

이를 캐릭터의 **전방 및 우측 벡터와 각각 내적**하여 공격 방향의 상대적 각도를 구하고

이를 통해 캐릭터의 정면 기준으로 공격이 발생한 방향을 판별한다.

이 방향에 따라 **다른 몽타주**를 재생하게 된다.


## 📍 화면에 피해량 표시(데미지 텍스트)
![DamageTextActor](https://github.com/user-attachments/assets/e3a68a2c-2427-4b75-a889-4eb805be84a2)

**위젯 컴포넌트**와 **타임라인 컴포넌트**를 가진 액터



![Curve](https://github.com/user-attachments/assets/fd676d29-5a0e-45e9-a1a7-e285afbe2f84)

타임라인에 사용된 **Float 커브**



![DamageTextInGame](https://github.com/user-attachments/assets/c66ffbfd-8e43-41a5-9bef-a7040e9bf552)


인게임 내에서 적용된 모습.


위젯 컴포넌트를 활용하여 화면 상에 **3D 위젯 형태로 피해량을 출력**하고


타임라인과 커브를 이용하여 텍스트가 **점진적으로 위로 이동**하게 만들었다.


또한 항상 플레이어 카메라를 향하도록 **회전 업데이트**를 통해 카메라가 어느 방향으로 회전하든 피해량을 바로 확인할 수 있다.


타임라인이 종료되면 **액터를 파괴**하도록 콜백함수를 등록했다.



## 📍 경고 메세지
![경고](https://github.com/user-attachments/assets/7a0ae0d7-c1f9-4819-8340-6ae7bfbb52ce)


**행동이 불가**할 경우 경고 메세지가 표시된다.


