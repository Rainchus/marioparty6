# Mario Party 6 recovery status

This is an evidence snapshot, not a completion claim. It was last verified on
2026-07-16 with the pinned project toolchain.

## Recovery target

The active target is the byte-identical non-minigame game loop: boot, menus,
party mode, boards, results, and ending. Minigame DLLs, instruction DLLs,
minigame-mode wrappers, and mic-quiz modes are excluded; their native-port
black-screen stubs are outside this decompilation repository's work.

Priority 1a `mdseldll.rel` is recovered and source-linked in Wave 70.
`include/ovl_table.h` places it at zero-based overlay 93, and the Matching
`selmenuDll` mode table maps `***:MODE SEL` directly to `DLL_mdseldll`. All
113 application functions and `0x10010` application text bytes are exact C;
the recovered `fn_1_5BF0` and `fn_1_BAB4` close the former `0x29FC` residual.
The separate compiler runtime has 19/19 exact functions and `0xA44` text bytes
under the authenticated original-assembly exception. Both owners are now
configured `Matching`, and the source-linked `0x10A54` code-byte REL passes
the complete object, relocation, REL, SHA, and DOL gates. The former
boot-to-mode-select blocker is therefore closed.

The current flow priority is `mdpartydll.rel`. Wave 84 materializes 24 more
target-backed `mdparty.c` functions and `0x3A9C` represented target text bytes;
23 functions and `0x3764` bytes prove exact. The application owner now
represents 181 of 258 functions and `0xEAB4/0x3F424` target text bytes; 177
functions and `0xDF8C` bytes are exact. Combined application/stage evidence is
now 238 of 315 represented functions and `0x15E8C/0x467FC` text bytes, of which
234 functions and `0x15364` bytes are exact. `mdparty.c` remains
`C-not-yet-matched`, so the module is still outside the conservative fully
recovered REL total. The separate 19-function, `0xA44` compiler runtime
remains exact and source-linked only under the authenticated original-assembly
exception. Evidence is retained in
[`docs/native_matching_wave84.md`](docs/native_matching_wave84.md).

The evidence-backed target ledger at this build is:

- DOL `game/` plus `board/`: `696000 / 1431136` target `.text` bytes Matching
  (48.63%), across 70/97 owners. `game/` is 57/57 and `board/` is 13/40;
  Branch is the latest Board promotion.
- required board/flow RELs: `73264 / ~3100000` target `.text` bytes in fully
  source-linked modules (approximately 2.36%). The conservative done set is
  `actmanDLL` (`2412`), `sequencedll` (`2672`), and `mdseldll` (`68180`:
  `65552` exact C application bytes plus `2628` authenticated compiler-runtime
  assembly bytes). `bootDll`, `selmenuDll`, and `fileseldll` are not counted
  because each still links a fallback runtime owner. No auto-generated or
  fallback-linked text owner is counted as recovered source.

## Verification

- `rtk ninja`: `137 files OK` (the DOL and all configured REL outputs)
- `build/tools/dtk.exe shasum -q -c config/GP6E01/build.sha1`:
  `137 files OK`
- `cmp orig/GP6E01/sys/main.dol build/GP6E01/main.dol`: byte-identical
- `build/GP6E01/main.dol` SHA-1:
  `b897e6ade6b3a0cd2f9907689f38a3b19c327e70`
- `cmp orig/GP6E01/files/dll/mdseldll.rel build/GP6E01/mdseldll/mdseldll.rel`:
  byte-identical
- `build/GP6E01/mdseldll/mdseldll.rel` SHA-1:
  `e6c20b24cfca8135ed8c49ce59108100277444c0`
- `cmp orig/GP6E01/files/dll/mdpartydll.rel build/GP6E01/mdpartydll/mdpartydll.rel`:
  byte-identical
- `build/GP6E01/mdpartydll/mdpartydll.rel` SHA-1:
  `519debb149ef42eda1ab3b0a4d2b3132b4f3e3cc`
- DTK progress at that build: 10.04% code and 31.85% data overall; 47.42% code
  and 74.59% data in the DOL; 2.12% code and 4.77% data in REL modules
- Matching owners at that build: 307 of 900 overall, 294 of 396 in the DOL,
  and 13 of 504 in the REL modules
- DOL policy split: 255 matching owners without the assembly exception, 39
  matching owners admitted under the source-authentication exception, 101
  `C-not-yet-matched` fallback owners, and 1 `original-was-asm` fallback owner
  awaiting target proof. These four numbers total all 396 DOL owners.

The current build also corrects the Game Speech SDK library from the global
GC/2.6 default to `GC/1.2.5n` and disables floating-point contraction for the
library. GC/2.6 produced systematic prologue and indirect-call instruction
divergence, while the numeric target objects use separate multiply/add
instructions where the default compiler contracts them. The corrected flags
reproduce the retained code and enabled nine clean-C owner promotions:
`gsapi/sid/sid.c`, `gsapi/extaudio.c`, `gsapi/mathusage.c`,
`common/csspi/csspi.c`, `common/osspi/osspi.c`, and
`common/safeh/safeh.c`, plus `gsapi/wrddata.c`,
`asrpho/common/fastallo/fastallo.c`, and
`asrpho/common/blocks/flblocks/spline.c`. This decision is based on emitted
bytes and the final container proof, not synthesized `.comment` metadata.

`gssdk_lib/common/rsrc/rsrc.c` is now an additional Matching clean-C owner.
Its callers prove that `SafeHandle` is an eight-byte aggregate passed by
value; correcting that shared ABI preserves the already-exact `safeh.c` and
allows the real `0x14` `Resource` layout, null resource, open/check/close
logic, all three retained functions, and all effective relocations to match.
The old `0x18` `.rodata` split incorrectly attributed a four-byte alignment
gap to the resource object; the proven semantic owner ends at `0x8021A4B4`.

Two additional Game Speech SDK owners are now Matching clean C.
`asrpho/common/blocks/flfxblks/lkahead.c` recovers the complete `0x4C`
look-ahead block, queue/flush behavior, history ring, and all four functions;
its `.text 0x2AC` and `.sdata2 0x8` match exactly. `asrpho/rec1600/initial.c`
recovers 45 typed profile key/value tables plus the typed `IniFile`,
`InitialProcs`, `QueueWriters`, and `QueueReaders` tables. Its `.rodata 0x9C8`
and all 102 relocations match exactly. MP7 binary parity was used only to
cross-check the table boundaries; the MP6 target symbols, bytes,
relocations, and consumers establish the retained definitions.
The ABI and per-owner evidence is retained in
[`docs/native_matching_wave34.md`](docs/native_matching_wave34.md).

The next easy-owner batch adds two more Matching clean-C GSSDK owners.
`asrpho/common/blocks/stacker.c` recovers the real `0x38` block, conditional
input copies, five control commands, profile-sized allocation, and all four
functions; its `.text 0x384` and all 19 relocations match exactly.
`asrpho/rec1600/ctrl.c` recovers the typed command headers, target pairs,
state records, callback pointers, architecture tables, and global
`ControlTable`; `.rodata 0x1000`, `.sdata2 0x28`, and all 390 relocations match
exactly. Detailed evidence for these promotions and the retained source
candidates is in [`docs/native_matching_wave35.md`](docs/native_matching_wave35.md).

Wave 36 moves five further owners from `NO-SOURCE` to compiled, typed C:
MSL `qsort.c` and GSSDK `exev_dp.c`, `dctlift.c`, `logexp.c`, and `vq1500.c`.
Together they recover 22 functions and the target-proven code-book, dynamic-
programming, DCT-lift, logarithm-table, and language-method layouts. Eight
`vq1500` functions, both small `exev_dp` wrappers, both `dctlift` lifecycle
functions, and the `logexp` lookup helper are byte-exact. At the Wave 36 gate,
every owner remained fallback-linked because its complete C object still
diverged; Wave 49 later closes `logexp.c`. The
same batch proves two more `board/audio.c` functions and replaces
`extaudio.c`'s truncated private `gGSAPI` declaration with the exact shared
header without changing its Matching object. Evidence is retained in
[`docs/native_matching_wave36.md`](docs/native_matching_wave36.md).

Wave 37 batches five more source recoveries without weakening the fallback
boundary: GSSDK `mtx.c`, `combiner.c`, and `pitchwin.c`, plus the authenticated
22-function `board/math.c` and eight-function `board/telop.c` clusters. The
batch materializes 51 functions and real matrix, signal-history, pitch-window,
taunt, language, and board-directory layouts. Thirteen newly materialized
functions are byte-exact, but no complete owner is exact, so all five remain
fallback-linked and `NonMatching`. Evidence is retained in
[`docs/native_matching_wave37.md`](docs/native_matching_wave37.md).

Wave 38 uses a recovery-first cadence: one source harvest, one batched object
comparison, and one full gate. It materializes 112 board functions across
`math.c`, `telop.c`, `coin.c`, `dice.c`, `single.c`, `star.c`, `tutorial.c`,
and `wipe.c`, plus all 30 retained MetroTRK `targimpl.c` functions. Fifty-four
board functions and all 30 retained MetroTRK functions are byte-exact. The
board owners remain fallback-linked because each owner is incomplete.
`targimpl.c` is Matching under the authenticated MP5 sibling exception after
restoring the real support prototypes; its seven inline-assembly routines are
reported separately from clean C. Evidence is retained in
[`docs/native_matching_wave38.md`](docs/native_matching_wave38.md).

Wave 39 keeps that batched cadence and materializes 87 source functions across
eight formerly `NO-SOURCE` board owners: `capselect.c`, `capsule.c`,
`config.c`, `last5.c`, `opening.c`, `scroll.c`, `shopevent.c`, and `snpc.c`.
Eighty-six functions pair with retained target functions and 63 are byte-exact,
covering `0x1A5C` target text bytes. The source recovers real capsule-color,
Last-5 coin, shop-call, pause-copy, opening-camera, scroll/map, and special-NPC
state instead of opaque work buffers. All eight owners remain fallback-linked
because their event/state-machine closures are incomplete. Evidence is retained
in [`docs/native_matching_wave39.md`](docs/native_matching_wave39.md).

Wave 40 closes the remaining board `NO-SOURCE` inventory with 58 source
functions across `capmove.c`, `capthrow.c`, `captrap.c`, `capspecial.c`,
`capevent.c`, and `mgcall.c`. All 58 functions pair with retained targets and
50 are byte-exact, covering `0x494` target text bytes. The batch recovers the
real `0x28` Teresa fade work, `0x10` minigame-list work, exact minigame history
arrays/tables, capsule-event globals, and 27 target-proven one-instruction kill
callbacks. All six owners remain fallback-linked because their large event
closures are incomplete. Evidence is retained in
[`docs/native_matching_wave40.md`](docs/native_matching_wave40.md).

Wave 41 materializes 11,033 lines and 461 compiled functions across thirteen
formerly `NO-SOURCE` MusyX runtime owners. The same-version MP5 donor and its
byte-identical MusyX headers supply source shape, while Matching MP4 owners at
the pinned upstream MusyX commit authenticate the common source family and
`StdReverb` assembly bodies. The MP6 object pass pairs 212 functions; 140 are
byte-exact and cover `0xB940` target text bytes. The false empty donor body for
MP6's real `0x255C` `salBuildCommandList` is omitted, and `dsp_import.c` stays
absent because its donor is a DSP firmware byte array rather than clean C.
All thirteen materialized owners remained fallback-linked at the wave-41
boundary. Evidence is retained in
[`docs/native_matching_wave41.md`](docs/native_matching_wave41.md).

Wave 42 closes the same-version MusyX sequence owner. Five uniquely sized and
ordered address-named functions plus ten sequence-state roots and the shared
`synthTrackVolume` bank now carry their source-authentic names. All 17 retained
`seq.c` functions pair; 15 are directly objdiff-exact, while `seqHandle` and
`HandleTrackEvents` differ only in compiler-local names for the same five
constant-pool relocations. Donor-only helpers/constants are linker-stripped,
and the exact `0x3FCC` linked owner passes the full DOL/container gate as clean
C. The same audit recovers 15 further function names and 44 further globals
across `synth.c`, `stream.c`, `synthdata.c`, `synthvoice.c`, and `s_data.c`;
those five owners remain `NonMatching` wherever source still diverges. Evidence
is retained in [`docs/native_matching_wave42.md`](docs/native_matching_wave42.md).

Wave 43 moves six Game Speech SDK signal-processing owners from `NO-SOURCE`
to compiled, target-derived C: `fft_maye.c`, `fftmod.c`, `window.c`,
`slidhist.c`, `subsamp.c`, and `voicing.c`. The batch recovers all 32 retained
functions, the real `0x4C` FFT and `0x50` window block layouts, and the complete
`0x19C` `TriggerLR` layout shared by the histogram, voicing, and subsampler
stages. Ten functions are byte-exact. No complete owner is exact, so all six
remain `NonMatching` and fallback-linked. The target's 23-double impulse
response is now assigned to its only semantic consumer, `subsamp.c`, rather
than `voicing.c`. Evidence is retained in
[`docs/native_matching_wave43.md`](docs/native_matching_wave43.md).

Wave 44 recovers the complete GSSDK `smoother.c` and `mel.c` owners from MP6
target evidence. All eight retained functions compile with typed state;
`smtConstruct`, `smtDestruct`, `InitMel`, and `ConstructMel` are byte-exact.
The remaining algorithm and control functions still diverge, so both owners
remain `NonMatching` and fallback-linked. Evidence is retained in
[`docs/native_matching_wave44.md`](docs/native_matching_wave44.md).

Wave 45 recovers the complete five-function GSSDK `trigglr.c` owner against
the already-proven `0x19C` `TriggerLR` layout. The source now expresses the
speech-trigger state machine, queue behavior, session transfer, sensitivity
profile, and lifecycle without assembly or opaque storage. `ConstructTriggerLR`
is byte-exact; the other four functions and whole object still diverge, so the
owner remains `NonMatching` and fallback-linked. Evidence is retained in
[`docs/native_matching_wave45.md`](docs/native_matching_wave45.md).

Wave 46 recovers the complete eight-function GSSDK `acne.c` owner. Its real
`0x98` layout, single allocation backing three band arrays, exact
`12*bandCount + 40` session payload, 21-band tables, queue lifecycle, smoothing,
and adaptation state now compile as typed C. `Reset` and `ConstructAcne` are
byte-exact. The complete object still diverges, so the owner remains
`NonMatching` and fallback-linked. Evidence is retained in
[`docs/native_matching_wave46.md`](docs/native_matching_wave46.md).

Wave 47 recovers the complete eight-function GSSDK `specsub.c` owner. The real
`0x98` layout, one arena backing the history ring and five spectrum vectors,
endpoint allocation, dynamic session payload, four-output queue behavior, and
full spectral-subtraction algorithm now compile as typed C. `ConstructSpecSub`
is byte-exact. The whole object remains structurally divergent and therefore
stays `NonMatching` and fallback-linked. Evidence is retained in
[`docs/native_matching_wave47.md`](docs/native_matching_wave47.md).

Wave 48 recovers the complete 16-function GSSDK `mqueue.c` owner and the
complete 25-function `tinyos.c` owner as one shared-runtime slice. The target-
derived source closes the real `0x118` `FastAllocator`, `0x16C` `TosContext`,
`0x28` `TosBaseBlock`, queue/reader, architecture-definition, profile, and
control-table layouts used throughout GSSDK. `qQueueInit`, `qQueueNbrElements`,
`tosDestruct`, and `DisabledProcess` are byte-exact, and the already-Matching
`fastallo.c` remains exact after moving its ABI into a shared header. Both new
complete owners still diverge as whole objects, so they remain `NonMatching`
and fallback-linked. Evidence is retained in
[`docs/native_matching_wave48.md`](docs/native_matching_wave48.md).

Wave 49 recovers all 12 target functions and the real `0x94` owner layout for
GSSDK `gender.c`, including the code-book, session, scoring, ring-buffer,
control, and lifecycle paths. `LoadCodeBook` and `ConstructGender` are exact;
the complete owner remains fallback-linked while helper visibility/inlining
and larger compiler shapes diverge. The same batch recovers the original
`difference / 2.0` expression shape in `logexp.c`, making both functions and
the linked owner byte-identical, and improves `vq1500.c` by separating the two
vector-search lifetimes without forcing registers. Evidence is retained in
[`docs/native_matching_wave49.md`](docs/native_matching_wave49.md).

Wave 50 moves five more GSSDK owners from `NO-SOURCE` to complete,
target-derived C: `dpscruw.c`, `dpgenuw.c`, `pitchdp.c`, `pitchco.c`, and
`shs_vuv.c`. The batch recovers all 58 retained functions and the real
`0x80`, `0xF4`, `0x68`, `0xC4`, and `0x60` owner layouts, including typed
user-word lattices/backtraces, pitch Viterbi state, pitch-combiner history,
and SHS/VUV spline and harmonic state. Twenty-one functions are byte-exact,
but no complete owner is exact, so all five remain `NonMatching` and
fallback-linked. The same evidence expands the shared `LanguageData` method
table with target-proven slots used by the user-word scorers. Evidence is
retained in [`docs/native_matching_wave50.md`](docs/native_matching_wave50.md).

