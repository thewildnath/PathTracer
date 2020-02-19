![Cornell box](Renders/Cornell.bmp?raw=true "Cornell box")

"A Physically Based Lighting Model for Interactive Direct Volume Rendering" was the title of my dissertation for BSc of Computer Science at the University of Bristol. This repo contains the source code for my project plus some interesting renders that provide a good example of what the program is capable of.

# Path Tracer

This project is based on [Exposure Render](https://github.com/ThomasKroes/exposure-render) by Thomas Kroes and it extends it by implementing a full Monte Carlo path tracer similar to [pbrt](https://github.com/mmp/pbrt-v3) (but much simpler). It is a direct volume renderer, meaning that it supports interactive updates to the transform function, which can change how the volume is rendered in real-time. The pictures below were rendered in the same session without requiring any restarts or recalculations of the data.

![Bone](Renders/TF1.bmp?raw=true "Bone")
![Transparent](Renders/TF7.bmp?raw=true "Transparent")
![Skin](Renders/TF4.bmp?raw=true "Skin")

Although the algorithm is optimised, the code is not due to time constraints. Written in C++ 17 and parallelised on the CPU using OpenMP. Depending on the rendering parameters, it runs between 2-10 FPS in 400x400 on an Intel Core i7-6700HQ.
