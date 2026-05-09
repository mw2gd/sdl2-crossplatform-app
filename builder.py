import argparse
import shutil
import subprocess
import sys
from pathlib import Path

PROJECT_ROOT = Path(__file__).resolve().parent
BUILD_TYPES = {
    "debug": "-DCMAKE_BUILD_TYPE=Debug",
    "release": "-DCMAKE_BUILD_TYPE=Release",
}
OUT_DIRS = {
    "macos": PROJECT_ROOT / "out" / "macos",
    "web": PROJECT_ROOT / "out" / "web",
}


def require_tool(name, install_hint):
    if shutil.which(name):
        return

    print(f"Missing required tool: {name}", file=sys.stderr)
    print(install_hint, file=sys.stderr)
    raise SystemExit(1)


def run(command):
    print(" ".join(str(part) for part in command))
    subprocess.run(command, cwd=PROJECT_ROOT, check=True)


def build_macos(build_type):
    require_tool("cmake", "Install CMake or add it to PATH, then rerun this command.")

    build_dir = OUT_DIRS["macos"] / "build"
    run(["cmake", BUILD_TYPES[build_type], "-S", str(PROJECT_ROOT), "-B", str(build_dir)])
    run(["cmake", "--build", str(build_dir)])
    run(["cmake", "--install", str(build_dir)])


def build_web(build_type, run_browser, browser):
    require_tool("emcmake", "Install and activate Emscripten, for example: source /path/to/emsdk/emsdk_env.sh")
    require_tool("cmake", "Install CMake or add it to PATH, then rerun this command.")

    build_dir = OUT_DIRS["web"] / "build"
    run(["emcmake", "cmake", BUILD_TYPES[build_type], "-S", str(PROJECT_ROOT), "-B", str(build_dir)])
    run(["cmake", "--build", str(build_dir)])
    run(["cmake", "--install", str(build_dir)])

    if run_browser:
        require_tool("emrun", "Install and activate Emscripten, for example: source /path/to/emsdk/emsdk_env.sh")
        command = ["emrun", str(build_dir / "src" / "app.html")]
        if browser:
            command.extend(["--browser", browser])
        run(command)


def main():
    parser = argparse.ArgumentParser(description="Build the SDL2 sample app.")
    parser.add_argument("target", choices=("macos", "web"), help="Build target")
    parser.add_argument("--type", choices=BUILD_TYPES.keys(), default="debug", help="CMake build type")
    parser.add_argument("--run", action="store_true", help="Run the web build with emrun")
    parser.add_argument("--browser", help="Optional browser executable to pass to emrun")
    args = parser.parse_args()

    if args.target == "macos":
        build_macos(args.type)
    elif args.target == "web":
        build_web(args.type, args.run, args.browser)


if __name__ == "__main__":
    main()
