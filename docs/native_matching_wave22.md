# Native matching wave 22: MusyX sibling exceptions

This wave audits every remaining MusyX owner with an authenticated Matching
sibling and admits two mixed C/assembly owners under the existing sibling
exception. The frozen base is `fork/main` commit
`34cdbd76952359695d4c0d44411bdfff8364350c`. Tests used the pinned DTK 0.9.2
toolchain and the existing MusyX `GC/1.3.2` flags.

Mario Party 5 commit `e246f9d9850ff53ac684b971068fbf87fdcf6acb`
marks five of MP6's nineteen remaining MusyX owners Matching. Mario Party 4
does not carry these MusyX runtime sources, and MP5 marks the other fourteen
owners NonMatching. The authenticated set is therefore closed: the two
accepted owners below, the two freshly rejected owners below, and the already
rejected `snd_service.c` candidate account for all five. No source from a
NonMatching sibling was treated as authentic original source.

## Accepted sibling-exception owners

### `Chorus/chorus_fx.c`

The source is the Matching MP5 tree blob
`d1498f6e80670880c97abfedb1b1cd0d9e141853`, with trailing whitespace
normalized. Its `do_src1` and `do_src2` routines are authentic `static asm`
CodeWarrior bodies carried by that sibling; the other retained routines are C.

| Evidence | Target | Compiled donor | Result |
| --- | ---: | ---: | --- |
| `.text` | `0x868` | `0x900` | the source-only `sndAuxCallbackUpdateSettingsChorus` (`0x98`) is linker-stripped |
| `.data` | `0x800` | `0x800` | exact resampling table |
| text relocations | 12 | 12 | same retained relocation closure |
| `.sdata2` | `0x8` | `0x18` | the shared `i2fMagic` value is retained; the `0x10` surplus belongs only to the stripped update helper |

Three of the five retained functions are raw objdiff-exact. `do_src1` and
`do_src2` differ only in two operands apiece: the target split label at
`0x802C2318` maps to the donor's semantic `i2fMagic` symbol. The linked owner
range and full DOL are exact.

### `CheapReverb/creverb.c`

The source is the Matching MP5 tree blob
`937a41151b7890a05c8d61cad000f4d499057e00`, with trailing whitespace
normalized. Its `HandleReverb` routine is an authentic `static asm`,
`nofralloc` CodeWarrior body; the remaining retained routines are C.

| Evidence | Target | Compiled donor | Result |
| --- | ---: | ---: | --- |
| `.text` | `0x8BC` | `0xBA4` | exact `0x2E8` surplus is four stripped helpers: `DLsetdelay 0x34`, `DLcreate 0xA8`, `DLdelete 0x24`, and `ReverbSTDModify 0x1E8` |
| `.data` | `0x10` | `0x10` | exact |
| `.sdata2` | `0x40` | `0x50` | donor-only constants are stripped |
| text relocations | 37 | 54 | the retained linked closure is exact |

The retained target functions exactly fill `0x8BC`: `ReverbSTDCreate 0x428`,
`HandleReverb 0x3B4`, `ReverbSTDCallback 0x24`, and `ReverbSTDFree 0xBC`.
Raw objdiff reports only constant-label operands in `ReverbSTDCreate` and the
semantic `HandleReverb` name versus target `fn_800EB9A8` in the callback. The
linked owner range and full DOL are exact.

Both owners were in the `C-not-yet-matched` fallback bucket while incomplete:
they are mixed owners, not complete `original-was-asm` owners. After the full
gate they are reported as Matching sibling-exception owners. Their assembly
bytes are authentic source but are not counted as clean decompiled C.

## Rejected authenticated candidates

### `hw_dolphin.c`

The Matching MP5 tree blob
`313d16260a824f291a6d6e2f0d94e0413c95b3f9` is clean C and compiles with
target/source `.text 0x4F8/0x5A4`, exact `.bss 0x2060`, exact `.sbss 0x28`,
and 97/110 relocations. Its exact `0xAC` surplus is donor-only
`callUserCallback 0x4C` plus `salStartDsp 0x60`; 15 of 17 retained functions
are raw exact. The target has no `.sdata2`, while the donor emits `0x10`. The
full link nevertheless fails on four donor symbols not owned by the MP6 split:
`dspCmdList`, `dspCmdFirstSize`, `dspSlave`, and `dspSlaveLength`. No aliases
or symbol reshaping were guessed. The source was removed and the owner remains
`NonMatching`/`NO-SOURCE`.

### `seq.c`

The Matching MP5 tree blob
`16585390b37cc6bca74ce23f7f92ec7857a1198a` is clean C. Sixteen of the
eighteen retained functions are raw exact; `seqStartPlay` and `seqHandle`
have exact target sizes but semantic label operands. Target/source `.text`
are `0x3FCC/0x5168`; `.data` is `0x50/0x50` but raw symbol/relocation
attribution scores 50%; `.sdata2` is `0x20/0x30` with an 80% raw score;
`.bss 0xD940` and `.sbss 0x20` are exact; and relocation counts are 442/546.
The full link fails on two references to the donor-only `synthTrackVolume`
data/global symbol. No call or symbol substitution was attempted. The source
and its temporary include were removed; the owner remains
`NonMatching`/`NO-SOURCE`.

`snd_service.c` remains rejected by the earlier native-port audit. Its five
sibling-derived names deliberately remain generic MP6 labels, as recorded in
[`easy_ports_wave.md`](easy_ports_wave.md); sibling naming alone is not target
proof.

## Full gate

After every rejected probe was removed, the accepted pair was validated with:

```powershell
rtk ninja -j1
rtk build/tools/dtk.exe shasum -q -c config/GP6E01/build.sha1
rtk cmp orig/GP6E01/sys/main.dol build/GP6E01/main.dol
```

The final result is `137 files OK`; the original and rebuilt DOLs compare
byte-for-byte, and the built `main.dol` SHA-1 is
`b897e6ade6b3a0cd2f9907689f38a3b19c327e70`. The final build was also checked
for generated `symbols.txt` changes before staging.
