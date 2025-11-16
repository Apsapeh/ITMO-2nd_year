package db;

import entities.HitResult;
import jakarta.enterprise.context.ApplicationScoped;
import jakarta.inject.Inject;
import org.hibernate.Session;
import org.hibernate.SessionFactory;
import org.hibernate.Transaction;

import java.util.ArrayList;
import java.util.List;

@ApplicationScoped
public class HitResultDB {

    @Inject
    private SessionFactory sessionFactory; 

    public void save(HitResult hitResult) {
        if (hitResult == null) {
            System.err.println("HitResult is null. Cannot save.");
            return;
        }
    
        Transaction transaction = null;
        try (Session session = sessionFactory.openSession()) {
            transaction = session.beginTransaction();
            session.persist(hitResult);
            transaction.commit();
        } catch (Exception e) {
            if (transaction != null && transaction.isActive()) {
                transaction.rollback();
            }
            System.err.println("Error saving hit result: " + e.getMessage());
            e.printStackTrace();
        }
    }

    public List<HitResult> findAll() {
        try (Session session = sessionFactory.openSession()) {
            return session.createQuery("FROM HitResult ORDER BY dateTime DESC", HitResult.class).getResultList();
        } catch (Exception e) {
            System.err.println("Error loading results: " + e.getMessage());
            e.printStackTrace();
            return new ArrayList<>();
        }
    }

    public void deleteAll() {
        Transaction transaction = null;
        try (Session session = sessionFactory.openSession()) {
            transaction = session.beginTransaction();
            session.createMutationQuery("DELETE FROM HitResult").executeUpdate();
            transaction.commit();
        } catch (Exception e) {
            if (transaction != null && transaction.isActive()) {
                transaction.rollback();
            }
            System.err.println("Error deleting results: " + e.getMessage());
            e.printStackTrace();
        }
    }
}