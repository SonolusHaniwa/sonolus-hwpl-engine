# Sonolus Hanipure Engine

A recreation of Honeyworks Premium Live engine in [Sonolus](https://sonolus.com). For Sonolus 0.7.x.

适用于 Sonolus 0.7.2+ 的 Honeyworks Premium Live 引擎。

Use [sonolus.h](https://github.com/SonolusHaniwa/sonolus.h) and [sonolus-server-cpp](https://github.com/SonolusHaniwa/sonolus-server-cpp) to develop.

Todo List:

- [x] Finished Chart Converter
- [x] Finished Initialization Archetype
- [x] Finished Input Manager Archetype
- [x] Finished Stage Archetype
- [x] Finished Note && Hold Note Archetype
- [x] Finished Flick Archetype
- [x] Finished Hold Line Archetype
- [x] Finished Hold End & Hold Flick End Archetype
- [x] Finished Sync Line Archetype

使用 Sonolus Server C++ v1.4.4 进行开发，并在 Ubuntu arm64 环境下编译通过。

## Links

- [Sonolus Website](https://sonolus.com) 
- [Sonolus Wiki](https://github.com/NonSpicyBurrito/sonolus-wiki)
- [sonolus.h](https://github.com/SonolusHaniwa/sonolus.h)

## Installation

```bash
sudo apt install libjsoncpp-dev openssl zlib1g-dev libssl-dev -y # 安装环境依赖
git clone https://github.com/SonolusHaniwa/sonolus-hwpl-engine # 拉取源码
cd sonolus-hwpl-engine # 修改目录
g++ main.cpp -o main -ljsoncpp -lssl -lcrypto -lz # 编译程序
./main # 在 dist 文件夹下生成 EngineData 和 EngineConfiguration
./main LevelData.json dist/LevelData # 在 dist 文件夹下生成 LevelData
```

## Custom Resources

### Skin Sprites

|ID|Sprite| 
|:-:|:-:|
|0|Hanipure Normal Note|
|1|Hanipure Highlighted Note|
|2|Hanipure Normal Flick|
|3|Hanipure Highlighted Flick|
|4|Hanipure Normal Hold|
|5|Hanipure Highlighted Hold|
|6|Hanipure Hold Line|
|7|Hanipure Judge Note|
|8|Hanipure Stage|
|9|Hanipure Judge Line|
|10|Hanipure Hold Body|
|11|Hanipure Sync Line|

### Effect Clips

|ID|Clip|
|:-:|:-:|
|0|Hanipure Perfect|
|1|Hanipure Great|
|2|Hanipure Good|
|3|Hanipure Stage|
|4|Hanipure Flick|
|5|Hanipure Hold|

### Particle Effects

|ID|Effect|
|:-:|:-:|
|||