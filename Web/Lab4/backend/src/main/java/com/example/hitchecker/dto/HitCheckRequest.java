package com.example.hitchecker.dto;

import jakarta.validation.constraints.DecimalMax;
import jakarta.validation.constraints.DecimalMin;
import jakarta.validation.constraints.Min;
import jakarta.validation.constraints.NotNull;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

@Data
@NoArgsConstructor
@AllArgsConstructor
public class HitCheckRequest {
    @NotNull(message = "X is required")
    @DecimalMin(value = "-3", message = "X must be between -3 and 5")
    @DecimalMax(value = "5", message = "X must be between -3 and 5")
    private Float x;
    
    @NotNull(message = "Y is required")
    @DecimalMin(value = "-5", message = "Y must be between -5 and 5")
    @DecimalMax(value = "5", message = "Y must be between -5 and 5")
    private Float y;
    
    @NotNull(message = "R is required")
    @Min(value = 1, message = "R must be between 1 and 5")
    private Integer r;
}

