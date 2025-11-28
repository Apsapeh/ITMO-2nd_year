package com.example.hitchecker.repository;

import com.example.hitchecker.entity.HitResult;
import com.example.hitchecker.entity.User;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.stereotype.Repository;

import java.util.List;

@Repository
public interface HitResultRepository extends JpaRepository<HitResult, Long> {
    List<HitResult> findByUserOrderByDateTimeDesc(User user);
    void deleteByUser(User user);
}

