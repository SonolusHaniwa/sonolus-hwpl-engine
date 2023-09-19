using namespace std;

class Effects {
    public:
	
	int noteCircular = 0;
	int noteLinear = 1;
	int flickCircular = 2;
	int flickLinear = 3;
	int holdCircular = 4;
	int holdLinear = 5;
	int stage = 6;
}Effects;

auto particles = defineParticles<class Effects>({
	{ParticleEffectName.NoteCircularTapGreen, Effects.noteCircular},
	{ParticleEffectName.NoteLinearTapGreen, Effects.noteLinear},
	{ParticleEffectName.NoteCircularTapPurple, Effects.flickCircular},
	{ParticleEffectName.NoteLinearTapPurple, Effects.flickLinear},
	{ParticleEffectName.NoteCircularTapCyan, Effects.holdCircular},
	{ParticleEffectName.NoteLinearTapCyan, Effects.holdLinear},
	{ParticleEffectName.LaneLinear, Effects.stage}
});
