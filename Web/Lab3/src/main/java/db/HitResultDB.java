package db;

import entities.HitResult;
import org.hibernate.Session;
import org.hibernate.SessionFactory;
import org.hibernate.Transaction;
import util.HibernateUtil;

import java.util.ArrayList;
import java.util.List;

public class HitResultDB {
    
    public void save(HitResult hitResult) {
        SessionFactory sessionFactory = HibernateUtil.getSessionFactory();
        if (sessionFactory == null) {
            System.err.println("Database connection not available. Cannot save result.");
            return;
        }
        
        if (hitResult == null) {
            System.err.println("HitResult is null. Cannot save.");
            return;
        }
        
        Session session = null;
        Transaction transaction = null;
        try {
            session = sessionFactory.openSession();
            transaction = session.beginTransaction();
            session.persist(hitResult);
            session.flush(); // Принудительно выполняем SQL
            transaction.commit();
        } catch (Exception e) {
            if (transaction != null && transaction.isActive()) {
                try {
                    transaction.rollback();
                } catch (Exception rollbackEx) {
                    System.err.println("Error during rollback: " + rollbackEx.getMessage());
                }
            }
            System.err.println("Error saving hit result: " + e.getMessage());
            e.printStackTrace();
            throw new RuntimeException("Failed to save hit result", e);
        } finally {
            if (session != null) {
                try {
                    session.close();
                } catch (Exception e) {
                    System.err.println("Error closing session: " + e.getMessage());
                }
            }
        }
    }

    public List<HitResult> findAll() {
        SessionFactory sessionFactory = HibernateUtil.getSessionFactory();
        if (sessionFactory == null) {
            System.err.println("Database connection not available. Returning empty list.");
            return new ArrayList<>();
        }
        
        Session session = null;
        try {
            session = sessionFactory.openSession();
            return session.createQuery("FROM HitResult ORDER BY dateTime DESC", HitResult.class).getResultList();
        } catch (Exception e) {
            System.err.println("Error loading results: " + e.getMessage());
            e.printStackTrace();
            return new ArrayList<>();
        } finally {
            if (session != null) {
                session.close();
            }
        }
    }

    public void deleteAll() {
        SessionFactory sessionFactory = HibernateUtil.getSessionFactory();
        if (sessionFactory == null) {
            System.err.println("Database connection not available. Cannot delete results.");
            return;
        }
        
        Session session = null;
        Transaction transaction = null;
        try {
            session = sessionFactory.openSession();
            transaction = session.beginTransaction();
            session.createMutationQuery("DELETE FROM HitResult").executeUpdate();
            transaction.commit();
        } catch (Exception e) {
            if (transaction != null) {
                transaction.rollback();
            }
            System.err.println("Error deleting results: " + e.getMessage());
            e.printStackTrace();
        } finally {
            if (session != null) {
                session.close();
            }
        }
    }
}

