# Native matching Wave 80: `mdpartydll` bootstrap and stage recovery

Wave 80 opens the party-mode driver without weakening the fallback boundary.
The frozen base is `fork/main` commit `2c903c7bb77757ab3e61a6fc41aff5123883194f`.
The application and stage owners remain `NonMatching`; their exact functions
are evidence inside fallback-linked owners and are not counted as decompiled
modules. The compiler runtime is admitted only through the authenticated
original-assembly exception and is never counted as clean C.

## Recovered ownership

The REL's section and function seams establish three owners:

| Owner | Target sections |
| --- | --- |
| `REL/mdpartydll/mdparty.c` | `.text 0x00000..0x3F424`, `.rodata 0x000..0x358`, `.data 0x000..0xF10`, `.bss 0x000..0xA80` |
| `REL/mdpartydll/stage.c` | `.text 0x3F424..0x467FC`, `.rodata 0x358..0x4D0`, `.data 0xF10..0xF34`, `.bss 0xA80..0xB64` |
| `REL/mdpartydll/runtime.c` | `.text 0x467FC..0x47240`, `.rodata 0x4D0..0x4E8` |

The `0x3F424` stage seam is not a size guess. `fn_1_3F424`, `fn_1_3F50C`,
and `fn_1_3F550` duplicate earlier scalar helpers and begin the stage constant
contribution at `.rodata 0x358`; the rendering closure consumes that constant
bank and coordinates with `.data 0xF10` and `.bss 0xA80`. The runtime seam is
the exact `0xA44` compiler-helper suffix shared by same-game `fileseldll` and
`mdseldll`.

`symbols.txt` now records the authenticated `ObjectSetup` and compiler-runtime
names. Four owner extents were also corrected from consumer evidence:

- `lbl_1_data_BCC` is the 33-byte callback string; byte `0xBED` is alignment.
- `lbl_1_data_ED8` is the 53-byte party-mode banner; `0xF0D..0xF0F` align the
  following stage data.
- `lbl_1_bss_104` is `HUWINID[2]`, size `0x4`.
- `lbl_1_bss_A68` is `HU3D_LIGHTID[2]`, size `0x4`.

No padding global or opaque data packet was introduced.

## Clean-C recovery

The source now represents 90 of the 315 application/stage target functions
and `0x73A4` of `0x467FC` clean-C text bytes. Relocation-aware objdiff with
`functionRelocDiffs=data_value` proves 87 functions and `0x6918` bytes exact:

| Owner | Target | Represented source | Byte-exact source |
| --- | ---: | ---: | ---: |
| `mdparty.c` | 258 functions, `0x3F424` bytes | 61 functions, `0x4D20` bytes | 60 functions, `0x48CC` bytes |
| `stage.c` | 57 functions, `0x73D8` bytes | 29 functions, `0x2684` bytes | 27 functions, `0x204C` bytes |
| Clean-C total | 315 functions, `0x467FC` bytes | 90 functions, `0x73A4` bytes | 87 functions, `0x6918` bytes |

The recovered bootstrap includes the exact `0xB64` `fn_1_2D1D4`
coordinator, its camera/light/window/shadow/sprite/model setup closure, the
default camera callback, and the real `0x38` model/animation record. The
coordinator's six source helpers compile to the target's inlined bodies. The
source also recovers the 11-model/22-motion lifecycle, group-12 digit display,
group-0 fade/show callbacks, and the authentic empty `fn_1_4A80`; no invented
sprite destructor was added.

The stage slice recovers framebuffer copy, the material pipeline, the exact
`0x9BC` layer hook, particle creation/update/destruction helpers, and typed
stage texture, model, animation, and particle globals. Target instruction
shape closes two otherwise subtle expressions:

- the layer loop uses multiplication by `-1`, retaining the target `mulli`
  node instead of unary `neg`;
- updated particle fields are compared in the same compound expression, so
  MWCC retains the target live result rather than reloading the field.

The compiler source oracle explains the layer-hook stack closure: local stack
objects are assigned by size class and then object-list order, so declaring
the `Mtx44` before the `Mtx` produces target slots `0x58` and `0x28`.

Three represented functions remain genuinely divergent and therefore remain
WIP:

