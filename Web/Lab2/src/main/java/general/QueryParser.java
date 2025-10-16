package general;

import exceptions.EBadResponse;
import exceptions.EInternalError;

import java.text.Format;
import java.text.SimpleDateFormat;
import java.util.HashMap;

public class QueryParser {
    final private HashMap<String, String> queryMap;
    final private long startTime;

    public QueryParser(String query) throws EInternalError {
        if (query == null)
            throw new EInternalError("Query is null");

        // parse query to map
        this.queryMap = new java.util.HashMap<>();
        for (String param : query.split("&")) {
            String[] pair = param.split("=", 2);
            if (pair.length > 1)
                queryMap.put(pair[0], pair[1]);
            else
                queryMap.put(pair[0], "on");
        }

        this.startTime = System.nanoTime();
    }

    public HitData parseToHitData() throws EBadResponse {
        float valueY = parseField("ValueY", "Y", -3, 3);
        float valueR = parseField("ValueR", "R", 1, 4);
        float valueX = parseField("ValueX", "X", -2, 2);

        Format formatter = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        String date = formatter.format(new java.util.Date());

        boolean isHit = HitChecker.isHit(valueX, valueY, valueR);

        long endTime = System.nanoTime();
        float elapsedTimeMs = (float) (endTime - startTime) / 1000000.0f;

        return new HitData(valueX, valueY, valueR, isHit, date, elapsedTimeMs);
    }

    private float parseField(String fieldName, String normName, float min, float max) throws EBadResponse{
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

    public boolean containsPointData() {
        return queryMap.containsKey("ValueY") && queryMap.containsKey("ValueR") && queryMap.containsKey("ValueX");
    }
}
