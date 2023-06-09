import { EffectClip, SkinSprite } from 'sonolus-core'
import {
    And,
    bool,
    Equal,
    Greater,
    GreaterOr,
    If,
    InputAccuracy,
    InputBucket,
    InputBucketValue,
    InputJudgment,
    InputOffset,
    Judge,
    Max,
    Multiply,
    Not,
    Or,
    Script,
    Subtract,
    Time,
    TouchId,
    TouchT,
} from 'sonolus.js'
import { options } from '../../configuration/options'
import { buckets } from '../buckets'
import { goodWindow, greatWindow, perfectWindow, slideWindow } from './common/constants'
import {
    checkTouchXInNoteHitbox,
    destroyNoteHoldEffect,
    drawNote,
    initializeNote,
    InputState,
    isNotHidden,
    NoteData,
    noteInputState,
    NoteSharedMemory,
    playNoteLaneEffect,
    playNoteTapEffect,
    prepareDrawNote,
    preprocessNote,
    scheduleNoteAutoSFX,
    scheduleNoteAutoSlide,
    stopNoteHoldSFX,
    touchProcessDiscontinue,
    touchProcessHead,
    updateNoteSlideScale,
} from './common/note'
import { playJudgmentSFX } from './common/sfx'
import { checkTouchYInHitbox, isTouchOccupied } from './common/touch'

export function slideTickNote(): Script {
    const bucket = buckets.slideTickNoteIndex

    const preprocess = preprocessNote(true, If(options.isStrictJudgment, goodWindow, slideWindow))

    const spawnOrder = NoteData.spawnTime

    const shouldSpawn = GreaterOr(Time, NoteData.spawnTime)

    const initialize = initializeNote(bucket, true, false)

    const updateSequential = scheduleNoteAutoSlide()

    const touch = Or(options.isAutoplay, [
        touchProcessHead(),
        And(
            Equal(noteInputState, InputState.Activated),
            Equal(TouchId, NoteSharedMemory.inputTouchId),
            [
                isTouchOccupied.set(true),
                And(
                    GreaterOr(Subtract(Time, InputOffset), NoteData.time),
                    checkTouchYInHitbox(),
                    checkTouchXInNoteHitbox(),
                    onComplete()
                ),
                touchProcessDiscontinue(),
            ]
        ),
    ])

    const updateParallel = [
        scheduleNoteAutoSFX(EffectClip.Perfect),

        Or(
            And(options.isAutoplay, GreaterOr(Time, NoteData.time)),
            And(
                Not(options.isAutoplay),
                Not(bool(noteInputState)),
                Greater(
                    Subtract(Time, NoteData.head.time, InputOffset),
                    If(options.isStrictJudgment, goodWindow, slideWindow)
                )
            ),
            Equal(noteInputState, InputState.Terminated),
            Greater(
                Subtract(Time, NoteData.time, InputOffset),
                If(options.isStrictJudgment, goodWindow, slideWindow)
            ),
            And(GreaterOr(Time, NoteData.visibleTime), isNotHidden(), [
                updateNoteSlideScale(),
                prepareDrawNote(),
                drawNote(SkinSprite.NoteTickGreen),
            ])
        ),
    ]

    const terminate = [
        And(options.isAutoplay, [playNoteLaneEffect(), playNoteTapEffect()]),

        And(Or(options.isAutoplay, bool(noteInputState)), destroyNoteHoldEffect()),
        Or(options.isAutoplay, stopNoteHoldSFX()),
    ]

    return {
        preprocess,
        spawnOrder,
        shouldSpawn,
        initialize,
        updateSequential,
        touch,
        updateParallel,
        terminate,
    }

    function onComplete() {
        return [
            noteInputState.set(InputState.Terminated),

            NoteSharedMemory.isInputSuccess.set(true),

            InputJudgment.set(
                If(
                    options.isStrictJudgment,
                    Judge(
                        Max(Subtract(TouchT, InputOffset), NoteData.time),
                        NoteData.time,
                        perfectWindow,
                        greatWindow,
                        goodWindow
                    ),
                    Judge(
                        Max(Subtract(TouchT, InputOffset), NoteData.time),
                        NoteData.time,
                        slideWindow,
                        slideWindow,
                        slideWindow
                    )
                )
            ),
            InputAccuracy.set(Max(Subtract(TouchT, InputOffset, NoteData.time), 0)),
            InputBucket.set(bucket),
            InputBucketValue.set(Multiply(1000, InputAccuracy)),

            playNoteLaneEffect(),
            playNoteTapEffect(),
            playJudgmentSFX(),
        ]
    }
}
