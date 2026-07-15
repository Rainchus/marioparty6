# Native matching wave 42: MusyX ownership closure

Wave 42 promotes `musyx/runtime/seq.c` as clean C and replaces 75
address-derived MusyX symbols with source-authenticated function and storage
ownership. The other five audited owners remain `NonMatching`; naming a target
does not make divergent source match.

## Tool and source provenance

- DTK: `dtk.exe 0.9.2 4d039140f2d2ed80572b1949b76a5ff9b3094e06`.
- Objdiff CLI: `3.7.3`.
- Compiler: the project-pinned MusyX `GC/1.3.2` configuration.
- Source: Mario Party 5 commit
  [`e246f9d`](https://github.com/mariopartyrd/marioparty5/commit/e246f9d9850ff53ac684b971068fbf87fdcf6acb),
  whose MusyX configuration is the same `2.0.4` family and marks
  [`src/musyx/runtime/seq.c`](https://github.com/mariopartyrd/marioparty5/blob/e246f9d9850ff53ac684b971068fbf87fdcf6acb/src/musyx/runtime/seq.c)
  Matching. The MP5 and MP6 `include/musyx/` trees are byte-identical.

The donor supplies names and source shape. MP6 target sizes, order, calls,
relocations, storage offsets, and the final linked container decide every
accepted mapping.

## `seq.c` promotion

Five retained address names have unique same-version source identities:

| MP6 address | Recovered function | Target size |
| --- | --- | ---: |
| `0x800CB398` | `StartPause` | `0x64` |
| `0x800CC984` | `GenerateNextTrackEvent` | `0x218` |
| `0x800CCB9C` | `HandleEvent` | `0xAC0` |
| `0x800CD65C` | `InitTrackEvents` | `0x174` |
| `0x800CD7D0` | `HandleTrackEvents` | `0x49C` |

Their sizes, relative order, callers, and callee relocations agree with the
same-version source. Recovering those names also closes the calls in
`seqStartPlay`; that function is now object-exact.

The target storage closure is likewise exact:

- `seqNote[256]` at `0x802899F0`, size `0x1400`;
- `seqInstance[64]` at `0x8028ADF0`, size `0xC440`;
- `seq_next_id`, `curFadeOutState`, `curSeqId`, `noteFree`, `cseq`,
  `seqFreeRoot`, `seqPausedRoot`, and `seqActiveRoot` at
  `0x802C0A98..0x802C0AB8`;
- the cross-owner `synthTrackVolume[64]` at `0x80297D84`.

All 17 retained target functions pair. Fifteen are objdiff-exact directly.
`HandleTrackEvents` and `seqHandle` have identical instruction bytes but DTK
names five target constant-pool relocations `lbl_...` while MWCC names the
same retained constants `@591` through `@596`. At link time the relocations
resolve to the same addresses. The source-only note helpers and its two weak
`sqrtf` constants are unreferenced and linker-stripped.

Target/source `.text` are `0x3FCC/0x5168`. The retained target owns
`.data 0x50`, `.sdata2 0x20`, `.bss 0xD940`, and `.sbss 0x20`. The linked
owner range and complete `main.dol` are byte-identical, so `seq.c` is now
`Matching`. No assembly, byte blob, padding object, or fake operation is
present in the source.

## Additional semantic ownership

The same target/source audit recovers 15 more function names:

- `synth.c`: `do_voice_portamento`, `StartLayer`, `StartKeymap`,
  `LowPrecisionHandler`, and `ZeroOffsetHandler`;
- `stream.c`: `SetHWMix`, `GetPrivateIndex`, `CheckOutputMode`, and
  `SetupVolumeAndPan`;
- `synthdata.c`: `maccmp`, `smpcmp`, `curvecmp`, `layercmp`, and `fxcmp`;
- `s_data.c`: `InsertData`, proven by its position and the calls from
  `sndPushGroup`/`sndPopGroup` as well as the same switch/data-registration
  shape. Its MP6/source sizes are `0x28C/0x27C`, so it remains divergent.

Fifty-five storage symbols are also closed:

- synth timing, fader, job, callback, real-time, auxiliary-controller, and
  global-variable roots;
- the `0x1900` stream record bank and its public-ID/call-delay state;
- keymap, layer, curve, sample-directory, FX-group, and macro registry banks
  plus their six `u16` counts;
- the VID list, voice priority arrays/roots, voice list, last-started banks,
  and eight small voice-list state roots.

This corrects several false aggregate symbols. In particular,
`synthIdleWaitActive` is one byte followed by alignment, `synthRealTime` is an
eight-byte value, `inpAuxA` is `0x480` bytes followed by a four-byte owner-tail
alignment gap, and the old `0xE00` voice blob is four real arrays of sizes
`0x800`, `0x100`, `0x100`, and `0x400`.

After the renames, the retained function evidence is:

| Owner | Paired target functions | Directly exact | Remaining source divergence |
| --- | ---: | ---: | --- |
| `seq.c` | 17/17 | 15 | two local constant-label identities; linked owner exact |
| `synth.c` | 27/27 | 21 | six functions, including size differences in `synthStartSound` and `synthFXStart` |
| `stream.c` | 18/18 | 13 | five functions; `sndStreamFrq` remains structurally different |
| `synthdata.c` | 27/27 | 25 | sample-reference add/remove remain different sizes |
| `synthvoice.c` | 18/21 | 16 | allocation/block logic and three unpaired target functions |
| `s_data.c` | 6/7 | 0 | the target-specific group/data closure remains different |

Only `seq.c` passes the complete owner gate. The other owners stay
fallback-linked and are not counted as decompiled owners.

## Final gate

- `rtk ninja -j1`: `137 files OK`.
- `rtk proxy build/tools/dtk.exe shasum -q -c config/GP6E01/build.sha1`:
  `137 files OK`.
- `rtk cmp orig/GP6E01/sys/main.dol build/GP6E01/main.dol`: exit `0`.
- Progress: 293/895 owners overall and 284/396 DOL owners; DOL code/data are
  46.30%/74.09%.
