# Native matching wave 79: pause-panel lifecycle and wipe ownership

Wave 79 closes seven Config functions totaling `0x588` target text bytes and
removes a source-only `0x10` math-header contribution from Wipe. Neither owner
is promoted: both remain fallback-linked and excluded from clean-C totals.

## Pause-panel lifecycle

The MP6 target object fixes the complete `0x5C` `PAUSE_PANEL_WORK`, its 20-entry
allocation, the owner-local guide process, and all field offsets used here.
The recovered functions are:

| Function | Target/source | Result |
| --- | ---: | ---: |
| `mbPauseGuideCreate` | `0x6C/0x6C` | 100% |
| `PauseGuideDestroy` | `0x68/0x68` | 100% |
| `mbPausePanelCreate` | `0x268/0x268` | 100% |
| `mbPausePanelKill` | `0x104/0x104` | 100% |
| `mbPausePanelSizeSet` | `0x48/0x48` | 100% |
| `mbPausePanelGrowSet` | `0xB8/0xB8` | 100% |
| `mbPausePanelShrinkSet` | `0x48/0x48` | 100% |

Target instructions and relocations recover the `DATA_bpause6` model files,
the two animation-link labels, process flags, model attributes, sprite
ownership, chained position/scale initialization, and the signed model-ID
temporaries passed through `mbObjAttrSet`. The final remaining register swap in
`mbPausePanelCreate` was resolved by the declaration-list ordering documented
by `mwcc-decomp` commit `094b96ca1df4a035b5f93c351f773306c0241f3f`:
`CParser_NewLocalDataObject` prepends locals before CodeGen assigns the equal-use
nonvolatile colors. No dead local or register-forcing construct was added.

Config now has 21/51 data-value-exact functions. Target/source `.text` are
`0x63BC/0xD0C`, scoring 13.070657%. The already-Matching `board/pause.c`
consumer retains all 22 exact functions after its second argument is corrected
to the target-proven unsigned ESP data number.

## Wipe math ownership

`board/wipe.c` used the public `<math.h>` body even though the target has none
of that header's local `sqrtf` data. The same-game Matching `game/wipe.c`
authenticates the original `_MATH_H` recipe and direct `sin`/`cos`
declarations; Board additionally calls `atan2`, whose target relocation fixes
that declaration. Removing the accidental header body deletes exactly the
source-only `_half` and `_three` objects. Target/source `.sdata2` are now
`0xA8/0xA8`, rather than `0xA8/0xB8`.

All 32 previously exact Wipe functions remain exact. The only residual,
`mbWipeSpecialFadeOutCreate`, stays WIP at target/source `0xB4/0xAC`.
MP6 and the corresponding MP7 retail function both prove an extra vanished
nonvolatile live range, but neither sibling source nor MWCC's allocator
authenticates a C spelling for it. No dummy local or call was invented.

## Rejected residuals

- `board/board.c` remains unchanged. Its 29 residuals are instruction- and
  relocation-identical register allocations whose missing helper/inline
  boundaries are not authenticated by MP5.
- `board/masu.c::mbMasuPlayerColorSet` remains unchanged. Its two color
  temporaries form the already-documented register cycle; bounded lexical
  probes are either byte-neutral or regressive.
- `board/audio.c::mbMusBoardFadeOut` remains unchanged. Its four memory-local
  slots are rotated by inline-temporary allocation, and the sibling source
  does not authenticate a different allocation boundary.

## Proof

Objdiff CLI `3.7.2` used `functionRelocDiffs=data_value` for every reported
function. The final serialized `rtk ninja -j1` build reports `137 files OK`;
the independent `dtk shasum -q -c config/GP6E01/build.sha1` command reports
the same. `cmp` confirms original and rebuilt `main.dol` are byte-identical,
and the final build leaves `config/GP6E01/symbols.txt` unchanged.
