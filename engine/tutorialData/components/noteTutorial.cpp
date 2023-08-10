class noteTutorial {
    public:

    var drop = {
        drawDrop(Sprites.NormalNote, 3, timesNow)
    };

    var froze = {
        drawStatic(Sprites.NormalNote, 3),
        instruction.set(Texts.tap),
        drawFrozenHand(3, ((timesNow - appearTimeLength) % frozenTimeLength) / frozenTimeLength * 100, 1, 1)
    };

    var click = {
        instruction.set(-1),
        tutorialNavigation.add(1),
        tutorialStartTime.set(times.now)
    };

    var play = {
        IF (timesNow < appearTimeLength) {
            drop
        } ELSE {
            IF (timesNow < appearTimeLength + frozenTimes * frozenTimeLength) {
                froze
            } ELSE {
                click
            } FI,
        } FI,
    };
}noteTutorial;