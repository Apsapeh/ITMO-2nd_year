package com.example.hitchecker.dto;

import lombok.AllArgsConstructor;
import lombok.Data;
import lombok.NoArgsConstructor;

import java.util.List;

@Data
@NoArgsConstructor
@AllArgsConstructor
public class HitTableResponse {
    private List<HitResultResponse> results;
    private Long totalCount;
    private Integer page;
    private Integer pageSize;
}

