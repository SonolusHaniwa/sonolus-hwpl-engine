#include<bits/stdc++.h>
#include"sonolus.h"
using namespace std;

const string dist = "./dist";
#include"engine/engine.cpp"

string readFile(string path) {
    ifstream fin(path.c_str());
    fin.seekg(0, ios::end);
    int len = fin.tellg();
    if (len == -1) return "";
    fin.seekg(0, ios::beg);
    char* ch = new char[len];
    fin.read(ch, len);
    string buffer;
    for (int i = 0; i < len; i++) buffer.push_back(ch[i]);
    delete[] ch;
    return buffer;
}

string randomRef(int len) {
	string res = "";
	for (int i = 0; i < len; i++) res += char(rand() % 26 + 'A');
	return res;
}

const int refLen = 32;
Json::Value fromHanipure(Json::Value chart, double bgmOffset = 0) {
	Json::Value res; map<int, string> lastNoteRef, stNoteRef;
	map<double, int> minLane, maxLane;
	srand(time(0));
	// 谱面转换
	Json::Value single;
	single["archetype"] = "Hanipure Initialization";
	single["data"][0]["name"] = "beat"; single["data"][0]["value"] = 0;
	res.append(single);
	single["archetype"] = "Hanipure Input Manager";
	res.append(single);
	single["archetype"] = "Hanipure Stage";
	res.append(single);
	for (int i = 0; i < chart.size(); i++) {
		Json::Value note = chart[i];
		Json::Value single, data; single["data"].resize(0);
		if (note[0].asInt() >= 10 && note[0].asInt() < 20) note[3] = 1e5 + note[3].asInt();
		switch(note[0].asInt()) {
			case 1: {
				single["archetype"] = "Hanipure Normal Note";
				single["data"][0]["name"] = "beat";
				single["data"][0]["value"] = note[1].asDouble();
				single["data"][1]["name"] = "lane";
				single["data"][1]["value"] = note[2].asInt();
			}; break; // Normal Note
			case 2: {
				single["archetype"] = "Hanipure Normal Flick";
				single["data"][0]["name"] = "beat";
				single["data"][0]["value"] = note[1].asDouble();
				single["data"][1]["name"] = "lane";
				single["data"][1]["value"] = note[2].asInt();
			}; break; // Normal Flick
			case 11: case 21: {
				single["archetype"] = "Hanipure Normal Hold";
				single["data"][0]["name"] = "beat";
				single["data"][0]["value"] = note[1].asDouble();
				single["data"][1]["name"] = "lane";
				single["data"][1]["value"] = note[2].asInt();
				string randomId = randomRef(refLen);
				lastNoteRef[note[3].asInt()] = randomId;
				stNoteRef[note[3].asInt()] = randomId;
				single["ref"] = randomId;
			}; break; // Normal Hold
			case 22: {
				single["archetype"] = "Hanipure Hold Line";
				single["data"][0]["name"] = "beat";
				single["data"][0]["value"] = note[1].asDouble();
				single["data"][1]["name"] = "lane";
				single["data"][1]["value"] = note[2].asInt();
				single["data"][2]["name"] = "last";
				single["data"][2]["ref"] = lastNoteRef[note[3].asInt()];
				single["data"][3]["name"] = "start";
				single["data"][3]["ref"] = stNoteRef[note[3].asInt()];
				string randomId = randomRef(refLen);
				lastNoteRef[note[3].asInt()] = randomId;
				single["ref"] = randomId;
			} break; // Hold Line
			case 12: case 23: {
				single["archetype"] = "Hanipure Hold End";
				single["data"][0]["name"] = "beat";
				single["data"][0]["value"] = note[1].asDouble();
				single["data"][1]["name"] = "lane";
				single["data"][1]["value"] = note[2].asInt();
				single["data"][2]["name"] = "last";
				single["data"][2]["ref"] = lastNoteRef[note[3].asInt()];
				single["data"][3]["name"] = "start";
				single["data"][3]["ref"] = stNoteRef[note[3].asInt()];
			} break; // Hold Normal Note
			case 13: case 24: {
				single["archetype"] = "Hanipure Hold Flick End";
				single["data"][0]["name"] = "beat";
				single["data"][0]["value"] = note[1].asDouble();
				single["data"][1]["name"] = "lane";
				single["data"][1]["value"] = note[2].asInt();
				single["data"][2]["name"] = "last";
				single["data"][2]["ref"] = lastNoteRef[note[3].asInt()];
				single["data"][3]["name"] = "start";
				single["data"][3]["ref"] = stNoteRef[note[3].asInt()];
			} break; // Hold Flick Note
		}
		if (note[0].asInt() != 22) {
			double beat = note[1].asDouble(); int lane = note[2].asInt();
			if (minLane.find(beat) == minLane.end()) minLane[beat] = lane;
			else minLane[beat] = min(minLane[beat], lane);
			if (maxLane.find(beat) == maxLane.end()) maxLane[beat] = lane;
			else maxLane[beat] = max(maxLane[beat], lane);
		}
		res.append(single);
	}
	// 计算同步线
	for (auto v : minLane) {
		double beat = v.first; int lane = v.second;
		if (lane == maxLane[beat]) continue;
		Json::Value single;
		single["archetype"] = "Hanipure Sync Line";
		single["data"][0]["name"] = "beat";
		single["data"][0]["value"] = beat;
		single["data"][1]["name"] = "minLane";
		single["data"][1]["value"] = lane;
		single["data"][2]["name"] = "maxLane";
		single["data"][2]["value"] = maxLane[beat];
		res.append(single);
	}
	Json::Value data;
	data["bgmOffset"] = bgmOffset;
	data["entities"] = res;
	return data;
}

int main(int argc, char** argv) {
    // 谱面转换测试
	if (argc >= 3) {
		Json::Value chart;
		string chartJson = readFile(argv[1]);
		json_decode(chartJson, chart);
		Json::Value LevelData = fromHanipure(chart);
		string dataJson = json_encode(LevelData);
		buffer data = compress_gzip(dataJson, 9);
		ofstream preFout(argv[2]);
		for (int i = 0; i < data.size(); i++) preFout << data.v[i];
		preFout.close();
	}

    engineConfiguration.ui = configurationUI;
#ifdef play
    buffer data, configuration;
    build<
        Initialization,
        InputManager,
        Stage,
        NormalNote,
        NormalFlick,
        HoldStart,
        HoldEnd,
        HoldFlickEnd,
        HoldLine,
        SyncLine
    >(configuration, data);
    ofstream fout((dist + "/EngineConfiguration"));
    for (int i = 0; i < configuration.size(); i++) fout << configuration.v[i];
    fout.close(); fout.open((dist + "/EngineData"));
    for (int i = 0; i < data.size(); i++) fout << data.v[i];
#elif tutorial
    buffer data, configuration;
    build(configuration, data);
    ofstream fout((dist + "/EngineConfiguration"));
    for (int i = 0; i < configuration.size(); i++) fout << configuration.v[i];
    fout.close(); fout.open((dist + "/EngineTutorialData"));
    for (int i = 0; i < data.size(); i++) fout << data.v[i];
#endif
}