package nothing.here;

import java.nio.charset.StandardCharsets;

public class ResponseFactory {
    public static String createOkResponseRaw(String content, String content_type) {

        return """
            Content-Type: %s
            Content-Length: %d
            
            %s
            """.formatted(content_type, content.getBytes(StandardCharsets.UTF_8).length, content);
    }

    public static String createOkResponseText(String content) {
        return createOkResponseRaw(content, "text/plain");
    }

    public static String createOkResponseJson(String content) {
        return createOkResponseRaw(content, "application/json");
    }



    public static String createBadResponseErrorRaw(String content, String content_type) {
        content = content.replace("\"", "\\\"");
        content = "{ \"error\": \"" + content + "\" }";

        return """
            Status: 400 Bad Request
            Content-Type: %s
            Content-Length: %d
            
            %s
            """.formatted(content_type, content.getBytes(StandardCharsets.UTF_8).length, content);
    }

    public static String createBadResponseErrorText(String content) {
        return createBadResponseErrorRaw(content, "text/plain");
    }

    public static String createBadResponseErrorJson(String content) {
        return createBadResponseErrorRaw(content, "application/json");
    }


    public static String createInternalErrorResponseRaw(String content, String content_type) {
        content = content.replace("\"", "\\\"");
        content = "{ \"error\": \"" + content + "\" }";

        return """
            Status: 500 Internal Server Error
            Content-Type: %s
            Content-Length: %d
            
            %s
            """.formatted(content_type, content.getBytes(StandardCharsets.UTF_8).length, content);
    }

    public static String createInternalErrorResponseText(String content) {
        return createInternalErrorResponseRaw(content, "text/plain");
    }

    public static String createInternalErrorResponseJson(String content) {
        return createInternalErrorResponseRaw(content, "application/json");
    }
}