Wave 51 restores four authenticated Dolphin SDK owners and advances two real C
owners. The paired-single FIFO helpers in `GXTransform.c`, the memory-mode and
real-mode routines in `OSMemory.c`, `Reset` in `OSReset.c`, and the startup
routines in `__start.c` come from fully matching Mario Party 4 source and pass
the MP6 object, retained-relocation, linked-range, and full-container gates.
They add four Matching exception owners, not clean-C owners. The same batch
restores the authenticated low-level routines in `OS.c`; 12/14 functions are
now exact, but both remaining C functions diverge, so that mixed owner stays
fallback-linked. `board/board.c` now carries the target-CFG and fully matching
MP5 turn-loop/return shape in `mbMain`, while the complete owner remains
`NonMatching`. Evidence is retained in
[`docs/native_matching_wave51.md`](docs/native_matching_wave51.md).

Wave 52 admits `dolphin/os/OS.c` under the sibling-authentication exception
after proving its effective linked owner. Fully matching M4 authenticates the
low-level assembly family; 12/14 retained functions are raw-object exact, and
the two C residuals differ only in compiler-local ownership of the same
`.data+0` relocation (`@1` versus `OS.c ...data.0`). Their instructions,
addends, and data order agree, `.data` agrees through `0x1F6` with one target
tail byte, `.sdata 0x10` is byte-identical, and the linked range plus full
container pass. The owner is Matching but remains excluded from clean-C
counts. The same batch recovers the target `mbMain` cycle joins in
`board/board.c`; it improves from 99.133330% to 99.541940% while the owner
remains fallback-linked. Evidence is retained in
[`docs/native_matching_wave52.md`](docs/native_matching_wave52.md).

Wave 53 admits `dolphin/os/OSExec.c` under the authenticated-vendor form of
the assembly exception. The 100%-complete dolsdk2004 source authenticates the
target-identical `Run` body; restoring it makes all eight retained functions
and their effective relocations exact, while the linked gate proves that
source-only helpers and split padding do not enter the MP6 owner. The same
batch improves `EXIBios.c` and recovers 39 target-backed Board utility
functions across seven owners, 28 of them byte-exact, with no prior mapped
function regressing. Those incomplete owners remain fallback-linked. Evidence
is retained in
[`docs/native_matching_wave53.md`](docs/native_matching_wave53.md).

Wave 54 recovers two substantial Board slices and two target-proven MIC data
semantics without changing the fallback boundary. `board/tutorial.c` now
pairs 40 functions, 34 exactly, up from 10/10; the new source recovers the
three typed ID registries, the `0x14` call state, the `0x80` window-timing
bank, camera/sprite/model helpers, and initialization/cleanup paths.
`board/opening.c` now pairs 20 of 22 functions, 15 exactly, up from 14/12; its
party event, coin/drop/order helpers, single-event destructor and particle
hook, typed globals, `.rodata 0x38`, `.data 0x1C0`, and `.sbss 0x38` are
materialized. The coupled single wrapper and `0x1094` single event remain
omitted rather than approximated. `dolphin/mic/mic.c` restores signed ring
sizes and arithmetic sample-index shifts proved by the MP6 instructions;
`m2s.c` restores both target `0x58` calibration-history copies. All four
owners remain `NonMatching` and fallback-linked. A GC/2.6 EXIBios trial was
also rejected after expanding source `.text` from the current `0x2100` to
`0x3BA0` with no new exact function. Evidence is retained in
[`docs/native_matching_wave54.md`](docs/native_matching_wave54.md).

Wave 55 completes source representation of the two remaining Board opening
functions, expands the tutorial lifecycle/window closure, and corrects two
further M2S source semantics. `board/opening.c` now pairs all 22 target
functions: the recovered single-mode owner includes its process cleanup,
256-entry route, adaptive Simpson curve lengths, Newton distance traversal,
guide flight, and particle/window/motion sequence. `board/tutorial.c` now
pairs 46 functions, 35 exactly; `mbTutorialCreate` and the typed `0x248`
message/guide-sound table are byte-exact, while five new window/watch bodies
retain close compiler-shape differences. `dolphin/mic/m2s.c` now initializes
only the target-proven `0x58` calibration history and preserves the target's
separate negative-sample guard. No owner is flipped: each complete object
still diverges and remains fallback-linked. Evidence is retained in
[`docs/native_matching_wave55.md`](docs/native_matching_wave55.md).

Wave 56 promotes `dolphin/exi/EXIBios.c` from clean C after the Matching MKDD
owner authenticates its scheduling pragmas. All 23 retained MP6 functions are
strict-exact; source-only helpers and data are linker-stripped, and the full
linked gate proves the retained owner. The same batch materially expands three
Board owners: `player.c` reaches 86 mapped/66 strict-exact functions with a
real `0x50` `MBPLAYERWORK` and exact `.bss 0x140`; `star.c` reaches 67/55 with
exact `.bss 0x1F48`; and `scroll.c` reaches 15/11 with exact `.sbss 0x28`.
`dolphin/mic/m2s.c` recovers its target BSS order, compound narrow shifts, and
debug-buffer copy shape, improving its data-value score to 79.922540%. The
three Board owners and M2S remain fallback-linked because their complete
objects still diverge. Evidence is retained in
[`docs/native_matching_wave56.md`](docs/native_matching_wave56.md).

Wave 57 completes source representation of all 56 `board/math.c` target
functions, including the real HSF culling hook, bounding-box/frustum paths,
object-init closure, random scaler, and exact two-bank `.bss 0xC0` owner.
Math's data-value score rises from 35.699078% to 47.239555%. The same easy-port
batch adds six Matching-MP5-authenticated Player functions and two Coin display
teardown functions. Player reaches 92 mapped/68 strict-exact functions and
Coin reaches 24/5; prior represented functions do not regress. No owner is
flipped because all three complete objects still diverge. Rejected M2S
scheduling, MIC branch-spelling, and Audio compiler-temporary probes are also
measured so they are not recycled as hypotheses. Evidence is retained in
[`docs/native_matching_wave57.md`](docs/native_matching_wave57.md).

Wave 58 completes source representation of all 119 `board/masu.c` target
functions by recovering its two GX draw paths, three space/event transitions,
player-color owner, and bounded PKinoko route search. Masu reaches 103/119
strict-exact and 110/119 data-value-exact functions, with exact `.rodata`,
`.bss`, and `.sdata` section sizes. `board/wipe.c` gains six special-wipe
lifecycle/state functions plus its exact `0x38` BSS and target-backed resource
tables. Direct control-block indexing makes `M2SSetBuffer` and `M2SSetMode`
byte-exact, raising `dolphin/mic/m2s.c` to 3/14 strict-exact functions. No
owner is flipped because all three complete objects still diverge; renderer
closure in Wipe and compiler-shape closure in Masu/M2S remain real C work.
Evidence is retained in
[`docs/native_matching_wave58.md`](docs/native_matching_wave58.md).

Wave 59 recovers 47 more Board functions: 31 turn/movement/collision/effect
functions in `board/player.c`, six special-wipe dispatch/update functions in
`board/wipe.c`, and ten angle/Hermite/Bezier helpers in `board/capevent.c`.
Player reaches 123 mapped/97 strict-exact functions; Wipe reaches 29/16, with
all six additions data-value exact; and Capevent reaches 38/28, with all 38
functions data-value exact. A measured `M2SGetSamples` lifetime correction
raises M2S to 79.992960% strict and 80.004690% data-value. All four owners
remain fallback-linked `C-not-yet-matched` work because their complete objects
still diverge. Evidence is retained in
[`docs/native_matching_wave59.md`](docs/native_matching_wave59.md).
The final serialized build and explicit DTK checksum both report `137 files
OK`; the rebuilt and original `main.dol` compare byte-identical, and the final
build leaves `config/GP6E01/symbols.txt` unchanged.

Wave 60 batches the remaining easy, instruction-proven ports in five active
owners. `board/capevent.c` gains 45 strict-exact functions and reaches 83
mapped/73 strict-exact/83 data-value-exact functions. `board/wipe.c` now
represents all 33 target functions after recovering its paper setup and three
GX draw bodies; the four additions are data-value exact. `board/player.c`
reaches 129 mapped/105 strict-exact/116 data-value-exact functions and restores
the sibling-authenticated `GWPlayerGet` source lifetime. The MIC compiler
matrix proves `sdk_mic` was built with GC/1.2.5n: `dolphin/mic/mic.c` rises
from 0 to 20 strict-exact functions and `m2s.c` rises from 3 to 13 of 14.
None of the five owners is promoted; each still diverges and remains
fallback-linked `C-not-yet-matched` work. Evidence is retained in
[`docs/native_matching_wave60.md`](docs/native_matching_wave60.md). The final
serialized build and explicit DTK checksum both report `137 files OK`; the
rebuilt and original `main.dol` compare byte-identical, and the final build
leaves `config/GP6E01/symbols.txt` unchanged.

Wave 61 promotes two clean-C DOL owners. `board/branch.c` is now 17/17 exact
with `.text 0x10E0`, all 210 text relocations, and every semantic data section
exact after assigning the six-byte `.data` and four-byte `.sbss` tails to
linker alignment. The authentic `if (--choiceTime == 0)` spelling recovers the
last `choice`/`choiceTime` register cycle. `dolphin/mic/m2s.c` is now 14/14
exact with `.text 0xD50` and all 143 relocations exact; restoring indexed
debug-buffer copying and placing `__M2SCalibration` after the public functions
recovers the target translation-unit order. Its target-only four-byte BSS tail
is the proven eight-byte alignment gap before `msm/msmsys.o`, not fake source
storage. `dolphin/mic/mic.c` remains fallback-linked but rises from 20/41 to
36/41 strict-exact functions and 98.147860% `.text` after recovering the real
two-dimensional timeout table, signed timeout field, early-return MIC/EXI
control flow, raw transactions, and callback lifetimes. The two near-exact
async residuals were not padded: MWCC `StackFrame.c` proves an unused local is
removed before final stack assignment, and no authenticated live aggregate
explains the target's four-byte slot displacement. Evidence is retained in
[`docs/native_matching_wave61.md`](docs/native_matching_wave61.md). The final
serialized build and explicit DTK checksum both report `137 files OK`; the
rebuilt and original `main.dol` compare byte-identical, and the final build
leaves `config/GP6E01/symbols.txt` unchanged.

Wave 62 advances the engine-core `board/masu.c` owner from 103/119 to 115/119
strict-exact functions. Restoring the target translation-unit data order,
the source-authentic math-header suppression, MP5-backed draw declaration
order, event switch shapes, typed capsule interfaces, and real party/team
boolean lifetimes closes twelve functions without padding or instruction
forcing. Target/source `.text` are now `0x5DC8/0x5DB0` at 99.614460% in both
strict and data-value comparison; `.data 0x128` and `.sdata2 0x90` are exact.
`MasuDraw`, `MasuFind`, `mbMasuPlayerColorSet`, and
`mbMasuPKinokoValueGet` remain divergent, so `masu.c` stays `NonMatching` and
does not change the clean-C target ledger. Evidence is retained in
[`docs/native_matching_wave62.md`](docs/native_matching_wave62.md). This was
an object-only WIP slice; no full-DOL gate or Matching promotion is claimed.

Wave 63 advances the engine-core `board/player.c` owner from 129 to 150 of 165
target functions represented, from 105 to 117 strict-exact functions, and
from 116 to 130 data-value-exact functions. Target/source `.text` grow from
`0xBEB0/0x484C` to `0xBEB0/0x80CC`, reaching 66.051620% strict and
66.114300% data-value. The recovered source covers normal/jump/climb movement,
rotation, walk-number display, all nine missing collision-engine owners,
player swap, position reset, and target-proven Player work fields. The Coin
motion-number ABI is corrected from signed byte to `int`, closing
`mbCoinObjMotSet` and raising `board/coin.c` to 6/24 exact functions. Fifteen
Player targets and the complete Coin renderer/effect closure remain, so both
owners stay `NonMatching` and do not change the clean-C target ledger. Evidence
is retained in [`docs/native_matching_wave63.md`](docs/native_matching_wave63.md).
This was an object-only WIP slice; no full-DOL gate or Matching promotion is
claimed.

Wave 64 advances the source-complete `board/board.c` main loop to exact owner
size and 99.902960% strict/data-value `.text` similarity. All 35 target
functions pair and 31 are exact. `mbMain` rises from `0x744/0x740` at
99.541940% to `0x744/0x744` at 99.935486% after recovering the separate
current-time/night lifetimes, function-scope minigame-call result, short-lived
day predicate, and explicit party comparisons. The remaining four functions
are exact-size and differ only in 34 register-color operands, so the owner
stays `NonMatching` and does not change the clean-C target ledger. Evidence is
retained in [`docs/native_matching_wave64.md`](docs/native_matching_wave64.md).
This was an object-only WIP slice; no full-DOL gate or Matching promotion is
claimed.

Wave 65 advances the engine-core `board/dice.c` owner from 15 to 43 of 68
target functions represented and from one to 35 data-value-exact functions.
Source `.text` grows from `0x254` to `0x1A5C`; target/source `.text` is now
`0x6390/0x1A5C` at 25.429379%, with `0x1328` target text bytes exact. The wave
recovers the real Dice, fade, number, SNpc, effect, and weighted-vertex work
layouts, the retained tables/globals, the complete number flight/shrink/mesh
deformation path, and the file-wide `-O4,p -schedule off -opt nopeephole`
compiler shape. The main process/object/event/effect closures remain, so
`dice.c` stays `NonMatching` and does not change the clean-C target ledger.
Evidence is retained in
[`docs/native_matching_wave65.md`](docs/native_matching_wave65.md). This was
an object-only WIP slice; no full-DOL gate or Matching promotion is claimed.

Wave 66 advances `board/dice.c` from 43 to all 68 target functions represented
and from 35 to 47 data-value-exact functions. Source `.text` grows from
`0x1A5C` to `0x603C`; target/source `.text` is now `0x6390/0x603C` at
92.468925%, with `0x337C` target text bytes exact. The slice recovers the main
process/destructor, help setup, complete dice-object state machine, input/hit
paths, number reset, equal-roll/inbound effects, and the complete SNpc number
path. Twenty-one compiler/source-shape residuals remain, so `dice.c` stays
`NonMatching` and does not change the clean-C target ledger. Evidence is retained in
[`docs/native_matching_wave66.md`](docs/native_matching_wave66.md). This was
an object-only WIP slice; no full-DOL gate or Matching promotion is claimed.

Wave 67 opens the Priority 1a `mdseldll.rel` owner and separates its application
range from the compiler runtime. The first slice represents 16 target
functions, of which 15 are data-value-exact for `0x1660` target text bytes.
Target/source `.text` is `0x10010/0x17BC` at 9.266537%; the sole represented
residual, `fn_1_2080`, has exact `0x15C` size and differs in two register-color
operands. The exact closure includes both module entry points, `ObjectSetup`,
the initial overlay and child-process transitions, window-message helpers, and
the first mode-flow state handlers. The source also recovers the retained
camera offsets, menu permutation, report strings, window IDs, stream handle,
and camera vectors from target data and relocation evidence. A full `rtk ninja`
after the final split/configuration changes reports `137 files OK`; both owners
remain `NonMatching`, no REL promotion is claimed, and no fallback-linked byte
is counted as recovered source. Evidence is retained in
[`docs/native_matching_wave67.md`](docs/native_matching_wave67.md).

Wave 68 batches the Priority 1a mode-select easy wins without weakening the
fallback boundary. It advances `REL/mdseldll/mdsel.c` from 16 represented / 15
exact functions to 92 represented / 92 data-value-exact functions, and from
`0x1660` to `0x6CA0` exact target text bytes. Target/source `.text` is now
`0x10010/0x6CA0` at 42.421284%; `.data` is `0x370/0x370` and 100%, while the
source BSS is `0x1A4C` against target `0x1A4E` at 99.94058%. The slice recovers
the scalar/vector interpolation family, model-facing camera transform, window
callback and helpers, object/effect setup and teardown, positional audio
records, Bezier setup twins, sprite placement, particle lifecycle, and the
mode-choice handler. Target instructions, relocations, same-game ABI consumers,
MP5/MP4 sibling shapes, and the pinned MWCC inline implementation establish the
retained source forms. All 92 paired functions pass objdiff with
`functionRelocDiffs=data_value`; the 21 unrepresented functions remain real C
work. This is an object-only WIP slice: both REL owners remain `NonMatching`, no
full REL/DOL gate or promotion is claimed, and no fallback-linked byte is
counted as recovered source. Evidence is retained in
[`docs/native_matching_wave68.md`](docs/native_matching_wave68.md).

Wave 69 advances the same Priority 1a owner from 92 to 111 represented and
data-value-exact functions, adding `0x6974` exact target text bytes. The exact
total is now `0xD614/0x10010` at 83.60386%; the only absent functions are
`fn_1_5BF0` (`0x2B4`) and the monolithic `fn_1_BAB4` (`0x2748`). The slice
recovers the paired model-motion state machines, the `0x88` twelve-record
model/effect owner, mode-input and audio tables, the complete object
initialization graph, and the typed microphone response callback. The callback
layout and signed result vector are established by target loads and the
same-game `game/mic.c` producer; the recovered structure parameter reproduces
all 442 target instructions and their register coloring. All 111 represented
functions pass objdiff with `functionRelocDiffs=data_value`. The two unresolved
functions remain absent, both REL owners remain `NonMatching`, and no full
REL/DOL promotion gate is claimed. Evidence is retained in
[`docs/native_matching_wave69.md`](docs/native_matching_wave69.md).

