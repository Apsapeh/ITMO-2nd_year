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

    private Float x = null;
    private Float y = null;
    private Integer r = 1;

    @Inject
    private ResultsBean resultsBean;

    @Inject
    private HitResultDB hitResultDB;

    public Float getX() {
        return x;
    }

    public void setX(Float x) {
        
        this.x = x;
    }

    public Float getY() {
        return y;
    }

    public void setY(Float y) {
        
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

    private void checkPointImpl() {
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

        resultsBean.refreshResults();
    }

    public void checkPoint() {
        if (x == null || y == null || r == null) {
            throw new IllegalArgumentException("X, Y and R must be set");
        }

        if (x < -3 || x > 5) {
            throw new IllegalArgumentException("X must be between -3 and 5");
        }

        if (y < -5 || y > 5) {
            throw new IllegalArgumentException("Y must be between -5 and 5");
        }

        checkPointImpl();
    }

    public void checkPointFromGraph(String clickXStr, String clickYStr) {
    try {
        Float clickX = Float.parseFloat(clickXStr);
        Float clickY = Float.parseFloat(clickYStr);
        
        if (clickX == null || clickY == null) {
            throw new IllegalArgumentException("Click coordinates must be provided");
        }
        
        if (r == null || r < 1 || r > 5) {
            throw new IllegalArgumentException("R must be set and valid (1-5) before checking from graph");
        }
        
        this.x = clickX;
        this.y = clickY;
        checkPointImpl();
    } catch (NumberFormatException e) {
        throw new IllegalArgumentException("Invalid coordinate format");
    }
}
}
