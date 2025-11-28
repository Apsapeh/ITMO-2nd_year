package com.example.hitchecker.entity;

import jakarta.persistence.*;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.util.Date;

@Entity
@Table(name = "hit_results")
@Data
@NoArgsConstructor
@AllArgsConstructor
public class HitResult {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
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
    
    @ManyToOne(fetch = FetchType.EAGER)
    @JoinColumn(name = "user_id", nullable = false)
    private User user;
}

