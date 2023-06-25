# Sonolus Hanipure Engine

A recreation of Honeyworks Premium Live engine in [Sonolus](https://sonolus.com).

Use [sonolus.h](https://github.com/SonolusHaniwa/sonolus.h) to develop.

Todo List:

- [x] Finished Chart Converter
- [x] Finished Initialization Archetype
- [x] Finished Input Manager Archetype
- [x] Finished Stage Archetype
- [ ] Finished Note && Hold Note Archetype
- [ ] Finished Flick Archetype
- [ ] Finished Hold Line Archetype
- [ ] Finished Hold End & Hold Flick End Archetype
- [ ] Finished Sync Line Archetype

## Links

- [Sonolus Website](https://sonolus.com) 
- [Sonolus Wiki](https://github.com/NonSpicyBurrito/sonolus-wiki)
- [sonolus.h](https://github.com/SonolusHaniwa/sonolus.h)

## Installation

```bash
sudo apt install libjsoncpp-dev openssl zlib1g-dev libssl-dev -y
git clone https://github.com/SonolusHaniwa/sonolus-hwpl-engine
cd sonolus-hwpl-engine && git checkout dev
g++ main.cpp -o main -ljsoncpp -lssl -lcrypto -lz
```

## Custom Resources

Engine ID: `1`

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
