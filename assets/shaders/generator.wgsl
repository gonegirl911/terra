struct TerrainVertex {
    coords: array<f32, 3>,
    normal: array<f32, 3>,
}

@group(0) @binding(0)
var<storage, read_write> vertices: array<TerrainVertex, 6>;

@compute @workgroup_size(1)
fn main(@builtin(global_invocation_id) id: vec3<u32>) {
    vertices = array(
        TerrainVertex(array(-0.5, 0.0, -0.5), array(0.0, 1.0, 0.0)),
        TerrainVertex(array(0.5, 0.0, -0.5), array(0.0, 1.0, 0.0)),
        TerrainVertex(array(-0.5, 0.0, 0.5), array(0.0, 1.0, 0.0)),
        TerrainVertex(array(0.5, 0.0, -0.5), array(0.0, 1.0, 0.0)),
        TerrainVertex(array(0.5, 0.0, 0.5), array(0.0, 1.0, 0.0)),
        TerrainVertex(array(-0.5, 0.0, 0.5), array(0.0, 1.0, 0.0)),
    );
}
