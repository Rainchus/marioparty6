# Native matching wave 32

Wave 32 batches the easy Game Speech SDK owners unlocked by the corrected
`GC/1.2.5n` compiler assignment. Candidate objects were compared individually;
the full build/hash/container gate was reserved for the batch boundary.

## Compiler evidence

Numeric target objects such as `mtxopt.o` and `logexp.o` contain separate
multiply and add instructions. With the default contraction setting, the same
recovered expressions compile to fused multiply-add instructions. The GSSDK
flags therefore include `-fp_contract off`; accepted owners and the final
container are still required to pass independently.

## Clean-C promotions

| Owner | Recovered source evidence | Object proof |
| --- | --- | --- |
| `gsapi/wrddata.c` | Real `{count, words}` word-list layout and `{reserved, count, words[]}` serialized-data layout; allocation uses `heap_Calloc`, the input string cursor advances before loop setup, and errors remain the target `0x80CC0080`/`0x80CC0086`. | Both functions fill the target `0x118` text range. `build/proof_wave32_gssdk_wrddata_final.json` has an empty mismatch list. |
| `asrpho/common/fastallo/fastallo.c` | Real `0x118` allocator layout: heap at `0x0`, chunk byte/word counts at `0x4`/`0x6`, chunk count/table at `0x8`/`0xC`, current pointer at `0x10`, and 65 free lists at `0x14`. The target proves a `u32` word request with explicit `u16` state updates. | The three functions fill the target `0x1EC` text range. `build/proof_wave32_gssdk_fastallo_final.json` has an empty mismatch list. |
| `asrpho/common/blocks/flblocks/spline.c` | Six typed `const f32` matrices (`17x8`, `17x8`, `1x8`, `21x10`, `21x10`, `1x10`) and two real `0x18` smoother descriptors. Float initializers were decoded as values that round-trip to the target IEEE-754 words; no integer array or byte blob is used. Six pointer relocations prove the descriptor closure. | There is no code. The typed globals reproduce all `0xB48` target rodata bytes and all six relocations; `build/proof_wave32_gssdk_spline_final.json` has an empty mismatch list. |

## Rejected probes

No near-match was promoted:

| Owner | Evidence for deferral |
| --- | --- |
| `blocks/stacker.c` | `ProcessStacker` remains target/source `0xD4/0xD8` at 95.754715%; the remaining owner is declaration/register shape, not an easy port. |
| `blocks/undersam.c` | Process and init remain 96.956520% and 94.375000%; control and construction alone are exact. |
| `flblocks/mtxopt.c` | Both fill helpers are exact after disabling contraction, but `QrPreMult` remains target/source `0x1A8/0x1A8` at 95.235850% with a pure register-allocation cycle. |
| `flfxblks/lkahead.c` | Process, control, and construction are exact; `InitLookAhead` remains target/source `0xD0/0xD0` at 94.326920%. |
| `flblocks/logexp.c` | The helper is exact, but `LogAdd` remains target/source `0x138/0x138` at 99.743590% with four floating-register operand mismatches. Reversing the multiply expression was byte-neutral, so the probe was removed. |
| `flfxblks/genfilt.c` | Three functions are exact; the constructor remains 85.806450% because callback-address materialization order differs. |
| `MSL_C.PPCEABI.bare.H/e_exp.c` | The Pikmin 2 Matching fdlibm donor at `46aecad6` was compiled once under the MP6 MSL flags. `__ieee754_exp` is target/source `0x21C/0x224` at 91.592590%, so the donor is not an exact MP6 port and was removed. |

All rejected owners remain `NonMatching` and fallback-linked.

## Final gate

The batch passed one final gate after all promotions and documentation changes:

```text
rtk ninja -j1
137 files OK

rtk build/tools/dtk.exe shasum -q -c config/GP6E01/build.sha1
137 files OK

rtk cmp orig/GP6E01/sys/main.dol build/GP6E01/main.dol
byte-identical (exit 0)
```

The DOL policy totals become 245 clean-C matching owners, 32 matching
sibling-exception owners, 118 `C-not-yet-matched` fallbacks, and one
`original-was-asm` fallback.
