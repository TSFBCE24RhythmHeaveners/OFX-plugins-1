# Examples on how to make OpenFX plugins

[adjust-rgb](./adjust-rgb) is the most basic plugin so start here

# Resources
There is little in the way of help for OpenFX on the internet but here are a few sites to use:

- [The official documentation](https://openfx.readthedocs.io/en/master/index.html) provides some tutorials for basic functionality and is the best starting point.

- Resolve's example plugins located at `C:\ProgramData\Blackmagic Design\DaVinci Resolve\Support\Developer\OpenFX`

# Set up
- C++ compiler is required - Tested with MSVC 19.28.29336.0

- Cmake is required to build project - Tested with version 3.19.4.

All openfx header files are included in this repo - Version 1.4.

# Testing Plugins
Natron 2.3.15 is used for testing plugins during development as it allows std::cout messages to be displayed.

Using the build script `build-windows.bat` the plugin will be automatically compiled, packaged and moved into the correct location. (Change permissions to 'full control' for the folder `C:\Program Files\Common Files\OFX\Plugins`, otherwise copy over manually)

Make sure that libtorch is located on PATH so that Natron/Resolve can load it.

# Issues
Resolve cannot load GPU library dependencies when running libtorch with CUDA
