package general;

import java.util.HashMap;

import exceptions.EInternalError;

public class RequestParser {
    final private HashMap<String, String> queryMap;
    final private String commandType;

    private static final String COMMAND_FIELD = "__cmd_type";
    private static final String ADD_COMMAND = "add";
    private static final String CLEAR_COMMAND = "clear";

    public RequestParser(String request) throws EInternalError {
        if (request == null)
            throw new EInternalError("Query is null");

        // parse query to map
        this.queryMap = new java.util.HashMap<>();
        for (String param : request.split("&")) {
            String[] pair = param.split("=", 2);
            if (pair.length > 1)
                queryMap.put(pair[0], pair[1]);
            else
                queryMap.put(pair[0], "on");
        }

        String cmd = queryMap.get(COMMAND_FIELD);
        if (cmd == null)
            commandType = "";
        else
            commandType = cmd;
    }

    public boolean isAddCommand() {
        return commandType.equals(ADD_COMMAND);
    }

    public boolean isClearCommand() {
        return commandType.equals(CLEAR_COMMAND);
    }

    public String get(String key) {
        return queryMap.get(key);
    }

    public boolean containsKey(String key) {
        return queryMap.containsKey(key);
    }
}
