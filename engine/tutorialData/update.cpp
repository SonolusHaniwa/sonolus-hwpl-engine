using namespace std;

auto update = defineUpdate({
    drawStage(),
    tutorialNavigation.mod(6),
    Switch(tutorialNavigation.get(), {
        {0, noteTutorial.play},
        {1, flickTutorial.play},
        {2, holdStartTutorial.play},
        {3, holdEndTutorial.play},
        {4, holdStartTutorial.play},
        {5, holdFlickEndTutorial.play}
    })
});