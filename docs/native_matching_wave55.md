# Native matching wave 55: Board opening/tutorial closure and M2S control flow

Wave 55 recovers the two remaining `board/opening.c` functions, six further
`board/tutorial.c` functions and their owned data, and two target-proven
`dolphin/mic/m2s.c` source corrections. No owner is promoted: all three
objects still diverge and therefore remain `NonMatching` and fallback-linked.

## Evidence and method

- DTK: `0.9.2` at `4d039140f2d2ed80572b1949b76a5ff9b3094e06`.
- Objdiff CLI: `3.7.3`, target on the left and source on the right, with
  `functionRelocDiffs=data_value`.
- Compiler: project-pinned `GC/2.6` with the configured per-owner flags.
- Primary evidence: MP6 target instructions, branches, relocations, callers,
  globals, initialized bytes, and retained function boundaries.
- Corroborating source: fully Matching Mario Party 5 commit `e246f9d` for the
  opening numerical method and tutorial process/window lifecycle. Sibling
  source was used only where MP6 independently proves the retained behavior.

No inline assembly, byte blobs, register forcing, guessed fields, or
fallback-to-Matching flip is introduced.

## `board/opening.c`

The owner now represents all 22 retained target functions. The two additions
recover the complete single-mode event rather than a placeholder:

- `mbev_OpeningSingle` owns the child process, target START-button cancel
  path, guide/window teardown, heap-array cleanup, particle/audio cleanup,
  and destructor lifecycle.
- `ev_OpeningSingle` owns the 256-entry space route, corner reduction,
  adaptive Simpson curve-length calculation, Newton iteration for
  constant-distance camera motion, five-point de Casteljau guide flight, and
  the particle/window/motion/fall sequence.
- `SINGLEGUIDEWORK` replaces anonymous guide state, while the existing typed
  route, camera, process, model, effect, and heap-array globals are consumed
  by their recovered owner.

The first coherent transcription compiled at `0x1094/0x12C8` for
`ev_OpeningSingle`. Restoring the MP6 pointer-segment lifetime and the
Matching-MP5 callback integrator shape reduced the source excess by `0x9C`.
The retained final evidence is:

| Function | Target/source | Objdiff |
| --- | ---: | ---: |
| `mbev_OpeningSingle` | `0x27C/0x280` | 99.371070% |
| `ev_OpeningSingle` | `0x1094/0x122C` | 71.556076% |

Whole target/source `.text` are `0x2B14/0x2CE4` at 87.271675%; 22 functions
pair and 15 are exact. The large residual is explicit: MWCC still inlines the
curve evaluator where the target retains an indirect `mtctr`/`bctrl` call.
That compiler/source-shape problem does not invalidate the recovered control
flow, but it prevents promotion.

## `board/tutorial.c`

Six additional target functions and their owned state are recovered:

- exact `mbTutorialCreate`, including the five flag clears, call-work reset,
  and watch/main child-process creation;
- `TutorialWatch`, including help-window positioning, exit gating, AR DMA
  drain, music/wipe shutdown, guide teardown, and exit request;
- `mbTutorialWinMesExec`, `mbTutorialWinMesMasuExec`,
  `mbTutorialWinCreate`, and `mbTutorialWinKeyWait`;
- the real `tutorialWatchProc`, `TUTORIALGUIDEDATA`, and
  `MBWIN_TYPE_GUIDE == 5` boundary.

The 72 message/guide-sound pairs plus `{-1, -1}` sentinel occupy source
`.data 0x248` and are byte-exact. Target `.data` is `0x298`; its only remaining
`0x50` is the still-unrecovered minigame Y-offset table. Function evidence:

| Function | Target/source | Objdiff |
| --- | ---: | ---: |
| `mbTutorialCreate` | `0xF0/0xF0` | 100% |
| `TutorialWatch` | `0x208/0x20C` | 98.576920% |
| `mbTutorialWinMesExec` | `0x184/0x180` | 98.659790% |
| `mbTutorialWinMesMasuExec` | `0x36C/0x354` | 95.584470% |
| `mbTutorialWinCreate` | `0xDC/0xD0` | 93.854546% |
| `mbTutorialWinKeyWait` | `0x114/0x108` | 95.492750% |

Whole target/source `.text` are `0x2C34/0x1D44`; mapped/exact coverage rises
from 40/34 to 46/35. The minigame-call state machine remains absent rather
than being filled with unproven storage or logic.

## `dolphin/mic/m2s.c`

MP6 passes `0x58` to the start-time calibration-history clear, matching the
two previously recovered `0x58` history transfers. The source now clears the
44 signed history samples, not the full `0xB0` backing array. MP6 also branches
separately for `!init || !open` and `samples < 0`; splitting the combined
source condition removes the five early control-flow differences in
`M2SAdvanceBuffer`.

`M2SAdvanceBuffer` improves from 61.227848% to 64.012660%, and whole-owner
data-value score improves from 78.733567% to 78.991780%. Target/source `.text`
remain `0xD50/0xD0C`, 1/14 functions are exact, and relocations remain
143/149. A target-inspired debug-index caching transcription grew the
function and regressed its score; it was reverted rather than retained as a
hypothesis.

## Gate

The three modified objects compile under their configured flags and the
object comparisons above are regenerated from the integrated main worktree.
No owner is flipped in this wave. The final full build/container gate is:

- `ninja -j1`: `137 files OK`
- `build/tools/dtk.exe shasum -q -c config/GP6E01/build.sha1`:
  `137 files OK`
- `cmp orig/GP6E01/sys/main.dol build/GP6E01/main.dol`: byte-identical
- progress remains 9.12% code/31.30% data overall and 46.76% code/74.41% data
  in the DOL, with 300/895 and 291/396 Matching owners respectively
