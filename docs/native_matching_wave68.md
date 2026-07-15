# Native matching wave 68: Mode-select engine recovery

Wave 68 keeps `mdseldll.rel` at Priority 1a and batches the target-backed easy
wins into one substantial application-owner slice. `REL/mdseldll/mdsel.c` now
represents 92 of 113 target functions. All 92 are byte-exact under objdiff's
required `functionRelocDiffs=data_value` comparison, accounting for `0x6CA0`
target `.text` bytes. The remaining 21 functions are absent rather than
approximated.

The application and compiler-runtime owners remain `NonMatching`. This report
is object proof for work in progress, not a REL promotion and not a claim that
fallback-linked bytes are decompiled source.

## Object result

The final owner-local comparison reports:

- target functions: 113 / `0x10010` bytes
- represented functions: 92 / `0x6CA0` bytes
- data-value-exact functions: 92 / `0x6CA0` bytes
- represented residuals: none
- target/source `.text`: `0x10010/0x6CA0`, 42.421284%
- target/source `.rodata`: `0x260/0x188`, 72.799995%
- target/source `.data`: `0x370/0x370`, 100%
- target/source `.bss`: `0x1A4E/0x1A4C`, 99.94058%

The comparison command was:

```powershell
rtk ..\..\tools\objdiff\v3.7.2\objdiff-cli.exe diff -p . `
  -u mdseldll/REL/mdseldll/mdsel `
  -o build/GP6E01/mdseldll/mdsel_objdiff.json `
  --format json-pretty -c functionRelocDiffs=data_value
```

The source object was rebuilt immediately before the final comparison with:

```powershell
rtk ninja build/GP6E01/src/REL/mdseldll/mdsel.o
```

## Recovered source families

The scalar and vector math closure now includes `fn_1_50C`, `fn_1_550`,
`fn_1_598`, `fn_1_724`, `fn_1_80C`, `fn_1_8E8`, `fn_1_9C4`, `fn_1_A20`,
`fn_1_C28`, `fn_1_C48`, and `fn_1_C8C`. The target proves component-wise
vector wrappers and the expanded scalar helpers. MP4's interpolation family
and MP5's unrolled Bezier helper authenticate that source family. `fn_1_C8C`
recovers the complete model-position interpolation and camera-facing yaw
normalization, including the target's inlined smoothing expression.

The window and lifecycle closure now includes the exact callback `fn_1_0`,
window open/close/wait/choice/message helpers through `fn_1_25F8`, light and
camera setup, sprite setup, both module entry points, `ObjectSetup`,
`fn_1_B304`, `fn_1_B414`, `fn_1_B804`, `fn_1_E7B0`, and `fn_1_A5D4`. The
callback target consistently uses a signed 16-bit character index even though
the shared window callback API carries a `char`; keeping that API unchanged
and using the owner-local callback cast preserves the already-matched window
owner and reproduces `fn_1_0` exactly.

The object/effect closure recovers the small state callbacks, destructors,
Bezier launch twins `fn_1_7EC4` and `fn_1_86F8`, sprite placement
`fn_1_8EC8`, model placement `fn_1_9160`, and the positional-audio state
initializer `fn_1_46DC`. The new owner-local structures retain only offsets
proved by target loads and stores:

- `MDSEL_BEZIER_WORK` is exactly `0x88`: opaque `0x00..0x03`, time and
  duration at `0x04/0x08`, three `HuVecF` control points at
  `0x0C/0x18/0x24`, and an opaque `0x58` tail.
- `LBL_1_BSS_8AC_ENTRY` is exactly `0x88`: its `s16` fields at
  `0x40..0x46` and floats at `0x48/0x4C` are instruction-proven; the rest
  remains opaque. The target owns two records at `lbl_1_bss_8AC` and 30 at
  `lbl_1_bss_9BC`.
- `LBL_1_BSS_1D4_ENTRY` remains the exact eight-byte model/audio-handle
  record used by all 15 positional effects.

The particle closure now includes `fn_1_ECAC`, `fn_1_ED60`, `fn_1_EEC8`,
`fn_1_F4E4`, `fn_1_F70C`, `fn_1_F790`, `fn_1_FA18`, and `fn_1_FA9C`.
`fn_1_FA9C` recovers its seven target colors and expands the proven particle
helpers for enable, disable, position, and color updates. MP5's particle
source authenticates `HU3D_PARTICLE_BLEND_ADDCOL`; no raw numeric spelling is
needed.

## Compiler-shape evidence

The pinned `repos/mwcc-decomp` `CInline.c` implementation explains the retained
plain-definition/inline-redeclaration forms. The target contains standalone
copies of several helpers while also expanding them into later callers. In
particular, `fn_1_2828` expands `fn_1_40C` and is itself expanded into
`fn_1_A5D4`; `fn_1_F4E4` is standalone and expands into `fn_1_F790`;
`fn_1_90FC` expands through `fn_1_9160`; and `fn_1_ED60` expands into
`fn_1_FA9C`. These depth-two relations are established by complete instruction
copies and the absence of call relocations, not register sweeping.

Two plausible-looking forms were rejected by object evidence. The motion shift
inside `fn_1_46DC` uses target constant `10.0f`, not `4.0f`. The alternate
sprite X calculation in `fn_1_8EC8` is division by `2.0f`; spelling it as a
multiplication by `0.5f` changes MWCC's load order even though the arithmetic is
equivalent. Only the exact target forms remain.

The WIP definition of `fn_1_F790` is kept after `fn_1_A5D4` as a compiler
visibility lever. Moving it into final target emission order causes this
toolchain to auto-inline it into `fn_1_A5D4`; a blanket `dont_inline` pragma
also suppresses the authentic nested `fn_1_F4E4` expansion. Final translation-
unit ordering therefore remains real work before an owner promotion. No pragma
or duplicated body was admitted to manufacture a match.

## Remaining application work

The 21 unrepresented target functions are:

`fn_1_11D4`, `fn_1_1370`, `fn_1_2A78`, `fn_1_346C`, `fn_1_3910`,
`fn_1_3CC0`, `fn_1_485C`, `fn_1_4CF4`, `fn_1_5290`, `fn_1_5614`,
`fn_1_5BF0`, `fn_1_607C`, `fn_1_651C`, `fn_1_70BC`, `fn_1_7868`,
`fn_1_809C`, `fn_1_92BC`, `fn_1_9910`, `fn_1_BAB4`, `fn_1_E1FC`, and
`fn_1_EF48`.

`fn_1_11D4` was deliberately left absent because its otherwise-dead local
`2.0f` initialization does not yet distinguish the authentic scalar, array,
or aggregate declaration. `fn_1_5BF0` likewise retains an unexplained emitted
boolean at the loop tail. The larger object callbacks and mode-flow state
machines remain normal C reverse-engineering work.

No full `mdseldll.rel` or DOL gate was run for this object-only WIP slice, and
no `Matching` flip is present. The prior Wave 67 full build remains the latest
container proof; a future promotion still requires the complete source owner,
runtime-policy decision, full `rtk ninja`, `137 files OK`, build SHA check, and
byte-identical REL plus `main.dol` comparisons.
