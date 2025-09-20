package nothing.here;

import java.util.List;
import java.util.Locale;

public class RequestData {
    private final float X;
    private final float Y;
    private final int R;
    private final String date;
    private final float time;

    public RequestData(float x, float y, int r, String date, float time) {
        X = x;
        Y = y;
        R = r;
        this.date = date;
        this.time = time;
    }

    public float getX() {
        return X;
    }

    public float getY() {
        return Y;
    }

    public int getR() {
        return R;
    }

    public String getDate() {
        return date;
    }

    public float getTime() {
        return time;
    }

    public String toJson() {
        double x = Math.round(getX() * 100.0) / 100.0;
        double y = Math.round(getY() * 100.0) / 100.0;
        return """
                {"x": %s, "y": %s, "r": %d, "hit": %b, "date": "%s", "time": "%s"}
                """.formatted(
                String.format(Locale.US, "%.2f", x),
                String.format(Locale.US, "%.2f", y),
                getR(),
                HitChecker.isHit(this),
                getDate(),
                String.format(Locale.US, "%.5f", getTime())
        );
    }

    public static String listToJson(List<RequestData> requests) {
        StringBuilder sb = new StringBuilder();
        sb.append("[");

        for (int i = 0; i < requests.size(); i++) {
            RequestData parsedQuery = requests.get(i);

            sb.append(parsedQuery.toJson());

            if  (i < requests.size() - 1) {
                sb.append(", ");
            }
        }
        sb.append("]");

        return sb.toString();
    }
}
