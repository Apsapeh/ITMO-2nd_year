use log::{error, info};
use std::{
    io::Write,
    sync::{LazyLock, Mutex},
};

use crate::response_factory::ResponseFactory;

mod commands;
mod response_factory;
mod types;

pub static HIT_HISTORY: LazyLock<Mutex<Vec<types::HitResponse>>> =
    LazyLock::new(|| Mutex::new(vec![]));

fn main() {
    env_logger::Builder::from_env(env_logger::Env::default().default_filter_or("info")).init();

    let addr = std::env::var("ADDR").unwrap_or(String::from("127.0.0.1"));
    let port = std::env::var("PORT").unwrap_or(String::from("33333"));
    let full_addr = format!("{}:{}", addr, port);
    
    info!("Server started at {}", full_addr);

    let listener = std::net::TcpListener::bind(full_addr).unwrap();
    fastcgi::run_tcp(request_handler, &listener);
}

fn send_response(req: &mut fastcgi::Request, response: &str) {
    if let Err(e) = write!(&mut req.stdout(), "{response}") {
        error!("{}", e)
    }
}

fn request_handler(mut req: fastcgi::Request) {
    let time_start = std::time::Instant::now();
    let Some(uri) = req.param("REQUEST_URI") else {
        send_response(&mut req, &ResponseFactory::internal_error_text(""));
        error!("REQUEST_URI not found");
        return;
    };

    let Some(method) = req.param("REQUEST_METHOD") else {
        send_response(&mut req, &ResponseFactory::internal_error_text(""));
        error!("REQUEST_METHOD not found");
        return;
    };
    let method = method.to_uppercase();

    let Some(query) = req.param("QUERY_STRING") else {
        send_response(&mut req, &ResponseFactory::internal_error_text(""));
        error!("QUERI_STRING not found");
        return;
    };
    
    info!("New request: '{}' via {}",  uri, method);

    let response;
    if uri.starts_with("/fcgi-bin/server.jar/add") && method == "GET" {
        response = commands::add_command(&query, time_start);
    } else if uri.starts_with("/fcgi-bin/server.jar/getAll") && method == "GET" {
        response = commands::get_all_command();
    } else {
        send_response(
            &mut req,
            &ResponseFactory::internal_error_text(&format!(
                "Unknown command {} via {}",
                uri, method
            )),
        );
        return;
    }

    send_response(&mut req, &response);
}
