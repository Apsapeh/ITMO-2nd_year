use gloo::{
    console::error,
    dialogs::alert,
    events::{EventListener, EventListenerOptions},
    utils::document,
};
use wasm_bindgen::prelude::*;
use wasm_bindgen_futures::spawn_local;
use web_sys::{
    FormData, HtmlFormElement, HtmlInputElement, HtmlTableRowElement, HtmlTableSectionElement,
    UrlSearchParams,
};

mod types;

#[wasm_bindgen(start)]
pub async fn main() {
    console_error_panic_hook::set_once();

    request_and_update_table(String::from("/fcgi-bin/server.jar/getAll")).await;

    set_main_form_submit_listener();
    set_value_y_rt_input_validator();
}

// =====> FastCGI Communication <=====

async fn send_request(url: &str) -> Option<Vec<types::HitResponse>> {
    let request_builder = gloo::net::http::Request::get(&url);

    match request_builder.send().await {
        Ok(response) => {
            if !response.ok() {
                let status = response.status();
                let message = response.text().await.unwrap_or(String::from(""));
                alert(&format!("Error [{}]: {}", status, message));
                return None;
            };

            match response.json::<Vec<types::HitResponse>>().await {
                Ok(j) => Some(j),
                Err(e) => {
                    error!(e.to_string());
                    None
                }
            }
        }
        Err(e) => {
            alert(&format!("Error: {}", e.to_string()));
            None
        }
    }
}

async fn request_and_update_table(url: String) {
    let Some(data) = send_request(&url).await else {
        alert("Response JSON parsing error (more in console)");
        return;
    };

    let tbody = document().get_element_by_id("resultsBody").unwrap();
    let tbody = tbody.dyn_ref::<HtmlTableSectionElement>().unwrap();

    for d in data {
        let row = document().create_element("tr").unwrap();
        let row = row.dyn_ref::<HtmlTableRowElement>().unwrap();

        #[rustfmt::skip]
        row.set_inner_html(&format!(
            "
            <td>{:.2}</td>
            <td>{:.2}</td>
            <td>{}</td>
            <td class=\"{}\">{}</td>
            <td>{}</td>
            <td>{} мс</td>
        ",
            d.x,
            d.y,
            d.r,
            if d.hit { "hit" } else { "miss" },
            if d.hit {"Попадание"} else {"Промах"},
            d.date,
            d.time
        ));

        tbody.append_child(row).unwrap();
    }
}

// =====> Listeners <=====

fn validate_form() -> bool {
    let y_input = document().get_element_by_id("ValueY").unwrap();
    let y_input = y_input.dyn_ref::<HtmlInputElement>().unwrap();

    if let Ok(y_value) = y_input.value().parse::<f32>() {
        if y_value >= -3.0 && y_value <= 3.0 {
            return true;
        }
    };

    alert("Y must be a number between -3 and 3");
    y_input.focus().ok();
    return false;
}

fn set_main_form_submit_listener() {
    let element = document().get_element_by_id("main_form").unwrap();

    let options = EventListenerOptions::enable_prevent_default();
    let listener =
        EventListener::new_with_options(&element.clone(), "submit", options, move |event| {
            event.prevent_default();

            if !validate_form() {
                return;
            }

            let form = event.target().unwrap();
            let form = form.dyn_ref::<HtmlFormElement>().unwrap();
            let form_data = FormData::new_with_form(form).unwrap();
            let params = UrlSearchParams::new_with_record_from_str_to_str(&form_data).unwrap();
            let url = format!("/fcgi-bin/server.jar/add?{}", params.to_string());

            spawn_local(request_and_update_table(url));
        });
    listener.forget();
}

fn set_value_y_rt_input_validator() {
    let element = document().get_element_by_id("ValueY").unwrap();

    let mut prev_value: String = String::new();
    let options = EventListenerOptions::enable_prevent_default();
    let listener =
        EventListener::new_with_options(&element.clone(), "input", options, move |event| {
            const AVAILABLE_CHARS: [&str; 12] =
                ["0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "-", "."];

            let element = element.dyn_ref::<HtmlInputElement>().unwrap();

            // event is InputEvent
            let Some(event) = event.dyn_ref::<web_sys::InputEvent>() else {
                return;
            };

            if ![
                "insertText",
                "deleteContentForward",
                "deleteContentBackward",
                "deleteWordForward",
                "deleteWordBackward",
                "deleteByCut",
                "historyUndo",
                "historyRedo",
            ]
            .contains(&event.input_type().as_str())
            {
                event.prevent_default();
                element.set_value(&prev_value);
                return;
            }

            // event.data isn't null
            if let Some(char) = event.data() {
                if !AVAILABLE_CHARS.contains(&char.as_str()) {
                    let mut new_value = element.value();
                    new_value.pop();
                    element.set_value(&new_value);
                }
            }

            prev_value = element.value();
        });
    listener.forget();
}
