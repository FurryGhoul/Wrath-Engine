# Wrath-Engine

This is a 3D Engine created by [Ivan Drofiak](https://github.com/FurryGhoul) student of the Bachelor’s Degree in Video Games by UPC at CITM.

Our Repository: https://github.com/FurryGhoul/Wrath-Engine

## Libraries Used
- SDL.
- STL.
- ImGui.
- OpenGL.
- MathGeoLib.
- DevIL.
- Assimp.
- Glew.
- Parson.

## Guide of use

In the scene window, the movement controls is as follows:

- Use 'Right Click +WASD' to move arround the scene.
- Use 'Q' to move upwards and 'E' to move downwards
- Use 'Alt' & Left Click to orbit arround the object.
- Use 'Mouse Wheel' to zoom In and Out.
- Use Mouse Middle Button to move the camera.
- Use 'F' to focus the camera on the object.
- Use 'Shift' to duplicate movement speed.

The Hierarchy window is still in progress, so far it supports creating a camera as well as a display of an fbx hierarchy with each childs in a tree manner. The user can also delete game objects as well as move them around chaning their parent. 
The Console window shows info about the actions done by the application.
The Inspector window shows information about the selected game objects regarding their transform, mesh and material. The users can also edit the transform through the window moving, rotating and scaling the objects. 
As for the cameras the user can decide which camera is active, which one is culling and move them.
The Configuration window has the following submenus:

- Application: This section displays the info about the engine as well as some data.
- Window: This section allows the user to configure some window options.
- Renderer: This section configures render options in the scene window.
- Input Info: So far this section only shows the mouse position, but will feature more options in further versions.	

From the menu, the user can:

# File
- And Save the scene, but with no load.
- Close the application.

# Help
- About: A short descrption of the engine and link to the libraries used. 
- Hardware Info: Information about the technical aspects of the engine.
- GitHub: A link to the repository where the source code can be found.
- Controls: A window that displays the controls for the scene window.

# Windows

This section allows the user to activate/desactivate the windows displayed on screen. 
The windows can be moved freely arround and organized as the user rathers using docking.

# Scene

There is a fully implemented mouse picking, so the users can select any game object in the scene, but their is no guizmo implemented. 

# Not Done

There are no resources implemented, the load functions aren't working henceworth they are disabled, there is no play, pause or stop button and you can't change the mesh or material of any game objects.

## License

MIT License

Copyright (c) 2020 Ivan Drofiak

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.