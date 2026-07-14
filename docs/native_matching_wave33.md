# Native matching wave 33

This wave promoted one complete Game Speech SDK owner and materialized three
additional fallback owners as real, typed C. Only the byte-identical owner is
configured Matching; the three partial owners remain fallback-linked and are
not counted as decompiled owners.

## Exact resource owner

`gssdk_lib/common/rsrc/rsrc.c` recovers the target `Resource` as a `0x14`
object with the `0x1E61` type tag, owning heap, and three payload pointers.
The target callers also prove a shared ABI correction: `SafeHandle` is an
eight-byte aggregate passed by value to `safeh_HandleCheck`,
`safeh_HandlesEqual`, `InitRsrcFunction`, and `rsrc_Close`. The existing
`safeh.c` instruction and relocation stream remains exact with that public
definition.

The final DTK 0.9.2 objdiff reports for both `safeh.o` and `rsrc.o` contain no
differences. All three resource functions, `g_NullResource`, and their
relocations match. The target resource data is `0x14` bytes; the previous
split ending at `0x8021A4B8` absorbed a four-byte linker-alignment gap, so the
owner now ends at `0x8021A4B4`.

## Typed fallback recovery

Three target-derived owners now have executable source and shared
`TosBaseBlock`, queue-port, and context definitions:

- `ctxdata.c` recovers the `ContextDataV2` layout through its packed lexical
  tail at offset `0xB8`, the 21-entry virtual table, and all accessors.
  Seventeen accessors are byte-exact. The three packed-tail pointer functions
  remain at 79.500000%, 66.933334%, and 68.000000%; the table-copy function
  has one source/target table-relocation identity difference.
- `delaybl.c` recovers the complete `0x48` delay block and all four routines.
  `InitDelayBlock` and `ConstructDelayBlock` are exact. The process and
  control routines have exact target/source sizes (`0x198` and `0xE4`) and
  score 99.931370% and 98.859650%; their remaining differences are
  compiler-local frame/register shape.
- `statio.c` recovers the complete `0x50` stationarity block, queue behavior,
  history/delay rings, and `0.0f`, `0.1f`, and `2.0f` constants.
  `ConstructStationarity` is exact. The other three routines score
  97.198110%, 93.457146%, and 97.703705%, and the constant closure is
  `0x10/0xC`, so the owner remains NonMatching.

No register-forcing or artificial byte construction was retained. These
owners link their extracted target objects until their compiled C closes the
remaining differences.

## Rejected ports and bounded evidence

The Matching `qsort.c` from `yannicksuter/smstrikers-decomp` commit
`795ee4831df62e73a9c728a81a8f79a0e3684ab4` was compiled as an authenticated
source-shape probe. Its function is `0x16C` versus the MP6 target's `0x170`:
the donor emits `slwi`, while MP6 retains `li 2` plus `mullw`. Equivalent
authenticated multiplication spellings were byte-neutral. The candidate was
deleted rather than retained or promoted.

`asrpho/rec1600/initial.c` has no accessible sibling source donor. MP7 commit
`f5ea780d07dab8188d3c02f7e9052360c591dc1b` carries an equivalent `0x9C8`
binary data block with the same 102 relocation words, proving 45 terminated
key arrays plus `IniFile`, `InitialProcs`, `QueueWriters`, and `QueueReaders`
layouts. The local MP7 DOL does not match that repository's configured hash,
so this was recorded as binary-layout evidence only; no purported source port
was made.

Fresh authenticated sibling review supplied no source-shape evidence for the
remaining allocator cycles in `board/audio.c` or `board/branch.c`. Both were
left unchanged.

## Full gate

The final build used the pinned DTK 0.9.2 toolchain and passed:

- `ninja -j1`: `137 files OK`
- `build/tools/dtk.exe shasum -q -c config/GP6E01/build.sha1`:
  `137 files OK`
- `cmp orig/GP6E01/sys/main.dol build/GP6E01/main.dol`: byte-identical

DTK reports 287/895 Matching owners overall and 278/396 in the DOL. The DOL
policy split is 246 clean-C owners, 32 authenticated assembly-exception
owners, 117 `C-not-yet-matched` owners, and one `original-was-asm` owner.
