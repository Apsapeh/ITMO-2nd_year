use wasm_bindgen::prelude::*;

#[wasm_bindgen]
extern "C" {
    pub fn _alert(s: &str);
}

#[wasm_bindgen]
pub fn greet(name: &str) {
    //alert(&format!("Hello, {}!", name));
    gloo::dialogs::alert(&format!("Hello, {}!", name.to_uppercase()));
    
    //gloo::utils::document().set_onloadend(Some([] => {}));
}

fn myFunction() {
  gloo::utils::document().get_element_by_id("change-me").unwrap().set_inner_html("Clicked");
}

#[wasm_bindgen(start)]
pub fn main() {
    println!("Hello");
    
    let text = gloo::utils::document().get_element_by_id("change-me").unwrap();
    text.set_text_content(Some("OEUOEUEOUEOU"));
    
    //gloo::utils::document().get_element_by_id("button").unwrap().add_event_listener_with_callback("click", );
}
