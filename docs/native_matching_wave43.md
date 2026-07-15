# Native matching wave 43: GSSDK signal pipeline

Wave 43 recovers six formerly `NO-SOURCE` Game Speech SDK owners as compiled,
target-derived C. All 32 retained functions are present and pair against their
target ranges. Ten functions are byte-exact, but no whole owner passes the
object gate, so every owner remains `NonMatching` and fallback-linked.

## Tool and evidence provenance

- DTK: `dtk.exe 0.9.2 4d039140f2d2ed80572b1949b76a5ff9b3094e06`.
- Objdiff CLI: `3.7.3`.
- Compiler: project-pinned Game Speech SDK `GC/1.2.5n`, with floating-point
  contraction disabled as required by the retained target instructions.
- Recovery inputs: MP6 target instructions, section bytes, relocations,
  function boundaries, callers, callees, and cross-owner field consumers.

The local `mwcc-decomp` repository was consulted read-only for one compiler
semantic: its PowerPC intrinsic table declares `__abs` as a signed-integer
intrinsic and lowers it through `abs_intrinsic`. That independently explains
why the target's `Subsampler_CheckImpulseResponse` converts a floating result
to an integer before taking its absolute value. No source or generated output
from the dirty reference repository was copied into this patch.

## Recovered ownership and ABI

The two FFT owners recover the Mayer fast-Hartley transform, the real-FFT
wrapper, both exact `0x40` trigonometric tables, and the complete `0x4C`
`FFTMod` block. The window owner recovers the complete `0x50` block and its
flush, process, control, initialization, and construction paths.

The shared `TriggerLR` header now describes the complete `0x19C` object rather
than opaque post-base storage. Target instructions and cross-owner accesses
place the histogram state at `0xE8`, voicing state at `0x114`, and subsampler
state at `0x158`; compile-time offset and size assertions protect those facts.
This layout supports all retained functions in `slidhist.c`, `subsamp.c`, and
`voicing.c`, including the histogram's dynamic session payload.

The 23-double impulse-response initializer at `0x8021A250..0x8021A308` is now
assigned to `subsamp.c`. `Subsampler_Init` is its only semantic consumer, while
`voicing.c` has no reference to it. The split change corrects ownership without
changing the linked bytes.

## Batched object evidence

All percentages below are raw `.text` objdiff scores from current DTK target
splits and freshly compiled source objects.

| Owner | Target/source `.text` | Raw score | Exact retained functions | Other exact sections |
| --- | ---: | ---: | --- | --- |
| `fft_maye.c` | `0x8F0/0x8BC` | 79.104900% | none | `.rodata 0x80` |
| `fftmod.c` | `0xC84/0xC78` | 93.288390% | `ControlFFTMod` | none |
| `window.c` | `0xBFC/0xC00` | 95.573660% | `ControlWindow`, `ConstructWindow` | `.sdata2 0x20` |
| `slidhist.c` | `0x53C/0x538` | 95.713430% | `SlidingHistogram_Clear`, `SlidingHistogram_Free`, `SlidingHistogram_sizeof_SessionData`, `SlidingHistogram_GetSessionData` | `.sdata2 0x28` |
| `subsamp.c` | `0x6E8/0x6E8` | 97.031680% | `Subsampler_Process`, `Subsampler_Free` | `.rodata 0xB8`, `.sdata2 0x18` |
| `voicing.c` | `0x878/0x910` | 84.142070% | `Voicing_Free` | none |

The closest non-exact functions are retained as real C work rather than being
forced: `SlidingHistogram_PutSessionData` is 99.912280%,
`Subsampler_CheckImpulseResponse` is 99.459460%, and
`Subsampler_Reset` is 98.486490%. `fft_maye.c`, `fftmod.c`, `window.c`, and
`voicing.c` retain larger compiler-shape differences. None is promoted merely
because individual functions or data sections match.

## Final gate

- `rtk ninja -j1`: `137 files OK`.
- `rtk proxy build/tools/dtk.exe shasum -q -c config/GP6E01/build.sha1`:
  `137 files OK`.
- `rtk cmp orig/GP6E01/sys/main.dol build/GP6E01/main.dol`: exit `0`.
- Matching counts remain 293/895 owners overall and 284/396 DOL owners; the
  six new source owners remain in the `C-not-yet-matched` fallback bucket.
