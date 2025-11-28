package com.example.hitchecker.service;

import com.example.hitchecker.dto.HitCheckRequest;
import com.example.hitchecker.dto.HitCheckGraphRequest;
import com.example.hitchecker.dto.HitResultResponse;
import com.example.hitchecker.entity.HitResult;
import com.example.hitchecker.entity.User;
import com.example.hitchecker.repository.HitResultRepository;
import com.example.hitchecker.repository.UserRepository;
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
        
        return _checkHit(request.getX(), request.getY(), request.getR(), username);
    }

    public HitResultResponse checkHitGraph(HitCheckGraphRequest request, String username) {
        return _checkHit(request.getX(), request.getY(), request.getR(), username);
    }
    
    @Transactional
    public List<HitResultResponse> getResults(String username) {
        User user = userRepository.findByUsername(username)
                .orElseThrow(() -> new IllegalArgumentException("User not found"));
        
        return hitResultRepository.findByUserOrderByDateTimeDesc(user)
                .stream()
                .map(this::convertToResponse)
                .collect(Collectors.toList());
    }

    @Transactional
    public void clearResults(String username) {
        User user = userRepository.findByUsername(username)
                .orElseThrow(() -> new IllegalArgumentException("User not found"));
        
        hitResultRepository.deleteByUser(user);
    }
    
    private HitResultResponse convertToResponse(HitResult hitResult) {
        return new HitResultResponse(
                hitResult.getId(),
                hitResult.getX(),
                hitResult.getY(),
                hitResult.getR(),
                hitResult.getIsHit(),
                hitResult.getDateTime(),
                hitResult.getExecutionTime()
        );
    }
}

