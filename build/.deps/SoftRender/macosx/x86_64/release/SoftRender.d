{
    values = {
        "/usr/bin/xcrun -sdk macosx clang++",
        {
            "-arch",
            "x86_64",
            "-mmacosx-version-min=12.3",
            "-isysroot",
            "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX12.1.sdk",
            "-stdlib=libc++",
            "-L/Users/sqwriwy/.xmake/packages/l/libomp/12.0.1/ab36b12aafa849ebbbb3c6837f2ea058/lib",
            "-Wl,-x",
            "-lomp",
            "-framework",
            "Cocoa",
            "-framework",
            "Foundation",
            "-framework",
            "CoreFoundation",
            "-lz"
        }
    },
    files = {
        "build/.objs/SoftRender/macosx/x86_64/release/src/main.cpp.o",
        "build/.objs/SoftRender/macosx/x86_64/release/src/common/helperfunc.cpp.o",
        "build/.objs/SoftRender/macosx/x86_64/release/src/graphics/image.cpp.o",
        "build/.objs/SoftRender/macosx/x86_64/release/src/graphics/scene.cpp.o",
        "build/.objs/SoftRender/macosx/x86_64/release/src/graphics/camera.cpp.o",
        "build/.objs/SoftRender/macosx/x86_64/release/src/graphics/framebuffer.cpp.o",
        "build/.objs/SoftRender/macosx/x86_64/release/src/graphics/globillum.cpp.o",
        "build/.objs/SoftRender/macosx/x86_64/release/src/renderer/renderer.cpp.o",
        "build/.objs/SoftRender/macosx/x86_64/release/src/platforms/macos.mm.o"
    }
}