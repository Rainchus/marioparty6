#!/usr/bin/env python3

###
# Generates build files for the project.
# This file also includes the project configuration,
# such as compiler flags and the object matching status.
#
# Usage:
#   python3 configure.py
#   ninja
#
# Append --help to see available options.
###

import argparse
import sys
from pathlib import Path
from typing import Any, Dict, List

from tools.project import (
    Object,
    ProjectConfig,
    calculate_progress,
    generate_build,
    is_windows,
)

# Game versions
DEFAULT_VERSION = 0
VERSIONS = [
    "GP6E01",  # 0
]

parser = argparse.ArgumentParser()
parser.add_argument(
    "mode",
    choices=["configure", "progress"],
    default="configure",
    help="script mode (default: configure)",
    nargs="?",
)
parser.add_argument(
    "-v",
    "--version",
    choices=VERSIONS,
    type=str.upper,
    default=VERSIONS[DEFAULT_VERSION],
    help="version to build",
)
parser.add_argument(
    "--build-dir",
    metavar="DIR",
    type=Path,
    default=Path("build"),
    help="base build directory (default: build)",
)
parser.add_argument(
    "--binutils",
    metavar="BINARY",
    type=Path,
    help="path to binutils (optional)",
)
parser.add_argument(
    "--compilers",
    metavar="DIR",
    type=Path,
    help="path to compilers (optional)",
)
parser.add_argument(
    "--map",
    action="store_true",
    help="generate map file(s)",
)
parser.add_argument(
    "--no-asm",
    action="store_true",
    help="don't incorporate .s files from asm directory",
)
parser.add_argument(
    "--debug",
    action="store_true",
    help="build with debug info (non-matching)",
)
if not is_windows():
    parser.add_argument(
        "--wrapper",
        metavar="BINARY",
        type=Path,
        help="path to wibo or wine (optional)",
    )
parser.add_argument(
    "--dtk",
    metavar="BINARY | DIR",
    type=Path,
    help="path to decomp-toolkit binary or source (optional)",
)
parser.add_argument(
    "--sjiswrap",
    metavar="EXE",
    type=Path,
    help="path to sjiswrap.exe (optional)",
)
parser.add_argument(
    "--verbose",
    action="store_true",
    help="print verbose output",
)
parser.add_argument(
    "--non-matching",
    dest="non_matching",
    action="store_true",
    help="builds equivalent (but non-matching) or modded objects",
)
args = parser.parse_args()

config = ProjectConfig()
config.version = str(args.version)
version_num = VERSIONS.index(config.version)

# Apply arguments
config.build_dir = args.build_dir
config.dtk_path = args.dtk
config.binutils_path = args.binutils
config.compilers_path = args.compilers
config.debug = args.debug
config.generate_map = args.map
config.non_matching = args.non_matching
config.sjiswrap_path = args.sjiswrap
if not is_windows():
    config.wrapper = args.wrapper
if args.no_asm:
    config.asm_dir = None

# Tool versions
config.binutils_tag = "2.42-1"
config.compilers_tag = "20240706"
config.dtk_tag = "v0.9.2"
config.sjiswrap_tag = "v1.1.1"
config.wibo_tag = "0.6.11"

# Project
config.config_path = Path("config") / config.version / "config.yml"
config.check_sha_path = Path("config") / config.version / "build.sha1"
config.asflags = [
    "-mgekko",
    "--strip-local-absolute",
    "-I include",
    f"-I build/{config.version}/include",
    f"--defsym version={version_num}",
]
config.ldflags = [
    "-fp hardware",
    "-nodefaults",
    # "-listclosure", # Uncomment for Wii linkers
]
# Use for any additional files that should cause a re-configure when modified
config.reconfig_deps = []

# Base flags, common to most GC/Wii games.
# Generally leave untouched, with overrides added below.
cflags_base = [
    "-nodefaults",
    "-proc gekko",
    "-align powerpc",
    "-enum int",
    "-fp hardware",
    "-Cpp_exceptions off",
    # "-W all",
    "-O4,p",
    "-inline auto",
    '-pragma "cats off"',
    '-pragma "warn_notinlined off"',
    "-maxerrors 1",
    "-nosyspath",
    "-RTTI off",
    "-fp_contract on",
    "-str reuse",
    "-multibyte",  # For Wii compilers, replace with `-enc SJIS`
    "-i include",
    f"-i build/{config.version}/include",
    "-DMUSY_TARGET=MUSY_TARGET_DOLPHIN",
    f"-DVERSION={version_num}",
]

# Debug flags
if config.debug:
    cflags_base.extend(["-sym on", "-DDEBUG=1"])
else:
    cflags_base.append("-DNDEBUG=1")

# Metrowerks library flags
cflags_runtime = [
    *cflags_base,
    "-use_lmw_stmw on",
    "-str reuse,pool,readonly",
    "-gccinc",
    "-common off",
    "-inline auto",
]

cflags_trk = [
    *cflags_base,
    "-use_lmw_stmw on",
    "-str reuse,readonly",
    "-common off",
    "-sdata 0",
    "-sdata2 0",
    "-inline auto,deferred",
    "-enum min",
    "-sdatathreshold 0",
]

# MusyX flags
cflags_musyx = [
    *cflags_base,
    "-str reuse,pool,readonly",
    "-fp_contract off",
    "-DMUSY_VERSION_MAJOR=2",
    "-DMUSY_VERSION_MINOR=0",
    "-DMUSY_VERSION_PATCH=4",
]

# REL flags
cflags_rel = [
    *cflags_base,
    "-O0,p",
    "-char unsigned",
    "-fp_contract off",
    "-sdata 0",
    "-sdata2 0",
]

