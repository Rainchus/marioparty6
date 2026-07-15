# Native matching wave 70: Complete mode select

Wave 70 closes Priority 1a by source-linking `mdseldll.rel`. The frozen base is
`fork/main` commit `ba49093`. Application code was compiled with the configured
GC/1.3.2 compiler and REL flags; the compiler runtime was compiled with GC/2.6
after its target metadata proved compiler `2.4.7.1`, processor `Gekko`. Object
comparison used objdiff-cli 3.7.2 with
`functionRelocDiffs=data_value`; the final container gate used the pinned DTK
0.9.2 build at commit `4d039140`.

The result has two distinct source classifications:

- `REL/mdseldll/mdsel.c` is exact C: 113/113 functions and `0x10010` text
  bytes.
- `REL/mdseldll/runtime.c` is authenticated original assembly: 19/19 functions
  and `0xA44` text bytes admitted under the sibling exception. These bytes are
  source-linked but are not clean decompiled C.

No fallback object remains in `mdseldll.rel`.

## Closing the application owner

`fn_1_BAB4` is the complete `0x2748` mode-select driver. Its recovered control
flow covers the initial menu state, selection movement, guide and microphone
paths, confirmation/cancellation transitions, object and audio sequencing,
and exit dispatch. The retained body is exact for all 2,514 target
instructions; no partial body or fallback range is linked.

`fn_1_5BF0` is the `0x2B4` positional-audio updater. Target loads and calls
establish a 15-entry owner with model ID and effect handle, camera-space
panning clamped to the 32..96 range, absolute world-X distance, full volume
inside 1500 units, linear attenuation over the next 500 units, and final
volume clamping to 0..127. Declaration order is material: `pan` before the
constant pan range reproduces the target `r29`/`r28` allocation.

The final zero comparison after `HuAudFXVolSet` is semantically dead in the
target. The retained plain-C equality expression emits those three target
instructions without volatile state, assembly, or a byte-producing dummy.
Matching same-game `src/game/actman.c` contains the same bare dead-equality
source idiom in `PlayerModeJumpAlt`; its exact object emits the corresponding
zero-test sequence. The pinned `mwcc-decomp` backend at
`094b96ca1df4a035b5f93c351f773306c0241f3f` also accounts for the
`extsh`/`cntlzw`/`srwi` lowering. No behavioral meaning beyond the emitted
dead comparison is claimed.

## Translation-unit order

The remaining application mismatch was function emission order, not function
code. Earlier waves appended recovered blocks after their target positions.
The target symbol table fixes the order unambiguously: `fn_1_0`, the numerical
address sequence through `fn_1_A5D4`, `fn_1_B0C8`, `ObjectSetup`, `_prolog`,
`_epilog`, the `fn_1_B304` through `fn_1_E7B0` closure, and finally
`fn_1_ECAC` through `fn_1_FEC0`. Moving complete definitions into that order
while retaining the proven post-definition `inline` redeclarations restores
all target addresses without changing generated bodies.

The final application object proves:

- 113/113 functions data-value-exact;
- 113/113 target/source address and size pairs identical;
- `.text`: `0x10010/0x10010`, 100%;
- `.data`: `0x370/0x370`, 100%;
- all target `.bss` bytes and symbols exact;
- 5,029 text relocations in each object, with the per-function relocation gate
  exact.

The split target object reports `.rodata 0x260` and `.bss 0x1A4E`; MWCC emits
`.rodata 0x25E` and `.bss 0x1A50`. The first `0x25E` rodata bytes are exact and
the only target rodata tail is two zero alignment bytes. Conversely, the
source BSS has a two-byte section-alignment tail after the final signed 16-bit
owner. The exact linked REL proves that normal inter-object rodata alignment
and final-BSS trimming resolve both boundaries. No padding global, artificial
symbol, or data blob was introduced.

## Authenticated runtime exception

The MP6 target runtime has `.text 0xA44`, `.rodata 0x18`, two relocations, and
19 compiler helpers. Its text and rodata are byte-identical to the target
runtime blocks in `fileseldll` and `selmenuDll`; their section SHA-256 values
are respectively
`568553ab22d2c601ab7c8c112c542d762bddeb92d1768ba1d5f9202f53cd0c52`
and
`5e79c53013b4e6e10f4c76062db1390a898f0653fbb1e8097c6f74190118e3a6`.

ProjectPiki/Pikmin commit
[`24378f0e`](https://github.com/projectPiki/pikmin/commit/24378f0e5126c382b524681e82d2c1f36ba5332b)
authenticates the source shape. Its
[`src/Runtime/PPCEABI/H/runtime.c`](https://github.com/projectPiki/pikmin/blob/24378f0e5126c382b524681e82d2c1f36ba5332b/src/Runtime/PPCEABI/H/runtime.c)
carries the same 19-function order as MWCC `ASM`/`nofralloc`, including the
`entry` save/restore pairs. The corresponding
[`configure.py`](https://github.com/projectPiki/pikmin/blob/24378f0e5126c382b524681e82d2c1f36ba5332b/configure.py#L1015-L1027)
declares that runtime owner Matching; commit `60b3c620` is its explicit
NonMatching-to-Matching promotion. MP5 commit
`e246f9d9850ff53ac684b971068fbf87fdcf6acb`,
`src/Runtime.PPCEABI.H/runtime.c`, independently corroborates the Mario Party
compiler-runtime assembly family.

The Pikmin linked split strips several unused helpers, so it is not cited as
19 separate linked-byte proofs. It authenticates the Matching owner and source
shape. Same-game MP6 runtime identity plus the MP6 object, relocation, REL,
and DOL gates prove the admitted target bytes.

The stale `-proc 7400` override was removed only from this runtime owner. The
target `.comment` identifies Gekko, and GC/2.6 Gekko with the authenticated
`#pragma altivec_model on` regions emits the exact vector save/restore bodies.
The final source object proves:

- 19/19 functions exact;
- `.text`: `0xA44/0xA44`, 100%;
- `.rodata`: `0x18/0x18`, 100%;
- relocations: 2/2 exact.

## Full gate

After both configure entries were changed to `Matching`, the final gate was:

```powershell
rtk ninja
rtk build/tools/dtk.exe shasum -q -c config/GP6E01/build.sha1
rtk cmp.exe orig/GP6E01/sys/main.dol build/GP6E01/main.dol
rtk cmp.exe orig/GP6E01/files/dll/mdseldll.rel `
  build/GP6E01/mdseldll/mdseldll.rel
```

Results:

- full build: success, `137 files OK`;
- explicit DTK SHA gate: `137 files OK`;
- original/rebuilt `main.dol`: byte-identical, SHA-1
  `b897e6ade6b3a0cd2f9907689f38a3b19c327e70`;
- original/rebuilt `mdseldll.rel`: byte-identical, SHA-1
  `e6c20b24cfca8135ed8c49ce59108100277444c0`;
- `config/GP6E01/symbols.txt`: no staged rewrite after the final build;
- `git diff --check`: clean.

The final REL contains `0x10A54` source-linked text bytes: `0x10010` exact C
application bytes and `0xA44` authenticated original-assembly runtime bytes.
