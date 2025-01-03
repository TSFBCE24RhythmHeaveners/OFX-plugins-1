SET PLUGIN_NAME="rgb"
SET CPP_SOURCE="Release"

mkdir build
cd build

cmake -G"Visual Studio 16 2019" -Ax64 -Thost=x64 ..
cmake --build . --config Release

cd ..
md %PLUGIN_NAME%.ofx.bundle\Contents
move /y build\%CPP_SOURCE% %PLUGIN_NAME%.ofx.bundle\Contents\Win64
rmdir /s /Q "C:\Program Files\Common Files\OFX\Plugins\AdjustRGB.ofx.bundle"
move /y AdjustRGB.ofx.bundle "C:\Program Files\Common Files\OFX\Plugins"
rmdir /s /Q build

::"C:\Program Files\Natron\bin\Natron.exe" dev-misc\ims-windows.ntp
"C:\Program Files\INRIA\Natron-2.3.15\bin\Natron.exe" dev-misc\natron-test-windows.ntp
