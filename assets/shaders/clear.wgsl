@group(0) @binding(0)
var<uniform> chunk_coords: vec3<f32>;

@group(0) @binding(1)
var<storage, read_write> vertices: array<u32>;

@group(0) @binding(2)
var<storage, read_write> size: u32;

@compute @workgroup_size(256, 1, 1)
fn main(@builtin(global_invocation_id) id: vec3<u32>) {
    vertices[id.x] = 0u;
    size = 0u;
}
