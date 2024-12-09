프로젝트명 : 발로란트<br>
개발기간 : 2023.08.01 ~ 2023.08.28 (28일) + (추후 약 1달간 개인적으로 스킬 추가 및 디버깅 진행)<br>
작인원 : 6명 (기획 2 + 프로그래머 3 + 그래픽 1)<br>
담당작업 : 소바 캐릭터 전담<br>
시연 링크 : https://youtu.be/0xMOAtAt0-o <br>

# Point 1. 튕김효과
[SB_ArrowVersion2.cpp](https://github.com/micalia/Valorant_ShinSeolBin/blob/main/Source/Valorant/Private/SB_ArrowVersion2.cpp#L155)
![refle](https://github.com/user-attachments/assets/c962da6f-856d-482a-bfb4-7d869743dd83)

# Point 2. 물리공식을 활용한 움직임
[SB_ArrowVersion2.cpp](https://github.com/micalia/Valorant_ShinSeolBin/blob/main/Source/Valorant/Private/SB_ArrowVersion2.cpp#L56)
![actorGravity](https://github.com/user-attachments/assets/3a417945-148b-4abb-a6e4-b6aa83bf946e)

# Point 3. 네트워크 동기화 보간처리
[SB_Hook.cpp](https://github.com/micalia/Valorant_ShinSeolBin/blob/main/Source/Valorant/Private/SB_Hook.cpp#L55)
![replicat](https://github.com/user-attachments/assets/c56bafb0-569c-46d3-8aab-295380b03703)

# Point 4. 마우스 좌표에 대응하는 월드 좌표 구하기
[AirSmokeMinimapWidget.cpp](https://github.com/micalia/Valorant_ShinSeolBin/blob/main/Source/Valorant/Private/AirSmokeMinimapWidget.cpp#L26)
![air1](https://github.com/user-attachments/assets/1797dfad-6b4e-4b87-b91e-b5db935c75d8)
![air2](https://github.com/user-attachments/assets/3c4c39d1-d0d4-48a0-bfe8-630bcf3ae3f0)

# Point 5. 서버 데미지 처리
[BaseCharacter.cpp](https://github.com/micalia/Valorant_ShinSeolBin/blob/main/Source/Valorant/Private/BaseCharacter.cpp#L338)
![구현설명](https://github.com/user-attachments/assets/721b1747-bf4e-4b31-a0da-de583cc3c629)

# Point 6. 가상함수를 활용한 궁극기 게이지 증가
[SB_Sova.cpp](https://github.com/micalia/Valorant_ShinSeolBin/blob/main/Source/Valorant/Private/SB_Sova.cpp#L1448)
![virtualFunctionExecute](https://github.com/user-attachments/assets/22db9c4a-0e2b-4fa6-b9e1-261f36baf36f)

