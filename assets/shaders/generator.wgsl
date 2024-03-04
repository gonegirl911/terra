struct TerrainVertex {
    coords: array<f32, 3>,
    normal: array<f32, 3>,
    ao: f32,
}

@group(0) @binding(0)
var<storage, read> corner_deltas: array<array<f32, 3>, 8>;

@group(0) @binding(1)
var<storage, read> triangulations: array<array<i32, 16>, 256>;

@group(0) @binding(2)
var<storage, read> edge_corners: array<array<u32, 2>, 12>;

@group(0) @binding(3)
var<storage, read> ray_dirs: array<array<f32, 3>>;

@group(1) @binding(0)
var<storage, read_write> vertices: array<TerrainVertex>;

@group(1) @binding(1)
var<storage, read_write> size: atomic<u32>;

@compute @workgroup_size(4, 4, 4)
fn main(@builtin(global_invocation_id) id: vec3<u32>) {
    let coords = vec3<f32>(id);
    let edges = &triangulations[config(coords)];
    let count = u32((*edges)[15]);
    let index = atomicAdd(&size, count);

    for (var i = 0u; i < count; i++) {
        let coords = coords(coords, edge_corners[((*edges)[i])]);
        let normal = normal(coords);
        let ao = ao(coords);

        vertices[index + i] = TerrainVertex(to_array(coords), to_array(normal), ao);
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

fn ao(coords: vec3<f32>) -> f32 {
    var accum = 0.0;

    for (var ray = 0u; ray < arrayLength(&ray_dirs); ray++) {
        let dir = to_vec(ray_dirs[ray]);
        var visibility = 1.0;

        for (var step = 1; step <= 16; step++) {
            let density = density(coords + dir * f32(step) / 4.0);
            visibility *= saturate(density * 100.0);
        }

        for (var step = 1; step <= 4; step++) {
            let density = density(coords + dir * f32(step) * 4.0);
            visibility *= saturate(density * 10.0);
        }

        accum += visibility;
    }

    return accum / f32(arrayLength(&ray_dirs));
}

fn to_array(values: vec3<f32>) -> array<f32, 3> {
    return array<f32, 3>(values.x, values.y, values.z);
}

fn delta(corner: u32) -> vec3<f32> {
    return to_vec(corner_deltas[corner]);
}

fn density(coords: vec3<f32>) -> f32 {
    return simplex3(coords / 32.0);
}

fn inv_lerp(coords: vec3<f32>, corners: array<u32, 2>) -> vec3<f32> {
    let a = density(coords + delta(corners[0]));
    let b = density(coords + delta(corners[1]));
    return vec3(-a / (b - a));
}

fn to_vec(values: array<f32, 3>) -> vec3<f32> {
    return vec3(values[0], values[1], values[2]);
}

fn simplex3(coords: vec3<f32>) -> f32 {
    let s = floor(coords + dot(coords, vec3(FRAC_1_3)));
    let x = coords - s + dot(s, vec3(FRAC_1_6));

    let e = step(vec3(0.0), x - x.yzx);
    let i0 = e * (1.0 - e.zxy);
    let i1 = 1.0 - e.zxy * (1.0 - e);

    let x0 = x - i0 + FRAC_1_6;
    let x1 = x - i1 + FRAC_1_3;
    let x2 = x - 0.5;

    let weights = max(0.6 - vec4(dot(x, x), dot(x0, x0), dot(x1, x1), dot(x2, x2)), vec4(0.0));
    let surflets = vec4(
        dot(random3(s), x),
        dot(random3(s + i0), x0),
        dot(random3(s + i1), x1),
        dot(random3(s + 1.0), x2),
    );

    return dot(surflets * pow2(pow2(weights)), vec4(52.0));
}

fn random3(coords: vec3<f32>) -> vec3<f32> {
    let base = vec3(1u << 18u, 1u << 15u, 1u << 21u);
    let r = sin(dot(coords, vec3(17.0, 59.4, 15.0)));
    return fract(vec3<f32>(base) * r) - 0.5;
}

fn pow2(values: vec4<f32>) -> vec4<f32> {
    return values * values;
}

const FRAC_1_3 = 1.0 / 3.0;
const FRAC_1_6 = 1.0 / 6.0;
