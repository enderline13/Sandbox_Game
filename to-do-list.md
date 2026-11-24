# -----Base Engine-----
** add .pack conversion for multiple .png textures
** save options in .json
** remake json load for objects and entities in their classes and factory interface

* turn off exceptions?
* engine API for modding and tools for data parcing?
* think about mutex lock in sprites and audio loaders?

# -----Game-----
** return/add all basic UI
** player (move, physics, basic world/entity interaction, sounds)
** world (objects, destroy/place, lighting, background?)
** entities (move, physics, spawn?, drop, sounds), also rework shaders and system init()
** new lighting system (with chunks and uint_vec3?)

# -----Vulkan Implementation-----
* check shaders code for bindings
* maybe check error_code for texture objects later

# descriptor bindings (should be unique in shader (0, 1, 2, ..., but no 0 two times, etc.)):
	UI: sampler2DArray array = 0 (in 2 shader pairs),
		UB = 1 (in 3 shader pairs).
	World: sampler2dArray = 0 in 1 shader pair,
		UB = 2 in 1 shader pair