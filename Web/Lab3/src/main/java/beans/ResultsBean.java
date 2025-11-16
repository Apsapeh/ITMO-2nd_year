package beans;

import db.HitResultDB;
import entities.HitResult;
import jakarta.annotation.PostConstruct;
import jakarta.enterprise.context.SessionScoped;
import jakarta.inject.Named;
import jakarta.inject.Inject;

import java.io.Serializable;
import java.util.ArrayList;
import java.util.List;


@Named("resultsBean")
@SessionScoped
public class ResultsBean implements Serializable {
    private static final long serialVersionUID = 1L;
    
    private List<HitResult> results;
    
    @Inject
    private HitResultDB hitResultDB;

    @PostConstruct
    public void init() {
        loadResults();
    }

    private synchronized void loadResults() {
        try {
            results = hitResultDB.findAll();
        } catch (Exception e) {
            System.err.println("Error loading results: " + e.getMessage());
            results = new ArrayList<>();
        }
    }

    public synchronized List<HitResult> getResults() {
        return results;
    }

    public synchronized void refreshResults() {
        loadResults();
    }

    public synchronized void clearResults() {
        try {
            hitResultDB.deleteAll();
            results = new ArrayList<>();
        } catch (Exception e) {
            System.err.println("Error clearing results: " + e.getMessage());
        }
    }
}

