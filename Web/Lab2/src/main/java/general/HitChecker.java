package general;

public class HitChecker {
    public static boolean isHit(HitData requestData) {
        float r = requestData.getR();
        float x = requestData.getX() / r;
        float y = requestData.getY() / r;

        boolean inRectangle = (x >= -1 && x <= 0) && (y >= 0 && y <= 0.5);

        boolean inTriangle = (x >= 0 && x <= 0.5) && (y >= 0 && y <= 0.5) && (y <= -x + 0.5);

        boolean inSemicircle = (x >= 0 && x <= 0.5) && (y >= -0.5 && y <= 0) && (x * x + y * y <= 0.5 * 0.5);

        return inRectangle || inTriangle || inSemicircle;
    }
}
