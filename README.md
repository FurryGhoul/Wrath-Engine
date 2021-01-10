# Wrath-Engine

This is a 3D Engine created by [Ivan Drofiak](https://github.com/FurryGhoul) student of the Bachelor’s Degree in Video Games by UPC at CITM, with a implmentation of the shader pipeline.

My Repository: https://github.com/FurryGhoul/Wrath-Engine
Web: https://furryghoul.github.io/Wrath-Engine/

## Fork
The engine was forked from [Tesseract-Engine](https://github.com/Adria-F/Tesseract-Engine), and engine created by [Adria Ferrer](https://github.com/Adria-F) and [Marc Garcia](https://github.com/MaxitoSama). The reason for that was due to them having a proper resource system and having worked on it last year, since I forked it as well. It must be noted that although forked, as github didn't allow to fork from a certain point the code used was from their second delivery [Delivery 2.2](https://github.com/FurryGhoul/Tesseract-Engine/releases/tag/Assignment2.2), as to not have any code related to the third assignment.

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

- The Hierarchy window displays all game objects on the scene, it supports deleting objects and reparenting them inside the hierarchy. 
- The Console window shows info about the actions done by the application: loading, errors, importing... .
- The Inspector window shows information about the selected game objects regarding their transform, mesh, material and shader. The users can also edit the transform through the window moving, rotating and scaling the objects, change the mesh, material and both edit and change the shader. 
- As for the cameras the user can decide which camera is active, which one is culling and move them.
- The scene also has a play and stop functionality, having a game scene. Some shaders that allows movement can only be fully seen when pressing the play button.

The Configuration window has the following submenus:

- Application: This section displays the info about the engine as well as some data.
- Window: This section allows the user to configure some window options.
- Renderer: This section configures render options in the scene window.
- Input Info: So far this section only shows the mouse position, but will feature more options in further versions.	

From the menu, the user can:

## File
- Save the scene.
- Load the scene.
- Create new scene.
- Close the application.

## Help
- About: A short descrption of the engine and link to the libraries used. 
- Hardware Info: Information about the technical aspects of the engine.
- GitHub: A link to the repository where the source code can be found.

## Tools
- You can desactivate different panels.
- You can open the shader edtiro window to edit the shaders in the engine.

## Debug
You cna close/open console and config window.

## Scene
There is a fully implemented mouse picking, so the users can select any game object in the scene, with a guizmo.

## Not Done
The user cant create new shaders in the editor, he can right them but they won't be saved. 

# Extra Notes
If the scene is loaded, and you can see the hierarchy with game objects but they don't appear on screen try turning camera culling on or off, inside the inspector by selecting the camera in the hierarchy. There are sometimes when the scene is loaded the culling doesn't render the game objects unless you turn it back on or off. 

# License

MIT License

Copyright (c) 2020 Ivan Drofiak

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
