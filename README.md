## 테트리스 프로젝트

![](https://velog.velcdn.com/images/codegod/post/912dbb07-41f7-4c2a-abdc-c68945b9dad5/image.png)

### 개요

- SFML 3.0.0을 이용한 클래식 테트리스 구현
- 고스트 블록, 저장 시스템, 보너스 이펙트, 메뉴화면 등 다양한 기능 포함

### 사용 기술
| 구분 | 내용 |
|------|------|
| 언어 | `C++` |
| 라이브러리 | `SFML 3.0.0` |
| 도구 | Visual Studio 2022, Git |
| 폰트 | NotoSansKR-Regular |

### 실행 방법
1. Visual Studio 2022로 프로젝트 열기
2. `assets/NotoSansKR-Regular.ttf` 경로에 폰트 존재 여부 확인
3. F5 (또는 Ctrl+F5)로 실행


### 주요 기능
- 키보드 조작: ←, →, ↑(회전), ↓(하드 드롭), C(홀드), P(일시정지)
- 점수 / 레벨 시스템
- 일시정지, 게임 오버 처리
- 고스트 블록 미리보기
- 다음 블록, 저장 블록 시각화
- 한글 폰트 및 보너스 메시지 지원

### 기본 작동과 회전
![](https://velog.velcdn.com/images/codegod/post/fc3eaf03-d460-4785-a511-01094e71d3a0/image.gif)

- 방향키 (좌, 우)를 이용하여 도형의 위치를 조정합니다.
- 방향키 (상)을 이용하여 도형을 우측으로 회전시킵니다.
```cpp
		shape[1][1] = '#'; shape[1][2] = '#';
        shape[2][1] = '#'; shape[2][2] = '#';
        //2차원 배열 활용하여 도형 설계
        if (key == sf::Keyboard::Key::Left) { temp.x--; if (!board.checkCollision(temp)) current.x--; }
        // 키보드 입력처리 예시
```
### 하드드롭 기능

![](https://velog.velcdn.com/images/codegod/post/e80a2860-0b89-4e84-a1f0-493a927d9ff3/image.gif)

- 방향키 (하)를 이용하여 도형을 강제로 드롭합니다.

아래 코드는 블록이 고정될 때 보드 위에 블록 데이터를 반영하는 부분입니다.  
블록은 4x4 shape 정보를 기준으로, 실제 보드 좌표로 매핑되며, 경계를 벗어나는 블록은 무시됩니다.

```cpp
void Board::placeTetromino(const Tetromino& t) {
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            if (t.shape[i][j] == '#') {
                int x = t.x + j;
                int y = t.y + i;
                if (y >= 0 && y < 20 && x >= 0 && x < 10) {
                    grid[y][x] = t.type; // 실제 보드 위치에 고정

```

### 일시정지

![](https://velog.velcdn.com/images/codegod/post/b59f87ad-b2a3-4c2c-bbf4-b1137d43a39a/image.gif)

- P를 입력하여 게임을 일시정지/시작 합니다.

토글 여부를 기반으로 일시 정지 화면을 출력합니다.
```cpp
void Game::drawPause(sf::RenderWindow& window) {
    sf::Text pause{ font, L"일시정지\nP 키로 계속", 32 };
    pause.setFillColor(sf::Color::Yellow);
    pause.setPosition({ 200.f, 250.f });
    window.draw(pause);
}
```

### 도형 저장 기능

![](https://velog.velcdn.com/images/codegod/post/2d44e1a5-48c4-47f0-9b53-4da88a2ecbc5/image.gif)

- C혹은 LShift를 입력하여 도형을 저장합니다.
- 연속 사용이 불가합니다.
- 기존 저장된 도형이 있다면 다음 도형으로 저장되어 있는 도형을 불러옵니다.
```cpp
else if ((key == sf::Keyboard::Key::C || key == sf::Keyboard::Key::LShift) && !holdUsedThisTurn) {
    if (!hasHold) {
        hold = current;
        current = next;
        next = Tetromino();
        hasHold = true;
    }
    else {
        std::swap(current, hold); // 저장된 블록이 있을 경우 교체
    }
    current.x = 3; current.y = 0;
    holdUsedThisTurn = true;
}
```

### 라인 클리어 및 점수

![](https://velog.velcdn.com/images/codegod/post/9c53997e-e7a5-49c5-9998-f26646801e34/image.gif)

- 한 줄당 100점의 점수를 부여합니다.

### 다중 라인 클리어

![](https://velog.velcdn.com/images/codegod/post/eabcd44c-0306-471f-a6a5-7329d61a8c6a/image.gif)

- 다중 라인 클리어 시 점수를 차등 부여합니다.
```cpp
 switch (lines) {
 case 1: score += 100; break;
 case 2: score += 300; bonusMessage = L"더블!"; showBonus = true; bonusTimer = now; break;
 case 3: score += 500; bonusMessage = L"트리플!"; showBonus = true; bonusTimer = now; break;
 case 4: score += 800; bonusMessage = L"TETRIS!"; showBonus = true; bonusTimer = now; break;
 }
```

### 레벨업
![](https://velog.velcdn.com/images/codegod/post/7c63e8ef-4ebd-4869-9df0-cb38a5dd3705/image.gif)
- 점수 1000점 마다 레벨이 증가합니다.
- 레벨에 따라 도형 하강 속도가 증가합니다.

```cpp
  level = score / 1000 + 1;
  speed = std::max(100, 500 - (level - 1) * 40);
```

### 게임 오버
![](https://velog.velcdn.com/images/codegod/post/e5ab8fa3-3656-4e96-9e8f-ab5143c9d84a/image.gif)
- 도형이 시작점까지 가득 찰 경우 게임 오버됩니다.
- 스페이스바를 이용하여 재시작할 수 있습니다.

[Velog]([https://github.com/Minhyeok-Create/TetrisProject](https://velog.io/@codegod/%ED%85%8C%ED%8A%B8%EB%A6%AC%EC%8A%A4-%ED%94%84%EB%A1%9C%EC%A0%9D%ED%8A%B8-C))

