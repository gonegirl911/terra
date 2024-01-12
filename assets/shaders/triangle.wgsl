struct VertexInput {
    @builtin(vertex_index) index: u32,
    @location(0) coords: vec2<f32>,
}

struct VertexOutput {
    @builtin(position) clip_position: vec4<f32>,
}

@vertex
fn vs_main(vertex: VertexInput) -> VertexOutput {
    return VertexOutput(vec4(vertex.coords, 0.0, 1.0));
}

@fragment
fn fs_main(in: VertexOutput) -> @location(0) vec4<f32> {
    return vec4(0.0, 0.4, 1.0, 1.0);
}