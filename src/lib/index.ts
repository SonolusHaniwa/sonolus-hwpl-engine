import { EngineInfo, LevelData } from 'sonolus-core'
import { ChartObject, fromHanipure as _fromHanipure } from './convert'
import { Resource } from './Resource'

// eslint-disable-next-line @typescript-eslint/no-var-requires
const archetypes = require('./archetypes')

export const version = '0.0.1'

export const engineInfo = {
    name: 'hwpl',
    version: 7,
    title: {
        en: 'Hanipure',
        ja: 'ハニプレ',
        zhs: 'Hanipure',
        zht: 'Hanipure',
    },
    subtitle: {
        en: 'Honeyworks Premium Live',
        ja: 'ハニプレ',
        zhs: 'Honeyworks Premium Live',
        zht: 'Honeyworks Premium Live',
    },
    author: {
        en: 'LittleYang0531',
    },
    description: {
        en: [
            'A recreation of Honeyworks Premium Live engine in Sonolus.',
            '',
            'Version:',
            version,
            '',
            'GitHub Repository:',
            'https://github.com/LittleYang0531/sonolus-hwpl-engine',
        ].join('\n'),
    },
} as const satisfies Partial<EngineInfo>

export const engineConfiguration = new Resource('EngineConfiguration')
export const engineData = new Resource('EngineData')
export const engineThumbnail = new Resource('thumbnail.png')

export function fromHanipure(chart: Array<Array<number>>): LevelData {
    return _fromHanipure(chart, archetypes)
}
