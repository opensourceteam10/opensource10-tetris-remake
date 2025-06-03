# OpenSource10 Tetris Remake

## 소개
- 본 프로젝트는 기존 오픈소스 테트리스(pixeltetris)를 기반으로 기본 테트리스를 구현하고, 리메이크 작업까지 확장하는 것을 목표로 합니다.
- 사용 언어: C++
- 사용 라이브러리: SDL2, SDL2_image, SDL2_ttf

## 설치 방법

**필수 패키지 설치**

- Mac (Homebrew 사용)

```bash
brew install sdl2 sdl2_image sdl2_ttf cmake
```

- Ubuntu (APT 사용)

```bash
sudo apt update
sudo apt install libsdl2-dev libsdl2-image-dev libsdl2-ttf-dev cmake
```

## 빌드 방법

1. 프로젝트 클론
```bash
git clone https://github.com/opensourceteam10/opensource10-tetris-remake.git
cd opensource10-tetris-remake
```
2. 빌드
```bash
mkdir build
cd build
cmake ..
make
```

## 실행 방법

빌드 후 실행
```bash
./PixelTetris
```

## 기여 방법

1. 기능 개발 시 브랜치 생성
```bash
git checkout -b feature/브랜치이름
```
2. 기능 개발 후 커밋, 푸시
```bash
git add .
git commit -m "기능 설명"
git push origin feature/브랜치이름
```
3. Pull Request(PR) 생성
4. 코드 리뷰 및 병합
5. 원격저장소의 새 브랜치 prototypeproject로 병합

## 라이선스

본 프로젝트는 [zlib License](https://opensource.org/licenses/Zlib) 하에 배포됩니다.

## 팀원

- 32243818 장신혁
- 32203282 이승찬
- 32220018 강대운