# Game flags
cflags_game = [
    *cflags_base,
    "-O0,p",
    "-char unsigned",
    "-fp_contract off",
]

# Zlib flags
cflags_zlib = [
    *cflags_base,
    "-O0,p",
    "-fp_contract off",
]


# Game flags
cflags_libhu = [
    *cflags_base,
    "-O0,p",
    "-char unsigned",
    "-fp_contract off",
]

# Game flags
cflags_msm = [
    *cflags_base,
]


# Mic SDK flags
cflags_dolphin = [
    *cflags_base,
]

cflags_thp = [
    *cflags_base,
]


# Mic SDK flags
cflags_sdk_mic = [
    *cflags_base,
]

# Game Speech SDK flags
cflags_gssdk = [
    *cflags_base,
    "-fp_contract off",
]

config.linker_version = "GC/2.6"
config.rel_strip_partial = False
config.rel_empty_file = "REL/empty.c"

# Helper function for Dolphin libraries
def DolphinLib(lib_name: str, objects: List[Object]) -> Dict[str, Any]:
    return {
        "lib": lib_name,
        "mw_version": "GC/1.2.5n",
        "cflags": cflags_dolphin,
        "host": False,
        "objects": objects,
    }


# Helper function for REL script objects
def Rel(lib_name: str, objects: List[Object]) -> Dict[str, Any]:
    return {
        "lib": lib_name,
        "mw_version": "GC/1.3.2",
        "cflags": cflags_rel,
        "host": True,
        "objects": objects,
    }


Matching = True                   # Object matches and should be linked
NonMatching = False               # Object does not match and should not be linked
Equivalent = config.non_matching  # Object should be linked when configured with --non-matching

