package servlets;

import java.util.Deque;
import java.util.concurrent.LinkedBlockingDeque;

import jakarta.servlet.ServletContext;
import jakarta.servlet.ServletContextEvent;
import jakarta.servlet.ServletContextListener;
import jakarta.servlet.annotation.WebListener;
import general.HitData;

@WebListener
public class AppContextListener implements ServletContextListener {
    @Override
    public void contextInitialized(ServletContextEvent sce) {
        Deque<HitData> requestDataDeque = new LinkedBlockingDeque<HitData>();
        ServletContext context = sce.getServletContext();
        context.setAttribute("globalHitHistoryDeque", requestDataDeque);
    }

}
