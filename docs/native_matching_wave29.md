# Native matching wave 29: Odemu and Runtime ptmf

This ledger records one clean-C main-DOL promotion,
`OdemuExi2/DebuggerDriver.c`, and one separately counted
sibling-assembly-exception promotion, `Runtime.PPCEABI.H/ptmf.c`. Tests use
pinned DTK 0.9.2 (`4d039140`) and the recovered owner-local CodeWarrior build
settings.

## OdemuExi2 clean C

Animal Crossing decomp commit
`bea941ac2bb19dacb44c3d4b14267007b40eef2c` provides a Matching
`src/static/dolphin/OdemuExi2/DebuggerDriver.c` reference. The target object
then proves the MP6 owner-specific closure and names:

- public `DBClose`, `DBOpen`, `DBWrite`, `DBRead`, `DBQueryData`,
  `DBInitInterrupts`, and `DBInitComm`;
- retained local `DBGHandler`, `MWCallback`, `DBGReadStatus`, `DBGWrite`,
  `DBGRead`, `DBGReadMailbox`, and `DBGEXIImm`;
- local state `SendCount`, `MTRCallback`, `DBGCallback`, `SendMailData`,
  `RecvDataLeng`, `pEXIInputFlag`, and `EXIInputFlag` at the exact target
  small-data offsets.

The authentic SDK build shape is `GC/1.2.5n` with deferred inlining. All 14
retained functions, `0xA80` text bytes, and 57 effective relocations match.
The source object also emits six authenticated unused helpers totaling
`0x270` bytes: `CheckMailBox`, `DBGWriteMailbox`, `DBGEXIDeselect`,
`DBGEXISync`, `DBGEXISelect`, and `DBGEXIInit`. They are absent from the
target linked range and are linker-stripped; no source operation or byte blob
was added to suppress them.

The target's `0x8` small-data section contains the one-byte `SendCount` plus
seven zero alignment bytes. Its `0x18` small-BSS section contains `0x15`
semantic bytes plus three alignment bytes. Source globals reproduce all
semantic offsets, and the linked owner range is byte-identical.

## Runtime `ptmf` exception

Mario Kart: Double Dash commit
`8a91a6b4a32834dfb689fbdcf1dea8796be9c75f` configures
`libs/PowerPC_EABI_Support/src/Runtime/ptmf.c` as Matching. That sibling
authenticates the original `nofralloc` assembly implementation; the source is
admitted only under the repository's sibling-authentication exception.

The retained `__ptmf_test` (`0x30`), `__ptmf_cmpr` (`0x3C`), and
`__ptmf_scall` (`0x28`) bodies reproduce all `0x94` target text bytes and
have no relocations. The donor-only `__ptmf_scall4` (`0x28`) and
`__ptmf_null` (`0xC`) are linker-stripped. This owner is authentic assembly,
not decompiled C, and is excluded from the clean-C total.

## Gate

The two flips were gated together after object and effective-relocation
comparison:

- `ninja -j1`: `137 files OK`
- `build/tools/dtk.exe shasum -q -c config/GP6E01/build.sha1`:
  `137 files OK`
- `cmp orig/GP6E01/sys/main.dol build/GP6E01/main.dol`: byte-identical
- `build/GP6E01/main.dol` SHA-1:
  `b897e6ade6b3a0cd2f9907689f38a3b19c327e70`

The machine-readable comparisons are retained locally as
`build/proof_wave29_odemuexi_named.json` and
`build/proof_wave29_ptmf_initial.json`; build outputs are not tracked.
