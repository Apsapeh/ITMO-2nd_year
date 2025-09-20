#[derive(serde::Deserialize, Debug)]
pub struct HitResponse {
    pub x: f32,
    pub y: f32,
    pub r: i8,
    pub hit: bool,
    pub date: String,
    pub time: String
}