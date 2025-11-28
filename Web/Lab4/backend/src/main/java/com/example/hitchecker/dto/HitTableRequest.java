package com.example.hitchecker.dto;

import jakarta.validation.constraints.Min;
import jakarta.validation.constraints.NotNull;
import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

@Data
@NoArgsConstructor
@AllArgsConstructor
public class HitTableRequest {
    @NotNull(message = "Page is required")
    @Min(value = 0, message = "Page must be at least 1")
    private Integer page;

    @NotNull(message = "Page size is required")
    @Min(value = 1, message = "Page size must be at least 1")
    private Integer pageSize;
}

