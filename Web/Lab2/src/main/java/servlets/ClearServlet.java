package servlets;

import java.io.IOException;
import java.util.concurrent.LinkedBlockingDeque;

import general.HitData;
import jakarta.servlet.ServletException;
import jakarta.servlet.annotation.WebServlet;
import jakarta.servlet.http.HttpServlet;
import jakarta.servlet.http.HttpServletRequest;
import jakarta.servlet.http.HttpServletResponse;

@WebServlet("/clear")
public class ClearServlet extends HttpServlet {

    @Override
    public void doPost(HttpServletRequest req, HttpServletResponse resp) throws ServletException, IOException {
        Object contextObject = getServletContext().getAttribute("globalHitHistoryDeque");
        assert (contextObject.getClass() == LinkedBlockingDeque.class);
        @SuppressWarnings("unchecked")
        LinkedBlockingDeque<HitData> hitResults = (LinkedBlockingDeque<HitData>)contextObject;

        hitResults.clear();

        //req.getRequestDispatcher("/result.jsp").forward(req, resp);
    }
}
