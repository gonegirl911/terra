struct VertexInput {
    @builtin(vertex_index) index: u32,
    @location(0) coords: vec3<f32>,
    @location(1) normal: vec3<f32>,
}

struct CameraUniform {
    vp: mat4x4<f32>,
    origin: vec3<f32>,
}

struct VertexOutput {
    @builtin(position) clip_position: vec4<f32>,
    @location(0) light_factor: f32,
}

@group(0) @binding(0)
var<uniform> camera: CameraUniform;

@vertex
fn vs_main(vertex: VertexInput) -> VertexOutput {
    return VertexOutput(
        camera.vp * vec4(-camera.origin + vertex.coords, 1.0),
        saturate(dot(vertex.normal, vec3(0.6, 0.8, 0.0))),
    );
}

@fragment
fn fs_main(in: VertexOutput) -> @location(0) vec4<f32> {
    return vec4(vec3(in.light_factor), 1.0);
}
