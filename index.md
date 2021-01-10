# Wrath-Engine

This is a 3D Engine created by Ivan Drofiak student of the Bachelor’s Degree in Video Games by UPC at CITM, as part of an assignment to simulate and create the basic functionalities of modern game engine; and adding a more specific system to it as the last part, in this case a shader pipeline. 

### Repository;
<img src="https://i.imgur.com/Qn7mTvJ.jpg" width="600">
Repository: [Wrath Engine]https://github.com/FurryGhoul/Wrath-Engine

### Team
![Ivan](https://imgur.com/Qn7mTvJ)
Created by [Ivan Drofiak]([img]https://i.imgur.com/Qn7mTvJ.jpg[/img]) 
- Implemented all included libraries
- Made an engine work frame with ImGui
- Exporting and impoting system
- Rendering system with buffers
- Hierarchy system for game objects
- Inspector system for game objects 
- Console
- Camera culling
- Mouse picking and guizmo on scene
- Gameobject and component class
- Shader pipeline 

## Sub-systems
- UI: everything interface related is done through ImGui, allowing to have a simple and user friendly interface to navegate through the engine. Windows have different functionalities and can be moved around as well as docked. 
- Hierarchy: loaded game objects display in a window in a tree manner, allowing reparenting and deleting.
- Inspector: displays info of all components the selected game object has. These components can be edited through this inspector window and are updated inside the scene window.
- File system: allow to save and load scene, as well as other objects. It also allows to create a new scene from scratch. Everyhting using rapidJSON.
- Resource system: stores all the information of the indicated classes, maintaining a copy of it for the code to access whenever its necessary. 
- GameObject and Components: when loading a fbx or other similar files, these will have a game object created inside the engine and each of their components created, divided into their type(mesh, material, camera, shader, transform) and added to the object.
- Renderer: every mesh is rendered using OpenGL and Assimp and textures using OpenGL and DevIL; using buffers.

## Shader Pipeline
The shader pipeline uses shaders to render both meshes and materials, subtituting the classic ways of rendering using Assimp and DevIL. To do so I've implemented a shader module, a component shader, a resource shader and edited the component resource mesh. As for in-game functionalities, it is posible to modify and look at the shader variables inside the inspector and with the shader editor edit the shader file. 

# Module Shader
This module is the one in charge of having all the function needed to create both the shaders and the final shader program, which will contain both the vertex shader and the fragment shader. First the module creates all shader programs for the shaders inside the engine, this is done by passing a name for the programm and getting the shaders with that same name. Once done, we load them in memory by adding the actual vertex and fragment shaders and storing them inside the program, and once stored we compile the program and store all the uniform varaibles. When we add a shader we also compile it with a function, each of those function have case to manage if the shader is a vertex or fragment. 

# Resource Shader
This class allows us to store all the important data related both for shaders and shader programs; such as their ID, name, path, code, the actual vertex and fragment shader for the program... . It also contains a list of uniforms from the shaders program, aswell as funtions to add them. Most importantly it has the functions to load and unload those shaders and programs onto the memory.

# Component Shader
The component in itself is what we actually add to the game object and is what will help us display information inside the inspector. When loading a scene or a new game object by default we create a shader component and add a default shader to it which will only display the mesh and texture or color if they have one. Inside the component class we get the name of the shader program and from it we print all the varibles inside the inspector, it also handles the case of having variable change aswell as the shader itself. 

Last but not least the component shader is the one in charge of saving and loading the shader information inside the scene, passing the type of component and name of the shader program used of each game object and laoding it.

# Component and Resources Mesh
This component was modified to have a draw function which gets the information from the components and drawing it with shaders. The funtion indicates with shader program we are to use with glUseProgram(), bind a vertex array and textture (if it exist), and finally gets the necessary matrix and textures uniform from the shaders and uses them to place and render the object inside the scene. The rest of variable (int, float, vec...) are loaded and stored in the GPU.
As for the resource, we load the VRAM. We have a function which loads the mesh by creating a buffer and saving space for the variables and their size, for the shader we do the same but only for the vertices, normals, texture coordinates and color of the mesh. Once stored we generated a vertex array and 2 buffers for the VBO and IBO (vertex and index buffer object) and bind them, we pass the data through the buffer from before and finally we make a vertex pointer to store varaibles in their group size. 

# In-game
The user can see in-game that each game object has a component shader, by deafult this will be the default shader which will display the mesh normaly with their correct texture or color. The shader can be change in the inspector to another one, but in case the shader is dynamic (represent a certain movement) the movement wont be visible until the user enter in game mode by pressing play. The inspector also allows to change non-predefined unifrom values. 
As to modify the shader itself, in the tools tab a shader editor can be opened which will allow us to open any shader (vertex or fragment) and modify it. The editor is meant to support c++ coding language, so be careful with modifying it. Once the editing is finished the user can save the shader and a recompile shader function will be called to update the shader inside the scene, so that the user is able to see the result in real time; and in case something goes wrong the error and the line in which happened will be displayed on the console window. 

<img src="https://i.imgur.com/llqu9gx.gif" width="600"> <img src="https://i.imgur.com/o9r054T.gif" width="600">

