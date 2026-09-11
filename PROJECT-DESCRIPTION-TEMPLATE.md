# Night Patrol

123/2022 - Mihailo Marković

This project showcases a night time scene featuring a military tank, a hangar,
a cobblestone floor, and streetlamps. It implements directional, spot, and point
lighting with parallax mapping on the cobblestone floor for added depth. The scene
includes interactive elements - spotlight toggle, manual tank movement, and an
automated tank event triggered by pressing T.

## Controls

W, A, S, D -> camera movement (forward, left, backward, right)
MOUSE -> camera rotation
I -> tank forward
K -> tank backward
J -> tank rotate left
L -> tank rotate right
O -> spotlight on/off
T -> start event (tank moves forward, then rotates 360 degrees)
ESC -> exit application

## Features

### Fundamental:

[X] Model with lighting
[X] Two types of lighting with customizable colors and movement through GUI or ACTIONS
[X] {T key pressed} --- AFTER_3_SECONDS---Triggers---> {Tank moves forward} ---> AFTER_2_SECONDS---Triggers---> {Tank
rotates 360 degrees}

### Group A:

[ ] Frame-buffers with post-processing   
[ ] Off-screen Anti-Aliasing  
[X] Parallax Mapping
[ ] Bloom with the use of HDR

### Group B:

[ ] Deferred Shading  
[X] Point Shadows  
[ ] SSAO

### Engine improvement:

[x] engine::graphics::GraphicsController::draw_parallax_map — parallax mapping rendering for models with height/depth
maps
[x] engine::graphics::PointShadow — cubemap shadow map for point light sources, using a geometry shader to render all 6
faces in a single pass

## Models:

[Tank - https://sketchfab.com/3d-models/ferdinand-world-of-tanks-blitz-203da88e351849509491197d8488a1bc ]
[Streetlamp - https://sketchfab.com/3d-models/street-lamp-98041e31c4ad4cecbd70eb7c1fd6ffb5 ]
[Streetlamp2 - https://sketchfab.com/3d-models/street-light-04a47d898e704e1a809d24433c409bf5 ]
[Hangar - https://sketchfab.com/3d-models/sci-fi-hangar-0625335964a5496980ccfbc3b76c49ff ]

## Textures

[Cobblestone floor texture - https://sketchfab.com/3d-models/cobblestone-pbr-texture-d203f2e9f5bd472c91a87964b40c866c ]
