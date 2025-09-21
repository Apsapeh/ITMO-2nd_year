#[derive(serde::Serialize, Debug, Clone)]
pub struct HitResponse {
    pub x: f32,
    pub y: f32,
    pub r: u32,
    pub hit: bool,
    pub date: String,
    pub time: String,
}

impl HitResponse {
    pub fn new(x: f32, y: f32, r: u32, time: f64) -> Self {
        let current_local = chrono::Utc::now();
        let date = current_local.format("%Y-%m-%d %H:%M:%S").to_string();

        Self {
            x,
            y,
            r,
            hit: Self::is_hit(x, y, r),
            date,
            time: format!("{:.5}", time),
        }
    }

    fn is_hit(x: f32, y: f32, r: u32) -> bool {
        let r = r as f32;
        let in_rectangle = (x >= 0.0 && x <= r) && (y <= 0.0 && y >= -r);

        let in_triangle = (x >= 0.0 && x <= r / 2.0) && (y >= 0.0 && y <= r) && (y <= -x + r / 2.0);

        let in_semicircle =
            (x <= 0.0 && x >= -r) && (y >= 0.0 && y <= r) && (x * x + y * y <= r * r);

        return in_rectangle || in_triangle || in_semicircle;
    }
}
