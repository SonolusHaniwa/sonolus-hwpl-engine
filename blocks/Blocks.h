const int LevelMemoryId = 2000;
const int LevelDataId = 2001;
const int LevelOptionId = 2002;
// const int LevelTransformId = 3;
// const int LevelBackgroundId = 4;
// const int LevelUIId = 5;
const int LevelBucketId = 2003;
const int LevelScoreId = 2004;
const int LevelLifeId = 2005;
// const int LevelUIConfigurationId = 9;
const int EntityInfoArrayId = 4103;
const int EntityDataArrayId = 4101;
const int EntitySharedMemoryArrayId = 4102;
const int EntityInfoId = 4003;
const int EntityMemoryId = 4000;
const int EntityDataId = 4001;
const int EntityInputId = 4005;
const int EntitySharedMemoryId = 4002;
const int ArchetypeLifeId = 5000;
const int EngineRomId = 3000;
const int TemporaryMemoryId = 10000;
// const int TemporaryTouchDataId = 101;
const int EntityDespawnId = 4004;
const int RuntimeEnvironmentId = 1000;
const int RuntimeUpdateId = 1001;
const int RuntimeTouchArrayId = 1002;
const int RuntimeSkinTransformId = 1003;
const int RuntimeParticleTransformId = 1004;
const int RuntimeBackgroundId = 1005;
const int RuntimeUIId = 1006;
const int RuntimeUIConfigurationId = 1007;

template<int identifierId>
class Pointer {
    public:
    
    FuncNode offset = 0;
    int size = -1;

    FuncNode get(FuncNode i) {
        if (size != -1 && i.isValue == true &&
            (i.value >= size || i.value < 0)) throwWarning("");
        return Get(identifierId, Add({i, offset}));
    }

    FuncNode set(FuncNode i, FuncNode value) {
        if (size != -1 && i.isValue == true &&
            (i.value >= size || i.value < 0)) throwWarning("");
        return Set(identifierId, Add({i, offset}), value);
    }

    FuncNode set2(FuncNode i, FuncNode value) {
        return set(i, value);
    }
};

template<typename T, int blockSize>
class PointerArray {
    public:

    T operator [] (int offset) {return T(Multiply({offset, blockSize}));}
    T operator [] (FuncNode offset) {return T(Multiply({offset, blockSize}));};
};

Pointer<RuntimeEnvironmentId> RuntimeEnvironment;
Pointer<RuntimeUpdateId> RuntimeUpdate;
Pointer<RuntimeTouchArrayId> RuntimeTouch;
Pointer<RuntimeSkinTransformId> RuntimeSkinTransform;
Pointer<RuntimeParticleTransformId> RuntimeParticleTransform;
Pointer<RuntimeBackgroundId> RuntimeBackground;
Pointer<RuntimeUIId> RuntimeUI;
Pointer<RuntimeUIConfigurationId> RuntimeUIConfiguration;
Pointer<LevelMemoryId> LevelMemory;
Pointer<LevelDataId> LevelData;
Pointer<LevelOptionId> LevelOption;
Pointer<LevelBucketId> LevelBucket;
Pointer<LevelScoreId> LevelScore;
Pointer<LevelLifeId> LevelLife;
Pointer<EngineRomId> EngineRom;
Pointer<EntityMemoryId> EntityMemory;
Pointer<EntityDataArrayId> EntityData;
Pointer<EntitySharedMemoryArrayId> EntitySharedMemory;
Pointer<EntityInfoArrayId> EntityInfo;
Pointer<EntityDespawnId> EntityDespawn;
Pointer<EntityInputId> EntityInput;
Pointer<ArchetypeLifeId> ArchetypeLife;
Pointer<TemporaryMemoryId> TemporaryMemory;

