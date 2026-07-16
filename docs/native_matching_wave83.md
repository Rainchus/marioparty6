# Native matching Wave 83: recover `mdparty` UI and flow

Wave 83 recovers 46 target-backed functions and `0x30E4` byte-exact text
bytes in `REL/mdpartydll/mdparty.c` from frozen `fork/main` commit
`33629ce85f9ca786edef290bc0b5a9bf3dc3963c`. Every added definition is exact
under relocation-aware objdiff. The incomplete owner remains fallback-linked
and `NonMatching`; none of this source is counted in the conservative
source-linked REL total.

No assembly, byte packet, volatile/register force, fake local, source padding,
or guessed symbol was introduced.

## Recovered spans

The four independently recovered spans are:

| Span | Functions | Exact target text | Recovered responsibility |
| --- | ---: | ---: | --- |
| `fn_1_884C` through `fn_1_C200` | 31 | `0x1E3C` | party HUD presentation, fades, character/sprite placement, animation setup, and cleanup |
| `fn_1_D678`, `fn_1_D6B0` | 2 | `0xD0` | object-state completion test and callback handoff |
| `fn_1_219C8`, `fn_1_22010`, `fn_1_23224`, `fn_1_23A00` | 4 | `0x55C` | party-entry presentation and four-player state transitions |
| `fn_1_2E338`, `fn_1_2E560`, `fn_1_2E5D4`, `fn_1_32904`, `fn_1_335F0`, `fn_1_34D10`, `fn_1_34FC4`, `fn_1_36264`, `fn_1_37260` | 9 | `0xC7C` | camera interpolation, stream/wipe transition, selection scans, and controller-state dispatch |
| Wave 83 total | 46 | `0x30E4` | all additions exact |

The target also fixes real retained storage and ABI declarations:

- `MDCAMERA_WORK + 0x40` is a `float` interpolation counter followed by the
  remaining `0xC`-byte tail, not an opaque 16-byte field;
- `LBL_1_BSS_9F4 + 0x10` and `+0x14` are 32-bit message identifiers;
- `lbl_1_data_0` is an initialized `s32` with value `-1`;
- `lbl_1_bss_34`, `lbl_1_bss_38[6]`, `lbl_1_bss_50[4]`, and the vector
  owners at `lbl_1_bss_80`, `lbl_1_bss_8C`, `lbl_1_bss_A4`, and
  `lbl_1_bss_D4` reproduce their target boundaries and first-reference order;
- the model-number ABI used by `fn_1_AD9C` and `fn_1_AF70` is 32-bit, with
  narrowing only at the signed-16-bit model-array boundary; and
- the state word used by the late selection callbacks is
  `OMOBJ::mtnId[0]` at offset `0x48`, not the unrelated pointer at `0x5C`.

## Compiler-source evidence

Two large residues closed by recovering calls which MWCC expands under
`-inline auto`, rather than transcribing the resulting instructions.

`fn_1_8D20` ends with `fn_1_89E0(obj)`. The helper is independently exact.
Its standalone floating-point register chronology is the same chronology seen
in the target caller after the caller's five earlier live values. MWCC
`CInline` and `StackFrame` behavior explains the shifted callee-local register
set and reproduces all 318 target instructions. A duplicated sprite switch
claimed a caller-local register too early and therefore could not be the
original source shape.

Likewise, `fn_1_22010` calls the independently exact `fn_1_C158` twice per
player. The definition-order plus trailing inline redeclaration produces the
target's two expanded animation/bank operations and its reused entry pointer.
The hand-expanded spelling was 140 bytes too large. Target register roles also
establish the opposite declaration orders required by `fn_1_22010` and
`fn_1_23224`; no register-forcing construct was used.

Other exact source-shape recoveries include the direct assignment of the
interpolation result in `fn_1_AB64`, scalar `Hu3DModelPosSet` arguments in
`fn_1_AD9C`, stream ID 4 plus `WIPE_MODE_IN` in `fn_1_2E5D4`, and the target
condition sense in the second `fn_1_34FC4` scan.

## Party-driver accounting

The final object report contains only the three pre-existing represented
residues. All 46 Wave 83 additions score 100% with exact effective
relocations.

| Owner | Target | Represented source | Byte-exact source |
| --- | ---: | ---: | ---: |
| `mdparty.c` | 258 functions, `0x3F424` bytes | 157 functions, `0xB018` bytes | 154 functions, `0xA828` bytes |
| `stage.c` | 57 functions, `0x73D8` bytes | 57 functions, `0x73D8` bytes | 57 functions, `0x73D8` bytes |
| Clean-C evidence total | 315 functions, `0x467FC` bytes | 214 functions, `0x123F0` bytes | 211 functions, `0x11C00` bytes |

The remaining represented residues are unchanged:

| Function | Target/source size | Relocation-aware score |
| --- | ---: | ---: |
| `fn_1_2B0` | `0x150/0x14C` | 98.095240% |
| `fn_1_400` | `0x24C/0x248` | 98.911570% |
| `fn_1_22A8` | `0x454/0x454` | 99.444046% |

Another 101 functions remain unrepresented. Therefore `mdparty.c` stays in
the `C-not-yet-matched` bucket and no `configure.py` flip is permitted.

## Verification

The final source was checked with:

```powershell
rtk ninja -j1 build/GP6E01/src/REL/mdpartydll/mdparty.o
rtk ..\..\tools\objdiff\v3.7.2\objdiff-cli.exe diff -p . `
  -u mdpartydll/REL/mdpartydll/mdparty `
  -o build/GP6E01/src/REL/mdpartydll/mdparty_objdiff_wave83_final_object.json `
  --format json-pretty -c functionRelocDiffs=data_value
rtk ninja -j1
rtk build/tools/dtk.exe shasum -q -c config/GP6E01/build.sha1
rtk cmp.exe orig/GP6E01/sys/main.dol build/GP6E01/main.dol
rtk cmp.exe orig/GP6E01/files/dll/mdpartydll.rel `
  build/GP6E01/mdpartydll/mdpartydll.rel
```

Results:

- objdiff: all 46 added functions and their effective relocations exact; only
  the same three earlier function residues remain;
- full build and explicit DTK checksum: `137 files OK`;
- `main.dol`: byte-identical, SHA-1
  `b897e6ade6b3a0cd2f9907689f38a3b19c327e70`;
- `mdpartydll.rel`: byte-identical, SHA-1
  `519debb149ef42eda1ab3b0a4d2b3132b4f3e3cc`; and
- `config/GP6E01/symbols.txt` SHA-1 before and after the final build:
  `3aaa758be28a24cc27e46fca2ab840f65168e05a`.
