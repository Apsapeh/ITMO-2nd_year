package general;

import exceptions.EBadResponse;
import exceptions.EInternalError;

import java.text.Format;
import java.text.SimpleDateFormat;

public class HitRequestParser {
    final private RequestParser reqParser;
    final private long startTime;

    private static final String VALUE_X_FIELD = "ValueX";
    private static final String VALUE_Y_FIELD = "ValueY";
    private static final String VALUE_R_FIELD = "ValueR";

    public HitRequestParser(RequestParser req) throws EInternalError {
        this.reqParser = req;
        this.startTime = System.nanoTime();
    }

    public HitData parseToHitData() throws EBadResponse {
        float valueY = parseField(VALUE_Y_FIELD, "Y", -3, 3);
        float valueR = parseField(VALUE_R_FIELD, "R", 1, 4);
        float valueX = parseField(VALUE_X_FIELD, "X", -2, 2);

        Format formatter = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
        String date = formatter.format(new java.util.Date());

        boolean isHit = HitChecker.isHit(valueX, valueY, valueR);

        long endTime = System.nanoTime();
        float elapsedTimeMs = (float) (endTime - startTime) / 1000000.0f;

        return new HitData(valueX, valueY, valueR, isHit, date, elapsedTimeMs);
    }

    private float parseField(String fieldName, String normName, float min, float max) throws EBadResponse{
        float value;
        String value_str = reqParser.get(fieldName);
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
        return reqParser.containsKey(VALUE_Y_FIELD) && reqParser.containsKey(VALUE_R_FIELD) && reqParser.containsKey(VALUE_X_FIELD);
    }
}
