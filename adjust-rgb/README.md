# adjust-rgb
A basic plugin to adjust the RGB values of a video. Use this as a starting example of how an OpenFX plugin is put together.

This plugin take a frame at time `t` and outputs the same frame with adjusted colours at the same time `t`.

3 parameters {R, G, B} are defined to adjust each colour channel.


# Overview/ Structure of plugin
[plugin.cpp](src/plugin.cpp) contains the desciption of the plugin, linking to functinality and host communication.

[imHost.cpp](src/imHost.cpp) creates a class to handle reading images from host memory.

[imHandler.cpp](src/imHandler.cpp) creates a class to convert images into a usable format for processing.

[imProcessing.hpp](include-local/imProcessing.hpp) defines the effect used on each image.


# Compiling
Run `build-windows.bat` to automatically compile and package the plugin. The plugin will be moved to `C:\Program Files\Common Files\OFX\Plugins` as this is the default location for hosts to search for plugins.


# Customising the plugin
1. Change `rgb` in [CMakeLists.txt](CMakeLists.txt) and [build-windows.bat](build-windows.bat) to the name of your package folder.

2. Change the plugin name, grouping and ID in [plugin.cpp](src/plugin.cpp).

3. Adjust the parameters if nessesary in [plugin.cpp](src/plugin.cpp):
    * Parameter name definitions

    * Definitions in `DescribeInContextAction`

    * Handles in `CreateInstanceAction` - change OfxParamHandle in [fetchData.hpp](include-local/fetchData.hpp) accordingly

    * Fetching Parameter values in `RenderAction`

    * Paramter checking in `IsIdentityAction`


4. Adjust the effects on the image in [PixelProcessing.hpp](src/PixelProcessing.hpp)
    * number/ type of params

    * pixel processing effect
