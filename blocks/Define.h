using namespace std;

template<typename T>
T defineOptions(vector<EngineConfigurationOption> options) {
    for (int i = 0; i < options.size(); i++) 
        engineConfiguration.options.push_back(options[i]);
    return T();
}

template<typename T>
T defineSkins(vector<pair<string, int> > skins) {
    for (int i = 0; i < skins.size(); i++)
        engineData.skin_sprites.push_back(skins[i]);
    return T();
}

template<typename T>
T defineEffects(vector<pair<string, int> > effects) {
    for (int i = 0; i < effects.size(); i++)
        engineData.effect_clips.push_back(effects[i]);
    return T();
}

template<typename T>
T defineParticles(vector<pair<string, int> > particles) {
    for (int i = 0; i < particles.size(); i++)
        engineData.particle_effects.push_back(particles[i]);
    return T();
}

template<typename T>
T defineBuckets(vector<EngineDataBucket> buckets) {
    for (int i = 0; i < buckets.size(); i++)
        engineData.buckets.push_back(buckets[i]);
    return T();
}