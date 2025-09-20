package nothing.here;

public class HitChecker {
    public static boolean isHit(RequestData requestData) {
        float x = requestData.getX();
        float y = requestData.getY();
        int r = requestData.getR();

        boolean inRectangle = (x >= 0 && x <= r) && (y <= 0 && y >= -r);

        boolean inTriangle = (x >= 0 && x <= r / 2.0f) && (y >= 0 && y <= r) && (y <= -x + r / 2.0f);

        boolean inSemicircle = (x <= 0 && x >= -r) && (y >= 0 && y <= r) && (x * x + y * y <= r * r);

        return inRectangle || inTriangle || inSemicircle;
    }
}
