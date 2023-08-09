using namespace std;

class Options {
    public:

    int autoplay = 0;
    int strictJudge = 1;
    int mirror = 2;
    int noteSpeed = 3;
    int noteSize = 4;
    int syncLine = 5;
    int lockAspectRatio = 6;
    int stage = 7;
}Options;

auto options = defineOptions<class Options>({
    {
        .name = NameText.AutoPlay,
        .scope = Scope,
        .standard = 1,
        .type = OptionType.Toggle,
        .def = 0,
    }, {
        .name = NameText.StrictJudgment,
        .scope = Scope,
        .standard = 1,
        .type = OptionType.Toggle,
        .def = 0,
    }, {
        .name = NameText.MirrorLevel,
        .scope = Scope,
        .standard = 0,
        .type = OptionType.Toggle,
        .def = 0
    }, {
        .name = NameText.NoteSpeed,
        .scope = Scope,
        .standard = 0,
        .type = OptionType.Slider,
        .def = 5,
        .min = 1,
        .max = 50,
        .step = 0.1,
    }, {
        .name = NameText.NoteSize,
        .scope = Scope,
        .standard = 0,
        .type = OptionType.Slider,
        .def = 1,
        .min = 0.5,
        .max = 2.0,
        .step = 0.1,
        .unit = UnitText.Percentage,
    }, {
        .name = NameText.SimultaneousLineVisibility,
        .scope = Scope,
        .standard = 1,
        .type = OptionType.Toggle,
        .def = 1,
    }, {
        .name = NameText.LockStageAspectRatio,
        .scope = Scope,
        .standard = 1,
        .type = OptionType.Toggle,
        .def = 0,
    }, {
        .name = NameText.StageVisibility,
        .scope = Scope,
        .standard = 1,
        .type = OptionType.Toggle,
        .def = 1,
    }
});