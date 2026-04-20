import os

PROJECT_NAME = "agent-simulation"
SRC_DIR = "src"
CMAKE_VERSION = "3.28"
CPP_VERSION = "23"

LINK_LIBS = [
    "fmt", "spdlog"
 ]

INCLUDE_DIRS = []

def collect_sources():
    sources = []
    for root, _, files in os.walk(SRC_DIR):
        for file in files:
            if file.endswith(".cpp") or file.endswith(".c"):
                path = os.path.join(root, file).replace("\\", "/")
                sources.append(path)
    return sources

def write_cmake(sources):
    with open ("CMakeLists.txt", "w") as file:
        file.write(f"cmake_minimum_required(VERSION {CMAKE_VERSION})\n")
        file.write(f"project({PROJECT_NAME})\n")
        file.write(f"set(CMAKE_CXX_STANDARD {CPP_VERSION})\n\n")

        #directories
        for inc in INCLUDE_DIRS:
            file.write(f'include_directories("{inc}")\n')

        file.write("\nadd_executable(${PROJECT_NAME}\n")
        for src in sources:
            file.write(f"   {src}\n")
        file.write(")\n\n")

        # link libs
        if LINK_LIBS:
            libs_str = " ".join(LINK_LIBS)
            file.write(f"target_link_libraries(${{PROJECT_NAME}} {libs_str})\n")

if __name__ == "__main__":
    srcs = collect_sources()
    write_cmake(srcs)
    print(f"Generated CMakeLists.txt with {len(srcs)} source file(s) and libraries: {', '.join(LINK_LIBS)}")
            