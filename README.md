# Learn the basics of Vulkan
This repository is for the GAME 308 - GPU Programming course for Humber College's Game Programming diploma.<br />
We were provided a sample code, that we had to use to add our assignments into it:
+ Add multiple light sources
+ Add multiple models to the scene
+ Add different textures to the models
+ Use push constants to transfer data to the shaders
+ Use Uniform Buffers to transfer bulk data to the shaders

The sample code was based on the tutorial from this page: https://vulkan-tutorial.com/Introduction<br />

### Additional tasks on top of the assignments
I separated the physical/logical device handling from the rest of the code, so until the point where we request a device from Vulkan is done in a separate class - DeviceHandler.<br />
I chose to create only one command buffer at the beginning of the game start and not recreate it every frame. On top of that I used the secondary command buffer approach, where
I created a separate secondary command buffer for every element that I had to render to the screen. Then I used the collection of these secondary command buffers in the Main render
loop to create one command buffer to draw the elements.<br />
I created separate classes for the Models and a base class for every VulkanRenderableObject that I need to render. For this I added the necessary Vulkan calls to these classes so
it can be used on demand during scene creation and render.<br />
I created point lights that are only using shader code to display them on the screen. I used this tutorial for inspiration:
https://www.youtube.com/watch?v=Z1lLwAEMt4M&ab_channel=BrendanGalea<br />

## How to run it
This solution is only working on MacOS with VulkanSDK installed.
