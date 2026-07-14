# Native matching wave 27: board space recovery

This ledger records the clean-C recovery added to `src/board/masu.c` on top
of `fork/main` commit `e4da21d8c7fa5fe6b51caf489a60499f4c00cdfb`.
Tests use the pinned DTK 0.9.2 binary (`4d039140`) and the configured `GC/2.6`
compiler. The owner remains `NonMatching`; none of its fallback-linked bytes
are counted as decompiled source.

No assembly, byte arrays, artificial padding, register forcing, or fake
operations were added.

## Recovered source and types

The wave adds 21 previously absent target functions and the real
`MASUNEXTWORK` layout. Target field accesses establish its `0x1C` stride and
members: player number and state at `0x0`/`0x4`, space ID at `0x8`, angle and
scale at `0xC`/`0x10`, alpha at `0x14`, and delay/time/duration at
`0x16`/`0x18`/`0x1A`. Four entries account exactly for the target `0x70`
allocation.

Mario Party 5 commit
`e246f9d9850ff53ac684b971068fbf87fdcf6acb` supplies two independent source
references:

- `src/game/board/masu.c` preserves the display-list quad construction,
  event-bit compaction, and the eight-corner ranking/rotation algorithms.
- `src/REL/w06sDll/world06.c` preserves the MP6 event-bit declaration types
  and loop shape in `MB6Ev_BitGet` and `MB6Ev_MasuFlagGet`.

The MP6 target object determines every version-specific adaptation: the two
`bmasu` animation tables, party/single selection, hook-model setup, next-space
state transitions, parent-link collection, capsule ownership checks, and
player prize fields. Sibling structure was accepted only where the resulting
MP6 instruction and relocation evidence agreed.

## Exact functions

Fourteen newly recovered functions are 100% in the fresh object comparison,
including instruction operands and relocations:

| Function | Size |
| --- | ---: |
| `mbMasuInit` | `0xE4` |
| `mbMasuClose` | `0x6C` |
| `MasuDispInit` | `0x120` |
| `MasuDispClose` | `0xB8` |
| `MasuNextCreate` | `0x94` |
| `MasuNextKill` | `0x84` |
| `ev_MasuHatena` | `0x88` |
| `mbev_MasuBitGet` | `0x78` |
| `mbev_MasuAttrGet` | `0x74` |
| `mbMasuCornerRotPosGet` | `0x188` |
| `mbMasuFind_MAttrNumGet` | `0xC8` |
| `mbMasuPlayerCapMoveCheck` | `0xB0` |
| `mbMasuPlayerCapStopCheck` | `0xB0` |
| `mbMasuPlayerPrizeReset` | `0xC4` |

These functions total `0xA28` target code bytes.

Seven more functions have the exact target size and instruction sequence but
are not reported as exact because the incomplete owner gives compiler-local
constant or static-data symbols different identities:

| Function | Size | Remaining object evidence |
| --- | ---: | --- |
| `MasuDisplayListMake` | `0x1A8` | 20 float-pool relocation operands |
| `MasuDisplayListKaoMake` | `0x1A8` | 20 float-pool relocation operands |
| `MasuNextMain` | `0x23C` | 11 numeric-pool relocation operands |
| `mbMasuNextSet` | `0xD0` | one zero-float relocation operand |
| `mbev_MasuMasuStart` | `0x134` | one zero-float relocation operand |
| `mbev_MasuMasuEnd` | `0x128` | one zero-float relocation operand |
| `mbMasuCornerPosGet` | `0x3F4` | numeric-pool and static-table symbols |

Objdiff reports only `DIFF_ARG_MISMATCH` for those operands: there are no
inserted, deleted, or replaced instructions. The literal values and the
`cornerNoTbl` bytes match, but relocation identity remains owner-closure work;
these seven are therefore not counted among the 100% functions.

## Owner state

The fresh source object now maps 112 of 119 target functions and has 103 exact
functions, up from 91 mapped and 89 exact. Its mapped-function weighted score
is 99.763774%. Target/source `.text` are `0x5DC8`/`0x3C88`; raw section
pairing is 64.42536% because seven target functions remain absent:

- `MasuDraw` (`0xB54`)
- `MasuNextDraw` (`0x3FC`)
- `mbev_MasuMove` (`0x104`)
- `mbev_MasuCapStop` (`0x460`)
- `mbev_MasuStop` (`0x238`)
- `mbMasuPlayerColorSet` (`0x260`)
- `mbMasuPKinokoValueGet` (`0x7EC`)

Those residuals are the GX draw paths, event state machines, status-color
logic, and the large mushroom-value routine. They were not filled with
guessed code. Existing residuals `mbMasuDataRead` and `MasuFind` also remain
non-exact.

The machine-readable comparison was generated as
`build/proof_wave27_board_masu_final.json`; build outputs are not tracked.

## Gate

`board/masu.c` remains selected through the original fallback because its
whole owner is not exact. The final patch was nevertheless checked with the
complete serial build, DTK SHA manifest, and original-versus-built DOL
comparison. The result is `137 files OK` and main.dol SHA-1
`b897e6ade6b3a0cd2f9907689f38a3b19c327e70`.
