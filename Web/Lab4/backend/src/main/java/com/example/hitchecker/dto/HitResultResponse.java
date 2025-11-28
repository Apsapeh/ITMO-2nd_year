package com.example.hitchecker.dto;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.util.Date;

@Data
@NoArgsConstructor
@AllArgsConstructor
public class HitResultResponse {
    private Long id;
    private Float x;
    private Float y;
    private Integer r;
    private Boolean isHit;
    private Date dateTime;
    private Float executionTime;
}

