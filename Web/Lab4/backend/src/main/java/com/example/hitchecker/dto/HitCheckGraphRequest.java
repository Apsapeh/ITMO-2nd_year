package com.example.hitchecker.dto;

import jakarta.validation.constraints.Min;
import jakarta.validation.constraints.NotNull;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

@Data
@NoArgsConstructor
@AllArgsConstructor
public class HitCheckGraphRequest {
    @NotNull(message = "X is required")
    private Float x;
    
    @NotNull(message = "Y is required")
    private Float y;
    
    @NotNull(message = "R is required")
    @Min(value = 1, message = "R must be at least 1")
    private Integer r;
}