Wave 70 closes Priority 1a. `fn_1_BAB4` recovers the complete `0x2748`
mode-select driver, and `fn_1_5BF0` recovers the `0x2B4` positional-audio
updater. Reordering definitions by target symbol address restores the original
translation-unit emission order without changing any function body: all
113/113 functions, their addresses and sizes, `0x10010` text bytes, `.data`,
and 5,029 text relocations pass the data-value gate. The apparent two-byte
object boundary (`.rodata 0x260/0x25E`, `.bss 0x1A4E/0x1A50`) resolves through
normal linker alignment and trimming; no padding datum or artificial owner was
added.

The separated runtime is admitted only under the original-assembly sibling
exception. ProjectPiki/Pikmin `24378f0e`,
`src/Runtime/PPCEABI/H/runtime.c`, is configured Matching and authenticates the
same `asm`/`nofralloc` 19-function source shape; MP5 `e246f9d`,
`src/Runtime.PPCEABI.H/runtime.c`, corroborates the compiler-runtime family.
The MP6 `mdseldll`, `fileseldll`, and `selmenuDll` target runtime text and
rodata ranges are byte-identical. The MP6 source object proves 19/19 functions,
`0xA44` text bytes, `0x18` rodata bytes, and both relocations exact. Both
`mdseldll` owners are Matching after the full `137 files OK`, byte-identical
REL, and byte-identical DOL gate. Runtime assembly remains separately reported
and is not counted as clean decompiled C. Full evidence is retained in
[`docs/native_matching_wave70.md`](docs/native_matching_wave70.md).

Wave 71 batches eight newly data-value-exact Board functions totaling `0x938`
target text bytes without promoting an incomplete owner. `board/coin.c` gains
the exact close/effect lifecycle and rises from 24/52 represented and 6 exact
functions to 27/52 and 9; source `.text` grows from `0x103C` to `0x1294`, at
20.446123%. The target-proven object-motion ABI and particle-result lifetime
close `DiceObjCreate`, `DiceInEffCreate`, and `DiceInDotEffCreate`, raising
`board/dice.c` from 47 to 50 data-value-exact functions and to 92.523540%
across its complete 68-function source representation. Exact
`BiriQEffect2Hook` plus the real particle-owner union and Biri-Q work fields
raise `board/player.c` to 151/165 represented and 131 data-value-exact
functions, at 66.687890%.

The same particle-owner field unlocks five formerly absent `board/star.c`
functions totaling `0xCF4` target text bytes: the object constructor, complete
five-mode update, rotation, particle effect, and material hook. `StarObjRotate`
is exact; Star rises from 67/90 represented and 60 exact functions to 72/90
and 61, with source `.text 0x2254` at 35.109547%. `board/object.c` remains
80/80 and byte-exact after correcting the public motion-ID return ABI. Coin,
Dice, Player, and Star remain `NonMatching` and fallback-linked, so this wave
does not change the clean-C owner ledger. Evidence and bounded rejected probes
are retained in
[`docs/native_matching_wave71.md`](docs/native_matching_wave71.md). The final
serialized build and explicit DTK checksum both report `137 files OK`; the
rebuilt and original `main.dol` compare byte-identical, and the final build
leaves `config/GP6E01/symbols.txt` unchanged.

Wave 72 represents another `0x1D04` target text bytes in recovered C across
Coin, Player, and Star and closes eight functions totaling `0x6B4` target
text bytes under the required data-value comparison. Seven Coin lifecycle,
renderer, and effect functions raise `board/coin.c` to 34/52 represented and
9 exact functions, with target/source `.text 0x4550/0x2204` at 41.045536%.
Exact `mbPlayerBiriQSet` and the recovered Biri-Q effect constructor raise
`board/player.c` to 153/165 represented and 132 exact functions, with
`.text 0xBEB0/0x8578` at 68.561040%. Five Star/Ztar flow functions raise
`board/star.c` to 77/90 represented and 62 exact functions, with
`.text 0x5C00/0x2C14` at 45.017155%; `ZtarObjRotate` is exact.

Six exact special-wipe lifecycle functions raise `board/wipe.c` to 31/33
exact functions and 99.679690% across target/source `.text 0x3664/0x3650`.
Only `mbWipeSpecialInit` and `mbWipeSpecialFadeOutCreate` remain divergent;
a target-led Init lifetime probe regressed and was fully reverted. All four
owners remain `NonMatching`, so the clean-C owner ledger is unchanged.
Evidence is retained in
[`docs/native_matching_wave72.md`](docs/native_matching_wave72.md). The final
serialized build and explicit DTK checksum both report `137 files OK`; the
rebuilt and original `main.dol` compare byte-identical, and the final build
leaves `config/GP6E01/symbols.txt` unchanged.

Wave 73 represents `0x484C` additional target text bytes in recovered C across
26 formerly absent Coin, Player, and Star functions. Seventeen functions
totaling `0x1178` target text bytes become exact under the required data-value
comparison. The recovered families establish the real `0x1C` Coin display
model, Player Metal/Biri-Q work and particle vertex ownership, and exact
`0x60` Star display work plus its number/sign tables. Coin rises to 44/52
paired and 16 exact functions at 63.219566%; Player rises to 160/165 and 133
at 81.179940%; Star rises to 86/90 and 64 at 69.696500%.

The same batch closes both represented CapSelect residues, the Config story
predicate, the Last5 stable-order helper, and all three Shop wrapper residues.
The Last5 40-coin event additionally recovers the target's floating-point
delay calculation and improves from 89.601070% to 98.781910%. Every owner
remains `NonMatching`, so the clean-C owner ledger is unchanged. Evidence,
compiler-oracle findings, and bounded rejected probes are retained in
[`docs/native_matching_wave73.md`](docs/native_matching_wave73.md). The final
serialized build and explicit DTK checksum both report `137 files OK`; the
rebuilt and original `main.dol` compare byte-identical, and the final build
leaves `config/GP6E01/symbols.txt` unchanged.

Wave 74 represents the final `0x4424` previously absent target text bytes in
Coin, Player, and Star. All 307 functions across those three owners now have
compiled source counterparts: Coin reaches 52/52 paired and 19 exact at
80.933500%, Player reaches 165/165 and 134 exact at 96.342760%, and Star
reaches 90/90 and 64 exact at 93.603770%. The source recovers Coin award and
team-distribution logic, Player initialization/dice/movement/space handling,
and the Star/Ztar acquisition and map-view event loops.

`mbCoinDispKillCheck`, both exact Coin all-player entries, and exact
`MasuCoinExec` account for `0x698` newly exact target bytes. Four represented
`board/mgcall.c` residues also close exactly (`0x184`), leaving all 12
represented minigame-call functions exact while 26 target functions remain
absent. In total, eight functions and `0x81C` target bytes close exactly in
this batch. All four owners remain `NonMatching`, so the clean-C owner ledger
is unchanged. Evidence, sibling-authenticated source shapes, target-only ABI
and stack facts, and bounded WIP residues are retained in
[`docs/native_matching_wave74.md`](docs/native_matching_wave74.md). The final
serialized build and explicit DTK checksum both report `137 files OK`; the
rebuilt and original `main.dol` compare byte-identical, and the final build
leaves `config/GP6E01/symbols.txt` unchanged.

Wave 75 closes four represented Board functions totaling `0x67C` target text
bytes. The MP5-authenticated `mbPlayerHandicapGet` inline restores the real
`mbSaveInit` call shape (`0x280` exact), target-proven RGB565 texture ownership
and allocation lifetimes close `mbWipeSpecialInit` (`0x1CC` exact), and the
compiler-oracle-backed declaration order closes `mbev_Scroll` (`0xF4`) plus
`ScrollMain` (`0x13C`). Board reaches 32/35 exact functions at 99.919136%,
Wipe reaches 32/33 at 99.936516%, and all 15 represented Scroll functions are
now exact under the required data-value comparison.

No owner is promoted: Board retains three code residues, Wipe retains one code
residue and a constant-pool difference, and Scroll still lacks 16 target
functions. Byte-neutral Audio and Board probes, a regressing Masu probe, and a
Wipe wrapper disproved by the target CFG were all rejected. Evidence is
retained in
[`docs/native_matching_wave75.md`](docs/native_matching_wave75.md). The final
serialized build and explicit DTK checksum both report `137 files OK`; the
rebuilt and original `main.dol` compare byte-identical, and the final build
leaves `config/GP6E01/symbols.txt` unchanged.

Wave 76 closes seven represented Board functions totaling `0x968` target text
bytes. The MP5-authenticated player-collision initialization order closes
`PlayerColCornerSnap` and both packed reset callers (`0x4FC` total); target
write order closes `mbStarObjCreate` (`0x78`); and the target's real
`GW_SYSTEM::curTime` predicate closes `mbTelopTimeStarSet` (`0xCC`). The same
batch ports MP5-authenticated `RotateScrollView` (`0x130`) and the target-proven
`mbev_ScrollCapsule` wrapper (`0x1F8`). Player reaches 137/165 exact functions,
Star reaches 65/90, Telop reaches 10/16 represented, and all 17 represented
Scroll functions are exact under the required data-value comparison.

No owner is promoted. Scroll still lacks 14 target functions, and the other
three owners retain represented or absent work. A helper-vs-inline Board
tradeoff, an unauthenticated Board register cycle, a regressing Masu
declaration probe, and the unclosed MP6 map-sprite layout were all left out.
Evidence is retained in
[`docs/native_matching_wave76.md`](docs/native_matching_wave76.md). The final
serialized build and explicit DTK checksum both report `137 files OK`; the
rebuilt and original `main.dol` compare byte-identical, and the final build
leaves `config/GP6E01/symbols.txt` unchanged.

Wave 77 closes eight represented Board functions totaling `0x133C` target text
bytes. Opening restores the authenticated team/group getter form, the target's
real `do ... while` single-opening loop, and the particle hook's automatic-
object/expression order; it reaches 18/22 exact functions. Player recovers the
shared camera-vector lifetime and Biri-Q teardown edge, reaching 139/165.
Coin recovers the compound alpha assignment and real team-stat local
lifetimes, reaching 21/52. Star's target-proven material-hook declaration
order raises it to 66/90. Wave 78's subsequent object audit disproved the
non-exact Last Five `activeNum = 0` association: the target zero word is a
write-only stack slot, while that initializer creates a live register and a
different allocation. The unsupported initializer was removed without
affecting Wave 77's eight exact closures or `0x133C` exact-byte total.

No owner is promoted. A regressing Player getter expansion, byte-neutral Star
inline qualifiers, a Dice direct-index loop that MWCC fully unrolled, body
casts for a shared Coin prototype issue, and ambiguous Effect/Star signature
residues were all rejected. Evidence is retained in
[`docs/native_matching_wave77.md`](docs/native_matching_wave77.md). The final
serialized build and explicit DTK checksum both report `137 files OK`; the
rebuilt and original `main.dol` compare byte-identical, and the final build
leaves `config/GP6E01/symbols.txt` unchanged.

Wave 78 closes 22 represented Board functions totaling `0x2468` target text
bytes. Opening closes its party event, coin shower, and player drop; Coin and
Star close their all-player award loops plus one owner-local model-ID root
each. Single recovers its save initializer, real `u8` space-type globals, and
step getter, while Board's caller declaration gains the same target-proven
argument ownership. Config gains the target-proven `0x5C` pause-panel work and
eight exact accessors. Shop Event gains the target-proven `0xB0` object work,
the named initialized tables and globals used by represented functions,
construction/update/open/message flows, and four exact functions. Effect's
false opaque animation structs are replaced by the established engine types,
but the owner remains divergent and gains no exact closure.

No owner is promoted. The Shop background-motion helper remains 98.865490%
WIP because the available MP7 counterpart is binary-only and cannot
authenticate its unexplained frame/register residue. Regressing Dice and
Config forms, byte-neutral Opening forms, unsupported shared-header widening,
and the disproven Last Five local were all rejected. Evidence is retained in
[`docs/native_matching_wave78.md`](docs/native_matching_wave78.md). The final
serialized build and explicit DTK checksum both report `137 files OK`; the
rebuilt and original `main.dol` compare byte-identical, and the final build
leaves `config/GP6E01/symbols.txt` unchanged.

Wave 79 closes seven Config pause-panel lifecycle functions totaling `0x588`
target text bytes. The target-proven `0x5C` work now owns its 20-entry
allocation, guide process/destructor, panel/model/animation/sprite construction
and destruction, and size/grow/shrink transitions. All seven are data-value
exact; Config reaches 21/51 exact functions at target/source `.text`
`0x63BC/0xD0C` and 13.070657%. Wipe adopts the same-game Matching math-header
recipe, removing the source-only `sqrtf` constants and making target/source
`.sdata2` exactly `0xA8/0xA8`; its 32 exact functions do not regress. Neither
owner is promoted, and the unexplained Wipe fade-out live range is not
invented. Evidence is retained in
[`docs/native_matching_wave79.md`](docs/native_matching_wave79.md). The final
serialized build and explicit DTK checksum both report `137 files OK`; the
rebuilt and original `main.dol` compare byte-identical, and the final build
leaves `config/GP6E01/symbols.txt` unchanged.

The exact build result includes extracted original objects and explicit
standalone assembly fallbacks for owners that are not yet byte-identical C.
Those owners remain `NonMatching` in `configure.py`; fallback-linked code is
never counted as decompiled source. An exception-bearing matching owner is
reported separately from clean C; admitting authentic assembly does not turn
that assembly into decompiled C.

## `game/` ownership

The `src/game/` path is fully matched: all 57 owners are configured Matching,
54 as clean C and `game/kerent.c`, `game/jmp.c`, and `game/malloc.c` under
authenticated assembly exceptions. It is therefore not 100% clean C. The
broader `Game` library is 60 of 61 owners Matching because it also owns the
still-fallback-linked `board/board.c`.

`game/mic.c` is matching and is linked from recovered C.

All configured `src/dolphin/` owners other than `dolphin/mic/mic.c` are now
Matching. `dolphin/mic/m2s.c` closes in Wave 61; `mic.c` remains real C
decompilation work and is tagged `SRC-DIVERGES` below.

## Assembly fallback boundary

Assembly is never admitted merely because a C transcription did not match.
Inline or standalone assembly is admitted only when an authenticated Matching
sibling carries the corresponding body/source shape, or an independently
complete vendor-library decompilation authenticates the exact target routine.
The vendor route additionally requires direct instruction identity. In both
cases the MP6 compile, object-byte, effective-relocation, linked-range, and
container gates must all pass. The current tree has 39 matching exception
owners:

- 23 Dolphin SDK owners authenticated by Matching Mario Party 4 sources or
  complete dolsdk2004 source, with `mtx44vec` independently authenticated by
  Matching Mario Kart: Double Dash
- `game/kerent.c`, `game/jmp.c`, and `game/malloc.c`, plus the authenticated
  `OSFastCast` closure used by matching Game/Board consumers
- the DOL Runtime owner, `__init_cpp_exceptions`, authenticated Runtime
  `ptmf`, and six low-level or mixed TRK owners, including `dolphin_trk` and
  `targimpl`
- the existing `GXLight` owner, whose paired-single `PushLight` helper is from
  Matching Pikmin 2 commit `46aecad6`
- MusyX `CheapReverb/creverb.c`, `Chorus/chorus_fx.c`, and `snd_service.c`,
  whose five retained assembly routines are authenticated by Matching Mario
  Party 5 sources

Commit-, path-, block-, and target-object-level evidence for the established
exception set is retained in
[`docs/easy_ports_wave.md`](docs/easy_ports_wave.md); the two subsequent
MusyX decisions are retained in
[`docs/native_matching_wave22.md`](docs/native_matching_wave22.md).

NonMatching owners still link extracted original objects or explicit
standalone `.s` fallbacks and are not counted as decompiled source. Promotion
requires matching retained instructions and effective relocations plus an
exact linked owner range and container; a container checksum by itself is not
sufficient. Linker-stripped donor helpers, `R_PPC_NONE` annotations, and split
padding are accepted only when documented by the object comparison and absent
from the target linked range.

### Two-bucket fallback taxonomy

Every current DOL fallback owner is in exactly one bucket:

- `original-was-asm`: the complete authentic owner was assembly. It is a
  policy/proof decision, recoverable only through the source-authentication
  exception above, and can never be counted as decompiled C.
- `C-not-yet-matched`: the owner is C or has a real C portion. Missing source,
  mixed C/assembly, or a divergent compiled object remains genuine
  decompilation work; an assembly substitute is not an acceptable escape.

The reason tags record why an owner is in its current state. `ASM-GATE-PENDING`
means the sibling authenticates assembly source shape but the MP6 object/link
gate has not passed. `SRC-DIVERGES` means a current source candidate was
compiled and objdiff still rejects it. `NO-SOURCE` means the named split was
never promoted on this lineage and no current source candidate exists; it was
not "de-flipped." Historical corrections use `ASM-BLANKET-REMOVAL` and
`HEADER-INVALIDATED` below.

#### Bucket 1: `original-was-asm` (1 current fallback owner)