| Function | Objdiff evidence |
| --- | --- |
| `fn_1_22A8` | target/source `0x454/0x454`, 99.444046%; semantic instruction sequence pairs, but parameter coloring and three `HuVecF` stack slots rotate. No register forcing was used. |
| `fn_1_3F5EC` | target/source `0x480/0x464`, 91.538190%; framebuffer/material semantics are complete, but BSS-address coalescing, local allocation, and the texture-scroll update shape still diverge. |
| `fn_1_3FC60` | target/source `0x1E4/0x1D4`, 88.727270%; setup semantics are complete, but the partial TU coalesces the adjacent stage BSS symbols and changes the saved-register/frame shape. |

## Authenticated runtime exception

`REL/mdpartydll/runtime.c` includes the shared compiler-runtime transcription.
ProjectPiki/Pikmin commit
[`24378f0e`](https://github.com/projectPiki/pikmin/commit/24378f0e5126c382b524681e82d2c1f36ba5332b)
and its Matching
[`src/Runtime/PPCEABI/H/runtime.c`](https://github.com/projectPiki/pikmin/blob/24378f0e5126c382b524681e82d2c1f36ba5332b/src/Runtime/PPCEABI/H/runtime.c)
authenticate the 19-function `ASM`/`nofralloc` source shape. MP5 commit
`e246f9d9850ff53ac684b971068fbf87fdcf6acb`,
`src/Runtime.PPCEABI.H/runtime.c`, corroborates the Mario Party compiler
runtime family. Same-game MP6 parity and the target gate prove the admitted
bytes:

- 19/19 functions exact;
- `.text 0xA44/0xA44`, 100%;
- `.rodata 0x18/0x18`, 100%.

This owner is Matching under the source-authentication exception. It remains
assembly and contributes zero clean-C bytes.

## Verification

The object comparisons were regenerated after the final source edits:

```powershell
rtk ninja -j1 build/GP6E01/src/REL/mdpartydll/mdparty.o `
  build/GP6E01/src/REL/mdpartydll/stage.o `
  build/GP6E01/src/REL/mdpartydll/runtime.o
rtk ..\..\tools\objdiff\v3.7.2\objdiff-cli.exe diff -p . `
  -u mdpartydll/REL/mdpartydll/mdparty `
  -o build/GP6E01/mdpartydll/mdparty_objdiff.json `
  --format json-pretty -c functionRelocDiffs=data_value
rtk ..\..\tools\objdiff\v3.7.2\objdiff-cli.exe diff -p . `
  -u mdpartydll/REL/mdpartydll/stage `
  -o build/GP6E01/mdpartydll/stage_objdiff.json `
  --format json-pretty -c functionRelocDiffs=data_value
rtk ..\..\tools\objdiff\v3.7.2\objdiff-cli.exe diff -p . `
  -u mdpartydll/REL/mdpartydll/runtime `
  -o build/GP6E01/mdpartydll/runtime_objdiff.json `
  --format json-pretty -c functionRelocDiffs=data_value
```

The final serialized container gate passed:

```powershell
rtk ninja -j1
rtk build/tools/dtk.exe shasum -q -c config/GP6E01/build.sha1
rtk cmp.exe orig/GP6E01/sys/main.dol build/GP6E01/main.dol
rtk cmp.exe orig/GP6E01/files/dll/mdpartydll.rel `
  build/GP6E01/mdpartydll/mdpartydll.rel
```

Results:

- full build: `137 files OK`;
- explicit DTK checksum: `137 files OK`;
- `main.dol`: byte-identical, SHA-1
  `b897e6ade6b3a0cd2f9907689f38a3b19c327e70`;
- `mdpartydll.rel`: byte-identical, SHA-1
  `519debb149ef42eda1ab3b0a4d2b3132b4f3e3cc`;
- `config/GP6E01/symbols.txt` hash before and after the final build:
  `3d98bb7d3266b132c543f9a5fb4212bf71df94ec`.

The exact REL is expected at this WIP boundary: `mdparty.c` and `stage.c`
still link their extracted originals, while the authenticated runtime links
from source. It is container proof and regression protection, not a claim
that either application owner is complete.
