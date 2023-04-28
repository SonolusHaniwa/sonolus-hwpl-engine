import { readJsonSync } from 'fs-extra'
import { archetypes } from '../../engine/data/archetypes'
import { fromHanipure } from '../../lib/convert'

export const levelData = fromHanipure(readJsonSync(__dirname + '/level.json'), archetypes)
