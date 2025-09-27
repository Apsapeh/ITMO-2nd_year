package general;

import java.util.Locale;

public class HitData {
    private final float X;
    private final float Y;
    private final float R;
    private final boolean isHit;
    private final String date;
    private final float time;

    public HitData(float x, float y, float r, String date, float time) {
        X = x;
        Y = y;
        R = r;
        isHit = HitChecker.isHit(this);
        this.date = date;
        this.time = time;
    }

    public float getX() {
        return X;
    }

    public float getY() {
        return Y;
    }

    public float getR() {
        return R;
    }

    public boolean isHit() {
        return isHit;
    }

    public String getDate() {
        return date;
    }

    public float getTime() {
        return time;
    }

    public String toHtml() {
        String hit_class = isHit() ? "hit" : "miss";
        String hit_text = isHit() ? "Попадание" : "Промах";
        return """
                <tr id="result-tr">
                    <td id="x">%s</td>
                    <td id="y">%s</td>
                    <td id="r" >%s</td>
                    <td class="%s" id="hit">%s</td>
                    <td id="date">%s</td>
                    <td id="time">%s ms</td>
                </tr>
                """.formatted(
                String.format(Locale.US, "%.2f", getX()),
                String.format(Locale.US, "%.2f", getY()),
                String.format(Locale.US, "%.2f", getR()),
                hit_class,
                hit_text,
                getDate(),
                String.format(Locale.US, "%.5f", getTime())
        );
    }
}
