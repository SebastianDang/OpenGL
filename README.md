# Using OpenGL in C++

Update 2018

This application is under construction to provide more efficient object oriented programming and a different approach to the system that was used before. Below are a few things that will be added from scratch.

- [x] Shader class for more general use.
- [x] Object classes to separate different types.
- [x] Resource Manager to contain all shaders, objects, lighting, camera... etc.
- [ ] Configuration Manager to contain resources in a single file.
- [ ] Environment Mapping
- [ ] Scene Graph
- [ ] Terrain
- [ ] Water
- [ ] Lines, Bezier Curves
- [ ] Particles
- [ ] Bounding Boxes
- [ ] View Frustum Culling
- [ ] OpenGl Selecting using raycasting or other concepts.
- [ ] Bloom
- [ ] Filters
- [ ] Clipping Planes for water, terrain, optimization.

*This application replicates water using Frame Buffer Objects for both reflection and refraction. It also integrates clipping planes to avoid rendering objects under the plane to the reflection FBO. DU/DV maps are added to create a wavy effect. There's also environment mapping using the skybox. Currently, I'm working on making this more realistic to mimic the basic properties of water.*
