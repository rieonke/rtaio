import os

lib_path = "./third_party/"


class lib:
    name = None
    url = None
    archive = None

    def __init__(self, name, url, archive):
        self.name = name
        self.url = url
        self.archive = archive


libs = [
    lib("xlnt", "https://github.com/tfussell/xlnt/archive/v1.3.0.tar.gz", "xlnt.tar.gz"),
    lib("glfw", "https://github.com/glfw/glfw/archive/3.3.tar.gz", "glfw.tar.gz"),
    lib("imgui", "https://github.com/ocornut/imgui/archive/v1.71.tar.gz ", "imgui.tar.gz"),
    lib("pfd", "https://github.com/samhocevar/portable-file-dialogs/archive/master.tar.gz", "pfd.tar.gz"),
    lib("imgui_code_editor", "https://github.com/BalazsJako/ImGuiColorTextEdit/archive/master.tar.gz", "imgui_code_editor.tar.gz"),
]

copy_files = [
    (lib_path + "imgui/examples/imgui_impl_glfw.h", lib_path + "imgui/include/"),
    (lib_path + "imgui/examples/imgui_impl_opengl2.h", lib_path + "imgui/include/"),
    (lib_path + "imgui/imgui.h", lib_path + "imgui/include"),
]

for lib in libs:
    path = lib_path + lib.name
    archive = lib_path + lib.archive

    if not os.path.isfile(archive):
        print("download from " + lib.url + " to " + archive)
        command = "curl -L " + lib.url + " --output " + archive
        os.system(command)

    if not os.path.isdir(path):
        command = "mkdir -p " + path
        os.system(command)
        if archive.endswith(".tar.gz"):
            command = "tar zxf " + archive + " -C " + path + " --strip-components=1"
            os.system(command)

for f in copy_files:
    if (f[1].endswith("/")):
        os.system("mkdir -p " + f[1])
    os.system("cp " + f[0] + " " + f[1])

os.system("sed -i '' '259,261 s/^/#/' ./third_party/xlnt/source/CMakeLists.txt")

os.system("mkdir build")
os.system("cd build && cmake .. -DCMAKE_BUILD_TYPE=Release  -DSTATIC=ON -DMAC_BUNDLE=ON .")
os.system("cd ./build && make rtaio -j 4")
