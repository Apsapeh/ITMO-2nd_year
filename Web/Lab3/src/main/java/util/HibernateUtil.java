package util;

import entities.HitResult;
import jakarta.annotation.PostConstruct;
import jakarta.annotation.PreDestroy;
import jakarta.enterprise.context.ApplicationScoped; 
import jakarta.enterprise.inject.Produces;
import org.hibernate.SessionFactory;
import org.hibernate.boot.registry.StandardServiceRegistryBuilder;
import org.hibernate.cfg.Configuration;
import org.hibernate.cfg.Environment;
import org.hibernate.service.ServiceRegistry;

import java.util.Properties;

@ApplicationScoped
public class HibernateUtil {

    private SessionFactory sessionFactory;
    
    @PostConstruct
    public void init() {
        try {
            Configuration configuration = new Configuration();

            Properties settings = new Properties();
            String dbUrl = System.getProperty("db.url", 
                System.getenv().getOrDefault("DB_URL", "jdbc:postgresql://localhost:5432/studs"));
            String dbUser = System.getProperty("db.user", 
                System.getenv().getOrDefault("DB_USER", "s414219"));
            String dbPassword = System.getProperty("db.password", 
                System.getenv().getOrDefault("DB_PASSWORD", "password"));
            String dbDriver = System.getProperty("db.driver", 
                System.getenv().getOrDefault("DB_DRIVER", "org.postgresql.Driver"));
            
            settings.put(Environment.DRIVER, dbDriver);
            settings.put(Environment.URL, dbUrl);
            settings.put(Environment.USER, dbUser);
            settings.put(Environment.PASS, dbPassword);
            settings.put(Environment.DIALECT, "org.hibernate.dialect.PostgreSQLDialect");
            settings.put(Environment.SHOW_SQL, 
                System.getProperty("hibernate.show_sql", System.getenv().getOrDefault("HIBERNATE_SHOW_SQL", "true")));
            settings.put(Environment.CURRENT_SESSION_CONTEXT_CLASS, "thread");
            settings.put(Environment.HBM2DDL_AUTO, 
                System.getProperty("hibernate.hbm2ddl.auto", System.getenv().getOrDefault("HIBERNATE_HBM2DDL_AUTO", "update")));
            
            System.out.println("Hibernate configuration: URL=" + dbUrl + ", User=" + dbUser);

            configuration.setProperties(settings);
            configuration.addAnnotatedClass(HitResult.class);

            ServiceRegistry serviceRegistry = new StandardServiceRegistryBuilder()
                    .applySettings(configuration.getProperties()).build();

            sessionFactory = configuration.buildSessionFactory(serviceRegistry);
            System.out.println("SessionFactory created successfully");
        } catch (Exception e) {
            System.err.println("Failed to create SessionFactory: " + e.getMessage());
            e.printStackTrace();
            throw new RuntimeException("Failed to initialize Hibernate SessionFactory", e);
        }
    }

    @Produces
    @ApplicationScoped
    public SessionFactory getSessionFactory() {
        return sessionFactory;
    }


    @PreDestroy
    public void shutdown() {
        if (sessionFactory != null) {
            sessionFactory.close();
        }
    }
}