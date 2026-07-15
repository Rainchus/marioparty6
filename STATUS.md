# Mario Party 6 recovery status

This is an evidence snapshot, not a completion claim. It was last verified on
2026-07-15 with the pinned project toolchain.

## Verification

- `ninja -j1`: `137 files OK` (the DOL and all configured REL outputs)
- `build/tools/dtk.exe shasum -q -c config/GP6E01/build.sha1`:
  `137 files OK`
- `cmp orig/GP6E01/sys/main.dol build/GP6E01/main.dol`: byte-identical
- `build/GP6E01/main.dol` SHA-1:
  `b897e6ade6b3a0cd2f9907689f38a3b19c327e70`
- DTK progress at that build: 9.04% code and 31.18% data overall; 46.30% code
  and 74.09% data in the DOL
- Matching owners at that build: 293 of 895 overall, 284 of 396 in the DOL,
  and 9 of 499 in the REL modules
- DOL policy split: 251 matching owners without the assembly exception, 33
  matching owners admitted under the sibling-authentication exception, 111
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
functions, and the `logexp` lookup helper are byte-exact, but every owner
remains fallback-linked because its complete C object still diverges. The
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

## Assembly fallback boundary

Assembly is never admitted merely because a C transcription did not match.
Inline or standalone assembly is admitted only when an authenticated sibling
project marks the same owner `Matching`, carries the corresponding instruction
body/source shape, and the MP6 compile, object-byte, effective-relocation,
linked-range, and container gates all pass. The current tree has 33 matching
exception owners:

- 17 Dolphin SDK owners authenticated by Matching Mario Party 4 sources, with
  `mtx44vec` independently authenticated by Matching Mario Kart: Double Dash
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
  policy/proof decision, recoverable only through the sibling-authentication
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

#### Bucket 2: `C-not-yet-matched` (111 current fallback owners)

