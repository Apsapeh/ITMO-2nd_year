package beans;

import db.HitResultDB;
import entities.HitResult;
import general.HitChecker;
import jakarta.enterprise.context.RequestScoped;
import jakarta.inject.Inject;
import jakarta.inject.Named;
import java.io.Serializable;
import java.util.Date;

@Named("pointBean")
@RequestScoped
public class PointBean implements Serializable {

    private static final long serialVersionUID = 1L;

    private Float x;
    private Float y;
    private Integer r = 1;

    @Inject
    private ResultsBean resultsBean;

    private final HitResultDB hitResultDB = new HitResultDB();

    public Float getX() {
        return x;
    }

    public void setX(Float x) {
        if (x == null || x < -3 || x > 5) {
            throw new IllegalArgumentException("X must be between -3 and 5");
        }
        this.x = x;
    }

    public Float getY() {
        return y;
    }

    public void setY(Float y) {
        if (y == null || y < -5 || y > 5) {
            throw new IllegalArgumentException("Y must be between -5 and 5");
        }
        this.y = y;
    }

    public Integer getR() {
        return r;
    }

    public void setR(Integer r) {
        if (r == null || r < 1 || r > 5) {
            throw new IllegalArgumentException("R must be between 1 and 5");
        }
        this.r = r;
    }

    public void checkPoint() {
        System.out.println(
            "checkPoint called: x=" + x + ", y=" + y + ", r=" + r
        );

        long startTime = System.nanoTime();

        boolean isHit = HitChecker.isHit(x, y, r);

        long endTime = System.nanoTime();
        float executionTime = (float) (endTime - startTime) / 1_000_000.0f;

        HitResult hitResult = new HitResult(
            x,
            y,
            r,
            isHit,
            new Date(),
            executionTime
        );

        try {
            hitResultDB.save(hitResult);
            System.out.println("HitResult saved successfully");
        } catch (Exception e) {
            System.err.println("Failed to save HitResult: " + e.getMessage());
            e.printStackTrace();
        }

        if (resultsBean != null) {
            resultsBean.refreshResults();
        }
    }

    public void checkPointFromGraph(Float clickX, Float clickY) {
        this.x = clickX;
        this.y = clickY;
        checkPoint();
    }
}
