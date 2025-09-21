use multiline_str::multiline_str;

pub struct ResponseFactory;

impl ResponseFactory {
    pub fn ok_raw(content: &str, content_type: &str) -> String {
        format!(
            multiline_str!(
                "Content-Type: {}"; 
                "Content-Length: {}"; 
                ""; 
                "{}"
            ),
            content_type,
            content.len(),
            content
        )
    }

    #[allow(unused)]
    pub fn ok_text(text: &str) -> String {
        Self::ok_raw(text, "text/plain")
    }
    
    pub fn ok_json(text: &str) -> String {
        Self::ok_raw(text, "application/json")
    }
    
    pub fn bad_response_raw(content: &str, content_type: &str) -> String {
        format!(
            multiline_str!(
                "Status: 400 Bad Request";
                "Content-Type: {}"; 
                "Content-Length: {}"; 
                ""; 
                "{}"
            ),
            content_type,
            content.len(),
            content
        )
    }

    pub fn bad_response_text(text: &str) -> String {
        Self::bad_response_raw(text, "text/plain")
    }
    
    pub fn internal_error_raw(content: &str, content_type: &str) -> String {
        format!(
            multiline_str!(
                "Status: 500 Internal Server Error";
                "Content-Type: {}"; 
                "Content-Length: {}"; 
                ""; 
                "{}"
            ),
            content_type,
            content.len(),
            content
        )
    }

    pub fn internal_error_text(text: &str) -> String {
        Self::internal_error_raw(text, "text/plain")
    }
    
}
