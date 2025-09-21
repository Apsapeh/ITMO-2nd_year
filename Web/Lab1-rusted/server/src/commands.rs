use std::collections::HashMap;

use crate::{
    response_factory::ResponseFactory,
    types::{self, HitResponse},
    HIT_HISTORY,
};

macro_rules! bad_request {
    ($text: expr) => {
        ResponseFactory::bad_response_text($text)
    };
}

macro_rules! internal_error {
    ($text: expr) => {
        ResponseFactory::internal_error_text($text)
    };
}

pub fn add_command(query: &str, start_time: std::time::Instant) -> String {
    if query.is_empty() {
        return bad_request!("GET request has no query");
    }

    let query_map: HashMap<&str, &str> = query
        .split('&')
        .filter_map(|param| {
            let mut pair = param.split('=');
            let key = pair.next()?;
            let value = pair.next().unwrap_or("on");
            Some((key, value))
        })
        .collect();

    const VALUE_X_PAIR: [(&str, f32); 9] = [
        ("ValueX_n_2", -2.0),
        ("ValueX_n_1_5", -1.5),
        ("ValueX_n_1", -1.0),
        ("ValueX_n_0_5", -0.5),
        ("ValueX_0", 0.0),
        ("ValueX_0_5", 0.5),
        ("ValueX_1", 1.0),
        ("ValueX_1_5", 1.5),
        ("ValueX_2", 2.0),
    ];

    let x_vec = VALUE_X_PAIR
        .into_iter()
        .filter_map(|v| {
            let param = query_map.get(v.0)?;
            match param == &"on" {
                true => Some(v.1),
                false => None,
            }
        })
        .collect::<Vec<f32>>();

    if x_vec.is_empty() {
        return bad_request!("No value for X value");
    }

    // Y value
    let Some(value_y_str) = query_map.get("ValueY") else {
        return bad_request!("No value for Y value");
    };

    let value_y = match value_y_str.parse::<f32>() {
        Ok(y) => y,
        Err(e) => return bad_request!(&format!("Y value parsing failed with '{}'", e.to_string())),
    };
    if value_y < -3.0 || value_y > 3.0 {
        return bad_request!("Invalid value for Y value. It should be in range [-3, 3]");
    }

    // R vlaue
    let Some(value_r_str) = query_map.get("ValueR") else {
        return bad_request!("No value for R value");
    };
    let value_r = match value_r_str.parse::<u32>() {
        Ok(y) => y,
        Err(e) => return bad_request!(&format!("R value parsing failed with '{}'", e.to_string())),
    };
    if value_r < 1 || value_r > 5 {
        return bad_request!("Invalid value for R value. It should be in range [1, 5]");
    }

    let time = start_time.elapsed().as_nanos() as f64 / 1000000.0;

    let hit_responses = x_vec
        .into_iter()
        .map(|x| types::HitResponse::new(x, value_y, value_r, time))
        .collect::<Vec<_>>();

    let Ok(mut history_lock) = HIT_HISTORY.lock() else {
        return internal_error!("");
    };
    history_lock.extend_from_slice(&hit_responses);

    let Ok(json) = serde_json::ser::to_string(&hit_responses) else {
        return internal_error!("");
    };

    ResponseFactory::ok_json(&json)
}

pub fn get_all_command() -> String {
    let history = HIT_HISTORY.lock().unwrap();
    let r: &Vec<HitResponse> = history.as_ref();

    let json = serde_json::ser::to_string(r).unwrap();
    ResponseFactory::ok_json(&json)
}
