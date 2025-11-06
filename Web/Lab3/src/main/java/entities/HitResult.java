package entities;

import jakarta.persistence.*;
import java.io.Serializable;
import java.util.Date;

@Entity
@Table(name = "hit_results")
public class HitResult implements Serializable {
    private static final long serialVersionUID = 1L;

    @Id
    @GeneratedValue(strategy = GenerationType.SEQUENCE, generator = "hit_seq")
    @SequenceGenerator(name = "hit_seq", sequenceName = "hit_seq", allocationSize = 1)
    @Column(name = "id")
    private Long id;

    @Column(name = "x_value", nullable = false)
    private Float x;

    @Column(name = "y_value", nullable = false)
    private Float y;

    @Column(name = "r_value", nullable = false)
    private Integer r;

    @Column(name = "is_hit", nullable = false)
    private Boolean isHit;

    @Column(name = "date_time", nullable = false)
    @Temporal(TemporalType.TIMESTAMP)
    private Date dateTime;

    @Column(name = "execution_time")
    private Float executionTime;

    public HitResult() {
    }

    public HitResult(Float x, Float y, Integer r, Boolean isHit, Date dateTime, Float executionTime) {
        this.x = x;
        this.y = y;
        this.r = r;
        this.isHit = isHit;
        this.dateTime = dateTime;
        this.executionTime = executionTime;
    }

    // Getters and Setters
    public Long getId() {
        return id;
    }

    public void setId(Long id) {
        this.id = id;
    }

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
        this.r = r;
    }

    public Boolean getIsHit() {
        return isHit;
    }

    public void setIsHit(Boolean isHit) {
        this.isHit = isHit;
    }

    public Date getDateTime() {
        return dateTime;
    }

    public void setDateTime(Date dateTime) {
        this.dateTime = dateTime;
    }

    public Float getExecutionTime() {
        return executionTime;
    }

    public void setExecutionTime(Float executionTime) {
        this.executionTime = executionTime;
    }
}