Eighty-seven owners have current source candidates and are tagged
`SRC-DIVERGES`.
These DTK 0.9.2 object comparisons were regenerated from the current source
and target splits on 2026-07-15; percentages are raw `.text` scores unless
otherwise noted.

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
| `board/board.c` | 99.583%; 31/35 functions exact; `mbObjectSetup`, `mbMain`, `mbNextTime`, and `mbSaveInit` diverge. |
| `board/math.c` | Matching MP5 source shape plus MP6 instructions and MP7 parity now recover 51/56 target functions: lifecycle, trigonometry, matrix construction/concatenation, projection, Bezier, Hermite, distance, and angle routines. Target/source `.text` are `0x2A44/0x2230`; `mbMathClose` is exact. `mbRandMod` and the cull/object tail remain absent rather than using unproven ownership. |
| `board/telop.c` | Sixteen pad, taunt, language, board-directory, telop-check, and time-display routines plus their real tables/globals are recovered. Target/source `.text` are `0x329C/0x8E0`; 6/16 mapped functions are exact, including five wave-38 additions. The large telop/time/taunt OM execution closure remains absent. |
| `board/coin.c` | Twenty-two allocation, lifecycle, transform, alpha, display, layer, and motion routines are recovered. The target proves the `0x40` public object, its caller-work tail used by `board/last5.c`, the `0x1144` 64-slot bank, and the `0x160` model/bank owner. Target/source `.text` are `0x4550/0xF44`; 5/22 mapped functions are exact. The renderer/effect/display closure remains absent. |
| `board/dice.c` | Six layout-free result/process/hook accessors and the exact five-slot owner arrays are recovered from MP6 target data plus Matching MP5 `sai.c`/`sai.h` shape. Target/source `.text` are `0x6390/0x130`; none of the six is byte-exact yet. Dice work and the state-machine closure remain absent. |
| `board/single.c` | The complete four-word minigame-unlock set and all seven bitset operations are recovered. Target/source `.text` are `0x98F0/0x2B4`; `mbSingleMgUnlockInit`, `mbSingleMgUnlockCheckAny`, and `mbSingleMgUnlockNumGet` are exact. The rest of the large single-mode owner remains absent. |
| `board/star.c` | Sixteen layout-free callback/global setters, Num/Flag/Next accessors, empty legacy entry points, and the no-random result are recovered and exact. Target/source `.text` are `0x5C00/0xDC`. Object wrappers were rejected because a real complete `STARWORK` prefix is not yet recovered. |
| `board/tutorial.c` | The real `0x14` call work (`scene`, `callNum`, `result`, `stat`, and `mode`), guide/process globals, exit flags, and ten accessors are recovered. `mbTutorialMultiCall` and `mbTutorialCall` prove the count field. All ten mapped functions are exact; target/source `.text` are `0x2C34/0x80`. |
| `board/wipe.c` | The create/wait functions and all twelve fixed-time/caller-time fade, white-fade, and dissolve wrappers are recovered. All 14 mapped functions are exact; target/source `.text` are `0x3664/0x6DC`. The special-wipe and state-machine remainder is still absent. |
| `board/capselect.c` | Eighteen result, story/type, capsule lookup/count, callback, and map-capsule object routines are recovered with the target-proven four-player result/model/type arrays and 16-entry map-object table. Target/source `.text` are `0x3460/0x32C`; 16/18 functions are exact. `mbCapSelectShrinkCheck` is 68.333336% and `CapSelectCapsuleGet` is 97.558136%. The selection state machines and unresolved `0x28` map-object work remain absent. |
| `board/capsule.c` | Twenty-nine retained target functions recover the four-player effect state, `s16 capsuleNum[33][2]`, packed space-capsule/player helpers, and the real `0x2C` `CAPSULE_OBJ_COLOR` over 128 entries. Target/source `.text` are `0x146CC/0x1130`; 18/29 mapped functions are exact. The compiler also retains the sibling-authenticated out-of-line search helper, which has no named target pairing. `mbCapObjColorCreate` is omitted because the capsule-data field at `0x1C` is not yet semantically closed. |
| `board/config.c` | Matching MP5 pause-copy shape plus MP6 instructions recover the framebuffer/model/counter globals, four-player display state, framebuffer create/kill/draw path, pad-disable setter, and story-mode helper. Target/source `.text` are `0x63BC/0x560`; `mbPauseDispCopyCreate`, `mbPauseDispCopyKill`, `PauseDispCopyDraw`, and `mbConfigPadDisableSet` are exact. `GWStorySingleCheck` is 72.272730%; the pause panel/guide/config state machines remain absent. |
| `board/last5.c` | The target-derived stable rank/order routine and 40-coin effect recover `0x658` retained target text plus the real `LAST5COINWORK { s16 delay; float velocity; }` overlay on public `MBCOINOBJ` caller work. Target/source `.text` are `0x2458/0x614`; the two functions are 99.266050% and 89.601070%. The roulette work still has unproven fields and is omitted. |
| `board/opening.c` | Fourteen curve, party wrapper, pad-delay, view/camera, hook, guide, and restore routines recover the real vectors, hook types, process pointer, pad-delay array, and initialized guide model ID. Target/source `.text` are `0x2B14/0x3D4`; 12/14 functions are exact. Only `mbev_Opening` (82.722220%) and `mbev_OpeningParty` (94.500000%) diverge; the large party/single events remain absent. |
| `board/scroll.c` | Nine lifecycle, star-space, map-view, camera, and hook routines recover the typed `HSF_FACE *` collision owner, map model/animation state, camera vectors/zoom, and callback types. Target/source `.text` are `0x3E4C/0x27C`; 7/9 functions are exact. `ScrollKill` is 80.208336% and `mbMapCameraSet` is 87.692310%; collision construction and map rendering remain absent. |
| `board/shopevent.c` | Six target-backed enable/hook/init/back-create/shop-call routines recover the typed object hook and real two-word `MBSHOPWORK { playerNo, shopNo }`. Target/source `.text` are `0x2FDC/0x160`; the enable setter, hook setter, and back-create wrapper are exact. The two init wrappers and shop-call wrapper are 92.395350%-95.357140%; shop construction and the large selection event remain absent. |
| `board/snpc.c` | `mbSNpcInit`, `mbSNpcMasuGet`, and the authentic empty `SNpcStarFunc` are exact, with the target-proven three-byte save state (`flags`, `masuId`, `effectMissCount`) and typed runtime/save pointers. Target/source `.text` are `0x9594/0x44`. The `0x298` runtime object and all movement/dice/star state machines remain absent. |
| `board/capmove.c` | Nine named capsule-movement kill callbacks are recovered from their complete target ranges. Every target/source function is `0x4/0x4` and byte-exact; target/source whole `.text` are `0x6F68/0x24`. The movement event bodies remain absent. |
| `board/capthrow.c` | Eight named capsule-throw kill callbacks are recovered from their complete target ranges. Every target/source function is `0x4/0x4` and byte-exact; target/source whole `.text` are `0x8CB0/0x20`. The throw event bodies remain absent. |
| `board/captrap.c` | Five named capsule-trap kill callbacks are recovered from their complete target ranges. Every target/source function is `0x4/0x4` and byte-exact; target/source whole `.text` are `0x6A40/0x14`. The trap event bodies remain absent. |
| `board/capspecial.c` | Thirteen target-backed functions recover the exact `0x28` `TERESA_FADE_WORK`, Teresa steal state, Miracle lifecycle hooks, and Koopa dice/motion hooks. Target/source `.text` are `0xE744/0x1E8`; 9/13 mapped functions are exact. Fade kill/object/setter and Koopa motion remain 88.214290%-92.826090%; the large special-capsule state machines remain absent. |
| `board/capevent.c` | Eleven move/stop, bank, duel-coin, and bubble-hook routines are exact, with the target's `10`/`5` duel defaults, four `-1` sentinels, and six-argument hook. Target/source `.text` are `0x14EFC/0x178`; the other 226 retained event functions remain absent. |
| `board/mgcall.c` | Twelve functions recover the exact `0x10` `MGLISTWORK`, `.bss 0xD8` history/status/list closure, `.rodata 0x70` status tables, and `.data 0x48` pointer/size tables. Target/source `.text` are `0x75FC/0x3DC`; 8/12 mapped functions are exact. Init/focus-kill are 99.965515%/99.875000%, while data-close/battle-message are 87.307690%/89.000000%; the roulette/UI executors remain absent. |
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
| `dolphin/os/OS.c` | 59.719%; 0/14 functions exact. |
| `dolphin/os/OSExec.c` | 80.972%; 2/8 functions exact. |
| `dolphin/os/OSMemory.c` | 52.707%; 4/8 functions exact. |
| `dolphin/os/OSReset.c` | 71.478%; 2/7 functions exact. |
| `dolphin/os/__start.c` | Raw section bytes alone report 100%, but only 4/7 functions map exactly; `__start`/`__init_registers` are unresolved and `__init_data` is 86.667%. |
| `dolphin/gx/GXTransform.c` | 70.599%; 9/16 functions exact. |
| `dolphin/exi/EXIBios.c` | 68.572%; 1/23 functions exact. |
| `dolphin/mic/mic.c` | 56.653%; 0/41 functions exact. |
| `dolphin/mic/m2s.c` | 78.721%; 1/14 functions exact. |
| `msm/msmsys.c` | 99.734%; 18/23 functions exact. |
| `msm/msmstream.c` | 99.487020%; 23/28 functions exact. `msmStreamDvdCallback` and `msmStreamDvdCallback2` are exact. `msmStreamData` is now target/source `0x2EC/0x2EC` at 99.759360% after recovering the target pause/linked-slot/ARAM-update control flow; `msmStreamSlotInit` is `0x224/0x224` at 99.635040% after recovering its three distinct size/offset lifetimes. Five functions still diverge, so the owner remains fallback-linked. |
| `board/player.c` | Raw section pairing is 1.155%; mapped-function weighted score is 99.965%, but only 10/165 functions are currently exact. |
| `board/audio.c` | 99.995735%; 48/49 functions exact. Recovering the unsigned board-number field and the Matching `board/camera.c` `BoardNoGet` inline makes `mbMusBoardPlay` (`0x12C`) and `MusBoardFade` (`0x1AC`) exact. Only `mbMusBoardFadeOut` remains: target/source are both `0x3F0` at 99.952380%, with twelve stack-slot operand differences. Whole target/source `.text` remain `0x2BF0/0x2BF0`, and all 444 text relocations match. |
| `board/masu.c` | Raw `.text` pairing is 64.42536%; mapped-function weighted score is 99.763774%, with 112/119 target functions represented and 103/119 exact. Wave 27 recovered 21 formerly absent functions and the real `0x1C` next-space work layout. Fourteen additions are fully exact; seven exact-size instruction streams retain only compiler-local constant/static-data relocation identities while the owner is incomplete. Seven large draw/event/color/value functions remain absent, so the owner stays fallback-linked. |
| `board/branch.c` | 99.816%; 16/17 functions exact. `ev_Branch` is now target/source `0x8F0`/`0x8F0`; all 19 remaining differences are one `choice`/`choiceTime` register cycle, with no inserted, deleted, or replaced instructions. Whole-owner `.text` is `0x10E0`/`0x10E0` and all 210 relocations match; configured `.data` remains `0x18`/`0x12`. |
| `board/effect.c` | 59.766%; 1/35 functions exact, target/source text sizes `0x3050`/`0x3074`, plus `.sdata` and `.sdata2` divergence. The restored FastCast header is not its blocker. |
| `gssdk_lib/asrpho/common/blocks/fft_maye.c` | The complete target-derived Mayer fast-Hartley and real-FFT pair is recovered, including both exact `0x40` trigonometric tables. Whole target/source `.text` is `0x8F0/0x8BC` at 79.104900%; `fht` is `0x73C/0x75C` at 85.051834% and `realfft` is `0x1B4/0x160` at 53.844036%. `.rodata 0x80` is exact, while target `.sdata2 0x18` remains 71.428570%. |
| `gssdk_lib/asrpho/common/blocks/fftmod.c` | The complete target-derived `0x4C` FFT block recovers its power/amplitude modes, queue processing, control, initialization, and construction. `ControlFFTMod` (`0xA4`) is exact. Whole target/source `.text` is `0xC84/0xC78` at 93.288390%; the other functions range from 80.234180% to 97.250000%, and target `.sdata2 0x48` is 80.000000%. |
| `gssdk_lib/asrpho/common/blocks/pitchwin.c` | The complete target-derived `0x44` pitch-window block recovers its Hamming coefficients, circular signed-sample history, queue lifecycle, and all five functions. `ConstructPitchWindow` (`0x44`) is exact. Whole target/source `.text` is `0x5C4/0x5C0`; `.sdata2 0x40` and all 37 relocations align. `ProcessPitchWindow` is `0x148/0x140` at 93.512190%, `ControlPitchWindow` is exact-size `0xF4` at 99.016396%, `CreateWindow` is exact-size `0x16C` at 76.703300%, and `InitPitchWindow` is `0x1D8/0x1DC` at 96.652540%. |
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
| `gssdk_lib/asrpho/common/blocks/flblocks/logexp.c` | The target-derived two-function owner restores its typed 353-float lookup table. `HLnOnePlusExpHFloat` (`0x90`), semantic `.rodata 0x584`, and `.sdata2 0x48` are exact. `LogAdd` is `0x138/0x138` at 99.743590%, with four floating-point operand-register differences; target/source `.text` are both `0x1C8`, and the target's `.rodata 0x588` includes a four-byte alignment tail. |
| `gssdk_lib/asrpho/common/blocks/flblocks/vq1500.c` | The target-derived `0x30` owner, `0x14` code-book descriptor, language/code-book method ABI, vector searches, queue lifecycle, and all nine functions are recovered. Eight functions are exact. `GetLabel` is exact-size `0x148` at 98.109760%; whole target/source `.text` are `0x4F0/0x4F0` at 99.509490%, the semantic float constant is exact, and both objects have 25 text relocations. |
| `gssdk_lib/asrpho/common/blocks/flblocks/smoother.c` | The complete target-derived `0x20` smoother and `0x18` static matrix-bank descriptor recover construction, destruction, active-column rejection, QR update, and spline coefficient evaluation. `smtConstruct` (`0x2AC`) and `smtDestruct` (`0x90`) are exact. `Smoothing` is target/source `0x358/0x35C` at 75.154205%; whole `.text` is `0x694/0x698` at 87.370544%, and target/source `.sdata2` are `0x18/0x14` at 90.909096%. |
| `gssdk_lib/asrpho/common/blocks/flblocks/mel.c` | The complete target-derived `0x40` mel-filter block recovers five functions, per-band weights/bin ranges, queue processing, profile allocation, and control/destruction. `InitMel` (`0x6C`) and `ConstructMel` (`0x44`) are exact. `ProcessMel` is exact-size `0x1B8` at 91.681816%, `ControlMel` is `0x158/0x188` at 72.534880%, and `MelInitBands` is `0x504/0x508` at 93.875390%. Whole `.text` is `0x8C4/0x8F8` at 90.654190%, while target/source `.sdata2` are `0x60/0x68` at 96.000000%. |
| `gssdk_lib/asrpho/common/blocks/flblocks/acne.c` | The complete target-derived `0x98` ACNE block recovers all eight functions, one three-vector band allocation, the exact `12*bandCount + 40` session layout, queue lifecycle, smoothing/adaptation state, and two 21-float tables. `Reset` (`0x108`) and `ConstructAcne` (`0x44`) are exact. Whole target/source `.text` is `0xC24/0xC64` at 82.891890%; `.rodata 0xA8` is exact, target/source `.sdata2` is `0x50/0x4C` at 97.435900%, and text relocations are 95/96. The four-byte slot at `0x80` is documented only as an unread/unwritten ABI reserve: MP6 allocation and neighboring fields prove the gap, while MP7 commit `f5ea780` has the same `0x98` allocation, exact initialization/construction parity, and no access at `0x80`. |
| `gssdk_lib/asrpho/common/ctxdata/ctxdata.c` | The binary-proven `ContextDataV2` layout through offset `0xB8`, virtual table, and all accessors are recovered. Seventeen accessors are byte-exact; the three packed-tail pointer computations remain at 79.500000%, 66.933334%, and 68.000000%, while `FillContextV2VirtualTable` retains one source/target table-relocation identity difference. The owner remains fallback-linked. |
| `gssdk_lib/asrpho/common/blocks/flblocks/mtx.c` | The complete real `0x18` float/integer matrix descriptors and all 12 allocation, association, compression, column-deletion, and QR-update functions are recovered. Nine functions are exact. `imtxDeleteCol` is exact-size `0x88` at 98.676470%, `mtxCompress` is `0xC4/0xBC` at 94.795920%, and `QrDeleteCol` is `0x308/0x314` at 94.118550%. Whole target/source `.text` is `0x7FC/0x800`, and both objects have 25 relocations; source `.sdata2 0x40` retains the compiler's additional `sqrtf` constants versus target `0x30`. |
| `gssdk_lib/asrpho/common/blocks/flblocks/mtxopt.c` | The shared recovered `0x18` `FloatMatrix` replaces the old private prefix and preserves existing object evidence. `mtxFillCopy` (`0x30`) and `mtxFillX` (`0x80`) are exact. `QrPreMult` is target/source `0x1A8/0x1A4` at 92.528305%, with the same unrolled arithmetic but divergent register allocation and outer-loop closure, so the owner remains fallback-linked. |
| `gssdk_lib/gsapi/callbacks.c` | Six callback wrappers totaling `0x260` are exact, as are `AsrSpiSignalCallBacks` (`0x18`) and `AsrSpiRecogCallBacks` (`0x8`) with their relocation order. The target's `0x580` `asrspi_cbResult` remains unrecovered, so the complete owner stays fallback-linked. |
| `gssdk_lib/gsapi/ctxfuncs.c` | Five small target-derived routines are materialized. `SessionDataFree`, `ContextAPIDeActivate`, `ContextSetActiveWords`, and `ContextSetCtxData` are exact; `ContextGetParam` is `0x50` at 84.500000%. Ten larger context/session routines remain absent, so the owner stays fallback-linked. |
| `gssdk_lib/asrpho/common/ctxdata/langdata.c` | The real `LanguageDataV2` prefix and code-book layouts support 36 exact field/pointer accessors. The target's `0x13C` language-method object and the code-book method slots consumed by `vq1500.c` are now typed in the shared header. `_langGetpErgodicPenalty` is `0x64` at 77.040000%; the deeper packed-layout functions and virtual-table source closure remain unrecovered. |
| `gssdk_lib/asrpho/common/tos/mqueue.c` | The target queue/list/reader prefix and three routines are recovered. `qQueueNbrElements` (`0x20`) is exact; `qDeQueueOne` (`0x50`) is 89.500000% and `qQueueConstruct` (`0x8C`) is 84.200000%. The remaining queue control, enqueue, reset, resize, and destruction routines are absent, so the owner remains fallback-linked. |

