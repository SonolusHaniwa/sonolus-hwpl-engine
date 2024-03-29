using namespace std;

class Initialization: public Archetype {
    public:

    string name = "Hanipure Initialization";
    
    var preprocess = {
        [](){
            // 界面开关参数
            var menuWidth = 0.15 * ui.menuConfiguration.scale;
            var menuHeight = 0.15 * ui.menuConfiguration.scale;
            var menuX = screen.r - interfaceGap;
            var menuY = screen.t - interfaceGap;
            // 首要指标参数
            var primaryMetricBarWidth = 0.75 * ui.primaryMetricConfiguration.scale;
            var primaryMetricBarHeight = 0.15 * ui.primaryMetricConfiguration.scale;
            var primaryMetricBarX = screen.l + interfaceGap;
            var primaryMetricBarY = screen.t - interfaceGap;
            var primaryMetricValueWidth = 0 * ui.primaryMetricConfiguration.scale;
            var primaryMetricValueHeight = 0.08 * ui.primaryMetricConfiguration.scale;
            var primaryMetricValueX = primaryMetricBarX + 0.715 * ui.primaryMetricConfiguration.scale;
            var primaryMetricValueY = primaryMetricBarY - 0.035 * ui.primaryMetricConfiguration.scale;
            // 次要指标参数
            var secondaryMetricBarWidth = 0.55 * ui.secondaryMetricConfiguration.scale;
            var secondaryMetricBarHeight = 0.15 * ui.secondaryMetricConfiguration.scale;
            var secondaryMetricBarX = screen.r - interfaceGap - menuWidth - interfaceGap;
            var secondaryMetricBarY = screen.t - interfaceGap;
            var secondaryMetricValueWidth = 0 * ui.secondaryMetricConfiguration.scale;
            var secondaryMetricValueHeight = 0.08 * ui.secondaryMetricConfiguration.scale;
            var secondaryMetricValueX = secondaryMetricBarX - 0.035 * ui.secondaryMetricConfiguration.scale;
            var secondaryMetricValueY = secondaryMetricBarY - 0.035 * ui.secondaryMetricConfiguration.scale;
            // 连击指标参数
            var comboValueWidth = 0 * ui.comboConfiguration.scale;
            var comboValueHeight = stage.h * 0.12 * ui.comboConfiguration.scale;
            var comboValueX = stage.w * 0.4;
            var comboValueY = Lerp(t, b, 0.5);
            var comboTextWidth = 0 * ui.comboConfiguration.scale;
            var comboTextHeight = stage.h * 0.05 * ui.comboConfiguration.scale;
            var comboTextX = stage.w * 0.4;
            var comboTextY = Lerp(t, b, 0.5);
            return Execute({
                ui.menu.set(menuX, menuY, 1, 1, menuWidth, menuHeight, 0, ui.menuConfiguration.alpha, HorizontalAlign.Center, true),
                ui.primaryMetricBar.set(primaryMetricBarX, primaryMetricBarY, 0, 1, primaryMetricBarWidth, primaryMetricBarHeight, 0, ui.primaryMetricConfiguration.alpha, HorizontalAlign.Left, true),
                ui.primaryMetricValue.set(primaryMetricValueX, primaryMetricValueY, 1, 1, primaryMetricValueWidth, primaryMetricValueHeight, 0, ui.primaryMetricConfiguration.alpha, HorizontalAlign.Right, false),
                ui.secondaryMetricBar.set(secondaryMetricBarX, secondaryMetricBarY, 1, 1, secondaryMetricBarWidth, secondaryMetricBarHeight, 0, ui.secondaryMetricConfiguration.alpha, HorizontalAlign.Left, true),
                ui.secondaryMetricValue.set(secondaryMetricValueX, secondaryMetricValueY, 1, 1, secondaryMetricValueWidth, secondaryMetricValueHeight, 0, ui.secondaryMetricConfiguration.alpha, HorizontalAlign.Right, false),
                ui.comboValue.set(comboValueX, comboValueY, 0.5, 0.5, comboValueWidth, comboValueHeight, 0, ui.comboConfiguration.alpha, HorizontalAlign.Center, false),
                ui.comboText.set(comboTextX, comboTextY, 0.5, 2.75, comboTextWidth, comboTextHeight, 0, ui.comboConfiguration.alpha, HorizontalAlign.Center, false),
                LevelScore.set(0, score.perfect),
                LevelScore.set(1, score.great),
                LevelScore.set(2, score.good),
                lifes[3].set(0, 0, 0, -100),
                lifes[4].set(0, 0, 0, -100),
                lifes[5].set(0, 0, 0, -100),
                lifes[6].set(0, 0, 0, -100),
                lifes[7].set(0, 0, 0, -100),
                lifes[8].set(0, 0, 0, -10),
                buckets[0].set(-1 * judgment.perfect, judgment.perfect, -1 * judgment.great, judgment.great, -1 * judgment.good, judgment.good),
                buckets[1].set(-1 * judgment.good, judgment.good, -1 * judgment.good, judgment.good, -1 * judgment.good, judgment.good),
                buckets[2].set(-1 * judgment.perfect, judgment.perfect, -1 * judgment.great, judgment.great, -1 * judgment.good, judgment.good),
                buckets[3].set(-1 * judgment.perfect, judgment.perfect, -1 * judgment.great, judgment.great, -1 * judgment.good, judgment.good),
                buckets[4].set(-1 * judgment.good, judgment.good, -1 * judgment.good, judgment.good, -1 * judgment.good, judgment.good),
                buckets[5].set(-1 * judgment.good, judgment.good, -1 * judgment.good, judgment.good, -1 * judgment.good, judgment.good)
            });
        }()
    };

    var spawnOrder = 0;

    var shouldSpawn = 1;

    var updateSequential = {
        EntityDespawn.set(0, 1)
    };
};  