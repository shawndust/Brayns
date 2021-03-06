Changelog {#Changelog}
=========

# master

* [#120](https://github.com/BlueBrain/Brayns/pull/120):
  Improved visualization of molecular systems
* [#116](https://github.com/BlueBrain/Brayns/pull/116):
  Added automated meshing of somas using metaballs algorithm
* [#103](https://github.com/BlueBrain/Brayns/pull/103:
  Added clipping planes (OSPRay engine only). --camera-type command line argument must
  be set to activate the feature
* [#99](https://github.com/BlueBrain/Brayns/pull/99):
  Add livre as a new rendering engine
* [#95](https://github.com/BlueBrain/Brayns/pull/95):
  Upgraded to OSPRay 1.1.2
* [#87](https://github.com/BlueBrain/Brayns/pull/87):
  Added morphology layout feature to Parameters event
* [#86](https://github.com/BlueBrain/Brayns/pull/86):
  Added simulation histogram event

# Release 0.2 (09-12-2016)

* [#80](https://github.com/BlueBrain/Brayns/pull/80):
  Added FullScreen and Escape keyboard shortcuts
* [#72](https://github.com/BlueBrain/Brayns/pull/72):
  Added environment map and splash scene option
* [#70](https://github.com/BlueBrain/Brayns/pull/70):
  Added support for textures in OptiX engine
* [#65](https://github.com/BlueBrain/Brayns/pull/65):
  Use adaptive motion speed for zooming depending on camera distance
* [#61](https://github.com/BlueBrain/Brayns/pull/61):
  Expose image jpeg size and quality in settings, add lexis/render/viewport
* [#59](https://github.com/BlueBrain/Brayns/pull/59):
  Added colormap event
* [#57](https://github.com/BlueBrain/Brayns/pull/57):
  Expose lexis/render/frame event in HTTP server for simulation slider
* [#54](https://github.com/BlueBrain/Brayns/pull/54):
  Added Datasource and settings initialization to ZeroEQ plugin
* [#47](https://github.com/BlueBrain/Brayns/pull/47):
  Stringification of PTX to avoid file deployment
* [#46](https://github.com/BlueBrain/Brayns/pull/46):
  Allow scenes to be reloaded dynamically through reset event
* [#39](https://github.com/BlueBrain/Brayns/pull/39):
  Added keyboard shortcuts management
* [#36](https://github.com/BlueBrain/Brayns/pull/36):
  Added support for sequences of volumes
* [#33](https://github.com/BlueBrain/Brayns/pull/33):
  Added volume rendering feature
* [#32](https://github.com/BlueBrain/Brayns/pull/32):
  Added OptiX as a rendering library. Engine can be activate using --engine optix
  command line option.
* [#31](https://github.com/BlueBrain/Brayns/pull/31):
  Add support for NEST circuits (and spikes visualization)
* [#27](https://github.com/BlueBrain/Brayns/pull/27):
  First commit for GPU screen space post processing
* [#26](https://github.com/BlueBrain/Brayns/pull/26):
  Added python lib wrapping the HTTP REST interface
* [#25](https://github.com/BlueBrain/Brayns/pull/25):
  Use memory map file to store simulation data
* [#24](https://github.com/BlueBrain/Brayns/pull/24):
  Updated Web UI example application
* [#23](https://github.com/BlueBrain/Brayns/pull/23):
  Added colormap management for simulation renderer
* [#20](https://github.com/BlueBrain/Brayns/pull/20):
  Added default Cornell Box scene when no datasource is provided. A new
  benchmarking application was also added to allow performance measurements.
* [#18](https://github.com/BlueBrain/Brayns/pull/18):
  ZeroEQ/Buf refactoring and cleanup to fix circular dependency and allow MacOSX
  support.
* [#17](https://github.com/BlueBrain/Brayns/pull/17)
  Fixed ImageMagick dependency
* [#16](https://github.com/BlueBrain/Brayns/pull/16)
  Introduced engines abstraction layer
* [#10](https://github.com/BlueBrain/Brayns/pull/15)
  Ispc correctness. Most variables in ispc code were not properly defined with
  corresponding uniform and varying keywords.
* [#9](https://github.com/BlueBrain/Brayns/pull/9)
  Improved random generator used by OSPRay renderers to avoid obvious pattern
  artefacts, especially on large images.
* [#8](https://github.com/BlueBrain/Brayns/pull/8)
  Fixes morphology loading. The data provided by the Brion API provides
  diameters, and the current implementation was considering radiuses.
* [#7](https://github.com/BlueBrain/Brayns/pull/7)
  README update
* [#6](https://github.com/BlueBrain/Brayns/pull/6)
  Added bounding box as a scene environment option
* [#5](https://github.com/BlueBrain/Brayns/pull/5)
  Fixed alpha in recursive rays on transparent surfaces.
* [#4](https://github.com/BlueBrain/Brayns/pull/4)
  Fixed Unit Tests and Alpha in ExtendedOBJRenderer
* [#1](https://github.com/BlueBrain/Brayns/pull/1)
  Fixed alpha in OBJExtendedRenderer

# Release 0.1 (06-16-2016)

* Initial commit