The other 24 owners are tagged `NO-SOURCE`. Each explicit brace group below
expands to the named owner files; the count audit is `1 + 23 = 24`.

- `musyx/runtime/` (1): `dsp_import.c`. The authenticated donor is a DSP
  firmware byte array, so it is preserved as reference evidence rather than
  counted as clean source recovery.
- `gssdk_lib/` (23):
  - `gsapi/gsapi.c`;
  - `asrpho/asrspi.c` and
    `asrpho/rec1600/{convert,creasp,creaspch,creaspt,creatree,crsptrch,spi1600,train,userword}.c`;
  - `asrpho/common/blocks/{dpgenuw,dpscruw,isoword,nbestdp,pitchdp}.c`;
  - `asrpho/common/blocks/flblocks/{gender,specsub,vad}.c`;
  - `asrpho/common/blocks/flfxblks/{dist16,pitchco,shs_vuv}.c`;
  - `asrpho/common/tos/tinyos.c`.

Mixed C/assembly owners stay in the C bucket until all retained C and assembly
passes the appropriate proof. `TRK_MINNOW_DOLPHIN/targimpl.c` no longer appears
here because all 30 retained functions and the linked owner passed that gate.

### Applied sibling-exception decisions

| MP6 owner | Sibling authentication | Decision and MP6 proof |
| --- | --- | --- |
| `TRK_MINNOW_DOLPHIN/targsupp.s` | [M5 `src/TRK_MINNOW_DOLPHIN/targsupp.s` at `e246f9d`](https://github.com/mariopartyrd/marioparty5/blob/e246f9d9850ff53ac684b971068fbf87fdcf6acb/src/TRK_MINNOW_DOLPHIN/targsupp.s), `Matching`; M4 carries the identical blob as `MatchingFor(USA,PAL)`. | **Admitted.** The donor blob is `0244131bd8219c6f5839ae2cda9254c2f28e005c`. Its four 8-byte `twui r0,0; blr` functions exactly fill the MP6 `0x20` target owner; source-object and final 137-file/hash/DOL gates pass. It is authentic standalone assembly, not decompiled C. |
| `TRK_MINNOW_DOLPHIN/dolphin_trk.c` | [M5 `src/TRK_MINNOW_DOLPHIN/dolphin_trk.c` at `e246f9d`](https://github.com/mariopartyrd/marioparty5/blob/e246f9d9850ff53ac684b971068fbf87fdcf6acb/src/TRK_MINNOW_DOLPHIN/dolphin_trk.c), configured `Matching`. | **Admitted.** The retained `0x104` `.init` and `0x140` `.text` ranges reproduce all five target functions; donor-only out-of-line copies of `__TRK_copy_vectors` (`0x104`) and `TRK_copy_vector` (`0x90`) are linker-stripped after their logic is inlined into `__TRK_reset`. The semantic `TRK_ISR_OFFSETS` and `lc_base` globals plus target alignment tails are named and the full 137-file/hash/DOL gate passes. `InitMetroTRK` is authentic sibling assembly, so the mixed owner is excluded from clean-C totals. |
| `TRK_MINNOW_DOLPHIN/targimpl.c` | [M5 `src/TRK_MINNOW_DOLPHIN/targimpl.c` at `e246f9d`](https://github.com/mariopartyrd/marioparty5/blob/e246f9d9850ff53ac684b971068fbf87fdcf6acb/src/TRK_MINNOW_DOLPHIN/targimpl.c), configured `Matching`; M4 carries byte-identical source. | **Admitted.** Restoring the real Matching-sibling support prototypes supplies the target's two missing `u8` coercions and makes `TRKTargetSupportRequest` exact at `0x208`. All 30 retained functions and target `.text 0x1894` map at 100%; `.rodata 0x88` and `.data 0x30` are exact. Eight sibling-authentic helper emissions are linker-stripped exactly as in M4/M5, and the final 137-file/hash/DOL gate passes. Seven inline-assembly routines are authenticated by the Matching sibling, so this mixed owner is excluded from clean-C totals. |
| `musyx/runtime/snd_service.c` | [M5 `src/musyx/runtime/snd_service.c` at `e246f9d`](https://github.com/mariopartyrd/marioparty5/blob/e246f9d9850ff53ac684b971068fbf87fdcf6acb/src/musyx/runtime/snd_service.c), configured `Matching`. | **Admitted.** All eight retained routines fill the exact MP6 `0x2D4` linked text range. The two additional source-object lower-half relocations in `sndSqrt` resolve to the target immediates; the linked owner and final DOL are byte-identical. `sndSintab`, `last_rnd`, `sqrtConsts`, `_sinConsts`, and `i2fMagic` recover the semantic data closure without source padding. `sndSqrt` and `sndCos` are authenticated sibling assembly, so the mixed owner is excluded from clean-C totals. |
| `game/kerent.c` | [M4 `src/game/kerent.c` at `147b165`](https://github.com/mariopartyrd/marioparty4/blob/147b165a83187ac9e6cfdc3bf52f2e73437b1ffd/src/game/kerent.c), `MatchingFor(USA,PAL)`, authenticates a single `asm void _kerent` with `nofralloc` and `entry`/`b` pairs. | **Admitted.** `ae2977e` de-flipped it through blanket inline-assembly removal (`ASM-BLANKET-REMOVAL`), despite the written exception. The exact MP6 source was restored from `git show ae2977e^:src/game/kerent.c` (blob `bd5b7b9448a659e186d733e3bc3e13dc8291d6a2`). Target/source `.text` are `0x26A0` and 100% identical; each has 2,472 `R_PPC_REL24` relocations. The final 137-file/hash/DOL gate passes. It is an authenticated assembly jump table, not decompiled C. |
| `Runtime.PPCEABI.H/ptmf.c` | [Mario Kart: Double Dash `libs/PowerPC_EABI_Support/src/Runtime/ptmf.c` at `8a91a6b`](https://github.com/doldecomp/mkdd/blob/8a91a6b4a32834dfb689fbdcf1dea8796be9c75f/libs/PowerPC_EABI_Support/src/Runtime/ptmf.c), configured `Matching`. | **Admitted.** The authenticated `__ptmf_test`, `__ptmf_cmpr`, and `__ptmf_scall` assembly bodies reproduce all `0x94` retained target text bytes with no relocations. Donor-only `__ptmf_scall4` (`0x28`) and `__ptmf_null` (`0xC`) are linker-stripped. The full 137-file/hash/DOL gate passes. This is authentic Runtime assembly and is excluded from clean-C totals. |
| `dolphin/os/OSCache.c` | [M4 `src/dolphin/os/OSCache.c` at `147b165`](https://github.com/mariopartyrd/marioparty4/blob/147b165a83187ac9e6cfdc3bf52f2e73437b1ffd/src/dolphin/os/OSCache.c), `MatchingFor(USA,PAL)`. | **Restored after de-flip.** `93d9438` replaced the authentic bodies and de-flipped the owner while isolating assembly (`ASM-BLANKET-REMOVAL`). All 14 retained MP6 assembly bodies have same-name sibling bodies. Whole-file blobs differ because M4 carries linker-stripped helpers; the audited retained differences are ABI register/symbol spelling, and the MP6 object/effective-relocation/full-container gate passes. |
| `dolphin/os/OSContext.c` | [M4 `src/dolphin/os/OSContext.c` at `147b165`](https://github.com/mariopartyrd/marioparty4/blob/147b165a83187ac9e6cfdc3bf52f2e73437b1ffd/src/dolphin/os/OSContext.c), `MatchingFor(USA,PAL)`. | **Restored after de-flip.** `93d9438` replaced the authentic bodies and de-flipped the owner while isolating assembly (`ASM-BLANKET-REMOVAL`). All nine retained MP6 assembly bodies have same-name sibling bodies. `OSLoadContext` differs textually only in target labels/operand aliases; object/effective-relocation/full-container proof passes. |
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
| `board/effect.c` | `NonMatching` | Genuine divergence: target/source `.text` are `0x3050`/`0x3074`, score 59.766%, only 1/35 functions is exact, `.sdata` is `0x18`/`0x14`, and `.sdata2` scores 57.353%. It was already NonMatching before `93d9438`; it was not de-flipped by the header rework. |

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

The 112 current DOL fallback owners and their causes are exhaustive in the
two-bucket ledger above. Three REL Runtime variants also remain `NonMatching`;
they are outside this main-DOL-first taxonomy and are not included in the 111
C-work/1-assembly-policy remaining counts.
