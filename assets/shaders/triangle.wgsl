struct VertexInput {
    @builtin(vertex_index) index: u32,
    @location(0) coords: vec3<f32>,
}

struct CameraUniform {
    vp: mat4x4<f32>,
}

struct VertexOutput {
    @builtin(position) clip_position: vec4<f32>,
}

@group(0) @binding(0)
var<uniform> camera: CameraUniform;

@vertex
fn vs_main(vertex: VertexInput) -> VertexOutput {
    return VertexOutput(camera.vp * vec4(vertex.coords, 1.0));
}

@fragment
fn fs_main(in: VertexOutput) -> @location(0) vec4<f32> {
    return vec4(0.3, 0.6, 0.0, 1.0);
}
