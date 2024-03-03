struct TerrainVertex {
    coords: array<f32, 3>,
    normal: array<f32, 3>,
}

@group(0) @binding(0)
var<storage, read> corner_deltas: array<array<f32, 3>, 8>;

@group(0) @binding(1)
var<storage, read> triangulations: array<array<i32, 16>, 256>;

@group(0) @binding(2)
var<storage, read> edge_corners: array<array<u32, 2>, 12>;

@group(1) @binding(0)
var<storage, read_write> vertices: array<TerrainVertex>;

@group(1) @binding(1)
var<storage, read_write> size: atomic<u32>;

@compute @workgroup_size(4, 4, 4)
fn main(@builtin(global_invocation_id) id: vec3<u32>) {
    let coords = vec3<f32>(id) - 16.0;
    let edges = &triangulations[config(coords)];
    let count = u32((*edges)[15]);
    let index = atomicAdd(&size, count);

    for (var i = 0u; i < count; i++) {
        let coords = coords(coords, edge_corners[((*edges)[i])]);
        let normal = normal(coords);

        vertices[index + i] = TerrainVertex(to_array(coords), to_array(normal));
    }
}

fn config(coords: vec3<f32>) -> u32 {
    var config = 0u;
    for (var i = 0u; i < 8u; i++) {
        config |= u32(density(coords + delta(i)) < 0.0) << i;
    }
    return config;
}

fn coords(coords: vec3<f32>, corners: array<u32, 2>) -> vec3<f32> {
    return coords + mix(delta(corners[0]), delta(corners[1]), inv_lerp(coords, corners));
}

fn normal(coords: vec3<f32>) -> vec3<f32> {
    return normalize(vec3(
        density(coords + vec3(1.0, 0.0, 0.0)) - density(coords - vec3(1.0, 0.0, 0.0)),
        density(coords + vec3(0.0, 1.0, 0.0)) - density(coords - vec3(0.0, 1.0, 0.0)),
        density(coords + vec3(0.0, 0.0, 1.0)) - density(coords - vec3(0.0, 0.0, 1.0)),
    ));
}

fn to_array(values: vec3<f32>) -> array<f32, 3> {
    return array<f32, 3>(values.x, values.y, values.z);
}

fn delta(corner: u32) -> vec3<f32> {
    return to_vec(corner_deltas[corner]);
}

fn density(coords: vec3<f32>) -> f32 {
    return dot(coords, coords) - 16.0 * 16.0;
}

fn inv_lerp(coords: vec3<f32>, corners: array<u32, 2>) -> vec3<f32> {
    let a = density(coords + delta(corners[0]));
    let b = density(coords + delta(corners[1]));
    return vec3(-a / (b - a));
}

fn to_vec(values: array<f32, 3>) -> vec3<f32> {
    return vec3(values[0], values[1], values[2]);
}
