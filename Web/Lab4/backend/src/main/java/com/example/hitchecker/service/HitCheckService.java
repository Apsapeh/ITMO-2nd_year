package com.example.hitchecker.service;

import com.example.hitchecker.dto.HitCheckRequest;
import com.example.hitchecker.dto.HitCheckGraphRequest;
import com.example.hitchecker.dto.HitResultResponse;
import com.example.hitchecker.dto.HitTableRequest;
import com.example.hitchecker.dto.HitTableResponse;
import com.example.hitchecker.entity.HitResult;
import com.example.hitchecker.entity.User;
import com.example.hitchecker.repository.HitResultRepository;
import com.example.hitchecker.repository.UserRepository;

import org.springframework.data.domain.Page;
import org.springframework.data.domain.PageRequest;
import org.springframework.data.domain.Pageable;
import org.springframework.data.domain.Sort;
import org.springframework.stereotype.Service;
import org.springframework.transaction.annotation.Transactional;

import java.util.Date;
import java.util.List;
import java.util.stream.Collectors;

@Service
public class HitCheckService {
    private final HitResultRepository hitResultRepository;
    private final UserRepository userRepository;
    
    public HitCheckService(HitResultRepository hitResultRepository, UserRepository userRepository) {
        this.hitResultRepository = hitResultRepository;
        this.userRepository = userRepository;
    }
    
    @Transactional
    private HitResultResponse _checkHit(Float x, Float y, Integer r, String username) {
        User user = userRepository.findByUsername(username)
                .orElseThrow(() -> new IllegalArgumentException("User not found"));
        
        long startTime = System.nanoTime();
        boolean isHit = HitChecker.isHit(x, y, r.floatValue());
        long endTime = System.nanoTime();
        float executionTime = (float) (endTime - startTime) / 1_000_000.0f;
        
        HitResult hitResult = new HitResult();
        hitResult.setX(x);
        hitResult.setY(y);
        hitResult.setR(r);
        hitResult.setIsHit(isHit);
        hitResult.setDateTime(new Date());
        hitResult.setExecutionTime(executionTime);
        hitResult.setUser(user);
        
        hitResult = hitResultRepository.save(hitResult);
        
        return convertToResponse(hitResult);
    }

    public HitResultResponse checkHit(HitCheckRequest request, String username) {
        // Validation
        if (request.getX() < -3 || request.getX() > 5) {
            throw new IllegalArgumentException("X must be between -3 and 5");
        }
        if (request.getY() < -5 || request.getY() > 5) {
            throw new IllegalArgumentException("Y must be between -5 and 5");
        }
        if (request.getR() < 1 || request.getR() > 5) {
            throw new IllegalArgumentException("R must be between 0 and 5");
        }
        
        return _checkHit(request.getX().floatValue(), request.getY(), request.getR(), username);
    }

    public HitResultResponse checkHitGraph(HitCheckGraphRequest request, String username) {
        if (request.getX() < -request.getR() * 1.7 || request.getX() > request.getR() * 1.7) {
            throw new IllegalArgumentException("X must be between -1.7R and 1.7R");
        }
        if (request.getY() < -request.getR() * 1.7 || request.getY() > request.getR() * 1.7) {
            throw new IllegalArgumentException("Y must be between -1.7R and 1.7R");
        }

        return _checkHit(request.getX(), request.getY(), request.getR(), username);
    }
    
    @Transactional(readOnly = true)
    public HitTableResponse getResults(HitTableRequest request) {
        Pageable pageable = PageRequest.of(request.getPage(), request.getPageSize(), Sort.by("dateTime").descending());
        Page<HitResult> hitResultsPage = hitResultRepository.findAll(pageable);

        List<HitResultResponse> responseList = hitResultsPage.getContent().stream()
                    .map(this::convertToResponse)
                    .collect(Collectors.toList());

        HitTableResponse response = new HitTableResponse();
        response.setResults(responseList);
        response.setTotalCount(hitResultsPage.getTotalElements());
        response.setPage(hitResultsPage.getNumber());
        response.setPageSize(hitResultsPage.getSize());

        return response;
    }

    @Transactional
    public void clearResults(String username) {
        User user = userRepository.findByUsername(username)
                .orElseThrow(() -> new IllegalArgumentException("User not found"));
        
        hitResultRepository.deleteByUser(user);
    }
    
    private HitResultResponse convertToResponse(HitResult hitResult) {
        HitResultResponse response = new HitResultResponse();

        response.setUsername(hitResult.getUser().getUsername());
        response.setId(hitResult.getId());
        response.setX(hitResult.getX());
        response.setY(hitResult.getY());
        response.setR(hitResult.getR());
        response.setIsHit(hitResult.getIsHit());
        response.setDateTime(hitResult.getDateTime());
        response.setExecutionTime(hitResult.getExecutionTime());
        
        return response;
    }
}

