struct TerrainVertex {
    coords: array<f32, 3>,
    normal: array<f32, 3>,
}

@group(0) @binding(0)
var<storage, read> corner_deltas: array<array<i32, 3>, 8>;

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
    let coords = vec3<i32>(id) - 16;
    let edges = &triangulations[config(coords)];
    let count = u32((*edges)[15]);
    let index = atomicAdd(&size, count);

    for (var i = 0u; i < count; i += 3u) {
        let p0 = point(coords, edge_corners[((*edges)[i])]);
        let p1 = point(coords, edge_corners[((*edges)[i + 1u])]);
        let p2 = point(coords, edge_corners[((*edges)[i + 2u])]);
        let normal = to_array(normalize(cross(p2 - p0, p1 - p0)));

        vertices[index + i] = TerrainVertex(to_array(p0), normal);
        vertices[index + i + 1u] = TerrainVertex(to_array(p1), normal);
        vertices[index + i + 2u] = TerrainVertex(to_array(p2), normal);
    }
}

fn config(coords: vec3<i32>) -> u32 {
    var config = 0u;
    for (var i = 0u; i < 8u; i++) {
        config |= u32(density(coords + delta(i)) < 0) << i;
    }
    return config;
}

fn point(coords: vec3<i32>, corners: array<u32, 2>) -> vec3<f32> {
    return f(coords) + mix(f(delta(corners[0])), f(delta(corners[1])), inv_lerp(coords, corners));
}

fn to_array(values: vec3<f32>) -> array<f32, 3> {
    return array<f32, 3>(values.x, values.y, values.z);
}

fn delta(corner: u32) -> vec3<i32> {
    return to_vec(corner_deltas[corner]);
}

fn density(coords: vec3<i32>) -> i32 {
    return dot(coords, coords) - 16 * 16;
}

fn f(coords: vec3<i32>) -> vec3<f32> {
    return vec3<f32>(coords);
}

fn inv_lerp(coords: vec3<i32>, corners: array<u32, 2>) -> vec3<f32> {
    let a = f32(density(coords + delta(corners[0])));
    let b = f32(density(coords + delta(corners[1])));
    return vec3(-a / (b - a));
}

fn to_vec(values: array<i32, 3>) -> vec3<i32> {
    return vec3(values[0], values[1], values[2]);
}
