package com.example.hitchecker.controller;

import com.example.hitchecker.dto.HitCheckRequest;
import com.example.hitchecker.dto.HitCheckGraphRequest;
import com.example.hitchecker.dto.HitResultResponse;
import com.example.hitchecker.service.HitCheckService;
import com.example.hitchecker.util.JwtUtil;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.validation.Valid;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@RestController
@RequestMapping("/api/hit")
@CrossOrigin(origins = "http://localhost:28081/")
public class HitController {
    private final HitCheckService hitCheckService;
    private final JwtUtil jwtUtil;
    
    public HitController(HitCheckService hitCheckService, JwtUtil jwtUtil) {
        this.hitCheckService = hitCheckService;
        this.jwtUtil = jwtUtil;
    }
    
    private String extractUsernameFromRequest(HttpServletRequest request) {
        String authHeader = request.getHeader("Authorization");
        if (authHeader != null && authHeader.startsWith("Bearer ")) {
            String token = authHeader.substring(7);
            return jwtUtil.extractUsername(token);
        }
        throw new IllegalArgumentException("No valid token found");
    }
    
    @PostMapping("/check")
    public ResponseEntity<HitResultResponse> checkHit(@Valid @RequestBody HitCheckRequest request, 
                                                       HttpServletRequest httpRequest) {
        try {
            String username = extractUsernameFromRequest(httpRequest);
            HitResultResponse response = hitCheckService.checkHit(request, username);
            return ResponseEntity.ok(response);
        } catch (IllegalArgumentException e) {
            return ResponseEntity.badRequest().build();
        }
    }

    @PostMapping("/checkGraph")
    public ResponseEntity<HitResultResponse> checkHitGraph(@Valid @RequestBody HitCheckGraphRequest request, 
                                                       HttpServletRequest httpRequest) {
        try {
            String username = extractUsernameFromRequest(httpRequest);
            HitResultResponse response = hitCheckService.checkHitGraph(request, username);
            return ResponseEntity.ok(response);
        } catch (IllegalArgumentException e) {
            return ResponseEntity.badRequest().build();
        }
    }
    
    @GetMapping("/results")
    public ResponseEntity<List<HitResultResponse>> getResults(HttpServletRequest request) {
        try {
            String username = extractUsernameFromRequest(request);
            List<HitResultResponse> results = hitCheckService.getResults(username);
            return ResponseEntity.ok(results);
        } catch (IllegalArgumentException e) {
            return ResponseEntity.badRequest().build();
        }
    }

    @DeleteMapping("/results/clear")
    public ResponseEntity<Void> clearResults(HttpServletRequest request) {
        try {
            String username = extractUsernameFromRequest(request);
            hitCheckService.clearResults(username);
            return ResponseEntity.ok().build();
        } catch (IllegalArgumentException e) {
            return ResponseEntity.badRequest().build();
        }
    }
}

