struct VertexInput {
    @builtin(vertex_index) index: u32,
}

struct VertexOutput {
    @builtin(position) clip_position: vec4<f32>,
}

@vertex
fn vs_main(vertex: VertexInput) -> VertexOutput {
    if vertex.index == 0u {
        return VertexOutput(vec4(-0.5, -0.5, 0.0, 1.0));
    } else if vertex.index == 1u {
        return VertexOutput(vec4(0.5, -0.5, 0.0, 1.0));
    } else {
        return VertexOutput(vec4(0.0, 0.5, 0.0, 1.0));
    }
}

@fragment
fn fs_main(in: VertexOutput) -> @location(0) vec4<f32> {
    return vec4(0.0, 0.4, 1.0, 1.0);
}
