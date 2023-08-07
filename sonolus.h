#include<jsoncpp/json/json.h>
#include<openssl/ssl.h>

#include"modules/modules.h"

#include"items/Exception.h"
#include"items/EngineEnums.h"
#include"items/FuncNode.h"
#include"items/EngineConfiguration.h"
#include"items/EngineData.h"

EngineData engineData;
EngineConfiguration engineConfiguration;

#include"functions/functions.h"
#ifndef DISABLE_REDEFINE
#include"functions/redefine.h"
#endif
#ifndef DISABLE_JSPREFER
typedef FuncNode var;
typedef FuncNode let;
#endif

#include"blocks/Archetype.h"
#include"blocks/Define.h"
#include"blocks/Blocks.h"

#include"items/Variables.h"

map<EngineDataNode, int> hashMap;

int buildScript(FuncNode script, int blockCounter = 0) {
    EngineDataNode res;
    if (script.isValue == true) res = EngineDataValueNode(script.value);
    else {
        // Return 函数判断
        if (script.func == "Return") {
            script.func = "Break"; FuncNode code = script.args[0];
            script.args = {blockCounter, code};
        }
        // 其余函数
        vector<double> args;
        for (int i = 0; i < script.args.size(); i++) 
            args.push_back(buildScript(
                script.args[i],
                blockCounter + (script.func == "Block")));
        res = EngineDataFunctionNode(script.func, args);
    } if (hashMap.find(res) != hashMap.end()) return hashMap[res];
    hashMap[res] = engineData.nodes.size(); engineData.nodes.push_back(res);
    return hashMap[res];
}

int buildFuncNode(FuncNode func) {
    FuncNode res = FuncNode("Block", {func});
    return buildScript(res);
}

template<typename T>
void buildArchetype(T unused) {
    T archetype; EngineDataArchetype newArchetype;
    newArchetype.name = archetype.name;
    newArchetype.hasInput = archetype.hasInput;
    newArchetype.preprocess.order = archetype.preprocessOrder;
    newArchetype.preprocess.index = buildFuncNode(archetype.preprocess);
    newArchetype.spawnOrder.order = archetype.spawnOrderOrder;
    newArchetype.spawnOrder.index = buildFuncNode(archetype.spawnOrder);
    newArchetype.shouldSpawn.order = archetype.shouldSpawnOrder;
    newArchetype.shouldSpawn.index = buildFuncNode(archetype.shouldSpawn);
    newArchetype.initialize.order = archetype.initializeOrder;
    newArchetype.initialize.index = buildFuncNode(archetype.initialize);
    newArchetype.updateSequential.order = archetype.updateSequentialOrder;
    newArchetype.updateSequential.index = buildFuncNode(archetype.updateSequential);
    newArchetype.touch.order = archetype.touchOrder;
    newArchetype.touch.index = buildFuncNode(archetype.touch);
    newArchetype.updateParallel.order = archetype.updateParallelOrder;
    newArchetype.updateParallel.index = buildFuncNode(archetype.updateParallel);
    newArchetype.terminate.order = archetype.terminateOrder;
    newArchetype.terminate.index = buildFuncNode(archetype.terminate);
    newArchetype.data = archetype.data;
    engineData.archetypes.push_back(newArchetype);
}

template<typename T, typename... Args> 
void buildArchetype(T unused, Args... args) {
    buildArchetype<T>(T()); buildArchetype<Args...>(args...);
}

template<typename T, typename... Args>
void build(buffer& configurationBuffer, buffer& dataBuffer) {
    Json::Value configuration = engineConfiguration.toJsonObject();
    buildArchetype<T, Args...>(T(), Args()...);
    Json::Value data = engineData.toJsonObject();
    configurationBuffer = compress_gzip(json_encode(configuration));
    dataBuffer = compress_gzip(json_encode(data));
}

#define IF(cond) If(cond, Execute(
#define ELSE ), Execute(
#define FI ))
#define FOR(i, st, en, step) [&](){\
    FuncNode i = ForPt.get(); \
    return Block(Execute({\
        ForPt.set(st - step), \
        While(ForPt.get() < en - step, Block(Execute({\
            ForPt.set(ForPt.get() + step), \
            Execute(
#define DONE )}))) \
    })); \
}()
#define CONTINUE Break(1, 0)
#define BREAK Break(2, 0)