config.warn_missing_config = True
config.warn_missing_source = False
config.libs = [
    {
        "lib": "Runtime.PPCEABI.H",
        "mw_version": config.linker_version,
        "cflags": cflags_runtime,
        "host": False,
        "objects": [
            Object(Matching, "Runtime.PPCEABI.H/__va_arg.c"),
            Object(Matching, "Runtime.PPCEABI.H/global_destructor_chain.c"),
            Object(
                NonMatching,
                "Runtime.PPCEABI.H/New.cp",
                extra_cflags=["-Cpp_exceptions on"],
            ),
            Object(
                NonMatching,
                "Runtime.PPCEABI.H/NewMore.cp",
                extra_cflags=[
                    "-Cpp_exceptions on",
                    "-RTTI on",
                    "-inline auto,deferred",
                ],
            ),
            Object(
                NonMatching,
                "Runtime.PPCEABI.H/NMWException.cpp",
                extra_cflags=["-Cpp_exceptions on", "-inline auto,deferred"],
            ),
            Object(Matching, "Runtime.PPCEABI.H/ptmf.c"),
            Object(Matching, "Runtime.PPCEABI.H/runtime.c"),
            Object(Matching, "Runtime.PPCEABI.H/__init_cpp_exceptions.cpp"),
            Object(
                NonMatching,
                "Runtime.PPCEABI.H/Gecko_ExceptionPPC.cpp",
                cflags=[
                    (
                        "-str reuse,readonly"
                        if flag.startswith("-str ")
                        else "-inline auto,deferred"
                        if flag.startswith("-inline ")
                        else flag
                    )
                    for flag in cflags_runtime
                ],
                extra_cflags=["-Cpp_exceptions on", "-RTTI on"],
            ),
            Object(
                Matching,
                "Runtime.PPCEABI.H/GCN_mem_alloc.c",
                cflags=[
                    "-str reuse,readonly" if flag.startswith("-str ") else flag
                    for flag in cflags_runtime
                ],
            ),
            Object(Matching, "Runtime.PPCEABI.H/__mem.c"),
        ],
    },
    {
        "lib": "MSL_C.PPCEABI.bare.H",
        "mw_version": config.linker_version,
        "cflags": [*cflags_runtime, "-inline deferred"],
        "host": False,
        "objects": [
            Object(Matching, "MSL_C.PPCEABI.bare.H/abort_exit.c"),
            Object(NonMatching, "MSL_C.PPCEABI.bare.H/alloc.c"),
            Object(Matching, "MSL_C.PPCEABI.bare.H/errno.c"),
            Object(Matching, "MSL_C.PPCEABI.bare.H/ansi_files.c"),
            Object(
                Matching,
                "MSL_C.PPCEABI.bare.H/ansi_fp.c",
                mw_version="GC/1.3",
            ),
            Object(Matching, "MSL_C.PPCEABI.bare.H/arith.c"),
            Object(Matching, "MSL_C.PPCEABI.bare.H/assert.c"),
            Object(Matching, "MSL_C.PPCEABI.bare.H/buffer_io.c"),
            Object(Matching, "MSL_C.PPCEABI.bare.H/ctype.c"),
            Object(Matching, "MSL_C.PPCEABI.bare.H/direct_io.c"),
            Object(
                Matching,
                "MSL_C.PPCEABI.bare.H/file_io.c",
                mw_version="GC/1.3",
            ),
            Object(Matching, "MSL_C.PPCEABI.bare.H/FILE_POS.c"),
            Object(
                Matching,
                "MSL_C.PPCEABI.bare.H/mbstring.c",
                mw_version="GC/1.3",
            ),
            Object(Matching, "MSL_C.PPCEABI.bare.H/mem.c"),
            Object(
                Matching,
                "MSL_C.PPCEABI.bare.H/mem_funcs.c",
                mw_version="GC/1.3",
            ),
            Object(Matching, "MSL_C.PPCEABI.bare.H/misc_io.c"),
            Object(NonMatching, "MSL_C.PPCEABI.bare.H/printf.c"),
            Object(NonMatching, "MSL_C.PPCEABI.bare.H/qsort.c"),
            Object(Matching, "MSL_C.PPCEABI.bare.H/float.c"),
            Object(Matching, "MSL_C.PPCEABI.bare.H/signal.c"),
            Object(Matching, "MSL_C.PPCEABI.bare.H/string.c"),
            Object(Matching, "MSL_C.PPCEABI.bare.H/uart_console_io.c"),
            Object(Matching, "MSL_C.PPCEABI.bare.H/wchar_io.c"),
            Object(Matching, "MSL_C.PPCEABI.bare.H/e_acos.c"),
            Object(Matching, "MSL_C.PPCEABI.bare.H/e_asin.c"),
            Object(Matching, "MSL_C.PPCEABI.bare.H/e_atan2.c"),
            Object(NonMatching, "MSL_C.PPCEABI.bare.H/e_exp.c"),
            Object(Matching, "MSL_C.PPCEABI.bare.H/e_fmod.c"),
            Object(Matching, "MSL_C.PPCEABI.bare.H/e_log.c"),
            Object(Matching, "MSL_C.PPCEABI.bare.H/e_log10.c"),
            Object(
                Matching,
                "MSL_C.PPCEABI.bare.H/e_pow.c",
                mw_version="GC/1.3",
            ),
            Object(Matching, "MSL_C.PPCEABI.bare.H/e_rem_pio2.c"),
            Object(Matching, "MSL_C.PPCEABI.bare.H/k_cos.c"),
            Object(Matching, "MSL_C.PPCEABI.bare.H/k_rem_pio2.c"),
            Object(Matching, "MSL_C.PPCEABI.bare.H/k_sin.c"),
            Object(Matching, "MSL_C.PPCEABI.bare.H/k_tan.c"),
            Object(
                Matching,
                "MSL_C.PPCEABI.bare.H/s_atan.c",
                mw_version="GC/1.3",
            ),
            Object(Matching, "MSL_C.PPCEABI.bare.H/s_ceil.c"),
            Object(Matching, "MSL_C.PPCEABI.bare.H/s_copysign.c"),
            Object(Matching, "MSL_C.PPCEABI.bare.H/s_cos.c"),
            Object(Matching, "MSL_C.PPCEABI.bare.H/s_floor.c"),
            Object(Matching, "MSL_C.PPCEABI.bare.H/s_frexp.c"),
            Object(Matching, "MSL_C.PPCEABI.bare.H/s_ldexp.c"),
            Object(Matching, "MSL_C.PPCEABI.bare.H/s_modf.c"),
            Object(Matching, "MSL_C.PPCEABI.bare.H/s_sin.c"),
            Object(Matching, "MSL_C.PPCEABI.bare.H/s_tan.c"),
            Object(Matching, "MSL_C.PPCEABI.bare.H/w_acos.c"),
            Object(Matching, "MSL_C.PPCEABI.bare.H/w_asin.c"),
            Object(Matching, "MSL_C.PPCEABI.bare.H/w_atan2.c"),
            Object(Matching, "MSL_C.PPCEABI.bare.H/w_exp.c"),
            Object(Matching, "MSL_C.PPCEABI.bare.H/w_fmod.c"),
            Object(Matching, "MSL_C.PPCEABI.bare.H/w_log.c"),
            Object(Matching, "MSL_C.PPCEABI.bare.H/w_log10.c"),
            Object(Matching, "MSL_C.PPCEABI.bare.H/w_pow.c"),
            Object(Matching, "MSL_C.PPCEABI.bare.H/math_ppc.c"),
        ],
    },
    {
        "lib": "TRK_MINNOW_DOLPHIN",
        "mw_version": "GC/1.3",
        "cflags": cflags_trk,
        "host": False,
        "objects": [
            Object(Matching, "TRK_MINNOW_DOLPHIN/mainloop.c"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/nubevent.c"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/nubinit.c"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/msg.c"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/msgbuf.c"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/serpoll.c"),
            Object(
                Matching,
                "TRK_MINNOW_DOLPHIN/usr_put.c",
                mw_version="GC/1.3.2",
            ),
            Object(Matching, "TRK_MINNOW_DOLPHIN/dispatch.c"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/msghndlr.c"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/support.c"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/mutex_TRK.c"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/notify.c"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/flush_cache.c"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/mem_TRK.c"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/targimpl.c"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/targsupp.s"),
            Object(NonMatching, "TRK_MINNOW_DOLPHIN/__exception.s"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/dolphin_trk.c"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/mpc_7xx_603e.c"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/main_TRK.c"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/dolphin_trk_glue.c"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/targcont.c"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/target_options.c"),
            Object(Matching, "TRK_MINNOW_DOLPHIN/mslsupp.c"),
        ],
    },
    {
        "lib": "musyx",
        "mw_version": "GC/1.3.2",
        "cflags": cflags_musyx,
        "host": False,
        "objects": [
            Object(Matching, "musyx/runtime/seq.c"),
            Object(NonMatching, "musyx/runtime/synth.c"),
            Object(Matching, "musyx/runtime/seq_api.c"),
            Object(Matching, "musyx/runtime/snd_synthapi.c"),
            Object(NonMatching, "musyx/runtime/stream.c"),
            Object(NonMatching, "musyx/runtime/synthdata.c"),
            Object(NonMatching, "musyx/runtime/synthmacros.c"),
            Object(NonMatching, "musyx/runtime/synthvoice.c"),
            Object(Matching, "musyx/runtime/synth_ac.c"),
            Object(Matching, "musyx/runtime/synth_dbtab.c"),
            Object(Matching, "musyx/runtime/synth_adsr.c"),
            Object(Matching, "musyx/runtime/synth_vsamples.c"),
            Object(NonMatching, "musyx/runtime/s_data.c"),
            Object(NonMatching, "musyx/runtime/hw_dspctrl.c"),
            Object(Matching, "musyx/runtime/hw_volconv.c"),
            Object(NonMatching, "musyx/runtime/snd3d.c"),
            Object(Matching, "musyx/runtime/snd_init.c"),
            Object(Matching, "musyx/runtime/snd_math.c"),
            Object(NonMatching, "musyx/runtime/snd_midictrl.c"),
            Object(Matching, "musyx/runtime/snd_service.c"),
            Object(NonMatching, "musyx/runtime/hardware.c"),
            Object(NonMatching, "musyx/runtime/dsp_import.c"),
            Object(NonMatching, "musyx/runtime/hw_aramdma.c"),
            Object(Matching, "musyx/runtime/hw_dolphin.c"),
            Object(Matching, "musyx/runtime/hw_memory.c"),
            Object(Matching, "musyx/runtime/CheapReverb/creverb_fx.c"),
            Object(Matching, "musyx/runtime/CheapReverb/creverb.c"),
            Object(Matching, "musyx/runtime/StdReverb/reverb_fx.c"),
            Object(NonMatching, "musyx/runtime/StdReverb/reverb.c"),
            Object(Matching, "musyx/runtime/Delay/delay_fx.c"),
            Object(Matching, "musyx/runtime/Chorus/chorus_fx.c"),
        ],
    },
    {
        "lib": "OdemuExi2",
        "mw_version": config.linker_version,
        "cflags": cflags_runtime,
        "host": False,
        "objects": [
            Object(
                Matching,
                "OdemuExi2/DebuggerDriver.c",
                cflags=[
                    "-nodefaults",
                    "-proc gekko",
                    "-align powerpc",
                    "-enum int",
                    "-multibyte",
                    "-char unsigned",
                    "-fp hardware",
                    "-Cpp_exceptions off",
                    '-pragma "cats off"',
                    '-pragma "warn_notinlined off"',
                    "-maxerrors 1",
                    "-nosyspath",
                    "-i include",
                    f"-i build/{config.version}/include",
                    "-DMUSY_TARGET=MUSY_TARGET_DOLPHIN",
                    f"-DVERSION={version_num}",
                    "-DNDEBUG=1",
                    "-O4,p",
                    "-inline all",
                    "-char signed",
                ],
                extra_cflags=["-inline deferred"],
                mw_version="GC/1.2.5n",
            ),
        ],
    },
    {
        "lib": "amcstubs",
        "mw_version": config.linker_version,
        "cflags": cflags_runtime,
        "host": False,
        "objects": [
            Object(Matching, "amcstubs/AmcExi2Stubs.c"),
        ],
    },
    {
        "lib": "odenotstub",
        "mw_version": config.linker_version,
        "cflags": cflags_runtime,
        "host": False,
        "objects": [
            Object(Matching, "odenotstub/odenotstub.c"),
        ],
    },
    {
        "lib": "Game",
        "mw_version": config.linker_version,
        "cflags": cflags_game,
        "host": False,
        "objects": [
            Object(Matching, "game/main.c"),
            Object(Matching, "game/pad.c"),
            Object(Matching, "game/dvd.c"),
            Object(Matching, "game/data.c"),
            Object(Matching, "game/decode.c"),
            Object(Matching, "game/font.c"),
            Object(Matching, "game/init.c"),
            Object(Matching, "game/jmp.c"),
            Object(Matching, "game/malloc.c"),
            Object(Matching, "game/memory.c"),
            Object(Matching, "game/printfunc.c"),
            Object(Matching, "game/process.c"),
            Object(Matching, "game/sprman.c"),
            Object(Matching, "game/sprput.c"),
            Object(Matching, "game/hsfload.c"),
            Object(Matching, "game/hsfdraw.c"),
            Object(Matching, "game/hsfman.c"),
            Object(Matching, "game/hsfmotion.c"),
            Object(Matching, "game/hsfanim.c"),
            Object(Matching, "game/hsfex.c"),
            Object(Matching, "game/perf.c"),
            Object(Matching, "game/objmain.c"),
            Object(Matching, "game/fault.c"),
            Object(Matching, "game/gamework.c"),
            Object(Matching, "game/objsysobj.c"),
            Object(Matching, "game/objdll.c"),
            Object(Matching, "game/frand.c"),
            Object(Matching, "game/audio.c"),
            Object(Matching, "game/EnvelopeExec.c"),
            Object(Matching, "game/gamemes.c"),
            Object(Matching, "game/esprite.c"),
            Object(Matching, "game/ovllist.c"),
            Object(Matching, "game/ClusterExec.c"),
            Object(Matching, "game/ShapeExec.c"),
            Object(Matching, "game/wipe.c"),
            Object(Matching, "game/window.c"),
            Object(Matching, "game/card.c"),
            Object(Matching, "game/armem.c"),
            Object(Matching, "game/charman.c"),
            Object(Matching, "game/mapspace.c"),
            Object(Matching, "game/THPSimple.c"),
            Object(Matching, "game/THPDraw.c"),
            Object(Matching, "game/thpmain.c"),
            Object(Matching, "game/mgdata.c"),
            Object(Matching, "game/objsub.c"),
            Object(Matching, "game/flag.c"),
            Object(Matching, "game/saveload.c"),
            Object(Matching, "game/sreset.c"),
            Object(Matching, "game/mgtimer.c"),
            Object(Matching, "game/mgscore.c"),
            Object(Matching, "game/seqman.c"),
            Object(Matching, "game/colman.c"),
            Object(Matching, "game/actman.c"),
            Object(Matching, "game/mggamemes.c"),
            Object(Matching, "game/mic.c"),
            Object(Matching, "game/code_80146BA0.c"),
            Object(Matching, "game/kerent.c"),
            Object(Matching, "board/malloc.c"),
            Object(Matching, "board/comchoice.c"),
            Object(NonMatching, "board/board.c"),
            Object(Matching, "board/exit.c"),
        ],
    },
    DolphinLib(
        "base",
        [
            Object(Matching, "dolphin/base/PPCArch.c"),
        ],
    ),
    DolphinLib(
        "os",
        [
            Object(
                Matching,
                "dolphin/os/OS.c",
                extra_cflags=["-char unsigned", "-DSDK_REVISION=1"],
            ),
            Object(Matching, "dolphin/os/OSAlarm.c"),
            Object(Matching, "dolphin/os/OSAlloc.c"),
            Object(Matching, "dolphin/os/OSArena.c"),
            Object(Matching, "dolphin/os/OSAudioSystem.c"),
            Object(Matching, "dolphin/os/OSCache.c"),
            Object(Matching, "dolphin/os/OSContext.c"),
            Object(Matching, "dolphin/os/OSError.c"),
            Object(Matching, "dolphin/os/OSExec.c"),
            Object(Matching, "dolphin/os/OSFont.c", extra_cflags=["-char unsigned"]),
            Object(Matching, "dolphin/os/OSInterrupt.c"),
            Object(Matching, "dolphin/os/OSLink.c"),
            Object(Matching, "dolphin/os/OSMessage.c"),
            Object(Matching, "dolphin/os/OSMemory.c"),
            Object(Matching, "dolphin/os/OSMutex.c"),
            Object(Matching, "dolphin/os/OSReboot.c"),
            Object(Matching, "dolphin/os/OSReset.c"),
            Object(Matching, "dolphin/os/OSResetSW.c"),
            Object(Matching, "dolphin/os/OSRtc.c"),
            Object(Matching, "dolphin/os/OSSemaphore.c"),
            Object(Matching, "dolphin/os/OSStopwatch.c"),
            Object(Matching, "dolphin/os/OSSync.c"),
            Object(Matching, "dolphin/os/OSThread.c"),
            Object(Matching, "dolphin/os/OSTime.c"),
            Object(Matching, "dolphin/os/__start.c"),
            Object(Matching, "dolphin/os/__ppc_eabi_init.c"),
        ],
    ),
    DolphinLib(
        "db",
        [
            Object(Matching, "dolphin/db/db.c"),
        ],
    ),
    DolphinLib(
        "mtx",
        [
            Object(Matching, "dolphin/mtx/mtx.c"),
            Object(Matching, "dolphin/mtx/mtxvec.c"),
            Object(Matching, "dolphin/mtx/mtx44.c"),
            Object(Matching, "dolphin/mtx/mtx44vec.c"),
            Object(Matching, "dolphin/mtx/vec.c"),
            Object(Matching, "dolphin/mtx/quat.c"),
            Object(Matching, "dolphin/mtx/psmtx.c"),
        ],
    ),
    DolphinLib(
        "dvd",
        [
            Object(Matching, "dolphin/dvd/dvdlow.c"),
            Object(Matching, "dolphin/dvd/dvdfs.c"),
            Object(Matching, "dolphin/dvd/dvd.c"),
            Object(Matching, "dolphin/dvd/dvdqueue.c"),
            Object(Matching, "dolphin/dvd/dvderror.c"),
            Object(Matching, "dolphin/dvd/dvdidutils.c"),
            Object(Matching, "dolphin/dvd/dvdFatal.c"),
            Object(Matching, "dolphin/dvd/fstload.c"),
        ],
    ),
    DolphinLib(
        "vi",
        [
            Object(Matching, "dolphin/vi/vi.c"),
        ],
    ),
    DolphinLib(
        "demo",
        [
            Object(Matching, "dolphin/demo/DEMOInit.c"),
            Object(Matching, "dolphin/demo/DEMOFont.c"),
            Object(Matching, "dolphin/demo/DEMOPuts.c"),
            Object(Matching, "dolphin/demo/DEMOStats.c"),
        ],
    ),
    DolphinLib(
        "pad",
        [
            Object(
                Matching,
                "dolphin/pad/Padclamp.c",
                extra_cflags=["-fp_contract off"],
            ),
            Object(Matching, "dolphin/pad/Pad.c"),
        ],
    ),
    DolphinLib(
        "ai",
        [
            Object(Matching, "dolphin/ai/ai.c"),
        ],
    ),
    DolphinLib(
        "ar",
        [
            Object(Matching, "dolphin/ar/ar.c"),
            Object(Matching, "dolphin/ar/arq.c"),
        ],
    ),
    DolphinLib(
        "dsp",
        [
            Object(Matching, "dolphin/dsp/dsp.c"),
            Object(Matching, "dolphin/dsp/dsp_debug.c"),
            Object(Matching, "dolphin/dsp/dsp_task.c"),
        ],
    ),
    DolphinLib(
        "gx",
        [
            Object(
                Matching,
                "dolphin/gx/GXInit.c",
                extra_cflags=["-opt nopeephole"],
            ),
            Object(Matching, "dolphin/gx/GXFifo.c"),
            Object(Matching, "dolphin/gx/GXAttr.c"),
            Object(Matching, "dolphin/gx/GXMisc.c"),
            Object(Matching, "dolphin/gx/GXGeometry.c"),
            Object(Matching, "dolphin/gx/GXFrameBuf.c"),
            Object(Matching, "dolphin/gx/GXLight.c", extra_cflags=["-fp_contract off"]),
            Object(Matching, "dolphin/gx/GXTexture.c"),
            Object(Matching, "dolphin/gx/GXBump.c"),
            Object(Matching, "dolphin/gx/GXTev.c"),
            Object(Matching, "dolphin/gx/GXPixel.c"),
            Object(Matching, "dolphin/gx/GXDraw.c"),
            Object(Matching, "dolphin/gx/GXDisplayList.c"),
            Object(
                Matching,
                "dolphin/gx/GXTransform.c",
                extra_cflags=["-fp_contract off"],
            ),
            Object(Matching, "dolphin/gx/GXPerf.c"),
        ],
    ),
    DolphinLib(
        "card",
        [
            Object(Matching, "dolphin/card/CARDBios.c"),
            Object(Matching, "dolphin/card/CARDUnlock.c"),
            Object(Matching, "dolphin/card/CARDRdwr.c"),
            Object(Matching, "dolphin/card/CARDBlock.c"),
            Object(Matching, "dolphin/card/CARDDir.c"),
            Object(Matching, "dolphin/card/CARDCheck.c"),
            Object(Matching, "dolphin/card/CARDMount.c"),
            Object(Matching, "dolphin/card/CARDFormat.c"),
            Object(Matching, "dolphin/card/CARDOpen.c"),
            Object(Matching, "dolphin/card/CARDCreate.c"),
            Object(Matching, "dolphin/card/CARDRead.c"),
            Object(Matching, "dolphin/card/CARDWrite.c"),
            Object(Matching, "dolphin/card/CARDDelete.c"),
            Object(Matching, "dolphin/card/CARDStat.c"),
            Object(Matching, "dolphin/card/CARDNet.c"),
        ],
    ),
    DolphinLib(
        "exi",
        [
            Object(Matching, "dolphin/exi/EXIBios.c"),
            Object(Matching, "dolphin/exi/EXIUart.c"),
        ],
    ),
    DolphinLib(
        "si",
        [
            Object(Matching, "dolphin/si/SIBios.c"),
            Object(Matching, "dolphin/si/SISamplingRate.c"),
        ],
    ),
    {
        "lib": "thp",
        "mw_version": "GC/1.2.5",
        "cflags": cflags_thp,
        "host": False,
        "objects": [
            Object(Matching, "dolphin/thp/THPDec.c"),
            Object(Matching, "dolphin/thp/THPAudio.c"),
        ],
    },
    {
        "lib": "sdk_mic",
        "mw_version": "GC/1.2.5n",
        "cflags": cflags_sdk_mic,
        "host": False,
        "objects": [
            Object(
                NonMatching,
                "dolphin/mic/mic.c",
                extra_cflags=["-use_lmw_stmw on"],
            ),
            Object(
                Matching,
                "dolphin/mic/m2s.c",
                extra_cflags=["-use_lmw_stmw on"],
            ),
        ],
    },
    {
        "lib": "gssdk",
        "mw_version": "GC/1.2.5n",
        "cflags": cflags_gssdk,
        "host": False,
        "objects": [
            Object(Matching, "gssdk_lib/gsapi/sid/sid.c"),
            Object(NonMatching, "gssdk_lib/gsapi/callbacks.c"),
            Object(NonMatching, "gssdk_lib/gsapi/ctxfuncs.c"),
            Object(Matching, "gssdk_lib/gsapi/extaudio.c"),
            Object(NonMatching, "gssdk_lib/gsapi/gsapi.c"),
            Object(Matching, "gssdk_lib/gsapi/mathusage.c"),
            Object(Matching, "gssdk_lib/gsapi/wrddata.c"),
            Object(NonMatching, "gssdk_lib/asrpho/asrspi.c"),
            Object(NonMatching, "gssdk_lib/asrpho/rec1600/convert.c"),
            Object(NonMatching, "gssdk_lib/asrpho/rec1600/creasp.c"),
            Object(NonMatching, "gssdk_lib/asrpho/rec1600/creaspch.c"),
            Object(NonMatching, "gssdk_lib/asrpho/rec1600/creaspt.c"),
            Object(NonMatching, "gssdk_lib/asrpho/rec1600/creatree.c"),
            Object(NonMatching, "gssdk_lib/asrpho/rec1600/crsptrch.c"),
            Object(Matching, "gssdk_lib/asrpho/rec1600/ctrl.c"),
            Object(Matching, "gssdk_lib/asrpho/rec1600/initial.c"),
            Object(NonMatching, "gssdk_lib/asrpho/rec1600/spi1600.c"),
            Object(NonMatching, "gssdk_lib/asrpho/rec1600/train.c"),
            Object(NonMatching, "gssdk_lib/asrpho/rec1600/userword.c"),
            Object(NonMatching, "gssdk_lib/asrpho/common/blocks/delaybl.c"),
            Object(NonMatching, "gssdk_lib/asrpho/common/blocks/dpgenuw.c"),
            Object(NonMatching, "gssdk_lib/asrpho/common/blocks/dpscruw.c"),
            Object(NonMatching, "gssdk_lib/asrpho/common/blocks/exev_dp.c"),
            Object(NonMatching, "gssdk_lib/asrpho/common/blocks/fft_maye.c"),
            Object(NonMatching, "gssdk_lib/asrpho/common/blocks/fftmod.c"),
            Object(NonMatching, "gssdk_lib/asrpho/common/blocks/isoword.c"),
            Object(NonMatching, "gssdk_lib/asrpho/common/blocks/nbestdp.c"),
            Object(NonMatching, "gssdk_lib/asrpho/common/blocks/pitchdp.c"),
            Object(NonMatching, "gssdk_lib/asrpho/common/blocks/pitchwin.c"),
            Object(Matching, "gssdk_lib/asrpho/common/blocks/stacker.c"),
            Object(NonMatching, "gssdk_lib/asrpho/common/blocks/undersam.c"),
            Object(NonMatching, "gssdk_lib/asrpho/common/blocks/flblocks/acne.c"),
            Object(NonMatching, "gssdk_lib/asrpho/common/blocks/flblocks/dctlift.c"),
            Object(NonMatching, "gssdk_lib/asrpho/common/blocks/flblocks/gender.c"),
            Object(Matching, "gssdk_lib/asrpho/common/blocks/flblocks/logexp.c"),
            Object(NonMatching, "gssdk_lib/asrpho/common/blocks/flblocks/mel.c"),
            Object(NonMatching, "gssdk_lib/asrpho/common/blocks/flblocks/mtx.c"),
            Object(NonMatching, "gssdk_lib/asrpho/common/blocks/flblocks/mtxopt.c"),
            Object(NonMatching, "gssdk_lib/asrpho/common/blocks/flblocks/smoother.c"),
            Object(Matching, "gssdk_lib/asrpho/common/blocks/flblocks/spline.c"),
            Object(NonMatching, "gssdk_lib/asrpho/common/blocks/flblocks/specsub.c"),
            Object(NonMatching, "gssdk_lib/asrpho/common/blocks/flblocks/vad.c"),
            Object(NonMatching, "gssdk_lib/asrpho/common/blocks/flblocks/vq1500.c"),
            Object(NonMatching, "gssdk_lib/asrpho/common/blocks/flblocks/window.c"),
            Object(NonMatching, "gssdk_lib/asrpho/common/blocks/flfxblks/combiner.c"),
            Object(NonMatching, "gssdk_lib/asrpho/common/blocks/flfxblks/dist16.c"),
            Object(NonMatching, "gssdk_lib/asrpho/common/blocks/flfxblks/genfilt.c"),
            Object(Matching, "gssdk_lib/asrpho/common/blocks/flfxblks/lkahead.c"),
            Object(NonMatching, "gssdk_lib/asrpho/common/blocks/flfxblks/median.c"),
            Object(NonMatching, "gssdk_lib/asrpho/common/blocks/flfxblks/pitchco.c"),
            Object(NonMatching, "gssdk_lib/asrpho/common/blocks/flfxblks/shs_vuv.c"),
            Object(NonMatching, "gssdk_lib/asrpho/common/blocks/flfxblks/slidhist.c"),
            Object(NonMatching, "gssdk_lib/asrpho/common/blocks/flfxblks/statio.c"),
            Object(NonMatching, "gssdk_lib/asrpho/common/blocks/flfxblks/subsamp.c"),
            Object(NonMatching, "gssdk_lib/asrpho/common/blocks/flfxblks/trigglr.c"),
            Object(NonMatching, "gssdk_lib/asrpho/common/blocks/flfxblks/voicing.c"),
            Object(NonMatching, "gssdk_lib/asrpho/common/ctxdata/ctxdata.c"),
            Object(NonMatching, "gssdk_lib/asrpho/common/ctxdata/langdata.c"),
            Object(NonMatching, "gssdk_lib/asrpho/common/tos/mqueue.c"),
            Object(NonMatching, "gssdk_lib/asrpho/common/tos/tinyos.c"),
            Object(Matching, "gssdk_lib/asrpho/common/fastallo/fastallo.c"),
            Object(Matching, "gssdk_lib/common/csspi/csspi.c"),
            Object(Matching, "gssdk_lib/common/safeh/safeh.c"),
            Object(Matching, "gssdk_lib/common/osspi/osspi.c"),
            Object(Matching, "gssdk_lib/common/rsrc/rsrc.c"),
        ],
    },
    {
        "lib": "libhu",
        "mw_version": config.linker_version,
        "cflags": cflags_libhu,
        "host": False,
        "objects": [
            Object(Matching, "libhu/setvf.c"),
            Object(Matching, "libhu/subvf.c"),
        ],
    },
    {
        "lib": "msm",
        "mw_version": "GC/1.2.5",
        "cflags": cflags_msm,
        "host": False,
        "objects": [
            Object(NonMatching, "msm/msmsys.c"),
            Object(Matching, "msm/msmmem.c"),
            Object(Matching, "msm/msmfio.c"),
            Object(Matching, "msm/msmmus.c"),
            Object(Matching, "msm/msmse.c"),
            Object(NonMatching, "msm/msmstream.c"),
        ],
    },
    {
        "lib": "zlib",
        "mw_version": config.linker_version,
        "cflags": cflags_zlib,
        "host": False,
        "objects": [
            Object(Matching, "zlib/adler32.c"),
            Object(Matching, "zlib/inflate.c"),
            Object(Matching, "zlib/infblock.c"),
            Object(Matching, "zlib/infcodes.c"),
            Object(Matching, "zlib/infutil.c"),
            Object(Matching, "zlib/inftrees.c"),
            Object(Matching, "zlib/inffast.c"),
            Object(Matching, "zlib/zutil.c"),
        ],
    },
    {
        "lib": "board",
        "mw_version": config.linker_version,
        "cflags": cflags_game,
        "host": False,
        "objects": [
            Object(NonMatching, "board/math.c", extra_cflags=["-O4,p"]),
            Object(Matching, "board/camera.c"),
            Object(NonMatching, "board/player.c"),
            Object(NonMatching, "board/snpc.c"),
            Object(Matching, "board/object.c"),
            Object(Matching, "board/window.c"),
            Object(NonMatching, "board/audio.c"),
            Object(NonMatching, "board/scroll.c"),
            Object(NonMatching, "board/masu.c"),
            Object(NonMatching, "board/coin.c"),
            Object(NonMatching, "board/star.c"),
            Object(Matching, "board/padall.c"),
            Object(
                NonMatching,
                "board/dice.c",
                extra_cflags=["-O4,p", "-schedule off", "-opt nopeephole"],
            ),
            Object(Matching, "board/status.c"),
            Object(NonMatching, "board/opening.c"),
            Object(Matching, "board/pause.c"),
            Object(NonMatching, "board/tutorial.c"),
            Object(Matching, "board/roulette.c"),
            Object(NonMatching, "board/capselect.c"),
            Object(NonMatching, "board/capmove.c"),
            Object(NonMatching, "board/capthrow.c"),
            Object(NonMatching, "board/captrap.c"),
            Object(NonMatching, "board/capspecial.c"),
            Object(NonMatching, "board/capsule.c"),
            Object(NonMatching, "board/capevent.c"),
            Object(NonMatching, "board/shopevent.c"),
            Object(Matching, "board/guide.c"),
            Object(Matching, "board/branch.c"),
            Object(NonMatching, "board/mgcall.c"),
            Object(NonMatching, "board/effect.c", extra_cflags=["-O4,p"]),
            Object(NonMatching, "board/config.c"),
            Object(Matching, "board/gate.c"),
            Object(NonMatching, "board/last5.c"),
            Object(NonMatching, "board/telop.c"),
            Object(NonMatching, "board/wipe.c"),
            Object(NonMatching, "board/single.c"),
        ],
    },
    {
        "lib": "REL",
        "mw_version": config.linker_version,
        "cflags": cflags_rel,
        "host": False,
        "objects": [
            Object(Matching, "REL/empty.c"),  # Must be marked as matching
            Object(
                NonMatching,
                "REL/runtime.c",
                source="REL/runtime.c",
                extra_cflags=["-DMP6_REL_RUNTIME=1", "-proc 7400"],
            ),
        ],
    },
    Rel(
        "bootDll",
        objects={
            Object(Matching, "REL/bootDll/boot.c", mw_version=config.linker_version),
            Object(Matching, "REL/bootDll/data.c"),
            Object(Matching, "REL/bootDll/opening.c", mw_version=config.linker_version),
        },
    ),
    Rel(
        "selmenuDll",
        objects={
            Object(Matching, "REL/selmenuDll/selmenu.c", mw_version=config.linker_version),
            Object(
                NonMatching,
                "REL/selmenuDll/runtime.c",
                source="REL/selmenuDll/runtime.c",
                mw_version=config.linker_version,
                extra_cflags=["-DMP6_REL_RUNTIME=1", "-DMP6_SELMENU_RUNTIME=1", "-proc 7400"],
            ),
        },
    ),
    Rel(
        "fileseldll",
        objects={
            Object(Matching, "REL/fileseldll/filesel.c", mw_version=config.linker_version),
            Object(Matching, "REL/fileseldll/filename.c", mw_version=config.linker_version),
            Object(Matching, "REL/fileseldll/saveload.c", mw_version=config.linker_version),
            Object(
                NonMatching,
                "REL/fileseldll/runtime.c",
                source="REL/fileseldll/runtime.c",
                mw_version=config.linker_version,
                extra_cflags=["-DMP6_REL_RUNTIME=1", "-DMP6_FILESEL_RUNTIME=1", "-proc 7400"],
            ),
        },
    ),
    Rel(
        "mdseldll",
        objects={
            Object(Matching, "REL/mdseldll/mdsel.c"),
            Object(
                Matching,
                "REL/mdseldll/runtime.c",
                source="REL/mdseldll/runtime.c",
                mw_version=config.linker_version,
                extra_cflags=["-DMP6_REL_RUNTIME=1", "-DMP6_MDSEL_RUNTIME=1"],
            ),
        },
    ),
    Rel(
        "mdpartydll",
        objects={
            Object(NonMatching, "REL/mdpartydll/mdparty.c"),
            Object(Matching, "REL/mdpartydll/stage.c"),
            Object(
                Matching,
                "REL/mdpartydll/runtime.c",
                source="REL/mdpartydll/runtime.c",
                mw_version=config.linker_version,
                extra_cflags=["-DMP6_REL_RUNTIME=1"],
            ),
        },
    ),
    Rel(
        "sequencedll",
        objects={
            Object(
                Matching,
                "REL/sequencedll/sequence.c",
                mw_version=config.linker_version,
                extra_cflags=["-pooldata off"],
            ),
        },
    ),
    Rel(
        "actmanDLL",
        objects={
            Object(Matching, "REL/actmanDLL/actman.c", mw_version=config.linker_version),
        },
    ),
    Rel(
        "meschkdll",
        objects={
            Object(
                NonMatching,
                "REL/meschkdll/meschkdll.c",
                mw_version=config.linker_version,
                extra_cflags=["-pooldata off"],
            ),
        },
    ),

]

if args.mode == "configure":
    # Write build.ninja and objdiff.json
    generate_build(config)
elif args.mode == "progress":
    # Print progress and write progress.json
    config.progress_each_module = args.verbose
    calculate_progress(config)
else:
    sys.exit("Unknown mode: " + args.mode)