// #include"archetypeLife.h"
// #include"engineRom.h"
// #include"entityData.h"
// #include"entityInfo.h"
// #include"entityInput.h"
// #include"entityMemory.h"
// #include"entitySharedMemory.h"
// #include"levelBackground.h"
// #include"levelBucket.h"
// #include"levelData.h"
// #include"levelLife.h"
// #include"levelMemory.h"
// #include"levelOption.h"
// #include"levelScore.h"
// #include"levelTransform.h"
// #include"levelUI.h"
// #include"levelUIConfiguration.h"
// #include"temporaryMemory.h"
// #include"temporaryTouchData.h"

class screen {
    public:

    FuncNode aspectRadio = RuntimeEnvironment.get(1);
    FuncNode l = -1 * aspectRadio;
    FuncNode b = -1;
    FuncNode r = aspectRadio;
    FuncNode t = 1;
    FuncNode w = aspectRadio * 2;
    FuncNode h = 2;
}screen;

class ui {
    public:

    class layout {
        public: 

        FuncNode offset = 0;
        layout(){};
        layout(FuncNode offset):offset(offset){};

        FuncNode anchorX = RuntimeUI.get(offset * 10 + 0);
        FuncNode anchorY = RuntimeUI.get(offset * 10 + 1);
        FuncNode pivotX = RuntimeUI.get(offset * 10 + 2);
        FuncNode pivotY = RuntimeUI.get(offset * 10 + 3);
        FuncNode width = RuntimeUI.get(offset * 10 + 4);
        FuncNode height = RuntimeUI.get(offset * 10 + 5);
        FuncNode rotation = RuntimeUI.get(offset * 10 + 6);
        FuncNode alpha = RuntimeUI.get(offset * 10 + 7);
        FuncNode horizontalAlign = RuntimeUI.get(offset * 10 + 8);
        FuncNode background = RuntimeUI.get(offset * 10 + 9);

        FuncNode set(FuncNode anchorX, FuncNode anchorY, FuncNode pivotX, FuncNode pivotY, FuncNode width, FuncNode height, FuncNode rotation, FuncNode alpha, FuncNode horizontalAlign, FuncNode background) {
            return Execute({
                RuntimeUI.set(offset * 10 + 0, anchorX),
                RuntimeUI.set(offset * 10 + 1, anchorY),
                RuntimeUI.set(offset * 10 + 2, pivotX),
                RuntimeUI.set(offset * 10 + 3, pivotY),
                RuntimeUI.set(offset * 10 + 4, width),
                RuntimeUI.set(offset * 10 + 5, height),
                RuntimeUI.set(offset * 10 + 6, rotation),
                RuntimeUI.set(offset * 10 + 7, alpha),
                RuntimeUI.set(offset * 10 + 8, horizontalAlign),
                RuntimeUI.set(offset * 10 + 9, background),
            });
        }
    };

    class configuration {
        public:

        FuncNode offset = 0;
        configuration(){};
        configuration(FuncNode offset):offset(offset){};

        FuncNode scale = RuntimeUIConfiguration.get(offset * 2 + 0);
        FuncNode alpha = RuntimeUIConfiguration.get(offset * 2 + 1);
    };

    layout menu = layout(0);
    layout judgment = layout(1);
    layout comboValue = layout(2);
    layout comboText = layout(3);
    layout primaryMetricBar = layout(4);
    layout primaryMetricValue = layout(5);
    layout secondaryMetricBar = layout(6);
    layout secondaryMetricValue = layout(7);

    configuration menuConfiguration = configuration(0);
    configuration judgmentConfiguration = configuration(1);
    configuration comboConfiguration = configuration(2);
    configuration primaryMetricConfiguration = configuration(3);
    configuration secondaryMetricConfiguration = configuration(4);
}ui;

class entityInfo {
    public:

    class info {
        public:

        FuncNode offset;
        info(){}
        info(FuncNode offset):offset(offset){};

        FuncNode id = EntityInfo.get(offset * 3 + 0);
        FuncNode archetype = EntityInfo.get(offset * 3 + 1);
        FuncNode state = EntityInfo.get(offset * 3 + 2);
    };

    info operator [] (FuncNode offset) {
        return info(offset);
    }
}entityInfo;

class Touch {
    public:

    FuncNode offset = 0;
    Touch(){};
    Touch(FuncNode offset):offset(offset){};
    Touch operator [] (FuncNode offset) {
        return Touch(offset);
    }
    FuncNode size = RuntimeUpdate.get(3);

    FuncNode id = RuntimeTouch.get(offset * 15 + 0);
    FuncNode started = RuntimeTouch.get(offset * 15 + 1);
    FuncNode ended = RuntimeTouch.get(offset * 15 + 2);
    FuncNode t = RuntimeTouch.get(offset * 15 + 3);
    FuncNode st = RuntimeTouch.get(offset * 15 + 4);
    FuncNode x = RuntimeTouch.get(offset * 15 + 5);
    FuncNode y = RuntimeTouch.get(offset * 15 + 6);
    FuncNode sx = RuntimeTouch.get(offset * 15 + 7);
    FuncNode sy = RuntimeTouch.get(offset * 15 + 8);
    FuncNode dx = RuntimeTouch.get(offset * 15 + 9);
    FuncNode dy = RuntimeTouch.get(offset * 15 + 10);
    FuncNode vx = RuntimeTouch.get(offset * 15 + 11);
    FuncNode vy = RuntimeTouch.get(offset * 15 + 12);
    FuncNode vr = RuntimeTouch.get(offset * 15 + 13);
    FuncNode vw = RuntimeTouch.get(offset * 15 + 14);
}touches;

int allocatorSize[10000] = {0};

template<int identifierId>
class Array {
    public:

    int offset;
    int capacity;
    int size = 0;

    Array(){}
    Array(int capacity):capacity(capacity){
        offset = allocatorSize[identifierId];
        allocatorSize[identifierId] += capacity;
    };
    FuncNode operator [] (int id) {
        if (id >= size) runtime_error("StorageArray: index out of range");
        return Get(identifierId, Add({id, offset}));
    }
    FuncNode add(FuncNode value) {
        if (size == capacity) runtime_error("StorageArray: array is full");
        return Set(identifierId, Add({size++, offset}), value);
    }
    FuncNode has(FuncNode value) {
        FuncNode res = false;
        for (int i = size - 1; i >= 0; i--) {
            res = If(
                Get(identifierId, Add({i, offset})) == value,
                true,
                res
            );
        } return res;
    }
    FuncNode indexOf(FuncNode value) {
        FuncNode res = -1;
        for (int i = size - 1; i >= 0; i--) {
            res = If(
                Get(identifierId, Add({i, offset})) == value,
                i,
                res
            );
        } return res;
    }
    FuncNode clear() {
        size = 0; FuncNode res = Execute({});
        for (int i = 0; i < capacity; i++) res.args.push_back(Set(identifierId, Add({i, offset}), 0));
        return res;
    }
};

template<int identifierId>
class Variable {
    public:

    int offset;
    Variable(){
        offset = allocatorSize[identifierId]++;
    }

    FuncNode get() {
        return Get(identifierId, offset);
    }
    FuncNode add(FuncNode value) {
        return Set(identifierId, offset, Get(identifierId, offset) + value);
    }
    FuncNode subtract(FuncNode value) {
        return Set(identifierId, offset, Get(identifierId, offset) - value);
    }
    FuncNode multiply(FuncNode value) {
        return Set(identifierId, offset, Get(identifierId, offset) * value);
    }
    FuncNode divide(FuncNode value) {
        return Set(identifierId, offset, Get(identifierId, offset) / value);
    }
    FuncNode mod(FuncNode value) {
        return Set(identifierId, offset, Get(identifierId, offset) % value);
    }
    FuncNode set(FuncNode value) {
        return Set(identifierId, offset, value);
    }
};

Variable<LevelMemoryId> touchCounter;

class HorizontalAlign {
    public:

    int Left = -1;
    int Center = 0;
    int Right = 1;
}HorizontalAlign;

class EntityState {
    public:

    int Waiting = 0;
    int Active = 1;
    int Despawned = 2;    
}EntityState;