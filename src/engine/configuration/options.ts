import { OptionName } from 'sonolus-core'
import { defineOptions } from 'sonolus.js'

export const options = defineOptions({
    isAutoplay: {
        name: OptionName.AutoPlay,
        scope: 'Bandori',
        standard: true,
        type: 'toggle',
        def: 0,
    },
    isNoteSpeedRandom: {
        name: OptionName.RandomNoteSpeed,
        scope: 'Bandori',
        standard: true,
        type: 'toggle',
        def: 0,
    },
    isStrictJudgment: {
        name: OptionName.StrictJudgment,
        scope: 'Bandori',
        standard: true,
        type: 'toggle',
        def: 0,
    },
    speed: {
        name: OptionName.LevelSpeed,
        standard: true,
        type: 'slider',
        def: 1,
        min: 0.5,
        max: 2,
        step: 0.05,
        display: 'percentage',
    },
    noteSpeed: {
        name: OptionName.NoteSpeed,
        scope: 'Bandori',
        type: 'slider',
        def: 5,
        min: 1,
        max: 15.9,
        step: 0.1,
        display: 'number',
    },
    noteSize: {
        name: OptionName.NoteSize,
        scope: 'Bandori',
        type: 'slider',
        def: 1,
        min: 0.1,
        max: 2,
        step: 0.05,
        display: 'percentage',
    },
    noteEffectSize: {
        name: OptionName.NoteEffectSize,
        scope: 'Bandori',
        type: 'slider',
        def: 1,
        min: 0.1,
        max: 2,
        step: 0.05,
        display: 'percentage',
    },
    connectorAlpha: {
        name: OptionName.ConnectorTransparency,
        scope: 'Bandori',
        type: 'slider',
        def: 0.8,
        min: 0.1,
        max: 1,
        step: 0.05,
        display: 'percentage',
    },
    stageCover: {
        name: OptionName.VerticalStageCover,
        scope: 'Bandori',
        type: 'slider',
        def: 0,
        min: 0,
        max: 1,
        step: 0.05,
        display: 'percentage',
    },
    isMirrorEnabled: {
        name: OptionName.MirrorLevel,
        type: 'toggle',
        def: 0,
    },
    isSimLineEnabled: {
        name: OptionName.SimultaneousLineVisibility,
        scope: 'Bandori',
        type: 'toggle',
        def: 1,
    },
    isSFXEnabled: {
        name: OptionName.SFX,
        scope: 'Bandori',
        type: 'toggle',
        def: 1,
    },
    isNoteEffectEnabled: {
        name: OptionName.NoteEffect,
        scope: 'Bandori',
        type: 'toggle',
        def: 1,
    },
    isLaneEffectEnabled: {
        name: OptionName.LaneEffect,
        scope: 'Bandori',
        type: 'toggle',
        def: 1,
    },
    isSlotEffectEnabled: {
        name: OptionName.SlotEffect,
        scope: 'Bandori',
        type: 'toggle',
        def: 1,
    },
    isStageAspectRatioLocked: {
        name: OptionName.LockStageAspectRatio,
        scope: 'Bandori',
        type: 'toggle',
        def: 1,
    },
})
