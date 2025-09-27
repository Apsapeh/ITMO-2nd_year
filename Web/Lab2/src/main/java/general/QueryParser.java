package general;

import exceptions.EBadResponse;
import exceptions.EInternalError;

import java.text.Format;
import java.text.SimpleDateFormat;
import java.util.HashMap;

public class QueryParser {
    static public HitData parse(String query) throws EBadResponse, EInternalError {
        if (query == null)
            throw new EInternalError("Query is null");
        if (query.isEmpty())
            throw new EBadResponse("POST request has no query");

        long startTime = System.nanoTime();

        HashMap<String, String> queryMap = getQueryMap(query);

        float valueY = parseField(queryMap, "ValueY", "Y", -3, 3);
        float valueR = parseField(queryMap, "ValueR", "R", 1, 4);
        float valueX = parseField(queryMap, "ValueX", "X", -2, 2);

        Format formatter = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        String date = formatter.format(new java.util.Date());

        long endTime = System.nanoTime();
        float elapsedTimeMs = (float) (endTime - startTime) / 1000000.0f;

        return new HitData(valueX, valueY, valueR, date, elapsedTimeMs);
    }

    public static float parseField(HashMap<String, String> queryMap, String fieldName, String normName, float min, float max) throws EBadResponse{
        float value;
        String value_str = queryMap.get(fieldName);
        try {
            value = Float.parseFloat(value_str);
        } catch (NumberFormatException | NullPointerException e) {
            throw new EBadResponse("Invalid value for " + normName + " value. It should be a real number");
        }

        if (value < min || value > max)
            throw new EBadResponse("Invalid value for " + normName + " value. It should be in range [" + min + ", " + max + "]");
        return value;
    }

    static public boolean containsPointData(String query) {
        HashMap<String, String> queryMap = getQueryMap(query);

        return queryMap.containsKey("ValueY") && queryMap.containsKey("ValueR") && queryMap.containsKey("ValueX");
    }

    static private HashMap<String, String> getQueryMap(String query) {
        HashMap<String, String> queryMap = new java.util.HashMap<>();
        for (String param : query.split("&")) {
            String[] pair = param.split("=", 2);
            if (pair.length > 1) {
                queryMap.put(pair[0], pair[1]);
            } else {
                queryMap.put(pair[0], "on");
            }
        }
        return queryMap;
    }
}