| Owner and reason | Authenticating sibling and sibling status | Current MP6 status |
| --- | --- | --- |
| `TRK_MINNOW_DOLPHIN/__exception.s` (`ASM-GATE-PENDING`) | [Mario Party 4 `src/TRK_MINNOW_DOLPHIN/__exception.s` at `147b165`](https://github.com/mariopartyrd/marioparty4/blob/147b165a83187ac9e6cfdc3bf52f2e73437b1ffd/src/TRK_MINNOW_DOLPHIN/__exception.s), `MatchingFor(USA,PAL)` | M4 authenticates the standalone `.init` exception-vector form and `0x1F34` size, but MP6 starts at a different address and has different vector/padding offsets. M5 has the MP6 bounds but is `NonMatching` with no source. A target-specific reconstruction and full gate are still required. |

#### Bucket 2: `C-not-yet-matched` (101 current fallback owners)

Eighty-five owners have current source candidates and are tagged
`SRC-DIVERGES`. The baseline DTK 0.9.2 comparisons were regenerated on
2026-07-15. Rows changed through Wave 78 were regenerated from the final
source and target objects on 2026-07-16; percentages are raw `.text` scores
unless otherwise noted.

| Owner | Objdiff reason for remaining fallback |
| --- | --- |
| `Runtime.PPCEABI.H/New.cp` | The real four-byte `std::__new_handler` global at `0x802C0C58` is recovered; the target `.sbss` size is `0x8` because of alignment, not a second global. Both delete functions remain exact (`0x4C` each). Canonical `operator new[]` and `operator new` bodies compile to `0x68` versus target `0x70`/`0x6C`, at 85.178570%/88.518520%, so the owner remains fallback-linked. |
| `Runtime.PPCEABI.H/NewMore.cp` | Recovered the sibling-authenticated `std::__throws_bad_alloc` name, direct `bad_alloc : exception` inheritance, RTTI, vtables, and all five functions. Both destructors and `extab`/`extabindex` are 100%; `__throw_bad_alloc` is target/source `0x58`/`0x58` at 99.545456% with two pooled-string label operands, while each `what()` is `0xC`/`0x10`. String pooling and aligned data closure still diverge. |
| `Runtime.PPCEABI.H/NMWException.cpp` | 7/8 functions exact (`0x3F4` bytes); `__construct_array` is target/source `0xF8`/`0xFC`, 93.854836%. Two authenticated loop-shape probes were byte-neutral, so the sibling form was restored and unsupported register forcing was rejected. |
| `Runtime.PPCEABI.H/Gecko_ExceptionPPC.cpp` | Matching M4 donor gives 11/13 raw-exact retained functions, but emits a linker-stripped base-exception closure and divergent exception/data sections. A trial flip produced only 115 files OK and a divergent DOL/22 RELs, so it was de-flipped (`SRC-DIVERGES`). |
| `MSL_C.PPCEABI.bare.H/printf.c` | The exact MP4/MP5 source blob gives 9/15 exact target functions totaling `0xC78`, including the target's hidden `0x12C` helper after a temporary name-only pairing probe. The retained authentic name is `round_decimal`. Target/source whole `.text` remain `0x2160`/`0x213C`; `fprintf` is absent, five numeric-format functions diverge, total relocations are 275/271, and `.rodata`/`.data`/`.sdata` sizes differ. |
| `MSL_C.PPCEABI.bare.H/alloc.c` | Matching FFCC and Super Mario Strikers sources authenticate the allocator family. The recovered `Block`, `SubBlock`, fixed-pool, and `0x34` pool layouts plus all 11 target routines compile, and the six fixed sizes in `.rodata 0x18` are exact. The MP6 GC/2.6 object still diverges substantially from the GC/2.7 donor shape: mapped text scores range from 29.324675% to 61.834644%, and six target-local helpers retain address names, so the owner remains fallback-linked. |
| `MSL_C.PPCEABI.bare.H/qsort.c` | Matching Super Mario Strikers commit `795ee483` authenticates the MSL heapsort. Target/source are `0x170/0x16C` at 98.260870%; the target retains `li 2; mullw` where the donor compiler emits `slwi`. GC/2.7 was byte-neutral and authenticated multiplication spellings did not close the difference, so the real source is retained without forcing a match. |
| `MSL_C.PPCEABI.bare.H/e_exp.c` | Matching Pikmin 2 commit `46aecad6` authenticates the fdlibm source. All `.rodata 0x30`, `.sdata2 0x78`, and 21 relocations match. `__ieee754_exp` remains target/source `0x21C/0x224` at 91.592590%, so exact constants do not justify promotion. |
| `board/board.c` | `SRC-DIVERGES`; `f15ed033` corrected a stale duplicate `Object()` configuration that had silently linked the wrong side and de-flipped the unproved owner. Target/source `.text` are `0x1CFC/0x1CFC` at 99.919136% under the required data-value comparison; all 35 functions pair, 32 are exact, and all 618 text relocations pair. MP5 authenticates the restored `mbPlayerHandicapGet` inline, making `mbSaveInit` exact at `0x280`. MP6 argument stores reconcile `mbSaveStoryInit` and its Single caller as `(teamChar, mgPack, storyComDif)` without changing bytes. `mbObjectSetup`, `mbMain`, and `mbNextTime` retain only 29 register-color operand differences, so the owner remains fallback-linked. |
| `board/math.c` | All 56 target functions are represented. The additions recover the multiply-high random scaler, real HSF min/max scan, eight-corner camera/shadow frustum test, transform hook, object walk, and two target-exact `HuVecF[8]` BSS banks. Target/source `.text` are `0x2A44/0x2F00`; strict score rises from 35.577080% to 47.086136% and data-value score from 35.699078% to 47.239555%, while all prior 51 scores are unchanged. `.bss 0xC0` is exact; source `.sbss 0x4` covers the target's named value before its four-byte alignment tail. `mbRandMod` is 91.941180%, `mbObjCullInit` is exact-size at 75.714290%, and the cull hook inherits major differences from the divergent matrix helpers. `mbMathClose` remains the sole strict-exact body, so the complete owner stays fallback-linked. |
| `board/telop.c` | Sixteen pad, taunt, language, board-directory, telop-check, and time-display routines plus their real tables/globals are recovered. The target-proven `GW_SYSTEM::curTime` predicate makes `mbTelopTimeStarSet` exact at `0xCC`. Target/source `.text` are `0x329C/0x8E0` at 16.132448%; 16/31 functions pair and 10 are data-value exact. Fifteen target functions in the large telop/time/taunt OM execution closure remain absent. |
| `board/coin.c` | All 52 target functions are represented, including allocation, lifecycle, transform, display, renderer, effect, award, team-minimum, and all-player distribution flows. The target proves the `0x40` public object, its caller-work tail, `0x1144` model/effect bank, real `0x160` `MBCOINOBJDATA`, and `0x1C` display-model entries. Same-TU compound-assignment codegen closes `CoinEffHook`, target/MWCC lifetime evidence closes `mbStatTeamMinValGet`, the target return width closes `CoinInit`, and the MP5-authenticated team getter plus MP6 live-range reuse close `CoinAddAllProc`. Target/source `.text` are `0x4550/0x3F78` at 81.157800% data-value; 23/52 functions are exact. The represented award and helper-expansion residues keep the owner fallback-linked. |
| `board/dice.c` | All 68 target functions are represented through the MP5-authenticated Sai framework reconciled against MP6 instructions, data, and ABIs. Target/source `.text` are `0x6390/0x6040` at 92.523540% data-value; 50/68 functions are exact. The recovered owner includes result/process/hook accessors, exact five-slot banks, max-value tables, complete creation/update state machines, and target-backed work layouts. Eighteen represented functions still diverge, so the owner remains fallback-linked. |
| `board/single.c` | The complete four-word minigame-unlock set, all seven bitset operations, prize reset, opponent-character lookup, team-character state, target-proven `u8 masuTypeNum`/`u8 masuType[5]`, space-type reset, and step getter are recovered. MP5 authenticates the save-initialization sequence; MP6 proves `(teamChar, mgPack, storyComDif)`, turn 50, and the flag writes. Target/source `.text` are `0x98F0/0x4EC` at 2.481099% data-value; 14/58 target functions are represented and 9 are exact. The rest of the large single-mode owner remains absent. |
| `board/star.c` | All 90 target functions are represented. The target-addressed `STARWORK`, exact `0x60` `STARDISPWORK`, 999-slot Star/Ztar banks, four-player display bank, process/animation/hook roots, add/display flow, lifecycle, transform, event/destructor, grow/shrink/effect, display helpers, and acquisition/map-view event loops are recovered. The target's single-load sentinel store order makes `mbStarObjCreate` exact at `0x78`; target/MWCC automatic-object order makes `StarMatHook` exact at `0xFC`; the target `extsh` authenticates Star's legacy signed-16-bit local declaration of the otherwise `int` Guide getter and closes `StarPauseHook`; and the MP5-authenticated team getter plus MP6 live-range reuse close `StarAddAllProc`. Target/source `.text` are `0x5C00/0x5A04` at 93.670350% data-value; 68/90 functions are data-value exact, source `.bss 0x1F48` is target-exact, and source `.sbss 0x34` covers the target's named prefix. Represented event and display residues keep the owner fallback-linked. |
| `board/tutorial.c` | The real `0x14` call work, typed ID registries, `0x80` window-timing bank, watch/main processes, exit state, and `TUTORIALGUIDEDATA` table are recovered. Target/source `.text` are `0x2C34/0x1D44`; 46 functions pair and 35 are exact, up from 40/34. `mbTutorialCreate` is exact, and the 72 message/sound pairs plus sentinel are byte-exact across the source `.data 0x248`; the target's remaining `.data 0x50` is the unrecovered minigame Y-offset table. Watch and four window/message functions are 93.854546%-98.659790%; the minigame-call closure remains absent. |
| `board/wipe.c` | All 33 target functions are represented. The lifecycle, dispatcher, material hook, update paths, paper geometry setup, and grid/paper/image GX draw paths recover the semantic `0x2C` `WIPE_SPECIAL_DATA`, typed RGB565 texture buffer, `0x14AC` grid, `0xD34` paper, `0x18` image work, real `WIPE_PAPER_VTX`, eight-entry fade table, and paper geometry/light globals. The same-game Matching math-header recipe removes the source-only `sqrtf` statics, so target/source `.data 0xA0`, `.bss 0x38`, and `.sdata2 0xA8` now agree in size. Coverage remains 32/33 exact functions and target/source `.text` are `0x3664/0x365C` at 99.933070%. `mbWipeSpecialFadeOutCreate` remains target/source `0xB4/0xAC`; MP6 and MP7 authenticate a vanished live range but not its C source, so the owner stays fallback-linked. |
| `board/capselect.c` | Eighteen result, story/type, capsule lookup/count, callback, and map-capsule object routines are recovered with the target-proven four-player result/model/type arrays and 16-entry map-object table. All 18 represented functions are data-value exact; target/source `.text` are `0x3460/0x330` at 6.085919%. The target proves the capsule getter's `int` ABI and the shrink check's explicit branch. Fourteen selection/map functions and unresolved `0x28` map-object work remain absent. |
| `board/capsule.c` | Thirty-three retained target functions recover the four-player effect state, `s16 capsuleNum[33][2]`, packed space-capsule/player helpers, the real `0x2C` `CAPSULE_OBJ_COLOR` over 128 entries, capsule stubs, the typed throw hook, and the auto-throw tail. Target/source `.text` are `0x146CC/0x1148`; 22/33 mapped functions are exact, up from 18/29. The compiler also retains the sibling-authenticated out-of-line search helper, which has no named target pairing. `mbCapObjColorCreate` is omitted because the capsule-data field at `0x1C` is not yet semantically closed. |
| `board/config.c` | Matching MP5 pause-copy shape plus MP6 instructions recover the framebuffer/model/counter globals, four-player display state, framebuffer create/kill/draw path, pad-disable setter, pause-guide kill state and process, story-mode helper, and complete target-proven `0x5C` `PAUSE_PANEL_WORK`. The recovered lifecycle owns the 20-entry allocation, panel/model/animation/sprite construction and destruction, and size/grow/shrink transitions. All 21 represented functions are data-value exact, including the eight earlier accessors and seven new lifecycle functions totaling `0x588`; target/source `.text` are `0x63BC/0xD0C` at 13.070657%. The guide/config state machines remain absent. |
| `board/last5.c` | The target-derived stable rank/order routine and 40-coin effect recover `0x650` source text plus the real `LAST5COINWORK { s16 delay; float velocity; }` overlay on public `MBCOINOBJ` caller work. Wave 78 disproved the earlier `activeNum = 0` association: target stack `+0x8` is write-only, the real arrays begin at `+0xC`/`+0x18`, and the initializer emits a live register and different allocation. Target/source `.text` are `0x2458/0x650` at 17.356405% data-value. `Last5PlayerOrderGet` is exact; `ev_Last5Coin40` is 98.781910% at `0x2F0/0x2E8`, and its unexplained stack/allocation residue and the roulette work are not invented. |
| `board/opening.c` | All 22 target functions recover the party and single opening flows, coin/drop/order helpers, the target `GW_PLAYER::playerNo` bitfield, shared target-proven `MBPLAYERWORK`, typed process/hook/effect state, and all initialized owner tables. The single closure includes its cleanup owner, 256-entry route, corner reduction, adaptive Simpson segment lengths, Newton constant-distance traversal, de Casteljau guide flight, and particle/window/motion/fall sequence. Authenticated getters close `PlayerOrderSet`, the target back edge closes `mbev_OpeningSingle`, target local/evaluation order closes `OpeningSingleEffHook`, and MP5 plus MP6 stack/relocation evidence close `ev_OpeningParty`, `OpeningCoinExec`, and `PlayerDropExec`. Target/source `.text` are `0x2B14/0x2CAC` at 89.108086% under the data-value comparison; 21/22 functions are exact. `ev_OpeningSingle` remains `0x1094/0x122C` at 71.697460% because the callback evaluator still inlines instead of retaining the target's indirect call. The complete owner remains fallback-linked. |
| `board/scroll.c` | Seventeen scroll/map lifecycle, event, collision cleanup, star-space, animation, bounded player-collision, map-sprite, camera, hook, view-rotation, and capsule-entry routines are recovered. All 17 represented functions are exact under the required data-value comparison. MP5 authenticates `RotateScrollView`; the target proves `mbev_ScrollCapsule` as the nested expansion of the already exact local scroll chain. Target/source `.text` are `0x3E4C/0x930` at 14.747931%; `.sbss` is exact at `0x28`. Fourteen target functions, including the unclosed collision and map-rendering families, remain absent, so the owner stays fallback-linked. |
| `board/shopevent.c` | Eleven target-backed functions recover the typed object hook, real two-word `MBSHOPWORK { playerNo, shopNo }`, complete target-proven `0xB0` `MBSHOPOMWORK`, named initialized data used by represented functions, object bank, and shop construction/update/open/message flows. Target symbol order restores the non-inlined init wrappers, and the shop-call wrapper retains the proven untyped-to-typed allocation lifetime. Target/source `.text` are `0x2FDC/0xC7C` at 26.054848% data-value; 10/13 target functions are data-value exact. `mbev_ShopBackMotCreate` remains represented at 98.865490% because Mario Party 7 commit `f5ea780d07dab8188d3c02f7e9052360c591dc1b` provides only `config/GP7E01/symbols.txt` and `orig/GP7E01/sys/main.dol` evidence for `MBShopBackMotObjCreate`, not authentic C for its frame/register residue; `ev_Shop` and `ev_ShopSelect` remain absent. |
| `board/snpc.c` | `mbSNpcInit`, `mbSNpcMasuGet`, and the authentic empty `SNpcStarFunc` are exact, with the target-proven three-byte save state (`flags`, `masuId`, `effectMissCount`) and typed runtime/save pointers. Target/source `.text` are `0x9594/0x44`. The `0x298` runtime object and all movement/dice/star state machines remain absent. |
| `board/capmove.c` | Nine named capsule-movement kill callbacks are recovered from their complete target ranges. Every target/source function is `0x4/0x4` and byte-exact; target/source whole `.text` are `0x6F68/0x24`. The movement event bodies remain absent. |
| `board/capthrow.c` | Eight named capsule-throw kill callbacks are recovered from their complete target ranges. Every target/source function is `0x4/0x4` and byte-exact; target/source whole `.text` are `0x8CB0/0x20`. The throw event bodies remain absent. |
| `board/captrap.c` | Five named capsule-trap kill callbacks are recovered from their complete target ranges. Every target/source function is `0x4/0x4` and byte-exact; target/source whole `.text` are `0x6A40/0x14`. The trap event bodies remain absent. |
| `board/capspecial.c` | Thirteen target-backed functions recover the exact `0x28` `TERESA_FADE_WORK`, Teresa steal state, Miracle lifecycle hooks, and Koopa dice/motion hooks. Target/source `.text` are `0xE744/0x1E8`; 9/13 mapped functions are exact. Fade kill/object/setter and Koopa motion remain 88.214290%-92.826090%; the large special-capsule state machines remain absent. |
| `board/capevent.c` | Eighty-three target functions are represented, 73 strict-exact and all 83 data-value exact. Wave 60 adds 45 strict-exact process/choice/player-move helpers, effect bank kill/get/set routines, four exact-stride active-entry counters, rotation/color helpers, and the target-exact seven-entry random-color table. MP6 accesses prove the partial coin, coin-man, star-man, cap-lose, move, ray, and space-hit work layouts and the `[8]` object banks; target instructions retain the `(t, a, b, c)` scalar Bezier ABI. Target/source `.text` are `0x14EFC/0x2068`, at 9.650637% strict and 9.673959% data-value. The other 154 target functions remain absent. |
| `board/mgcall.c` | Twelve functions recover the exact `0x10` `MGLISTWORK`, `.bss 0xD8` history/status/list closure, `.rodata 0x70` status tables, and `.data 0x48` pointer/size tables. All 12 represented functions are exact; target/source `.text` are `0x75FC/0x3E4` at 3.297576%. The other 26 target functions, including the roulette/UI executors, remain absent. |
| `musyx/runtime/synth.c` | The real job queue, controller destinations, voice/fader state, and 41 functions are materialized. Five target helpers and thirteen storage roots now have their same-version names, including `do_voice_portamento`, `StartLayer`, `StartKeymap`, the two precision handlers, `synthTicksPerSecond`, both auxiliary-controller banks, and the real one-byte/eight-byte `synthIdleWaitActive`/`synthRealTime` split. Target/source `.text` are `0x349C/0x433C`; all 27 target functions pair and 21 are exact. Six functions remain divergent, including size differences in `synthStartSound` and `synthFXStart`, and source BSS is four bytes shorter. |
| `musyx/runtime/stream.c` | Twenty-two functions restore 64 `STREAM_INFO` records and exact `.bss 0x1900`. `SetHWMix`, `GetPrivateIndex`, `CheckOutputMode`, `SetupVolumeAndPan`, and the stream ID/delay roots are now named from exact size/order/call evidence. Target/source `.text` are `0x3C04/0x4728`; all 18 target functions pair and 13 are exact. Five functions still diverge; `sndStreamFrq` remains structurally different at 70.902435%. |
| `musyx/runtime/synthdata.c` | Twenty-seven functions restore typed sample, keymap, curve, layer, macro, and FX registries. The five exact comparison helpers, seven registry banks, and six `u16` counts now have their same-version names. Target/source `.text` are `0x1D98/0x1D90`; all 27 functions pair and 25 are exact. Sample-reference add/remove remain 95.818184%/95.128204% with four-byte size differences. |
| `musyx/runtime/synthmacros.c` | Ninety-nine functions restore macro command dispatch, queues, variables, envelopes, controller selection, and exact target `.data 0x448`/`.bss 0x20`. Target/source `.text` are `0x4FA0/0x619C`; 8/11 mapped functions are exact. `varGet`, `macHandleActive`, and `macStart` are 97.607410%-99.423080%; the command closure remains unpaired. |
| `musyx/runtime/synthvoice.c` | Twenty-five functions restore VID lists, voice priority/free lists, and exact target `.bss 0xFC0`. The old aggregate VID blob is now split into the real `vidList`, two priority arrays, root list, voice list, last-started banks, and eight small list-state roots. Target/source `.text` are `0x1D64/0x1F98`; 16/18 mapped functions are exact. `voiceAllocate` does not align, `voiceBlock` is 51.840710%, and three target functions remain unpaired. |
| `musyx/runtime/s_data.c` | Twenty-six registry/group/sequence functions and typed group-stack state are materialized. The target `0x28C` registration switch is now authenticated as `InsertData` by its calls and same-version switch shape; its source form is `0x27C` at 97.515335%. Target/source `.text` are `0xBF0/0x1584`; six of seven target functions pair but none is exact, so the owner remains substantial real C work. |
| `musyx/runtime/hw_dspctrl.c` | Nineteen voice/studio/DSP-control functions are materialized; the false empty donor `salBuildCommandList` is deliberately omitted because MP6 retains real `0x255C` code. Target/source `.text` are `0x35D4/0x12A4`; 3/8 mapped functions are exact and five lifecycle/voice/aux functions are 99.797470%-99.875000%. |
| `musyx/runtime/snd3d.c` | Forty-five emitter/listener/room/door/3D functions and their typed roots are materialized. Target/source `.text` are `0x2238/0x360C`; 2/13 mapped functions are exact and eleven diverge. The larger room/door and 3D setup closures remain unpaired. |
| `musyx/runtime/snd_midictrl.c` | Thirty-eight MIDI/RPN/controller functions and their state tables are materialized. Target/source `.text` are `0x2300/0x1E00`; 23/32 mapped functions are exact. Nine controller/default functions diverge; semantic `.data` is exact and `.rodata` is 99.631000%. |
| `musyx/runtime/hardware.c` | Fifty-five hardware/voice/studio/stream functions restore real callback and state ownership. Target/source `.text` are `0x1094/0x1130`; 15/41 mapped functions are exact and 26 remain 93.775510%-99.829270%. Target/source `.rodata` are both `0x100`. |
| `musyx/runtime/hw_aramdma.c` | Fourteen active Dolphin functions restore typed ARAM transfer jobs, stream buffers, queues, and callbacks. Target/source `.text` are `0xCE0/0xCF4`; 7/11 mapped functions are exact. Init/store/remove/stream allocation are 73.500000%-98.594600%, and three helpers remain unpaired. |
| `musyx/runtime/StdReverb/reverb.c` | The typed delay-line/reverb owner compiles nine functions; create/callback/free are exact, target/source `.text` are `0xD38/0x1058`, and `.data 0x20` is exact. M4 `147b165` configures the owner `MatchingFor(USA,PAL)` from pinned [upstream MusyX `reverb.c` at `adc8df9`](https://github.com/AxioDL/musyx/blob/adc8df9a959f1e37f71bdf3155e229f9f87ad166/src/musyx/runtime/StdReverb/reverb.c), authenticating both inline-assembly bodies. Donor `DoCrossTalk` is `0x190` versus the remaining target placeholder's `0x184`; the mixed owner stays `NonMatching`, fallback-linked, and excluded from clean-C totals. |
| `dolphin/mic/mic.c` | The GC/1.2.5n compiler assignment remains proven by the five-version matrix. Wave 61 recovers the two-dimensional timeout table, signed timeout field, synchronous and asynchronous early-return CFGs, mount/gain switch shapes, all four raw EXI transactions, timeout and transmit callbacks, control-block acquisition, and button-update lifetimes. Target/source `.text` are `0x2020/0x1FC0`, at 98.147860% in both strict and data-value modes; 36/41 functions are strict-exact. `.data 0x40` and `.bss 0x150` are exact, and source `.sbss 0x4` covers the named value before the target alignment tail. `MICStartAsync` and `MICStopAsync` are 99.885710%/99.918365% with only eight/four uses of one stack-slot offset divergent; `__MICDoMount`, `__MICExiHandler`, and `__MICUpdateStatus` remain 89.031910%-91.986755%. MWCC stack compression proves a dead local cannot lawfully reserve the missing word, and no sibling authenticates a live aggregate, so fakematching was rejected. Three target text relocations remain unmatched and the owner stays fallback-linked. |
| `msm/msmsys.c` | 99.734%; 18/23 functions exact. |
| `msm/msmstream.c` | 99.487020%; 23/28 functions exact. `msmStreamDvdCallback` and `msmStreamDvdCallback2` are exact. `msmStreamData` is now target/source `0x2EC/0x2EC` at 99.759360% after recovering the target pause/linked-slot/ARAM-update control flow; `msmStreamSlotInit` is `0x224/0x224` at 99.635040% after recovering its three distinct size/offset lifetimes. Five functions still diverge, so the owner remains fallback-linked. |
| `board/player.c` | All 165 target functions are represented. The target-proven `0x50` `MBPLAYERWORK` carries typed movement timers, collision state, and Metal/Biri-Q owners while exact `.bss 0x140` retains the four-player bank. Matching MP5 authenticates the shared families and the restored `motStartF`-before-`killF` initialization order; MP6 proves the inserted collision state, differing flags, constants, particle `vertexNo`, by-value color ABI, and bin ownership. Target stack identity closes `mbMoveNumCreateColor` with one real camera vector, and same-TU condition codegen plus the target return edge close `PlayerBiriQOMExec`. Target/source `.text` are `0xBEB0/0xBDC0` at 96.373980%; 139/165 functions are data-value exact. Represented initialization, turn, movement, dice, space, and effect-builder residues keep the owner fallback-linked. |
| `board/audio.c` | 99.995735%; 48/49 functions exact. Recovering the unsigned board-number field and the Matching `board/camera.c` `BoardNoGet` inline makes `mbMusBoardPlay` (`0x12C`) and `MusBoardFade` (`0x1AC`) exact. Only `mbMusBoardFadeOut` remains: target/source are both `0x3F0` at 99.952380%, with twelve stack-slot operand differences. Whole target/source `.text` remain `0x2BF0/0x2BF0`, and all 444 text relocations match. |
| `board/masu.c` | All 119 target functions are represented. Wave 62 closes twelve more strict residuals by recovering the authentic data-definition order, math-header suppression, shared draw-local layout, atlas expression, event CFGs, party/team conditions, and capsule return types. Target/source `.text` are `0x5DC8/0x5DB0`, at 99.614460% in both strict and data-value modes; 115/119 functions are exact. `.rodata 0x40`, `.data 0x128`, `.bss 0x700`, `.sdata 0x10`, and `.sdata2 0x90` are exact. `.sbss 0x78/0x74` covers the named values plus the target alignment tail. The four remaining functions are `MasuDraw` (`0xB54/0xB54`, 99.484140%), `MasuFind` (`0x62C/0x62C`, 99.154430%), `mbMasuPlayerColorSet` (`0x260/0x260`, 99.868420%), and `mbMasuPKinokoValueGet` (`0x7EC/0x7D4`, 96.871796%). The owner remains fallback-linked and excluded from clean-C totals. |
| `board/effect.c` | MP5 authenticates the loop shape while MP6 offsets align the placeholder structs with the established `ANIMDATA -> ANIMBANK -> ANIMFRAME` layout, removing the false opaque types and `void *` API. This typed recovery is not an exact closure: target/source `.text` remain `0x3050/0x3074` at 59.852844% data-value, 1/35 functions exact, plus `.sdata` and `.sdata2` divergence. The restored FastCast header is not its blocker. |
| `gssdk_lib/asrpho/common/blocks/fft_maye.c` | The complete target-derived Mayer fast-Hartley and real-FFT pair is recovered, including both exact `0x40` trigonometric tables. Whole target/source `.text` is `0x8F0/0x8BC` at 79.104900%; `fht` is `0x73C/0x75C` at 85.051834% and `realfft` is `0x1B4/0x160` at 53.844036%. `.rodata 0x80` is exact, while target `.sdata2 0x18` remains 71.428570%. |
| `gssdk_lib/asrpho/common/blocks/fftmod.c` | The complete target-derived `0x4C` FFT block recovers its power/amplitude modes, queue processing, control, initialization, and construction. `ControlFFTMod` (`0xA4`) is exact. Whole target/source `.text` is `0xC84/0xC78` at 93.288390%; the other functions range from 80.234180% to 97.250000%, and target `.sdata2 0x48` is 80.000000%. |
| `gssdk_lib/asrpho/common/blocks/pitchwin.c` | The complete target-derived `0x44` pitch-window block recovers its Hamming coefficients, circular signed-sample history, queue lifecycle, and all five functions. `ConstructPitchWindow` (`0x44`) is exact. Whole target/source `.text` is `0x5C4/0x5C0`; `.sdata2 0x40` and all 37 relocations align. `ProcessPitchWindow` is `0x148/0x140` at 93.512190%, `ControlPitchWindow` is exact-size `0xF4` at 99.016396%, `CreateWindow` is exact-size `0x16C` at 76.703300%, and `InitPitchWindow` is `0x1D8/0x1DC` at 96.652540%. |
| `gssdk_lib/asrpho/common/blocks/dpscruw.c` | The complete target-derived `0x80` user-word scoring owner recovers all 14 functions, typed 12-byte backtrace entries, recognition-language state, Viterbi initialization, dynamic programming, active-distance construction, result scoring, and controls. Seven functions are exact: `InitBacktrace`, `FreeBacktrace`, `InitViterbi`, `InitDpScoreUw`, `IdleDpScoreUwProcess`, `DpScoreUwProcess`, and `ConstructDpScoreUw`. Whole target/source `.text` is `0x11B4/0x129C` at 73.921450%; `.sdata2 0x10` is exact and relocations are 60/62. |
| `gssdk_lib/asrpho/common/blocks/dpgenuw.c` | The complete target-derived `0xF4` user-word generator recovers all 12 functions, a 22-state lattice, typed 12-byte backtrace cells, free-list ownership, dynamic programming, result emission, and control/lifecycle paths. `FreeBacktrace`, `InitDpGenUw`, `MakeUserWordsActiveDist`, `IdleDpGenUwProcess`, and `ConstructDpGenUw` are exact. Target/source `.text` is `0x1330/0x1484`, `.sdata2` is exact at `0x10`, and relocations are 64/66; `DpGenUwProcess` is `0x160/0x15C` at 95.602270%, while the larger DP/control shapes still diverge. |
| `gssdk_lib/asrpho/common/blocks/pitchdp.c` | The complete target-derived `0x68` pitch dynamic-programming owner recovers all 11 functions, typed 12-byte Viterbi cells, state/jump factors, DP/backtracking, buffering, flush, reset, control, and initialization. `ConstructPitchDP` (`0x44`) is exact; `Output` is exact-size `0xFC` at 99.603170% and `InitPitchDP` is `0x36C/0x368` at 99.086760%. Target/source `.text` is `0x15C0/0x15CC` at approximately 63.19% target-byte-weighted similarity, `.sdata2` is `0x30/0x2C` at 78.260870%, and all 70 relocations pair. |
| `gssdk_lib/asrpho/common/blocks/flfxblks/pitchco.c` | The complete target-derived `0xC4` pitch combiner recovers all 13 functions, voiced-boundary checks, energy normalization, logarithmic pitch, output buffering, reset/control, and initialization. Six functions are exact: `MaxFloatInArray`, `CheckBeginVoicedFlag`, `CheckEndVoicedFlag`, `OutputProcedure`, `ControlPitchCombiner`, and `ConstructPitchCombiner`. Whole target/source `.text` is `0x13F8/0x13C8` at 96.021910%, `.sdata2` is `0x48/0x40` at 91.176470%, and all 81 relocations pair. |
| `gssdk_lib/asrpho/common/blocks/flfxblks/shs_vuv.c` | The complete target-derived `0x60` SHS/VUV owner recovers all eight functions: 65-bin spectrum handling, peak enhancement, cubic-spline work arrays, five-octave log interpolation, harmonic summation, V/UV thresholding, process, and initialization. `ControlSHS_VUV` (`0x10C`) and `ConstructSHS_VUV` (`0x44`) are exact. Target/source `.text` is `0x151C/0x14F0`, `.sdata2` is `0x78/0x74`, and 92/93 relocations pair; `ProcessSHS_VUV` is exact-size `0x290` at 91.975610% while the larger numerical kernels remain divergent. |
| `gssdk_lib/asrpho/common/blocks/delaybl.c` | The target-derived `DelayBlock` and shared `TosBaseBlock` ABI recover all four functions. `ProcessDelayBlock`, `InitDelayBlock`, and `ConstructDelayBlock` are exact. `ControlDelayBlock` is target/source `0xE4/0xE4` at 98.947365%; its only retained instruction difference is target `addi r4,r5,0` versus source `li r4,0` in the clear loop. Evidence-backed declaration and assignment variants did not reproduce it, so the owner remains fallback-linked. |
| `gssdk_lib/asrpho/common/blocks/undersam.c` | The target-derived `0x2C` undersampler, signed-sample grouping, profile defaults, and error path are recovered. `ProcessUndersampler`, `ControlUndersampler`, and `ConstructUndersampler` are exact. `InitUndersampler` is target/source `0xC0/0xC0` at 99.687500%; only the quotient's volatile register differs (`r5` target, `r4` source), so the whole owner remains fallback-linked. |
| `gssdk_lib/asrpho/common/blocks/flfxblks/genfilt.c` | The target-derived `0x30` gender filter, first-non-null input selection, profile-sized ports, and destructor control are recovered. `ProcessGenderFilter`, `ControlGenderFilter`, and `InitGenderFilter` are exact. The `0x7C` constructor remains at 79.000000% because its profile-probe and callback-address scheduling diverge; the owner remains fallback-linked. |
| `gssdk_lib/asrpho/common/blocks/flfxblks/median.c` | The target-derived `0x44` median block and `0xC` doubly-linked node recover the ring replacement, sorted insertion, median selection, allocation, and destruction logic. `ControlMedian`, `InitMedian`, and `ConstructMedian` are exact. `ProcessMedian` is target/source `0x2F4/0x2F4` at 78.952380%, so the owner remains fallback-linked. MP7 has the same retained machine-code shape but supplies no source donor. |
| `gssdk_lib/asrpho/common/blocks/flfxblks/combiner.c` | The complete target-derived `0x5C` block recovers eight history lanes, the rotating second-order feature combiner, flush behavior, allocation ownership, and all four functions. `CombinerControl` (`0xEC`) and `ConstructCombiner` (`0x44`) are exact. `CombinerInit` is exact-size `0x128` at 98.378380% and `CombinerProcess` is target/source `0x348/0x34C` at 87.404760%; whole `.text` is `0x5A0/0x5A4`, while `.sdata2 0x10` and all 21 relocations align. |
| `gssdk_lib/asrpho/common/blocks/flfxblks/statio.c` | The target-derived `0x50` stationarity block, queue/context fields, constants, and four functions are recovered. `ControlStationarity`, `InitStationarity`, and `ConstructStationarity` are exact. `ProcessStationarity` is target/source `0x1A8/0x1A8` at 99.575470%, with seven floating-point register-assignment differences. The target `.sdata2` is `0x10` versus source `0xC`, including a four-byte alignment tail, so the owner remains fallback-linked. |
| `gssdk_lib/asrpho/common/blocks/flblocks/window.c` | The complete target-derived `0x50` window block recovers flush, ring-window processing, control, initialization, and construction. `ControlWindow` (`0xB8`) and `ConstructWindow` (`0x44`) are exact. Whole target/source `.text` is `0xBFC/0xC00` at 95.573660%; the other functions range from 93.070540% to 96.896550%, and `.sdata2 0x20` is exact. |
| `gssdk_lib/asrpho/common/blocks/flfxblks/slidhist.c` | The complete histogram stage and its dynamic session payload recover all eight retained functions. `SlidingHistogram_Clear`, `SlidingHistogram_Free`, `SlidingHistogram_sizeof_SessionData`, and `SlidingHistogram_GetSessionData` are exact; `SlidingHistogram_PutSessionData` is exact-size `0xE4` at 99.912280%. Whole target/source `.text` is `0x53C/0x538` at 95.713430%, and `.sdata2 0x28` is exact. |
| `gssdk_lib/asrpho/common/blocks/flfxblks/subsamp.c` | The complete subsampler stage recovers all five functions and the target's 23-double impulse-response initializer. `Subsampler_Process` (`0x170`) and `Subsampler_Free` (`0xB0`) are exact; check/reset/init are 92.430380%-99.459460%. Whole target/source `.text` is `0x6E8/0x6E8` at 97.031680%, while `.rodata 0xB8` and `.sdata2 0x18` are exact. |
| `gssdk_lib/asrpho/common/blocks/flfxblks/voicing.c` | The complete voicing stage recovers all six retained functions against the shared typed `TriggerLR`. `Voicing_Free` (`0x78`) is exact; add/maintain/max/reset/init range from 66.503310% to 97.000000%. Whole target/source `.text` is `0x878/0x910` at 84.142070%, and target `.sdata2 0x40` remains 82.352940%. |
| `gssdk_lib/asrpho/common/blocks/flfxblks/trigglr.c` | The complete target-derived speech-trigger block recovers all five retained functions against the shared `0x19C` `TriggerLR`, including its state machine, queue controls, dynamic session transfer, sensitivity mapping, and lifecycle. `ConstructTriggerLR` (`0x44`) is exact. `TriggerLR_FindSpeech` is `0x6E4/0x6B8` at 85.267570%, `ProcessTriggerLR` is `0x1AC/0x198` at 86.158880%, `ControlTriggerLR` is exact-size `0x430` at 65.037315%, and `InitTriggerLR` is `0x3C0/0x3B8` at 87.937500%. Whole target/source `.text` is `0x10C4/0x107C` at 81.134200%; target/source `.sdata2` is `0x40/0x3C` at 96.774190%. |
| `gssdk_lib/asrpho/common/blocks/exev_dp.c` | The target-derived `0x6C` block and `0x18` context-information layout recover all six extra-event dynamic-programming functions. MP7 target-binary parity confirms the core Viterbi and DP shapes, and the MP6 `convert.c` consumer establishes the shared context fields. `ProcessExtraEventDP` and `ConstructExtraEventDp` are exact; whole target/source `.text` are `0x4D8/0x54C` at 72.774190%, so the larger control closure remains real C work. |
| `gssdk_lib/asrpho/common/blocks/flblocks/dctlift.c` | The complete target-derived `0x3C` DCT-lift block recovers its matrix process, coefficient generation, profile fields, and lifecycle. `ControlDCTLift` and `ConstructDCTLift` are exact; `ProcessDCTLift` is `0x1E4/0x1E4` at 98.950420% and `InitDCTLift` is `0x1FC/0x200` at 85.968506%. Whole `.text` is `0x484/0x488` at 93.394460%; `.sdata2 0x38` and all 27 relocations align. |
| `gssdk_lib/asrpho/common/blocks/flblocks/gender.c` | All 12 target functions and the real `0x94` owner are recovered from MP6 instructions and consumers, including the code-book descriptor, dynamic-session tail, gender scoring, frame ring, controls, and lifecycle. Language-data virtual slots at `0x88` and `0xD0` are now typed. `LoadCodeBook` (`0x170`) and `ConstructGender` (`0x44`) are exact. Whole target/source `.text` is `0x10A8/0x124C` at 86.567540% with 70/72 relocations; helper visibility/order changes `ControlGender` inlining and emits an extra source `OutputFrame` body, so the owner remains fallback-linked. |
| `gssdk_lib/asrpho/common/blocks/flblocks/vq1500.c` | The target-derived `0x30` owner, `0x14` code-book descriptor, language/code-book method ABI, vector searches, queue lifecycle, and all nine functions are recovered. Eight functions are exact. Separate compound scopes now recover the first- and second-search pointer lifetimes and the semantic `secondSearchCount * label` indexing order. `GetLabel` remains exact-size `0x148` at 99.512190%, with only six second-pass `i`/`indices` register-color instruction differences; target/source `.text` are both `0x4F0`, both objects have 25 text relocations, and the target retains a four-byte `.sdata2` alignment tail. |
| `gssdk_lib/asrpho/common/blocks/flblocks/smoother.c` | The complete target-derived `0x20` smoother and `0x18` static matrix-bank descriptor recover construction, destruction, active-column rejection, QR update, and spline coefficient evaluation. `smtConstruct` (`0x2AC`) and `smtDestruct` (`0x90`) are exact. `Smoothing` is target/source `0x358/0x35C` at 75.154205%; whole `.text` is `0x694/0x698` at 87.370544%, and target/source `.sdata2` are `0x18/0x14` at 90.909096%. |
| `gssdk_lib/asrpho/common/blocks/flblocks/mel.c` | The complete target-derived `0x40` mel-filter block recovers five functions, per-band weights/bin ranges, queue processing, profile allocation, and control/destruction. `InitMel` (`0x6C`) and `ConstructMel` (`0x44`) are exact. `ProcessMel` is exact-size `0x1B8` at 91.681816%, `ControlMel` is `0x158/0x188` at 72.534880%, and `MelInitBands` is `0x504/0x508` at 93.875390%. Whole `.text` is `0x8C4/0x8F8` at 90.654190%, while target/source `.sdata2` are `0x60/0x68` at 96.000000%. |
| `gssdk_lib/asrpho/common/blocks/flblocks/acne.c` | The complete target-derived `0x98` ACNE block recovers all eight functions, one three-vector band allocation, the exact `12*bandCount + 40` session layout, queue lifecycle, smoothing/adaptation state, and two 21-float tables. `Reset` (`0x108`) and `ConstructAcne` (`0x44`) are exact. Whole target/source `.text` is `0xC24/0xC64` at 82.891890%; `.rodata 0xA8` is exact, target/source `.sdata2` is `0x50/0x4C` at 97.435900%, and text relocations are 95/96. The four-byte slot at `0x80` is documented only as an unread/unwritten ABI reserve: MP6 allocation and neighboring fields prove the gap, while MP7 commit `f5ea780` has the same `0x98` allocation, exact initialization/construction parity, and no access at `0x80`. |
| `gssdk_lib/asrpho/common/blocks/flblocks/specsub.c` | The complete target-derived `0x98` spectral-subtraction block recovers all eight functions, one `(historyLength + 6) * spectrumSize` float arena, a segment-endpoint array, the exact dynamic session layout, four queued outputs, and the full update/gain/normalization algorithm. `ConstructSpecSub` (`0x44`) is exact. Whole target/source `.text` is `0x10F8/0x15FC` at 42.579190%; target/source `.sdata2` is `0x40/0x58` at 81.578950%, and text relocations are 92/102. The source is complete but still far from original compiler shape. The unread/unwritten four-byte slot at `0x64` is recorded only as an ABI reserve: MP7 commit `f5ea780` has instruction parity apart from linked relocation words and preserves the same untouched gap between proven `0x60` and `0x68` fields. |
| `gssdk_lib/asrpho/common/ctxdata/ctxdata.c` | The binary-proven `ContextDataV2` layout through offset `0xB8`, virtual table, and all accessors are recovered. Seventeen accessors are byte-exact; the three packed-tail pointer computations remain at 79.500000%, 66.933334%, and 68.000000%, while `FillContextV2VirtualTable` retains one source/target table-relocation identity difference. The owner remains fallback-linked. |
| `gssdk_lib/asrpho/common/blocks/flblocks/mtx.c` | The complete real `0x18` float/integer matrix descriptors and all 12 allocation, association, compression, column-deletion, and QR-update functions are recovered. Nine functions are exact. `imtxDeleteCol` is exact-size `0x88` at 98.676470%, `mtxCompress` is `0xC4/0xBC` at 94.795920%, and `QrDeleteCol` is `0x308/0x314` at 94.118550%. Whole target/source `.text` is `0x7FC/0x800`, and both objects have 25 relocations; source `.sdata2 0x40` retains the compiler's additional `sqrtf` constants versus target `0x30`. |
| `gssdk_lib/asrpho/common/blocks/flblocks/mtxopt.c` | The shared recovered `0x18` `FloatMatrix` replaces the old private prefix and preserves existing object evidence. `mtxFillCopy` (`0x30`) and `mtxFillX` (`0x80`) are exact. `QrPreMult` is target/source `0x1A8/0x1A4` at 92.528305%, with the same unrolled arithmetic but divergent register allocation and outer-loop closure, so the owner remains fallback-linked. |
| `gssdk_lib/gsapi/callbacks.c` | Six callback wrappers totaling `0x260` are exact, as are `AsrSpiSignalCallBacks` (`0x18`) and `AsrSpiRecogCallBacks` (`0x8`) with their relocation order. The target's `0x580` `asrspi_cbResult` remains unrecovered, so the complete owner stays fallback-linked. |
| `gssdk_lib/gsapi/ctxfuncs.c` | Five small target-derived routines are materialized. `SessionDataFree`, `ContextAPIDeActivate`, `ContextSetActiveWords`, and `ContextSetCtxData` are exact; `ContextGetParam` is `0x50` at 84.500000%. Ten larger context/session routines remain absent, so the owner stays fallback-linked. |
| `gssdk_lib/asrpho/common/ctxdata/langdata.c` | The real `LanguageDataV2` prefix and code-book layouts support 36 exact field/pointer accessors. The target's `0x13C` language-method object and the code-book slots consumed by `vq1500.c` and `gender.c` remain typed; Wave 50 additionally names the target-proven recognition, phenome, speech-unit, user-word, ergodic-state, and penalty slots consumed by `dpscruw.c` and `dpgenuw.c`. `_langGetpErgodicPenalty` is `0x64` at 77.040000%; the deeper packed-layout functions and virtual-table source closure remain unrecovered. |
| `gssdk_lib/asrpho/common/tos/mqueue.c` | All 16 target queue functions are recovered against the real queue element, reader-array, `TosContext`, and embedded allocator layouts. `qQueueInit` (`0xF0`) and `qQueueNbrElements` (`0x20`) are exact. Whole target/source `.text` is `0x102C/0xFD4` at 82.081160%; both objects have 29 text relocations. The complete owner remains fallback-linked because its control, enqueue, reader-update, resize, and allocation compiler shapes still diverge. |
| `gssdk_lib/asrpho/common/tos/tinyos.c` | All 25 target TinyOS functions are recovered with typed architecture construction/destruction, block and queue definitions, profiles, control tables, scheduling, callbacks, and ownership. The target proves `TosContext` size `0x16C`, `TosBaseBlock` size `0x28`, and the shared profile-owner prefix. `tosDestruct` (`0x48`) and `DisabledProcess` (`0x4`) are exact. Whole target/source `.text` is `0x1D90/0x1D88` at 74.948204%, `.sdata2 0x8` is exact, and both objects have 70 text relocations. The complete owner remains fallback-linked because the larger construction, run, control, and base-block compiler shapes diverge. |

The other 16 owners are tagged `NO-SOURCE`. Each explicit brace group below
expands to the named owner files; the count audit is `1 + 15 = 16`.

- `musyx/runtime/` (1): `dsp_import.c`. The authenticated donor is a DSP
  firmware byte array, so it is preserved as reference evidence rather than
  counted as clean source recovery.
- `gssdk_lib/` (15):
  - `gsapi/gsapi.c`;
  - `asrpho/asrspi.c` and
    `asrpho/rec1600/{convert,creasp,creaspch,creaspt,creatree,crsptrch,spi1600,train,userword}.c`;
  - `asrpho/common/blocks/{isoword,nbestdp}.c`;
  - `asrpho/common/blocks/flblocks/vad.c`;
  - `asrpho/common/blocks/flfxblks/dist16.c`.

Mixed C/assembly owners stay in the C bucket until all retained C and assembly
passes the appropriate proof. `TRK_MINNOW_DOLPHIN/targimpl.c` no longer appears
here because all 30 retained functions and the linked owner passed that gate.

### Applied source-authentication exception decisions

| MP6 owner | Source authentication | Decision and MP6 proof |
| --- | --- | --- |
| `TRK_MINNOW_DOLPHIN/targsupp.s` | [M5 `src/TRK_MINNOW_DOLPHIN/targsupp.s` at `e246f9d`](https://github.com/mariopartyrd/marioparty5/blob/e246f9d9850ff53ac684b971068fbf87fdcf6acb/src/TRK_MINNOW_DOLPHIN/targsupp.s), `Matching`; M4 carries the identical blob as `MatchingFor(USA,PAL)`. | **Admitted.** The donor blob is `0244131bd8219c6f5839ae2cda9254c2f28e005c`. Its four 8-byte `twui r0,0; blr` functions exactly fill the MP6 `0x20` target owner; source-object and final 137-file/hash/DOL gates pass. It is authentic standalone assembly, not decompiled C. |
| `TRK_MINNOW_DOLPHIN/dolphin_trk.c` | [M5 `src/TRK_MINNOW_DOLPHIN/dolphin_trk.c` at `e246f9d`](https://github.com/mariopartyrd/marioparty5/blob/e246f9d9850ff53ac684b971068fbf87fdcf6acb/src/TRK_MINNOW_DOLPHIN/dolphin_trk.c), configured `Matching`. | **Admitted.** The retained `0x104` `.init` and `0x140` `.text` ranges reproduce all five target functions; donor-only out-of-line copies of `__TRK_copy_vectors` (`0x104`) and `TRK_copy_vector` (`0x90`) are linker-stripped after their logic is inlined into `__TRK_reset`. The semantic `TRK_ISR_OFFSETS` and `lc_base` globals plus target alignment tails are named and the full 137-file/hash/DOL gate passes. `InitMetroTRK` is authentic sibling assembly, so the mixed owner is excluded from clean-C totals. |
| `TRK_MINNOW_DOLPHIN/targimpl.c` | [M5 `src/TRK_MINNOW_DOLPHIN/targimpl.c` at `e246f9d`](https://github.com/mariopartyrd/marioparty5/blob/e246f9d9850ff53ac684b971068fbf87fdcf6acb/src/TRK_MINNOW_DOLPHIN/targimpl.c), configured `Matching`; M4 carries byte-identical source. | **Admitted.** Restoring the real Matching-sibling support prototypes supplies the target's two missing `u8` coercions and makes `TRKTargetSupportRequest` exact at `0x208`. All 30 retained functions and target `.text 0x1894` map at 100%; `.rodata 0x88` and `.data 0x30` are exact. Eight sibling-authentic helper emissions are linker-stripped exactly as in M4/M5, and the final 137-file/hash/DOL gate passes. Seven inline-assembly routines are authenticated by the Matching sibling, so this mixed owner is excluded from clean-C totals. |
| `musyx/runtime/snd_service.c` | [M5 `src/musyx/runtime/snd_service.c` at `e246f9d`](https://github.com/mariopartyrd/marioparty5/blob/e246f9d9850ff53ac684b971068fbf87fdcf6acb/src/musyx/runtime/snd_service.c), configured `Matching`. | **Admitted.** All eight retained routines fill the exact MP6 `0x2D4` linked text range. The two additional source-object lower-half relocations in `sndSqrt` resolve to the target immediates; the linked owner and final DOL are byte-identical. `sndSintab`, `last_rnd`, `sqrtConsts`, `_sinConsts`, and `i2fMagic` recover the semantic data closure without source padding. `sndSqrt` and `sndCos` are authenticated sibling assembly, so the mixed owner is excluded from clean-C totals. |
| `game/kerent.c` | [M4 `src/game/kerent.c` at `147b165`](https://github.com/mariopartyrd/marioparty4/blob/147b165a83187ac9e6cfdc3bf52f2e73437b1ffd/src/game/kerent.c), `MatchingFor(USA,PAL)`, authenticates a single `asm void _kerent` with `nofralloc` and `entry`/`b` pairs. | **Admitted.** `ae2977e` de-flipped it through blanket inline-assembly removal (`ASM-BLANKET-REMOVAL`), despite the written exception. The exact MP6 source was restored from `git show ae2977e^:src/game/kerent.c` (blob `bd5b7b9448a659e186d733e3bc3e13dc8291d6a2`). Target/source `.text` are `0x26A0` and 100% identical; each has 2,472 `R_PPC_REL24` relocations. The final 137-file/hash/DOL gate passes. It is an authenticated assembly jump table, not decompiled C. |
| `Runtime.PPCEABI.H/ptmf.c` | [Mario Kart: Double Dash `libs/PowerPC_EABI_Support/src/Runtime/ptmf.c` at `8a91a6b`](https://github.com/doldecomp/mkdd/blob/8a91a6b4a32834dfb689fbdcf1dea8796be9c75f/libs/PowerPC_EABI_Support/src/Runtime/ptmf.c), configured `Matching`. | **Admitted.** The authenticated `__ptmf_test`, `__ptmf_cmpr`, and `__ptmf_scall` assembly bodies reproduce all `0x94` retained target text bytes with no relocations. Donor-only `__ptmf_scall4` (`0x28`) and `__ptmf_null` (`0xC`) are linker-stripped. The full 137-file/hash/DOL gate passes. This is authentic Runtime assembly and is excluded from clean-C totals. |
| `dolphin/os/OSCache.c` | [M4 `src/dolphin/os/OSCache.c` at `147b165`](https://github.com/mariopartyrd/marioparty4/blob/147b165a83187ac9e6cfdc3bf52f2e73437b1ffd/src/dolphin/os/OSCache.c), `MatchingFor(USA,PAL)`. | **Restored after de-flip.** `93d9438` replaced the authentic bodies and de-flipped the owner while isolating assembly (`ASM-BLANKET-REMOVAL`). All 14 retained MP6 assembly bodies have same-name sibling bodies. Whole-file blobs differ because M4 carries linker-stripped helpers; the audited retained differences are ABI register/symbol spelling, and the MP6 object/effective-relocation/full-container gate passes. |
| `dolphin/os/OSContext.c` | [M4 `src/dolphin/os/OSContext.c` at `147b165`](https://github.com/mariopartyrd/marioparty4/blob/147b165a83187ac9e6cfdc3bf52f2e73437b1ffd/src/dolphin/os/OSContext.c), `MatchingFor(USA,PAL)`. | **Restored after de-flip.** `93d9438` replaced the authentic bodies and de-flipped the owner while isolating assembly (`ASM-BLANKET-REMOVAL`). All nine retained MP6 assembly bodies have same-name sibling bodies. `OSLoadContext` differs textually only in target labels/operand aliases; object/effective-relocation/full-container proof passes. |
| `dolphin/os/OSMemory.c` | [M4 `src/dolphin/os/OSMemory.c` at `147b165`](https://github.com/mariopartyrd/marioparty4/blob/147b165a83187ac9e6cfdc3bf52f2e73437b1ffd/src/dolphin/os/OSMemory.c), `MatchingFor(USA,PAL)`. | **Admitted.** The sibling authenticates `Config24MB`, `Config48MB`, and `RealMode` as the original assembly routines. All eight retained MP6 functions are exact; target/source `.text` is `0x2F0/0x3B4`, with source-only C `OSProtectRange` (`0xC4`) and five extra relocations linker-stripped. All 26 retained relocations, the linked owner, and the 137-file/hash/DOL gate pass. The mixed owner is excluded from clean-C totals. |
| `dolphin/os/OSReset.c` | [M4 `src/dolphin/os/OSReset.c` at `147b165`](https://github.com/mariopartyrd/marioparty4/blob/147b165a83187ac9e6cfdc3bf52f2e73437b1ffd/src/dolphin/os/OSReset.c), `MatchingFor(USA,PAL)`. | **Admitted.** The sibling authenticates `Reset`; restoring it also recovers the target `__OSDoHotReset` inlining shape. All seven retained functions are exact. Target/source `.text` is `0x484/0x634`; source-only `__OSShutdownDevices` (`0x1B0`) and 11 extra relocations are linker-stripped, while all 47 retained relocations match. Target `.data 0x50` versus semantic source `0x4E` is split tail padding. The linked owner and full gate pass; the mixed owner is not clean C. |
| `dolphin/os/__start.c` | [M4 `src/dolphin/os/__start.c` at `147b165`](https://github.com/mariopartyrd/marioparty4/blob/147b165a83187ac9e6cfdc3bf52f2e73437b1ffd/src/dolphin/os/__start.c), `MatchingFor(USA,PAL)`. | **Admitted.** The sibling authenticates `__start` and `__init_registers` and the surrounding startup source family. All seven retained MP6 functions are exact. Target/source `.init` is `0x300/0x38C`; source-only copy/BSS helpers total `0x8C` with three extra relocations and are linker-stripped. All 30 retained relocations, linked startup range, and full gate pass. This is authentic startup assembly, not clean C. |
| `dolphin/gx/GXTransform.c` | [M4 `src/dolphin/gx/GXTransform.c` at `147b165`](https://github.com/mariopartyrd/marioparty4/blob/147b165a83187ac9e6cfdc3bf52f2e73437b1ffd/src/dolphin/gx/GXTransform.c), `MatchingFor(USA,PAL)`. | **Admitted.** The sibling authenticates the paired-single projection/copy/matrix FIFO helpers. All 16 retained MP6 functions, `.text 0x72C`, `.sdata2 0x10`, and 31 retained relocations match exactly. Six helpers plus `__GXSetProjection` total `0x10C` of source-only text; their bodies inline into the retained functions and the out-of-line copies are linker-stripped. The linked owner and full gate pass. The paired-single helpers are authentic assembly and excluded from clean-C totals. |
| `dolphin/os/OS.c` | [M4 `src/dolphin/os/OS.c` at `147b165`](https://github.com/mariopartyrd/marioparty4/blob/147b165a83187ac9e6cfdc3bf52f2e73437b1ffd/src/dolphin/os/OS.c), `MatchingFor(USA,PAL)`. | **Admitted.** The sibling authenticates the FPR, vector, default-exception, paired-single, and init assembly family. Twelve of 14 retained functions are raw-object exact. `OSInit` (`0x4E0`, 97.500000%) and `OSExceptionInit` (`0x280`, 98.500000%) retain the target instructions and addends; objdiff names target `@1` versus compiler-created `OS.c ...data.0`, both at `.data+0`. Data bytes match through `0x1F6`, the target has one tail byte, and `.sdata 0x10` is exact. Target/source `.text` are `0xACC/0xC2C` with 166/191 relocations because sibling-authentic source-only helpers are linker-stripped. The effective linked owner and 137-file/hash/DOL gate pass. This mixed owner is excluded from clean-C totals. |
| `dolphin/os/OSExec.c` | [dolsdk2004 `src/os/OSExec.c` at `2328b416`](https://github.com/doldecomp/dolsdk2004/blob/2328b4164b1a98422a2255d83ce9a5a7548990cc/src/os/OSExec.c); that complete vendor decompilation reports the OS library 100%. | **Admitted.** The vendor source authenticates `Run` as a `0x3C` `nofralloc` body with cache invalidation, `sync`/`isync`, entry transfer, and its unreachable epilogue; all 15 instructions are byte-identical to MP6. Restoring it also recovers the target inlining/call shape, making all eight retained functions and effective relocations exact. Target/source `.text` are `0x960/0xD28` with 90/129 total relocations because authentic helpers are emitted only on the source side and linker-stripped. `.data` is `0x10/0xB` and `.sdata` is `0x8/0x3`, each semantic string plus target split padding; `.sbss 0x8` is exact. The linked owner and 137-file/hash/DOL gate pass. This mixed owner is excluded from clean-C totals. |
| `game/jmp.c` | [M4 `src/game/jmp.c` at `147b165`](https://github.com/mariopartyrd/marioparty4/blob/147b165a83187ac9e6cfdc3bf52f2e73437b1ffd/src/game/jmp.c), `Matching`. | **Restored after de-flip.** `ae2977e` de-flipped the owner through blanket inline-assembly removal (`ASM-BLANKET-REMOVAL`). MP6 and M4 are the exact Git blob `8313bd8cb5c154bca116a9a75a518e8faa0a5856`; the owner contains C `gcsetjmp` plus authentic assembly `gclongjmp`, and passes the MP6 full gate. |
| `musyx/runtime/Chorus/chorus_fx.c` | [M5 `src/musyx/runtime/Chorus/chorus_fx.c` at `e246f9d`](https://github.com/mariopartyrd/marioparty5/blob/e246f9d9850ff53ac684b971068fbf87fdcf6acb/src/musyx/runtime/Chorus/chorus_fx.c), `Matching`. | **Admitted.** The authenticated `do_src1` and `do_src2` `static asm` bodies accompany three retained C functions. Target/source `.text` are `0x868/0x900`; the donor-only `0x98` update helper and its constants are linker-stripped, and the linked owner/full-container gate passes. This mixed owner is not clean decompiled C. |
| `musyx/runtime/CheapReverb/creverb.c` | [M5 `src/musyx/runtime/CheapReverb/creverb.c` at `e246f9d`](https://github.com/mariopartyrd/marioparty5/blob/e246f9d9850ff53ac684b971068fbf87fdcf6acb/src/musyx/runtime/CheapReverb/creverb.c), `Matching`. | **Admitted.** The authenticated `HandleReverb` `static asm` body accompanies three retained C functions. Target/source `.text` are `0x8BC/0xBA4`; four donor-only C helpers and their constants are linker-stripped, and the linked owner/full-container gate passes. This mixed owner is not clean decompiled C. |

### OSFastCast re-verification after `93d9438`

Commit `93d9438` replaced the authenticated paired-single implementation in
`OSFastCast.h` with scalar C and de-flipped four consumers solely because that
header/include-graph change invalidated their prior byte proof
(`HEADER-INVALIDATED`). Its separate OSCache/OSContext assembly de-flips are
recorded above. The current header is again byte-identical to the M4 and M5
Matching donor blob `b52cabfe94bf7882b26318063efba922fb977534`.

| Owner | Current decision | Fresh object evidence |
| --- | --- | --- |
| `game/hsfdraw.c` | `Matching`; `93d9438` reason was `HEADER-INVALIDATED` | 54/54 functions and `.text 0xCF00` are 100%; raw `.rodata`/`.sdata` attribution differences are compiler-local split labels, and the linked range/container are exact. |
| `game/hsfanim.c` | `Matching`; `93d9438` reason was `HEADER-INVALIDATED` | 71/71 functions and `.text 0x65CC` are 100%; linked range/container exact. |
| `game/init.c` | `Matching`; `93d9438` reason was `HEADER-INVALIDATED` | `.text 0x998` is 99.943% in raw objdiff because seven `LoadMemInfo` operands pair target labels with compiler-local labels at the same offsets; effective relocations and linked bytes are exact. |
| `board/roulette.c` | `Matching`; `93d9438` reason was `HEADER-INVALIDATED` | `.text 0x1838` is 99.735% in raw objdiff; all reported differences are target `@sda21` names versus compiler-local constant names at the same effective addresses. Linked bytes and the container are exact. |
| `board/effect.c` | `NonMatching` | Genuine divergence: target/source `.text` are `0x3050`/`0x3074`, data-value score 59.852844%, only 1/35 functions is exact, `.sdata` is `0x18`/`0x14`, and `.sdata2` scores 57.353%. It was already NonMatching before `93d9438`; it was not de-flipped by the header rework. |

The four flips above are accepted only because their compiled source objects
are selected exactly once by `configure.py` and the final `ninja`, explicit
DTK SHA manifest, and original-vs-built `main.dol` comparison all pass.

### Board status recovery and branch evidence

`board/status.c` is now Matching clean C. `StatusUpdate` reuses the existing
function-scoped `i` and `j` locals for the single-board space-number path and
uses explicit comparisons around `GWPartyGet`; that source shape produces the
target register lifetimes and exact `0x718` function. All 44 functions,
`.text 0x3CB8`, `.rodata 0x18`, `.data 0x3F4`, `.bss 0xD8`, `.sbss 0x8`,
`.sdata2 0x58`, and 807 relocations match. The old `.data` end at `0x802485A8`
incorrectly assigned the four-byte linker-alignment gap before
`board/opening.c` to this owner; the proven owner ends at `0x802485A4`.

`board/branch.c` remains NonMatching. Reusing function-scoped `i` for the
tutorial result and placing `padNo` before `masuPlayer` removed its stack
spill, eight-byte size excess, and an unrelated register cycle. The only
remaining `ev_Branch` differences are 19 operands in the coupled
`choice`/`choiceTime` `r26`/`r27` assignment. No speculative declaration or
scope rewrite is accepted without resolving that cycle. Full proof details
are retained in [`docs/native_matching_wave17.md`](docs/native_matching_wave17.md).

`board/audio.c` remains NonMatching, but four delayed-effect functions now
have exact source: `mbAudFXPlay`, `mbAudFXPosPlay`, `mbAudFXEmitterPlay`, and
`mbAudGuidePlay` (combined target/source code size `0x518`). The generic
reconstructed helper did not preserve the target call-site lifetimes; spelling
out the delayed queue paths and retaining the small position-copy helper makes
all four instruction streams and their relocations exact. This function-level
recovery is not counted as a matching owner. The object and rejected-probe
evidence is retained in
[`docs/native_matching_wave18.md`](docs/native_matching_wave18.md).

`msm/msmstream.c` now has exact source for both asynchronous DVD callbacks.
The target tests the callback `result` argument directly rather than issuing a
second DVD status query, and its status-5 path deactivates and clears the
stream before closing it. Together `msmStreamDvdCallback` and
`msmStreamDvdCallback2` account for `0x550` bytes of newly exact source. The
owner remains NonMatching at 23/28 exact functions, so none of its fallback-
linked bytes are counted as decompiled source. Proof and rejected-probe details
are retained in
[`docs/native_matching_wave19.md`](docs/native_matching_wave19.md).

`Runtime.PPCEABI.H/GCN_mem_alloc.c` is now Matching clean C. Both `0xB8`
functions, all 32 relocations, and the 113 semantic constant bytes match; the
target's seven-byte zero tail is reproduced by linker alignment. Four other
Runtime owners now have authenticated source candidates without being
promoted: `New.cp` has two exact delete functions (`0x98` code bytes),
`NMWException.cpp` has seven exact functions (`0x3F4` bytes), while
`NewMore.cp` and `Gecko_ExceptionPPC.cpp` retain documented objdiff/linker-
closure differences. All four remain fallback-linked and are not counted as
decompiled owners. The accepted and rejected evidence is retained in
[`docs/native_matching_wave20.md`](docs/native_matching_wave20.md).

Seven recent MSL owners are now Matching clean C. The authenticated sibling
sources for `ansi_fp.c`, `file_io.c`, `mbstring.c`, `mem_funcs.c`, and
`e_pow.c` require owner-local `GC/1.3`; direct probes proved that `GC/2.6`
changes their inline and fdlibm code shape. The target-led `string.c` and
`math_ppc.c` closure instead matches under the configured `GC/2.6`. Together
the seven owners contribute 34 exact functions, `0x1F28` linked code bytes,
`0x358` configured constant/data bytes, and 245 relocations. `printf.c` now
retains authenticated source and nine exact functions totaling `0xC78`, but
remains fallback-linked because the formatter closure genuinely diverges.
The accepted and rejected evidence is retained in
[`docs/native_matching_wave21.md`](docs/native_matching_wave21.md) and
[`docs/native_matching_wave25.md`](docs/native_matching_wave25.md).

Wave 28 adds three more Matching clean-C MSL owners: `assert.c`, `s_atan.c`,
and `w_log.c`. The target establishes the older three-argument assertion
routine and its exact diagnostic string; the source function and all six
relocations match, while the target object's `0x30` rodata includes the
linker's zero alignment tail. The Matching-MP5 fdlibm `s_atan.c` source under
owner-local `GC/1.3` reproduces `0x200` text bytes, `0xC0` constant bytes, and
ten effective relocations. `w_log.c` reproduces its authentic old-style
`0x20` wrapper and sole relocation under `GC/2.6`. Together the three owners
add `0x26C` linked code bytes, `0xF0` configured constant bytes, and 17
relocations. Evidence and rejected donor probes are retained in
[`docs/native_matching_wave28.md`](docs/native_matching_wave28.md).

Wave 29 adds `OdemuExi2/DebuggerDriver.c` as Matching clean C and
`Runtime.PPCEABI.H/ptmf.c` under the sibling-assembly exception. The Odemu
owner recovers 14 retained functions, seven real local globals, `0xA80` text
bytes, `0x20` configured small-data/BSS bytes, and 57 effective relocations
under its authenticated `GC/1.2.5n` deferred-inline build shape. Six unused
SDK helpers totaling `0x270` are linker-stripped. Runtime `ptmf` contributes
three exact retained assembly routines totaling `0x94`; Matching MKDD
authenticates their original source form, so none of those bytes enter the
clean-C total. Evidence is retained in
[`docs/native_matching_wave29.md`](docs/native_matching_wave29.md).

Three MusyX owners are now Matching under the sibling-assembly exception:
`CheapReverb/creverb.c`, `Chorus/chorus_fx.c`, and `snd_service.c`. Their 17
retained functions contribute `0x13F8` exact linked code bytes. Each owner
retains authenticated Matching-MP5 assembly and is therefore excluded from
clean-C totals. At the wave-22 boundary, probes of the other Matching-MP5
candidates `hw_dolphin.c` and `seq.c` were rejected on target/source text
sizes `0x4F8/0x5A4` and `0x3FCC/0x5168` respectively, and fully removed. Wave
42 later proves that `seq.c`'s extra source text is entirely linker-stripped
after its retained helper/global identities are recovered; `hw_dolphin.c`
remains rejected. The first two accepted owners are documented in
[`docs/native_matching_wave22.md`](docs/native_matching_wave22.md).

Wave 30 adds the Matching-MP5 `dolphin_trk.c` and `snd_service.c` mixed owners
under the sibling-assembly exception. `dolphin_trk` contributes `0x104` init
and `0x140` text bytes; its five retained functions, real interrupt-vector
table, and local-cache base match, while two out-of-line helpers totaling
`0x194` are linker-stripped. `snd_service` contributes eight retained
functions and `0x2D4` text bytes. Its six C routines plus authenticated
assembly `sndSqrt`/`sndCos`, five real data symbols, and target-proven
inter-owner alignment reproduce the exact linked range. Together the owners
add `0x518` linked code bytes and `0x898` configured data/BSS bytes, but no
clean-C owner. The initial `targimpl.c` probe was correctly rejected at 29/30
exact functions and removed. Wave 38 later recovered the missing support
prototypes, closed the final function, and admitted the owner through the full
exception gate. The original rejection is retained in
[`docs/native_matching_wave30.md`](docs/native_matching_wave30.md); the resolved
proof is in [`docs/native_matching_wave38.md`](docs/native_matching_wave38.md).

Wave 23 advanced two fallback owners at function level. `board/object.c` has
since passed the complete object and DOL gates and is reported below as
Matching clean C. `msm/msmstream.c::msmStreamData` retains the target pause,
linked-slot, buffer-clear, and ARAM-update behavior at the exact `0x2EC` size;
`msmStreamSlotInit` retains the target's distinct half-buffer/read-size/
wrap-distance lifetimes. Those two stream functions remain 99.759360% and
99.635040% respectively, with only operand-allocation differences. The stream
owner stays `NonMatching`, and none of its fallback-linked bytes are counted
as decompiled source. Evidence and bounded rejected probes are retained in
[`docs/native_matching_wave23.md`](docs/native_matching_wave23.md).

`board/object.c` is now Matching clean C. Recovering the two motion-create
functions and the target `.sbss` emission order makes all 80 functions,
`0x2838` text bytes, `0x18` BSS bytes, `0x48` constant bytes, and 334 text
relocations exact. The first linked trial was deliberately rejected despite a
100% raw object score because its zero-filled BSS globals had different symbol
offsets; the corrected declaration order reproduces every target offset and
passes the final DOL gate. Source provenance, the rejected `board/audio.c`
probes, and full proof are retained in
[`docs/native_matching_wave26.md`](docs/native_matching_wave26.md).

Wave 27 materially advances `board/masu.c` without promoting the owner.
Twenty-one formerly absent functions now have clean C, including the
lifecycle/display setup, next-space state, event-bit transforms, corner
ranking/rotation, capsule ownership checks, and player prize reset. Fourteen
new functions totaling `0xA28` bytes are fully exact. Seven more totaling
`0xCAC` have target-identical sizes and instruction sequences; objdiff reports
only compiler-local numeric-pool or static-table relocation identities caused
by the still-incomplete owner. The source object now maps 112/119 functions
and has 103 exact. The seven remaining absent functions are the large GX draw
paths, event state machines, player-color logic, and mushroom-value routine;
they were left unimplemented rather than guessed. Evidence is retained in
[`docs/native_matching_wave27.md`](docs/native_matching_wave27.md).

Runtime allocation-exception recovery now names the map-authenticated
`std::__new_handler` global and the sibling-authenticated
`std::__throws_bad_alloc` state, and restores the `std::bad_alloc :
std::exception` class closure, including RTTI and vtables.
In `NewMore.cp`, both virtual destructors plus `extab` and `extabindex` are
exact; the exact-size throw helper differs only in two pooled-string label
operands. The canonical allocation bodies and two authenticated
`__construct_array` spellings were compiled and rejected where they still
diverged. `New.cp`, `NewMore.cp`, and `NMWException.cpp` therefore remain
`NonMatching`, and none of their fallback-linked bytes are counted as
decompiled owners. Evidence is retained in
[`docs/native_matching_wave24.md`](docs/native_matching_wave24.md).

### Active REL fallback taxonomy

The same two buckets apply to every configured REL fallback owner. These five
owners are separate from the 396-owner DOL ledger:

| Owner | Bucket and reason | Authentication or current evidence |
| --- | --- | --- |
| `REL/runtime.c` | `original-was-asm`; `ASM-GATE-PENDING`; longstanding fallback, never de-flipped | MP5 `src/Runtime.PPCEABI.H/runtime.c` authenticates the compiler-assembly source family. This sibling checkout does not provide a locally runnable Matching manifest for the owner, so the exception has not been exercised. |
| `REL/selmenuDll/runtime.c` | `original-was-asm`; `ASM-GATE-PENDING`; longstanding fallback, never de-flipped | Same MP5 Runtime source authentication; MP6 object/link proof remains pending. |
| `REL/fileseldll/runtime.c` | `original-was-asm`; `ASM-GATE-PENDING`; longstanding fallback, never de-flipped | Same MP5 Runtime source authentication; MP6 object/link proof remains pending. |
| `REL/meschkdll/meschkdll.c` | `C-not-yet-matched`; `SRC-DIVERGES`; never de-flipped | Five functions are exact; `fn_1_188` remains divergent, so the owner stays fallback-linked. |
| `REL/mdpartydll/mdparty.c` | `C-not-yet-matched`; `SRC-DIVERGES`; new owner, never de-flipped | 181/258 functions and `0xEAB4/0x3F424` target text bytes are represented. 177 functions and `0xDF8C` bytes are exact. Wave 84 adds 24 represented functions, of which 23 are exact. The represented residues are `fn_1_2B0` (`0x150/0x14C`, 98.095240%), its `fn_1_400` inline copy (`0x24C/0x248`, 98.911570%), `fn_1_22A8` (`0x454/0x454`, 99.444046%), and new WIP `fn_1_2EBEC` (`0x338/0x338`, 99.902916%). No fake local, register force, or unproved Matching flip was admitted. |

Wave 70 resolves the two former `mdseldll` fallback rows. Application owner
`REL/mdseldll/mdsel.c` leaves `C-not-yet-matched` after all 113 functions and
the linked REL prove exact. Runtime owner `REL/mdseldll/runtime.c` leaves
`original-was-asm`/`ASM-GATE-PENDING` under the sibling-authentication
exception: ProjectPiki/Pikmin `24378f0e`
`src/Runtime/PPCEABI/H/runtime.c` supplies the Matching-owner source-shape
citation, MP5 `e246f9d` corroborates the family, and the MP6 object plus full
container gate proves the admitted target bytes. This promotion does not
reclassify its assembly as clean C.

Wave 80 resolves `REL/mdpartydll/runtime.c` in the same exception bucket.
ProjectPiki/Pikmin `24378f0e`,
`src/Runtime/PPCEABI/H/runtime.c`, supplies the Matching-owner source-shape
citation; MP5 `e246f9d` corroborates the compiler-runtime family. The MP6
object proves all 19 functions, `.text 0xA44`, and `.rodata 0x18` exact, and
the final 137-file, `main.dol`, and `mdpartydll.rel` gates are byte-identical.
The runtime is source-linked assembly and contributes zero clean-C bytes.

Wave 81 does not change any fallback bucket or exercise another assembly
exception. It adds 69 represented functions and `0x5420` target bytes against
the corrected Wave 80 baseline, while adding 69 exact functions and `0x56E8`
exact bytes after closing both former stage residues. `mdparty.c` and `stage.c`
remain `C-not-yet-matched`; the full 137-file gate and both `main.dol` and
`mdpartydll.rel` comparisons remain byte-identical. Detailed function, layout,
and no-fakematch evidence is retained in
[`docs/native_matching_wave81.md`](docs/native_matching_wave81.md).

Wave 82 removes `REL/mdpartydll/stage.c` from `C-not-yet-matched`. The nine
former holes add `0x2B1C` target bytes, bringing the owner to 57/57 functions
and exact `.text 0x73D8`, `.rodata 0x178`, `.data 0x24`, `.bss 0xE4`, and
effective relocations. The real by-value `GXColor` ABI and compound-literal
temporary order replace the former pointer-shaped approximation. The
authenticated SDK math-header plus `pool_data off` recipe reproduces the
target object without claiming the linker-discarded weak `sqrtf` copy. The
owner is now clean C and `Matching`; `mdparty.c` remains the module's sole C
fallback owner. The 137-file checksum and both `main.dol` and
`mdpartydll.rel` comparisons remain byte-identical. Detailed evidence is in
[`docs/native_matching_wave82.md`](docs/native_matching_wave82.md).

Wave 83 adds 46 exact `REL/mdpartydll/mdparty.c` functions and `0x30E4`
target text bytes. The recovered spans cover party HUD presentation, player
and animation setup, object-state transitions, camera/wipe flow, and selection
state scans. Target register chronology plus MWCC inliner behavior recovers the
real `fn_1_89E0(obj)` helper call in `fn_1_8D20`; the independently exact
helper expands to the target's nested FPR pattern. The same evidence recovers
the `fn_1_C158` calls in `fn_1_22010`, real 32-bit model-number ABI at two
sprite/model boundaries, and the `OMOBJ::mtnId` state owner at offset `0x48`.
No assembly, fake local, register force, byte packet, or source padding was
admitted. `mdparty.c` remains fallback-linked because 101 functions are still
unrepresented and three earlier represented functions still diverge. The
full 137-file checksum and both `main.dol` and `mdpartydll.rel` comparisons
remain byte-identical. Detailed evidence is in
[`docs/native_matching_wave83.md`](docs/native_matching_wave83.md).

Wave 84 materializes another 24 `REL/mdpartydll/mdparty.c` functions and
`0x3A9C` target text bytes. Twenty-three functions and `0x3764` bytes are
exact; `fn_1_2EBEC` remains an explicitly non-exact WIP at 99.902916% because
the constant-address temporary is colored into source `r3` instead of target
`r4`. Recovered source shape includes the real `0x38`-byte character-model
entry with vector position, rotation, and scale fields, the exact
`fn_1_D3E0` model-transition helper, signed-16-bit narrowing at the two
`fn_1_C158` call boundaries, and nested `fn_1_A880`/`fn_1_9A24` inlining that
reproduces the target allocation order in `fn_1_2C874`. No assembly, register
force, fake local, byte packet, or speculative residue fix was admitted. The
owner remains fallback-linked with 77 functions unrepresented and four
represented residues. The full 137-file checksum and both `main.dol` and
`mdpartydll.rel` comparisons remain byte-identical. Detailed evidence is in
[`docs/native_matching_wave84.md`](docs/native_matching_wave84.md).

## Named DOL ownership

The current snapshot descends from fork commit `353fa30`, which replaced every
DOL `auto_*` blob with 121 named Runtime, MSL, MusyX, MetroTRK, and
support-library owners. The pre-wave-26 `fork/main` tip was `f69d158`. Neither
`config/GP6E01/splits.txt` nor `configure.py` contains an `auto_*` owner.

## Native library recovery

The consolidated recovery series through `685f514` contains 103 accepted DOL
owners: 97 net-new promotions relative to `353fa30` plus six restorations of
owners that were already `Matching` there. Together they account for 188,368
code bytes and 46,100 configured data/BSS bytes. The latest consolidated wave
contributes 52 of those owners, 166,580 code bytes, and 34,860 data/BSS bytes.
The kerent exception restored by this policy correction is reported
separately and is deliberately excluded from clean-C totals. The subsequent
clean-C `board/status.c` recovery adds `0x3CB8` code bytes and `0x544`
configured data/BSS bytes.

- Nine core MSL owners: `errno`, `arith`, `float`, `s_copysign`, `w_acos`,
  `w_asin`, `w_atan2`, `w_fmod`, and `w_pow`. Seven owners are whole-section
  exact. `arith` retains an exact `abs` while the sibling-authenticated unused
  `labs` is linker-stripped; `errno` owns an exact four-byte symbol followed by
  the target split's four-byte alignment gap.
- Six GC/1.3-shaped MSL owners: `ansi_fp`, `file_io`, `mbstring`,
  `mem_funcs`, `e_pow`, and `s_atan`. Their sibling-authenticated C contributes
  7,360 exact linked code bytes and 1,048 configured data bytes. Object-level symbol
  attribution differences in `ansi_fp` and `e_pow` are resolved by the exact
  linked owner ranges; `s_atan` likewise resolves compiler-local pool labels
  through its exact linked range. No assembly or source padding was added.
- Four GC/2.6-shaped MSL owners: `string`, `math_ppc`, `assert`, and `w_log`.
  Their 20 clean-C functions contribute `0x4D4` exact linked code bytes,
  `0x30` configured constant bytes, and 18 relocations.
  `printf` additionally retains nine exact functions but remains a fallback
  owner and is not counted as decompiled source.
- Two Runtime owners: `__va_arg` is exact at `0xC8`; `__mem` is exact at
  `0x138`, including the target order `memset`, `__fill_mem`, `memcpy` and its
  sole `R_PPC_REL24` relocation.
- Runtime `GCN_mem_alloc` adds `0x170` exact code bytes and `0x78` configured
  constant bytes. The target's final seven constant bytes are documented
  linker-alignment padding, not fabricated source data.
- `OdemuExi2/DebuggerDriver` adds `0xA80` exact clean-C code bytes, `0x20`
  configured small-data/BSS bytes, 14 retained functions, seven named local
  globals, and 57 effective relocations. Six authenticated unused SDK helpers
  are linker-stripped from the exact owner range.
- Runtime `ptmf` adds `0x94` exact retained code bytes under the Matching-MKDD
  sibling exception. Its three `nofralloc` routines are authentic assembly,
  so this owner is reported separately from clean C.
- Six MusyX owners: `hw_memory`, `synth_ac`, `synth_dbtab`, `seq_api`,
  `creverb_fx`, and `reverb_fx`. These promote 1,392 code bytes and 2,976 data
  bytes and recover the `dspAttenuationTab`, `dspScale2IndexTab`,
  `dspDLSVolTab`, `seqMIDIPriority`, `synthInfo`, and `salHooks` owners.
- MusyX `seq` adds 17 retained clean-C functions, `0x3FCC` linked code bytes,
  and `0xD9D0` configured data/BSS/constant bytes. The larger same-version
  source object closes only after five helper and ten owner-local storage
  identities are recovered; all other donor helpers/constants are
  linker-stripped.
- Three MusyX sibling-exception owners, `CheapReverb/creverb`,
  `Chorus/chorus_fx`, and `snd_service`, add 5,112 exact linked code bytes.
  Their five retained assembly routines are authenticated by Matching Mario
  Party 5 sources, so these owners are reported separately from clean C.
- Five additional native owners: MSL `misc_io` and `signal`, plus MusyX
  `delay_fx`, `hw_volconv`, and `snd_math`. They promote 3,868 code bytes and
  664 data bytes.
- Eleven fdlibm owners: `s_tan`, `s_frexp`, `k_sin`, `s_cos`, `s_sin`,
  `s_modf`, `s_floor`, `s_ldexp`, `e_atan2`, `e_fmod`, and `k_rem_pio2`.
  Their 7,032 text bytes, 392 constant bytes, and 108 relocations match.
- Seven MetroTRK core owners: `dispatch`, `main_TRK`, `mem_TRK`, `msgbuf`,
  `mutex_TRK`, `target_options`, and `usr_put`. They promote 3,292 code bytes
  and 6,768 data bytes.
- Five MetroTRK control-path owners: `mainloop`, `mslsupp`, `notify`,
  `nubinit`, and `targcont`. Their eight retained functions contribute 1,276
  code bytes, 40 configured data bytes, and 45 exact relocations. `nubinit`'s
  semantic data is followed by four-byte rodata and BSS alignment tails.
- MetroTRK `dolphin_trk` adds `0x244` exact retained code bytes under the
  Matching-MP5 sibling exception. `InitMetroTRK` is authentic assembly; two
  donor-only out-of-line helpers totaling `0x194` are linker-stripped.
- `GXLight` contributes 1,760 code bytes, 104 configured data bytes, 13 exact
  functions, and 62 relocations. Its only inline assembly is the authenticated
  Matching-donor paired-single upload helper; the target split ends with a
  four-byte constant-data alignment tail.
- Five deferred-emission MSL owners: `mem`, `uart_console_io`, `direct_io`,
  `FILE_POS`, and `abort_exit`. Their 2,440 text bytes, 280 configured data/BSS
  bytes, 13 functions, and 51 relocations match. Authenticated GC/2.6
  `-inline deferred` emission produces the target function order without
  reordering source; `abort_exit` owns the exact `__atexit_funcs` table and
  four small-data globals.
- Six game and board owners: `init`, `jmp`, `malloc`, `hsfdraw`, `hsfanim`,
  and `roulette`. They contribute 89,256 code bytes and 18,336 data/BSS bytes;
  all 172 retained functions and 4,304 target relocations pass the strict
  linked-range gate.
- Board object management contributes 10,296 exact clean-C code bytes, 96
  configured BSS/constant bytes, 80 functions, and 334 target relocations.
  Its small-data global offsets are target-proven rather than inferred from
  zero-filled section bytes.
- Seventeen Dolphin SDK owners: `PPCArch`, `OSAlarm`, `OSCache`, `OSContext`,
  `OSInterrupt`, `OSSync`, `OSTime`, `__ppc_eabi_init`, `db`, `mtx`, `mtxvec`,
  `mtx44vec`, `vec`, `quat`, `psmtx`, `ai`, and `THPDec`. Their 48,164 code
  bytes, 2,264 configured data/BSS bytes, 179 retained functions, and 770
  target relocations match. Each inline-assembly owner is also `Matching` in
  an authenticated MP4 or MKDD sibling.
- Thirty-one Runtime, MSL, TRK, MusyX, and stub owners contribute 36,032 code
  bytes, 15,924 configured data/BSS bytes, 150 retained functions, and 1,067
  target relocations. This includes the DOL Runtime owner,
  `__init_cpp_exceptions`; five low-level/mixed and five clean-C TRK owners;
  14 MSL owners; three MusyX synth owners; and the AmcExi2 and Odemu stubs. Target
  object order is preserved for the exception init/fini and AmcExi2 functions.
  Relocations also recover real ownership for
  `__ctype_map`, `__lower_map`, `synthVoice`, the aux callback and MIDI globals,
  `synthFlags`, `vs`, and `gWriteBuf`. The address, definition, and target
  relocation ledger is retained in `docs/easy_ports_wave.md`.

The 102 current DOL fallback owners and their causes are exhaustive in the
two-bucket ledger above: 101 are C work and one is an assembly-policy decision.
The six configured REL fallback owners are catalogued separately above and are
not included in those DOL counts.
