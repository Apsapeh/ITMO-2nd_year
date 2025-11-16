package beans;

import db.HitResultDB;
import entities.HitResult;
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

    public List<HitResult> getResults() {
        if (results == null) {
            try {
                results = hitResultDB.findAll();
            } catch (Exception e) {
                System.err.println("Error loading results: " + e.getMessage());
                results = new ArrayList<>();
            }
        }
        return results;
    }

    public void refreshResults() {
        try {
            results = hitResultDB.findAll();
        } catch (Exception e) {
            System.err.println("Error refreshing results: " + e.getMessage());
            results = new ArrayList<>();
        }
    }

    public void clearResults() {
        try {
            hitResultDB.deleteAll();
            results = new ArrayList<>();
        } catch (Exception e) {
            System.err.println("Error clearing results: " + e.getMessage());
            results = new ArrayList<>();
        }
    }
}

