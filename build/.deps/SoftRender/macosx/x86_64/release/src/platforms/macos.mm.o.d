{
    files = {
        "src/platforms/macos.mm"
    },
    values = {
        "/usr/bin/xcrun -sdk macosx clang",
        {
            "-pipe",
            "-DIBOutlet=__attribute__((iboutlet))",
            "-DIBOutletCollection(ClassName)=__attribute__((iboutletcollection(ClassName)))",
            "-DIBAction=void)__attribute__((ibaction)",
            "-Qunused-arguments",
            "-mmacosx-version-min=12.3",
            "-arch",
            "x86_64",
            "-isysroot",
            "/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX12.1.sdk",
            "-fobjc-arc",
            "-fvisibility=hidden",
            "-O3",
            "-std=c++17",
            "-Isrc",
            "-framework",
            "Cocoa",
            "-framework",
            "Foundation",
            "-framework",
            "CoreFoundation",
            "-isystem",
            "/Users/sqwriwy/.xmake/packages/e/eigen/3.4.0/28e6c56be0f547ca9b6de500a3a2ba8a/include",
            "-isystem",
            "/Users/sqwriwy/.xmake/packages/e/eigen/3.4.0/28e6c56be0f547ca9b6de500a3a2ba8a/include/eigen3",
            "-isystem",
            "/Users/sqwriwy/.xmake/packages/s/spdlog/v1.9.2/958ecc747c6847c0a857bcc0daf64664/include",
            "-isystem",
            "/Users/sqwriwy/.xmake/packages/s/stb/2021.09.10/88c8b7d5f192477b8e00b7c2917ece1f/include",
            "-isystem",
            "/Users/sqwriwy/.xmake/packages/l/libomp/12.0.1/ab36b12aafa849ebbbb3c6837f2ea058/include",
            "-fno-objc-arc"
        }
    },
    depfiles_gcc = "build/.objs/SoftRender/macosx/x86_64/release/src/platforms/macos.mm.o:   src/platforms/macos.mm src/graphics/platform.h src/graphics/image.h   src/graphics/framebuffer.h src/common/mathtype.h   src/renderer/renderer.h src/graphics/camera.h src/graphics/scene.h   src/graphics/object.h src/graphics/material.h src/graphics/light.h\
"
}